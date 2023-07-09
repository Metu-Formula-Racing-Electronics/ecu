#include <Arduino.h>
#include "sensor.h"
#include "mp3.h"
#include "SPIFFS.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include <freertos/task.h>
#include "driver/gpio.h"
#include "driver/can.h"
#include <Bamocar_can.h>
#include "esp_log.h"

// Do NOT use GPIO 25 and 26, its used by R2DS audio (RELAY2 unusable)
//  GPIO 16 and 17 are used for Serial2
#define CAND GPIO_NUM_19
#define CANR GPIO_NUM_18
#define AMP_EN 27       // Amplifier enable pin, HIGH = ON, LOW = OFF
#define COOLING_PUMP 12 // Cooling Pump
#define COOLING_FAN1 32 // Cooling Fan 1
#define COOLING_FAN2 33 // Cooling Fan 2
// Buttons Definitions
#define R2D_BUTTON 14 // R2D BUTTON WITH PULLUP
#define ACT_BUTTON 13 // ACTIVATION BUTTON
// Relay Definitions
#define PRECHARGE 15
#define AIR 4
// Lights Definitions
#define TSAL_RED COOLING_PUMP   // TSAL RED Light // 2Hz min
#define TSAL_GREEN COOLING_FAN1 // TSAL GREEN Light
#define ASSI_BLUE 2             // ASSI BLUE Light // 1Hz min
#define ASSI_YELLOW 5           // ASSI YELLOW Light // 1Hz min
// APPS Definitions
#define APPS1PIN 39
#define APPS2PIN 36
#define APPS1LOW 150
#define APPS1HIGH 2150
#define APPS2LOW 155
#define APPS2HIGH 2125
#define BRAKEPIN 34
#define BRAKE_LIGHT 33

bool autonomous = true;
bool apps_implausable = false;
uint8_t autonomous_apps = 0;
uint8_t state = 0;
int apps_avg = 0;
uint64_t last_bamocar_rx = 0;
#define BAMOCAR_TIMEOUT 1000
bool apps_reverse = false;

Bamocar_data bamocar(0x181, 0x201); // Bamocar CAN ID's

void read_apps();

// nextlcd lcd(&Serial2);
// musicPlayer mp(&Serial2);

bool is_bamocar_connected()
{
  return millis() - last_bamocar_rx < BAMOCAR_TIMEOUT;
}

static void can_task(void *args)
{
  can_message_t message;
  while (1)
  {
    if (can_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK)
    {
      if (bamocar.parseMessage(message))
      {
        last_bamocar_rx = millis();
        return;
      }
    }
  }
}

static void debug_task(void *args)
{
  while (1)
  {
    read_apps();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

#define TSAL_BLINK_HZ 3

static void blink_red_tsal_async()
{
  static unsigned long last_blink = 0;
  static bool led_state = false;
  if (millis() - last_blink > 500 / TSAL_BLINK_HZ)
  {
    led_state = !led_state;
    digitalWrite(TSAL_RED, led_state);
    last_blink = millis();
  }
}

static void TSAL_task(void *args)
{
  while (1)
  {
    if (!is_bamocar_connected())
    {
      digitalWrite(TSAL_RED, HIGH);
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    if (bamocar.getBusVoltage() > 10)
    {
      digitalWrite(TSAL_GREEN, LOW);
      blink_red_tsal_async();
    }
    else
    {
      digitalWrite(TSAL_GREEN, HIGH);
      digitalWrite(TSAL_RED, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static void can_init()
{
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(CAND, CANR, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
  if (can_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
  {
    printf("Failed to install driver\n");
    esp_restart();
  }
  // Start CAN driver
  if (can_start() != ESP_OK)
  {
    printf("Failed to start driver\n");
    esp_restart();
  }
}

static void bamocar_init()
{
  bamocar.requestBusVoltage(200);
  bamocar.requestSpeed(200);
  bamocar.requestStatus(200);
  bamocar.setSoftEnable(false);
  bamocar.requestHardEnabled(200);
}

static void motor_control_loop(void *args)
{
  static uint8_t last_state = 0;
  static bool bamocar_enabled = false;

  while (1)
  {
    if (!is_bamocar_connected())
    {
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    read_apps(); // deneme için

    if (apps_implausable && bamocar_enabled) // implausable check
    {
      bamocar.setSoftEnable(false);
      bamocar.setSpeed(0);
      bamocar_enabled = false;
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    if (state != last_state && state != 3 && bamocar_enabled)
    {
      bamocar.setSoftEnable(false);
      bamocar.setSpeed(0);
      bamocar_enabled = false;
    }

    if (state != last_state && state == 3 && !bamocar_enabled)
    {
      bamocar.setSoftEnable(true);
    }

    last_state = state;

    if (state != 3)
    {
      yield();
      continue;
    }

    if (autonomous)
    {
      if (Serial2.available())
      {
        autonomous_apps = Serial2.read();
        bamocar.setSpeed(((autonomous_apps <= 100 ? autonomous_apps : 0) * -0x7fff) / 100); // reversed on purpose, its the forward direction
      }
    }
    else
    {
      bamocar.setSpeed((apps_avg * -0x7fff) / 100); // reversed on purpose, its the forward direction
    }
    yield();
  }
}

void read_apps()
{
  static int apps1 = 0;
  static int apps2 = 0;
  static int diff = 0;
  static int correction = 0;
  static uint8_t apps_implausable_counter = 0;
  static uint64_t last_plausable_read_time = 0;

  if (apps_reverse)
  {
    apps1 = 100 - ((float)constrain(analogRead(APPS2PIN), APPS1LOW, APPS1HIGH) - APPS1LOW) / (APPS1HIGH - APPS1LOW) * 100;
    apps2 = ((float)constrain(analogRead(APPS1PIN), APPS2LOW, APPS2HIGH) - APPS2LOW) / (APPS2HIGH - APPS2LOW) * 100;
  }
  else
  {
    apps1 = 100 - ((float)constrain(analogRead(APPS1PIN), APPS1LOW, APPS1HIGH) - APPS1LOW) / (APPS1HIGH - APPS1LOW) * 100;
    apps2 = ((float)constrain(analogRead(APPS2PIN), APPS2LOW, APPS2HIGH) - APPS2LOW) / (APPS2HIGH - APPS2LOW) * 100;
  }
  apps_avg = (apps1 + apps2) / 2;

  if (apps_avg < 50)
  {
    correction = sqrt(2.5 * apps_avg);
    apps1 += correction;
    apps2 -= correction;
  }
  else
  {
    correction = sqrt(2.5 * (100 - apps_avg));
    apps1 += correction;
    apps2 -= correction;
  }
  diff = abs(apps1 - apps2);
  ESP_LOGI("APPS", "%d %d %d %d", apps1, apps2, diff, apps_avg);
  if (diff > 10)
  {
    apps_implausable_counter++;
    if (apps_implausable_counter > 3 && millis() - last_plausable_read_time > 100)
    {
      apps_implausable = true;
    }
  }
  else
  {
    apps_implausable_counter = 0;
    last_plausable_read_time = millis();
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(AMP_EN, OUTPUT);
  pinMode(APPS1PIN, INPUT);
  pinMode(APPS2PIN, INPUT);
  pinMode(R2D_BUTTON, INPUT_PULLUP); // Active LOW
  pinMode(ACT_BUTTON, INPUT);        // Active HIGH
  pinMode(TSAL_GREEN, OUTPUT);
  pinMode(TSAL_RED, OUTPUT);
  pinMode(ASSI_BLUE, OUTPUT);
  pinMode(ASSI_YELLOW, OUTPUT);
  pinMode(BRAKE_LIGHT, OUTPUT);
  pinMode(BRAKEPIN, INPUT);
  digitalWrite(PRECHARGE, LOW);
  digitalWrite(AIR, LOW);
  digitalWrite(TSAL_GREEN, HIGH);
  digitalWrite(TSAL_RED, LOW);
  digitalWrite(AMP_EN, HIGH); // Energize the amplifier

  read_apps();
  if (apps_avg < 50)
  {
    apps_reverse = false;
  }
  else
  {
    apps_reverse = true;
  }

  can_init();

  xTaskCreate(&can_task, "can_task", 4096, NULL, 5, NULL);
  ESP_LOGI("SETUP", "CAN task created");

  xTaskCreate(&TSAL_task, "TSAL_task", 4096, NULL, 5, NULL);
  ESP_LOGI("SETUP", "TSAL task created");

  xTaskCreate(&debug_task, "debug_task", 4096, NULL, 5, NULL);
  ESP_LOGI("SETUP", "Debug task created");

  xTaskCreate(&motor_control_loop, "motor_control_loop", 4096, NULL, 5, NULL);
  ESP_LOGI("SETUP", "Motor control task created");

  // Serial2.begin(9600, SERIAL_8N1, 17, 16);

  // mp3_setup();
}

bool get_shutdown_state()
{
  static bool shutdown_state = false;
  static uint8_t shutdown_state_counter = 0;
  bool measured_shutdown_state = digitalRead(ACT_BUTTON);

  if (measured_shutdown_state != shutdown_state)
  {
    shutdown_state_counter++;
    if (shutdown_state_counter > 2)
    {
      shutdown_state_counter = 0;
      shutdown_state = measured_shutdown_state;
    }
  }
  else
  {
    shutdown_state_counter = 0;
  }

  return shutdown_state;
}

bool get_r2d_state()
{
  static bool r2d_state = false;
  static uint8_t r2d_state_counter = 0;
  bool measured_r2d_state = !digitalRead(R2D_BUTTON);

  if (measured_r2d_state != r2d_state)
  {
    r2d_state_counter++;
    if (r2d_state_counter > 2)
    {
      r2d_state_counter = 0;
      r2d_state = measured_r2d_state;
    }
  }
  else
  {
    r2d_state_counter = 0;
  }

  return r2d_state;
}

void loop()
{
  //mp3_loop();

  if (!is_bamocar_connected())
  {
    digitalWrite(PRECHARGE, LOW);
    digitalWrite(AIR, LOW);
    state = 0;
    yield();
    return;
  }

  if (!get_shutdown_state())
  {
    digitalWrite(PRECHARGE, LOW);
    digitalWrite(AIR, LOW);
    state = 0;
    yield();
    return;
  }

  if (state = 0)
  {
    digitalWrite(PRECHARGE, HIGH);
    state = 1;
  }

  if (bamocar.getBusVoltage() < 30)
  {
    state = 1;
    digitalWrite(AIR, LOW);
    yield();
    return;
  }

  if (state == 1)
  {
    state = 2;
    digitalWrite(AIR, HIGH);
    vTaskDelay(pdMS_TO_TICKS(20));
    digitalWrite(PRECHARGE, LOW);
  }

  if (!get_r2d_state())
  {
    state = 2;
    yield();
    return;
  }

  if (state == 2)
  {
    state = 3;
  }

  vTaskDelay(pdMS_TO_TICKS(100));
}