

#ifndef UART_H_
#define UART_H_



void Init_UART0(uint32_t baud_rate);

extern circularbuffer_t Txbuf, Rxbuf;

#endif /* UART_H_ */
