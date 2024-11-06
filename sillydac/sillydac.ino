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
  setLSBExpander(setting & 0xffff);
  setMSBExpander((setting >> 16) & 0xffff);
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
  set_expander(expander_val);

}

void loop() {
  // Print out the control menu
  int option;
  int value1, value2;
  uint32_t confused = expander_val;
  Serial.println("CUSTOM POT TEST CODE\n");
  for (size_t i = 0; i < menuLength; i++){
    Serial.print(i);
    Serial.print(menuOptions[i]);
  }
  Serial.print("Option >>> ");
  waitForSerial();
  option = Serial.parseInt();
  clearSerBuf();
  Serial.println(String(option));

  switch(option){
    case 0: // Set individual pin
      Serial.print("Pin Number? >>> ");
      waitForSerial();
      value1 = Serial.parseInt();
      clearSerBuf();
      Serial.println(String(value1));
      
      Serial.print("Pin State? >>> ");
      waitForSerial();
      value2 = Serial.parseInt();
      clearSerBuf();
      Serial.println(String(value2) + "\n\n");
      

      if (value2){
        confused = confused | (1 << value1);
      } else {
        confused = confused & (~(1 << value1));
      }
      set_expander(confused);
      Serial.println("DEBUG: " + String(confused, BIN));
      expander_val = confused;

      break;
    case 1:
      Serial.print("Decimal Value? >>> ");
      waitForSerial();
      value1 = Serial.parseInt();
      clearSerBuf();
      set_expander(value1);
      expander_val = value1;
      Serial.println(String(expander_val) + "\n\n");

      break;
    default:
      return;
  }






  
  
}
