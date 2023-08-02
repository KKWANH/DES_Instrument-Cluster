#include <SPI.h>
#include <mcp2515.h>
#define slaveID 0x010

MCP2515 mcp2515(10);

struct can_frame canMsg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    if(slaveID != canMsg.can_id) return;
    int distance = canMsg.data[0];
    Serial.print("Distance: ");
    Serial.println(distance);

    delay(500);
  }
}
