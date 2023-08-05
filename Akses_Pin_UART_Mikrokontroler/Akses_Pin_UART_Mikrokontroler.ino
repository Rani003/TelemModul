// AKSES PIN UART MIKROKONTROLER
// menerima pesan Mavlink
// mengurai pesan tMavlink
// mencetak informasi penting dari pesan ke Serial Monitor

#include <mavlink.h>

#define PIN_LED 13
#define BAUD_RATE 57600

uint16_t freq_debug = 3;
uint32_t last_debug = 0;
bool led_state = false;
double batt_voltage = 0.0;
double batt_current = 0.0;
uint16_t base_mode = 0;
uint16_t custom_mode = 0;
uint16_t sat_count = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);  // Turn off LED initially
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE);
}

void loop() {
  if (millis() - last_debug >= 1000 / freq_debug) {
    last_debug = millis();
    led_state = !led_state;
    digitalWrite(PIN_LED, led_state);
  }

  while (Serial1.available()) {
    uint8_t c = Serial1.read();
    mavlink_message_t msg;
    mavlink_status_t status;

    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_SYS_STATUS:
          mavlink_sys_status_t sys_status;
          mavlink_msg_sys_status_decode(&msg, &sys_status);
          batt_voltage = sys_status.voltage_battery / 1000.0; // Convert from mV to V
          batt_current = sys_status.current_battery / 100.0; // Convert from cA to A
          Serial.print("SYS STATUS\t");
          Serial.print("Batt_volt = ");
          Serial.print(batt_voltage, 2); // Print with 2 decimal places
          Serial.print(" V\t");
          Serial.print("Batt_curr = ");
          Serial.print(batt_current, 2); // Print with 2 decimal places
          Serial.println(" A");
          break;

        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          base_mode = heartbeat.base_mode;
          custom_mode = heartbeat.custom_mode;
          Serial.print("HEARTBEAT\t");
          Serial.print("base_mode = ");
          Serial.print(base_mode);
          Serial.print("\tcustom_mode = ");
          Serial.println(custom_mode);
          break;

        case MAVLINK_MSG_ID_GPS_RAW_INT:
          mavlink_gps_raw_int_t gps_raw;
          mavlink_msg_gps_raw_int_decode(&msg, &gps_raw);
          sat_count = gps_raw.satellites_visible;
          Serial.print("GPS_RAW\t");
          Serial.print("satellite count = ");
          Serial.println(sat_count);
          break;

        default:
          // Jika pesan yang diterima bukan pesan yang diharapkan, abaikan saja
          break;
      }
    }
  }
}
