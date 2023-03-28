`timescale 1ns / 1ps

module Signal_Generator_Wrapper_sim( );
    reg clk = 0;
    reg [4:0] sw;
    wire [11:0] signal;
    
    Signal_Generator_Wrapper test(
        .clk100_in(clk),
        .sws_12bits_tri_i(sw),
        .signal(signal)
    );
    
    //sw[1:0] waveform_sel
    //sw[3:2] period
    //sw[4] reset
    
    always
        #10 clk = !clk;
    
    initial
        begin
            sw = 5'b10000;
            #200;
            sw = 5'b00000; 
            #1000;
            sw = 5'b00001;
            #100000;
            sw = 5'b00010;
            #100000;
            sw = 5'b00011;
            #100000;
        end


endmodule
