#include <Wire.h>

#define __I2C_REPEATER_I2C_ADDR 0b1100000
#define __IO_EXPANDER_0_I2C_ADDR 0b0100000
#define __IO_EXPANDER_1_I2C_ADDR 0b0100001
#define __LTC4302_CMD_CONNECT_CARD 0b11100000

inline void clearSerBuf(){
  while(Serial.available()>0){
    Serial.read();
  }
}

inline void waitForSerial(){
   while(Serial.available() < 1);
}

const char* menuOptions[] = {
  ". Set individual pin\n",
  ". Set pins by decimal value\n"
};
const size_t menuLength = 2;
uint32_t expander_val = 0;

int connect_i2c_bus(uint8_t address)
{
  Wire.beginTransmission(__I2C_REPEATER_I2C_ADDR + address);
  Wire.write(__LTC4302_CMD_CONNECT_CARD); // Connect IIC Bus
  return Wire.endTransmission();
}

int setLSBExpander(uint16_t pinsetting)
{
  Wire.beginTransmission(__IO_EXPANDER_0_I2C_ADDR);
  Wire.write((uint8_t)(pinsetting & 0xFF));
  Wire.write((uint8_t)((pinsetting >> 8) & 0xFF));
  return Wire.endTransmission();
}
int setMSBExpander(uint16_t pinsetting)
{
  Wire.beginTransmission(__IO_EXPANDER_1_I2C_ADDR);
  Wire.write((uint8_t)(pinsetting & 0xFF));
  Wire.write((uint8_t)((pinsetting >> 8) & 0xFF));
  return Wire.endTransmission();
}

int set_expander(uint32_t setting){
  
  return 0;  
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  Serial.println("Attempting to connect LTC4302...");
  if (connect_i2c_bus(1) == 0){
    Serial.println("Success");
  } else {
    while (1==1){
      Serial.println("Failed to communicate/command I2C Repeater chip LTC4302. Please fix & restart arduino.");
      delay(500);
    }
  }
  // put your setup code here, to run once:

}

void loop() {
  // Print out the control menu
  int option;
  int value1, value2;
  Serial.println("CUSTOM POT TEST CODE\n");
  for (size_t i = 0; i < menuLength; i++){
    Serial.print(i);
    Serial.print(menuOptions[i]);
  }
  Serial.println("Option >>> ");
  waitForSerial();
  option = Serial.parseInt();
  clearSerBuf();

  switch(option){
    case 0:
      Serial.println("Pin Number? >>>");
      waitForSerial();
      value1 = Serial.parseInt();
      clearSerBuf();
      Serial.println("Pin State? >>>");

      waitForSerial();
      value2 = Serial.parseInt();
      clearSerBuf();
      
      break;
  }






  
  
}
