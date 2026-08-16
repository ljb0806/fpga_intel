
module nios (
	clk_clk,
	led_export,
	reset_reset_n,
	uart_rxd,
	uart_txd);	

	input		clk_clk;
	output	[7:0]	led_export;
	input		reset_reset_n;
	input		uart_rxd;
	output		uart_txd;
endmodule
