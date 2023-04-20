`timescale 1ns / 1ps

module Signal_Generator_Wrapper(
        input clk100_in,
        input [4:0] sws_12bits_tri_i_0,
        output [11:0] sample_signal
    );
    
    Signal_Generator_Counter gen( //generate the signal output based on parameters of waveform and period
        .sample_signal(sample_signal),
        .clk(clk100_in),
        .waveform_sel(sws_12bits_tri_i_0[1:0]),
        .period(sws_12bits_tri_i_0[3:2]),
        .reset(sws_12bits_tri_i_0[4])
    );
    
    
    
endmodule
