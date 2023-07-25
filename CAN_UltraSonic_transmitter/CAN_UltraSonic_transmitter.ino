#include <mcp2515.h>
#include <SPI.h>

#define TRIG_PIN 4
#define ECHO_PIN 3

MCP2515 mcp2515(10); //set the pin number where SPI CS is connected.

struct can_frame canMsg;

uint8_t detectUltraSonic() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  uint8_t distance_cm = duration * 0.0343 / 2;
  Serial.print("Distance :");
  Serial.println(distance_cm); 
  
  return distance_cm;
}


void setup() {
  canMsg.can_id = 0x010;
  canMsg.can_dlc = 2;
  canMsg.data[0] = 0xff;
  canMsg.data[1] = 0xff;

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT); //Ultra sonic
  pinMode(ECHO_PIN, INPUT); //Ultra sonic

  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  uint8_t distance = detectUltraSonic();
  canMsg.data[0] = distance;
  mcp2515.sendMessage(&canMsg);
  delay(500);
}