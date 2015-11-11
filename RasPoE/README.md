#RasPoE user instructions
##Caution

Always disconnect cable between RasPoE and Rasberry Pi before connecting or disconnecting cable between RasPoE and PoE switch to avoid damage to your Raspberry Pi!

##Usage
Setting up RasPoE is simple: you need a Power over Ethernet (PoE) switch or injector to provide power to your Raspberry Pi. Mount RasPoE on top of your Raspberry Pi, connect RasPoE to PoE switch and finally connect RasPoE to Raspberry Pi. No special software is needed, start-up is automatic.

If you disconnect the RasPoE, always disconnect the cable to Raspberry Pi first! The PoE voltages can jump over transformer when cable is disconnected, disconnecting cable to Raspberry Pi prevents damage.

RasPoE uses only 5V and GND pins and it has tall header, which means you can stack Raspberry Pi HAT on top of it.
Specification

RasPoE is based on AG9805-M PoE DC/DC module which automatically negotiates the PoE power and provides 5V DC for Raspberry Pi.

While AG9805M can provide 1.8 A (9W) of power, current to Raspberry Pi is limited by 1.25 A self-resettible fuse. Please note the total current if you are using power-hungry USB peripherals

##Open Hardware
RasPoE is open source hardware, you can download the design files at ELL-i Open Source Co-operative's github-repository here. You can find out more about ELL-i at [ELL-i's website](http://ell-i.org).

##Hacking
RasPoE is made for hackers and makers. We cannot guess every possible application where RasPoE will be used so we have made things simple for some modifications.

###Raw PoE power
If your project needs unregulated high voltage, you can tap into RasPoE's "Raw PoE" pads. These pads expose the PoE voltage for your convenience, you can solder wires to them. Please note that any load on these pads will disrupt the PoE negotiation, so you must use relay controlled by Raspberry Pi to connect high voltage load only after the power has been setup. Do not add a lot of capacitance on these pins either to avoid inrush current tripping your PoE switch overcurrent protection.

###DC output pins
If you need to power some small external load, such as Arduino, you can add connector to the "DC OUT" pins. This output is not protected by fuse, so please be careful to not overload the AG9805 to avoid damage to your RasPoE.
Adjust

If you need to tune the output voltage a bit, you can solder a resistor to ADJUST pin as instructed in [AG9805-M datasheet](http://www.semiconductorstore.com/pages/asp/DownloadAttachment.asp?d=83464&e=i). Please be careful to not exceed your Raspberry Pi's maximum voltage.

###Other voltages

AG9803 and AG98012 modules provide 3.3 V and 12 V outputs respectively. You can desolder the AG9805 using hot air, disconnect the fuse F1 and solder a replacement module in if you want to! This is perfect for Raspberry Pi HATs which require 12V and provide 5V to RaspBerry Pi.

##Troubleshooting

If your Raspberry Pi does not power up or network connection fails, please check:

    Are you using a CAT5 or better Ethernet cable?
    Is the RasPoE properly connected?
    Does your switch support PoE technology?

If the problem remains, please contact us at support@ell-i.org and we'll help you with the issue. 

RasPoE emits high-pitched noise when there is  low load, this is normal. You can add optional load resistors R6, R7 to avoid this noise.

##How to buy
RasPoE is available through [Elecrow's webshop](http://www.elecrow.com/raspoeraspberry-pi-poe-shield-p-1479.html). 
