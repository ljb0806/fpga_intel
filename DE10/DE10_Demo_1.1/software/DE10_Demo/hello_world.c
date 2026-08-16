#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>
#include "altera_avalon_uart_regs.h"

void UART_ISR_Init(void);    //³õÊ¼»¯´®¿Ú
void UART_Irq_Handler(void);
void UART_Transmit_Byte(char data);
void UART_Transmit_String(char *data);
alt_u8 rx_data = 0;
alt_u8 rx_flag = 0;

int main()
{
  UART_ISR_Init();

  while(1)
  {
	  UART_Transmit_String("Hajimi.\n");
	  if(rx_flag == 1)
	  {
		  printf("Got Data\n");
		  rx_flag = 0;
	  }
	  usleep(1000000);
  }
  return 0;
}

void UART_ISR_Init(void)
{
	IOWR_ALTERA_AVALON_UART_STATUS(UART_BASE, 0x0);
	alt_irq_register(UART_IRQ, NULL, UART_Irq_Handler);
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_BASE, 0x0080);
}

void UART_Irq_Handler(void)
{
	rx_flag = 1;
	rx_data = IORD_ALTERA_AVALON_UART_RXDATA(UART_BASE);
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_BASE, rx_data);
	IOWR_ALTERA_AVALON_UART_STATUS(UART_BASE, 0x0);
}

void UART_Transmit_Byte(char data)
{
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_BASE, data);
}

void UART_Transmit_String(char *data)
{
	alt_u8 length = strlen(data);
	alt_u8 i = 0;
	while(i < length)
	{
		while ((IORD_ALTERA_AVALON_UART_STATUS(UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK) == 0);
		IOWR_ALTERA_AVALON_UART_TXDATA(UART_BASE, data[i]);
		i = i + 1;

	}
}
