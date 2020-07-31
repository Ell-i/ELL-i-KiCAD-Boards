#include "Tsys01.h"
#include <Wire.h>
#include <SPI.h>
#define TSYS_DEBUG 0


// Use this constructor only for creating global variables
Tsys01::Tsys01() {
}

Tsys01::Tsys01(uint8_t _communicationProtocol, uint8_t _powerPin)
{
  this->powerPin = _powerPin;
  #if TSYS_DEBUG
    Serial.println("TSYS01: Setting communication protocol");
  #endif  
    this->communicationProtocol = _communicationProtocol;
    #if TSYS_DEBUG
    Serial.println("TSYS01: Resetting sensor");
  #endif
  this->reset();
  #if TSYS_DEBUG
    Serial.println("TSYS01: Starting to read calibration");
  #endif
  delay(4);
    this->readCalibration();
  #if TSYS_DEBUG
    Serial.println("TSYS01: Creating library object done");
  #endif
}

Tsys01::Tsys01(uint8_t _communicationProtocol, uint8_t _powerPin, uint8_t _slaveSelect)
{
  this->powerPin = _powerPin;
  #if TSYS_DEBUG
    Serial.println("TSYS01: Setting communication protocol");
  #endif
  this->slaveSelectPin = _slaveSelect;
  this->communicationProtocol = _communicationProtocol;
  #if TSYS_DEBUG
    Serial.println("TSYS01: Resetting sensor");
  #endif
  pinMode(_slaveSelect, OUTPUT);
  digitalWrite(_slaveSelect, HIGH);
  this->reset();
  #if TSYS_DEBUG
    Serial.println("TSYS01: Starting to read calibration");
  #endif
  delay(4);
  this->readCalibration();
}


Tsys01::Tsys01(uint8_t _communicationProtocol, uint8_t _powerPin, uint8_t _slaveSelect, uint8_t _protocolPin)
{
  this->powerPin = _powerPin;
  this->slaveSelectPin = _slaveSelect;
  this->communicationProtocol = _communicationProtocol;
  this->protocolPin = _protocolPin;

  pinMode(_slaveSelect, OUTPUT);
  digitalWrite(_slaveSelect, HIGH);
  
  if(this->communicationProtocol == TSYS01_SPI)
  {
     digitalWrite(this->protocolPin, LOW);
     pinMode(this->protocolPin, OUTPUT);
  }
  if(this->communicationProtocol == TSYS01_I2C)
  {
     digitalWrite(this->protocolPin, LOW);
     pinMode(this->protocolPin, INPUT);
  }
  
  this->reset();
  delay(4);
  this->readCalibration();
}

void Tsys01::readCalibration(void)
{

  uint8_t coefficient[2];
  //K0, K1, K2, K3, K4
  uint8_t coefficientADDR[5] = {  TSYS01_K0ADR,
                                  TSYS01_K1ADR,
                                  TSYS01_K2ADR,
                                  TSYS01_K3ADR,
                                  TSYS01_K4ADR};
  
  for(int iii = 0; iii < 5; iii++)
  {
    uint8_t addr = coefficientADDR[iii];
    uint8_t coefficient[2];
    this->readBytes(addr, coefficient, 2);
    this->coefficients[iii] = (uint16_t)((uint16_t)coefficient[0] << 8) + (uint16_t)coefficient[1];
    #if TSYS_DEBUG
      Serial.print(iii);
      Serial.print(". coefficient: ");
      Serial.println(this->coefficients[iii]);
    #endif
  }
}

void Tsys01::reset()
{
  uint8_t payload[0];
  this->writeBytes(TSYS01_RESET, payload, 0);
}

void Tsys01::powerOn()
{
  pinMode(this->powerPin, INPUT);
  this->reset();
}

void Tsys01::powerOff()
{
  digitalWrite(this->powerPin, LOW);
  pinMode(this->powerPin, OUTPUT);
}

void Tsys01::writeBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{
  if(this->communicationProtocol == TSYS01_SPI)
    return this->spiWriteBytes(address, bytes, numBytes);
  else 
    return this->i2cWriteBytes(address, bytes, numBytes);
}

uint8_t* Tsys01::readBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{
  if(this->communicationProtocol == TSYS01_SPI)
    this->spiReadBytes(address, bytes, numBytes);
  else 
    this->i2cReadBytes(address, bytes, numBytes);
}

uint8_t* Tsys01::i2cReadBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{
  //Write Address to read from
  Wire.begin();
  Wire.beginTransmission(TSYS01_I2C_ADDR);
  Wire.write(address);
  Wire.endTransmission();
  Wire.end();

  Wire.begin();
  Wire.requestFrom((uint8_t)TSYS01_I2C_ADDR, numBytes);
  for(int iii = 0; iii < numBytes && Wire.available(); iii++)
  {
    bytes[iii] = Wire.read();
  }
  Wire.end();
  return bytes;

}

void Tsys01::i2cWriteBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{

  Wire.begin();
  Wire.beginTransmission(TSYS01_I2C_ADDR); 
  Wire.write(address);
  
  for(int iii = 0; iii < numBytes; iii++)
  {
    Wire.write(bytes[iii]);
  }

  Wire.endTransmission();
  Wire.end();
}

void Tsys01::spiReadBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{
  //select sensor
  #if TSYS_DEBUG
    Serial.println("SPI read bytes");
  #endif
  
  digitalWrite(this->slaveSelectPin, LOW);

  SPI.begin();
  SPI.beginTransaction(this->spiSettingsObject);
  SPI.transfer(address);
  // Read response
  for(int iii = 0; iii < numBytes; iii++) {
    bytes[iii] = SPI.transfer(0x00);
  }
  SPI.endTransaction();
  SPI.end();
  
  //Deselect sensor
  digitalWrite(this->slaveSelectPin, HIGH);  

}

void Tsys01::spiWriteBytes(uint8_t address, uint8_t* bytes, uint8_t numBytes)
{
  //select sensor
  digitalWrite(this->slaveSelectPin, LOW);
  
  SPI.begin();
  SPI.beginTransaction(this->spiSettingsObject);
  SPI.transfer(address);

  for(int iii = 0; iii < numBytes; iii++)
  {
    SPI.transfer(bytes[iii]);
  }

  SPI.endTransaction();
  SPI.end();
  
  //deselect sensor
  digitalWrite(this->slaveSelectPin, HIGH);
}

float Tsys01::readTemperature(void)
{
  //ADC must have been started first and
  //10ms must have passed from ADC start
  //to get a good value.
  uint8_t rawAdc[3] = {};
  this->readBytes(TSYS01_READ, rawAdc, 3); 
  
  uint32_t ADC24 = ((uint32_t)rawAdc[0]<<16) + ((uint32_t)rawAdc[1]<<8) + (uint32_t)rawAdc[2];

  #if TSYS_DEBUG
    Serial.print("rawAdc: ");
    Serial.println((long unsigned int) (((long unsigned int)rawAdc[0] << 16) + ((long unsigned int)rawAdc[1] << 8) + (long unsigned int)rawAdc[2]));
    Serial.print("ADC24: ");
    Serial.println(ADC24);
  #endif

  // calculation of the terms, one by one to avoid accumulating error
  float term1 = (-2.0) * this->coefficients[4] * pow(TSYS_POW_A * ADC24, 4);
  float term2 = 4.0    * this->coefficients[3] * pow(TSYS_POW_B * ADC24, 3);
  float term3 = (-2.0) * this->coefficients[2] * pow(TSYS_POW_C * ADC24, 2);
  float term4 =          this->coefficients[1] * TSYS_POW_D * ADC24;
  float term5 = (-1.5) * this->coefficients[0] * TSYS_POW_E;

  //final temperature
  float temperature = term1 + term2 + term3 + term4 + term5;

  #if TSYS_DEBUG
    Serial.print("temp24: ");
    Serial.println(temperature,6);
  #endif

  return temperature;
}

void Tsys01::startAdc(void)
{
  uint8_t temp[0] = {};
  this->writeBytes(TSYS01_START, temp, 0); //Send start only
}
