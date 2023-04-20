`timescale 1ns / 1ps

module Signal_Generator_Counter (
        input clk, reset,
        input [1:0] waveform_sel, period,
        output reg[11:0] sample_signal
    );
    
    reg [1:0] counter = 2'b00; //counter to track how many clock cycles have passed since last change of output signal
    reg phase = 0; //variable to track if triangle or sinewave signals are increasing or decreasing
   
    //initialize sine wave LUT
    parameter SIZE = 1024; //number of values in the lookup table   
    reg [11:0] rom_memory [SIZE-1:0]; //initialize a register that stores the current value from the sinusoid
    integer i; //counter to track line number from LUT read
    initial begin
        $readmemh("Sine.mem", rom_memory); //Open mem file with the signal values
        i = 0; //initialize line counter to start of the file
    end    
    
    always @(posedge(clk)) begin
        if (reset) //reset output to 0
            sample_signal <= 12'b000000000000;
        else
            if (counter == period) //check if enough clock cycles have passed to maintain signal period parameter
                begin
                    counter <= 2'b00; //reset counter
                    case (waveform_sel)
                        2'b00: //square wave
                            if (sample_signal != 12'd0 && sample_signal != 12'hFFF) //start square wave when waveform_sel is set to 00
                                sample_signal <= 12'd0; 
                            else
                                sample_signal <= ~sample_signal; //alternate signal between 000 and FFF to simulate square wave
                        2'b01: //sawtooth wave
                            sample_signal <= sample_signal + 1; //increment the sample_signal value by 1 each clock cycle. The value will reset to 0 with overflow
                        2'b10: //triangle wave
                            if (phase == 0) //phase 0 = increasing 
                                begin
                                    sample_signal <= sample_signal + 1; //increment sample_signal value by +1
                                    if (sample_signal == 12'hFFE) //check if max value has been met
                                        phase <= 1; //change phase to decreasing
                                end
                            else 
                                begin
                                    sample_signal <= sample_signal - 1; //increment sample_signal value by -1
                                    if (sample_signal == 12'h001) //check if min value has been met
                                        phase <= 0; //change phase to decreasing
                                end
                        2'b11: //sine wave
                            begin
                                sample_signal <= rom_memory[i]; //set sample_signal to read value from mem file
                                i <= i + 1; //increase line counter to next line
                                if(i == SIZE)
                                    i <= 0; //reset to start of file when end is reached
                            end
                        default: //reset
                            sample_signal <= 12'd0;
                     endcase;
                 end
             else
                counter <= counter + 1;
     end
endmodule
    