# Measuring Air Flow and Volume with Sensirion i2c SFM3300 - examples for Arduino (C, C++)

Arduino sketches in this project show application of Sensirion SFM3000 Series Mass Flow Meters for measuring the air flow and air volume.

<img src="assets/SFM3300-new.jpg" width="800"/>

## The race for ventilators
Current coronavirus outbreak may lead to a dear shortage of many medical supplies quickly. Mechanical lung ventilators are in high demand already as of the end of March 2020. There are several teams of enthusiasts and volounteers across the Globe who are trying to create mechanical ventilators of different levels of sophistication. In an attempt to help to the "AquaLung" ventilator project one SFM3300 was purchased (while it was still widely available at distributors). The "AquaLung" project will most probably be using different sensors in an attempt to not drain the supply chain and not interfere with professional medical equipment manufacturers.
Still, if this little code may help some other team out there to advance quicker and save lives - that was totally worth doing it!

## What you'll find here
The code was written with the following goals in mind:
* Show how simple it is to communicate with the sensor via i2c
* Provide easy to use snippets inside the working code
  * CRC calculation
  * Air Volume calculation
  * "Soft Timer Interrupts" technique with 1ms resolution
* Validate precision of Air Volume measurements achievable with SFM3300
  * Preliminary results appear to be very good (sub-cc resolution, good repeatability)
  * Volume measuremets with repeatable presision - work in progress
  * Once validated - this project can be used for testing (cross-checking) other Air Flow and Volume sensors implementations

The code (or at least the snippets) is very compact and well commented. It can be adapted easily to any other C/C++ programming environment, or serve as examples for implementations in other programming languages.

## Connecting to the sensor
While the sensor manufacturer may be able to provide specialized connection caps for the sensors, in this one-off project simple pogo-pins and maketing PCB were used to mock-up a detacheable connector.

<img src="assets/Connector-pogo-pins.jpg" width="800"/>

<img src="assets/SFM3300-DIY-assembly.jpg" width="800"/>

## Compatibility
The sensor requires 5V power supply and 5V-compatible levels and pull-ups on i2c. As is it will work fine with any AVR based 5V Arduino board. The 5V i2c can be adapted to a lower voltage controller (e.g.: 3.3V - RPi, Arduino Due, ESP-8266, etc.) using a simple level converter with two low power MOSFET transistors, or an integrated i2c level converter or isolator.

The project was tested with Arduino NANO.

## How to Run

`git clone https://github.com/MyElectrons/sfm3300-arduino.git`

Run Arduino environment, load a sample sketch, compile and upload it

Open Serial Monitor: `Arduino --> Tools --> Serial Monitor`, set it to 115200 baud
