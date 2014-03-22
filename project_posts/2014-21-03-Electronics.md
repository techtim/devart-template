## Electroinics

There was a need in fully contollable light source. Having experience with addressable LED modules like ShiftBrite, was choosen to use this technology for preciese control of light.

![LPD8806 digital LED strip](../project_images/LPD8806digitalledstrip.jpg "LPD8806 digital LED strip")

After hard Googling, on Adafruit was found LED strip (https://www.adafruit.com/products/306), that has addressable contol over each led, with 7bit PWM and fast enought. It consists from SMD 5050 LED and LPD8806 IC. I had experience with previous model with HL1606 chip, but it was too slow and had poor pwm capabilities.
Ordered 5 meters for tests. After tests with Arduino and LPD8806 strip, discovered that it can't be controlled with sended from PC bitmaps, because it doesn't have enought memory to srore frame bigger then 150 pixels. Searching for solution continues and I found PixelPi software (https://github.com/scottjgibson/PixelPi) for Raspberry Pi, and it worked.