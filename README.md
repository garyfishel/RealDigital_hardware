# RealDigital_hardware
Programming modules for the RealDigital oscilloscope hardware

For Use with the Real Digital Blackboard Rev.D only

Verilog source files can be found under Sample_Signal_Generator.srcs

To run program, create a new platform project in Vitis 2020.2 using the design_1_wrapper.xsa platform. 
Build the platform

Create a new application project with the new platform and copy "Signal_Generator.h" and "Signal_Generator.c" into the src file. 
Connect a Real Digital Blackboard Rev.D to a usb port on your pc and configure jp3 to "USB" and jp2 to "JTAG".
Build the project then run. 

Using the built in Vitis Serial Terminal or CoolTerm (https://freeware.the-meiers.org/), connect to port COM4. 

Press btn0 on the Blackboard

To configure the signal generator, switches sw4-sw0 can be used. 

sw1-sw0: Signal Type
00 Square Wave
01 Sawtooth Wave
10 Triangular Wave
11: Sinusoidal Wave

sw3-sw2: Signal Period
00: Signal value changes once per FPGA clock cycle
01: Signal value changes once every other FPGA clock cycle
10: Signal value changes once every three FPGA clock cycles
11: Signal value changes once every four FPGA clock cycles

sw4: Reset Signal
Sets the signal output of the FPGA to 0 while sw4 is enabled
