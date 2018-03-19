#include "xuart.h"
#include "../Inc/usart.h"

uint8_t xgetc(UART_HandleTypeDef *huart) {
	uint8_t c = 0;
	HAL_UART_Receive(huart, &c, 1, 0xf);
	return c;
}

void xputc(UART_HandleTypeDef *huart, uint8_t c) {
	HAL_UART_Transmit(huart, &c, 1, 0xffff);
}

void xputs(UART_HandleTypeDef *huart, char *str) {
	while (*str) {
		xputc(huart, *str++);
	}
}

char * uint_to_str(char *buf, unsigned src, int base) {
	char *p = buf;
	char *p1, *p2;

	do {
		*p++ = "0123456789ABCDEF"[src % base];
	} while (src /= base);

	// Terminate BUF
	*p = 0;

	// Reverse BUF
	for (p1 = buf, p2 = p - 1; p1 < p2; p1++, p2--) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
	}

	return buf;
}

void xprint(UART_HandleTypeDef *huart, char *str, ...) {
	char **arg = (char **) &str;
	char c;
	arg++;

	while ((c = *str++) != 0) {
		if (c != '%') {
			xputc(huart, c);
		} else {
			char buf[64];
			char *p;
			c = *str++;
			switch (c) {
			case 'd':
				if (0 > *((int *) arg)) {
					xputc(huart, '-');
					*((int *) arg) *= -1;
				}
				p = uint_to_str(buf, *((unsigned *) arg++), 10);
				xprint(huart, p);
				break;
			case 'x':
				xputs(huart, "0x");
				p = uint_to_str(buf, *((unsigned *) arg++), 16);
				xprint(huart, p);
				break;
			default:
				xputc(huart, c);
			}
		}
	}
}

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&PRINTF_USE_UART, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
