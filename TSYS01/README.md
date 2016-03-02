# TSYS01 Temperature Sensor Board
Temperature sensor board built around TSYS01, complete with an Arduino driver.

This temperature sensor board provides very high accuracy(± 0.1°C) and resolution(± 0.01°C) temperature sensing using the TSYS01 digital single chip temperature sensor.
The board enables user selectable digital communication with the TSYS01 temperature sensor with either I2C or SPI on any 3.3V compatible microcontroller.
TSYS01 includes a 24 bit ADC at the temperature sensor, which enables very low noise conversion results when compared to most ADCs embedded in a microcontroller. 
The sensor board also provides limited overvoltage, reverse voltage and ESD protection for TSYS01 in all of the I/O pins.
This will make accidentally breaking the temperature sensor board unlikely in prototyping applications.

Every TSYS01 chip provides factory calibrated coefficients for converting the ADC values to temperature(°C).
The temperature sensor board provides the peripherals and connectors required for using the TSYS01 temperature sensor chip without the need for SMD soldering.
It also includes a dedicated linear regulator with low drop out voltage and good ripple rejection for providing accurate power to the temperature sensor.
This makes it much easier to actualise the promised accuracy of ± 0.1°C with this temperature sensor board than with any analog temperature sensor.

The TSYS01 has been designed for low power, low time constant and low self heating.
The regulator of the sensor board can also be turned off to reduce the idle power consumption of the board.
This makes the TSYS01 temperature sensor board a good option for obtaining accurate temperature measurements in applications where low power consumption is required. 

There is also an Arduino library available for this temperature sensor board enabling fast and easy deployment on the Arduino platform.
The library can be used to communicate with multiple temperature sensors using the I2C or SPI interface.
It also calculates temperature values using the calibration coefficients read from the TSYS01 and provides functions for turning on and off the linear regulator of the sensor board.

#User instructions

![Image of TSYS01 temperature board](/images/tsys01_top.jpeg)
![Image of TSYS01 temperature board](/images/tsys01_bottom.jpeg)


##Caution
Handle with care.

Do not submerge in water.

Do not use outside the specified temperature range of -40C to 125C.

Make sure to protect the device from water condensation when used in low temperature and high air moisture environments.

##Usage

##Hacking
TODO: remove resistor for lower power.

###Pin layout

###Voltages
20V abs max
recommended < 12V
>3V compatible

Recommended 3.3V or 5V

##Troubleshooting

##How to buy