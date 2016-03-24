/*
	Tsys01.h - library for using ELL-i's Tsys01 breakout.
	Created by Otso Jousimaa and Jaakko Laine Dec 2015.
	MIT licensed.
*/
#ifndef Tsys01_h
#define Tsys01_h

#include "Arduino.h"
#include <math.h> //for calculating powers
#include <SPI.h>

#define TSYS01_SPI 0
#define TSYS01_I2C 1

//Change this is you change I2C address through CS pin of board!
//You can use either read or write address, appropriate bit is set / unset by code
#define TSYS01_I2C_ADDR (0xEC >> 1) //arduino library uses 7-bit form of address
//Use Address EE if you pull down CS while using I2C.
//#define TSYS01_I2C_ADDR (EE >> 1)

#define TSYS01_RESET 0x1E
#define TSYS01_START 0x48
#define TSYS01_READ  0x00
#define TSYS01_ADDR0 0xA0
#define TSYS01_ADDR1 0xA2
#define TSYS01_ADDR2 0XA4
#define TSYS01_ADDR3 0XA6
#define TSYS01_ADDR4 0XA8
#define TSYS01_ADDR5 0xAA
#define TSYS01_ADDR6 0xAC
#define TSYS01_ADDR7 0xAE

#define TSYS01_K0ADR 0xAA
#define TSYS01_K1ADR 0xA8
#define TSYS01_K2ADR 0xA6
#define TSYS01_K3ADR 0xA4
#define TSYS01_K4ADR 0xA2

//Made by Apocalyt, precalculated constants of powers of 10
//This resolves loss of accuracy in calculation of the temperature values resulting from
const float TSYS_POW_A = 0.0000056234132519034908039495103977648123146825104309869166408/ 256.f; //10^(-21/4) / 256.f
const float TSYS_POW_B = 0.0000046415888336127788924100763509194465765513491250112436376/ 256.f; //10^(-16/3) / 256.f
const float TSYS_POW_C = 0.0000031622776601683793319988935444327185337195551393252168268/ 256.f; //10^(-11/2) / 256.f
const float TSYS_POW_D = 0.000001/ 256.f; // 10^(-6/1) / 256.f
const float TSYS_POW_E = 0.01; //10^(-2/1)

class Tsys01
{
  public: 
    //constructor
    Tsys01(uint8_t communicationProtocol, uint8_t powerPin );
    Tsys01(uint8_t communicationProtocol, uint8_t powerPin, uint8_t slaveSelect);
    //Use this constructor if software selection of protocol is required, e.g. factory testing.
    Tsys01(uint8_t communicationProtocol, uint8_t powerPin, uint8_t slaveSelect, uint8_t protocolPin);
    //This constructor is only for creating global variables.
    //A constructor with parameters should always be used to get a working Tsys01 object.
    Tsys01();
    
    void startAdc(void);
    float readTemperature(); //float?
    //comment about power savings, reboot times
    void powerOn(); //Remember to check isReady() after power on.
    void powerOff();

  private:
    uint16_t coefficients[5];
    uint8_t communicationProtocol;
    uint8_t powerPin; 
    uint8_t slaveSelectPin;
    uint8_t protocolPin;
            
    uint8_t* readBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);
    void writeBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);

    SPISettings spiSettingsObject = SPISettings(1000000UL, MSBFIRST, SPI_MODE0);
    
    // Read and write functions for I2C
    uint8_t* i2cReadBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);
    void i2cWriteBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);

    // Read and write functions for SPI
    void spiReadBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);
    void spiWriteBytes(uint8_t address, uint8_t bytes[], uint8_t numBytes);
    
    void readCalibration(void);
    void reset(void);
};

#endif

