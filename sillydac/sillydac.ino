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

int setLSBExpander(uint32_t pinsetting)
{
  //cut to lower bits
  pinsetting = pinsetting & 0x3FF;
  Wire.beginTransmission(__IO_EXPANDER_0_I2C_ADDR);
  Wire.write((uint8_t)(pinsetting & 0xFF));
  Wire.write((uint8_t)((pinsetting >> 8)));
  return Wire.endTransmission();
}
int setMSBExpander(uint32_t pinsetting)
{
  //bring the higher level bits down to base.
  pinsetting = pinsetting >> 10;
  Wire.beginTransmission(__IO_EXPANDER_1_I2C_ADDR);
  Wire.write((uint8_t)(pinsetting & 0xFF));
  Wire.write((uint8_t)((pinsetting >> 8)));
  return Wire.endTransmission();
}

int set_expander(uint32_t setting){
  setLSBExpander(setting);
  setMSBExpander(setting);
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
  long int option;
  long int value1, value2;

  // Serial.println(sizeof(int));
  // Serial.println(sizeof(long int));
  // uint32_t confused = expander_val;
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

      if (value1 > 0x3ff){}
      
      if (value2){
        expander_val = expander_val | (1 << value1);
      } else {
        expander_val = expander_val & (~(1 << value1));
      }
      set_expander(expander_val);
      break;
    case 1:
      Serial.print("Decimal Value? >>> ");
      waitForSerial();
      value1 = Serial.parseInt();
      clearSerBuf();
      expander_val = value1;
      set_expander(expander_val);
      Serial.println(String(value1) + "\n\n");


      break;
    default:
      return;
  }






  
  
}
