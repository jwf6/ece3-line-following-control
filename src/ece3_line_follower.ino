#include <ECE3.h>

uint16_t sensorValues[8];

const int left_nslp_pin = 31;  // Awake and ready for PWM.
const int left_dir_pin = 29;
const int left_pwm_pin = 40;

const int right_nslp_pin = 11;
const int right_dir_pin = 30;
const int right_pwm_pin = 39;

const int LED_RF = 41;
const int base_speed = 180;

uint16_t minimum_sensor_values[8] = {665, 596, 550, 573, 527, 550, 574, 527};
uint16_t maximum_sensor_values[8] = {1835, 1904, 1748, 1927, 1927, 1950, 1926, 1973};
int sensor_weights[8] = {9, 6, 2, 1, -1, -2, -6, -9};

int bar_reached;
int previous_error;
int spin_time;
int straight_adjust;
int leftSpd;
int rightSpd;

void setup() {
  bar_reached = 0;
  previous_error = 0;
  spin_time = 41;
  straight_adjust = 35;

  pinMode(left_nslp_pin, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  digitalWrite(left_dir_pin, LOW);
  digitalWrite(left_nslp_pin, HIGH);

  pinMode(LED_RF, OUTPUT);

  pinMode(right_nslp_pin, OUTPUT);
  pinMode(right_dir_pin, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);
  digitalWrite(right_dir_pin, LOW);
  digitalWrite(right_nslp_pin, HIGH);

  ECE3_Init();
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  ECE3_read_IR(sensorValues);

  leftSpd = base_speed;
  rightSpd = base_speed;

  int weighted_average = 0;
  int proportional_change = 0;
  int derivative_change = 0;
  int total_sensor_value = 0;

  for (unsigned int i = 0; i < 8; i++) {
    if (sensorValues[i] < minimum_sensor_values[i]) {
      sensorValues[i] = 0;
    } else {
      sensorValues[i] = sensorValues[i] - minimum_sensor_values[i];
    }

    sensorValues[i] =
        (sensorValues[i] * 1000L) /
        (maximum_sensor_values[i] - minimum_sensor_values[i]);

    total_sensor_value = total_sensor_value + sensorValues[i];
    weighted_average =
        weighted_average + ((sensorValues[i] * sensor_weights[i]) / 4);
  }

  if (total_sensor_value > 11500) {
    bar_reached = bar_reached + 1;
  }

  proportional_change = (base_speed * weighted_average) / 3000;
  derivative_change = (weighted_average - previous_error) * 0.65;
  previous_error = weighted_average;

  if (bar_reached >= 2) {
    if (spin_time > 0) {
      digitalWrite(left_dir_pin, HIGH);
      spin_time = spin_time - 1;
    } else if (straight_adjust > 0) {
      digitalWrite(left_dir_pin, LOW);
      straight_adjust = straight_adjust - 1;
    } else {
      if (spin_time == -99) {
        leftSpd = 0;
        rightSpd = 0;
      } else {
        bar_reached = 0;
      }
      spin_time = -99;
    }
  } else {
    leftSpd = leftSpd + (proportional_change + derivative_change);
    rightSpd = rightSpd - (proportional_change + derivative_change);
  }

  leftSpd = constrain(leftSpd, 0, 255);
  rightSpd = constrain(rightSpd, 0, 255);

  analogWrite(left_pwm_pin, leftSpd);
  analogWrite(right_pwm_pin, rightSpd);

  digitalWrite(LED_RF, LOW);
  digitalWrite(LED_RF, HIGH);
}


