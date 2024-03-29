# RealDigital_hardware
Programming modules for the RealDigital oscilloscope hardware

For Use with the Real Digital Blackboard Rev.D only

Verilog source files can be found under Sample_Signal_Generator.srcs

To run program, create a new platform project in Vitis 2020.2 using the design_1_wrapper.xsa platform. 
Build the platform

There is a known issue with building the new platform in vitis 2020.2. To fix this issue, see the top response to the following forum post:
https://support.xilinx.com/s/question/0D52E00006hpOx5SAE/drivers-and-makefiles-problems-in-vitis-20202?language=en_US

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

Alpha Prototype Directions:
To connect the device with the SimpleScope web-based application, clone the application's repository and use CoolTerm to connect to the serial COM port. 
https://github.com/Chi-Cheng-Chuang/Project-Hawk/tree/gfishel

Under the Connection tab in CoolTerm, click options. Navigate to the File Capture settings and disable the "Leave File open while capturing" setting.
Close settings and click open the Connection tab again. Click on "Capture to Text/Binary File"  then "Start". Navigate to "data.csv" found in the web-based application repository.

Following the directions in the "README.md" file in the web-based application repository, enable the file deletion from root directory then run the server. 

To use the application correctly, click the "reset" button. Next, click btn0 on the RealDigital Blackboard. Finally, wait for LD13 to turn off and click "arm". 
The application should now display the sample data. 
Repeat these steps and adjust the switches on the Blackboard as desired. It is not recommended to adjust the slide switches while LD13 is on
