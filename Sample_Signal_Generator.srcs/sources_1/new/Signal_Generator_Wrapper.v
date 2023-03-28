`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/25/2023 05:44:21 PM
// Design Name: 
// Module Name: Signal_Generator_Wrapper
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Signal_Generator_Wrapper(
        input clk100_in,
        input [4:0] sws_12bits_tri_i,
        output [11:0] signal
    );
    
    Signal_Generator_Counter gen( //generate the signal output based on parameters of waveform and period
        .signal(signal),
        .clk(clk100_in),
        .waveform_sel(sws_12bits_tri_i[1:0]),
        .period(sws_12bits_tri_i[3:2]),
        .reset(sws_12bits_tri_i[4])
    );
    
    
endmodule
