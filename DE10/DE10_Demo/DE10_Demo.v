module DE10_Demo
(
	 input clk,
	 input rst_n,
	 
	 input RX,
	 output TX,
	 
	 output [7:0] LED
);

nios u0 (
		.clk_clk       (clk),       //   clk.clk
		.reset_reset_n (rst_n), // reset.reset_n
		.uart_rxd      (RX),      //  uart.rxd
		.uart_txd      (TX),      //      .txd
		.led_export    (LED)     //   led.export
	);

endmodule