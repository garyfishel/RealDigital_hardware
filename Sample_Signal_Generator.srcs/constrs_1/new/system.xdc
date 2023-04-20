# clk100_in is from the 100 MHz oscillator on Blackboard
# create_clock -period 10.000 -name gclk [get_ports clk100_in]
set_property -dict {PACKAGE_PIN H16 IOSTANDARD LVCMOS33} [get_ports clk100_in]

# switches sw4-sw0 on Blackboard
set_property PACKAGE_PIN R17 [get_ports sws_12bits_tri_i[0]]
set_property PACKAGE_PIN U20 [get_ports sws_12bits_tri_i[1]]
set_property PACKAGE_PIN R16 [get_ports sws_12bits_tri_i[2]]
set_property PACKAGE_PIN N16 [get_ports sws_12bits_tri_i[3]]
set_property PACKAGE_PIN R14 [get_ports sws_12bits_tri_i[4]]
set_property IOSTANDARD LVCMOS33 [get_ports sws_12bits_tri_i[*]]

# Buttons
set_property -dict {PACKAGE_PIN W14 IOSTANDARD LVCMOS33} [get_ports btn_0]

