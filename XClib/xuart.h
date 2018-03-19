#ifndef XUART_H_
#define XUART_H_

#include "../Inc/usart.h"
#include "stdint.h"

/** Function to receive 1 byte
 * @param UART handle Structure definition
 * @retval receive data 1byte
 */
extern uint8_t xgetc (UART_HandleTypeDef *huart);

/**
 * @brief Function to send 1 byte
 * @param *huart	UART handle Structure definition
 * @param c			send data 1byte
 */
extern void xputc (UART_HandleTypeDef *huart, uint8_t c);

/**
 * @brief Function to send character string
 * @param *huart	UART handle Structure definition
 * @param *str		String to send
 */
extern void xputs (UART_HandleTypeDef *huart, char *str);

/**
 * @brief
 * @param *huart	UART handle Structure definition
 * @param *str		printf wwwww
 */
extern void xprint (UART_HandleTypeDef *huart, char *str, ...);

/*** integer ***/
#define gan(X) ((int32_t)X)
/*** Decimal ***/
#define dec(X, DIGIT) ((X < 0) ? ((int)((X-(int)X)*-DIGIT)) : ((int)((X-(int)X)*DIGIT)))

/** Declaration for using printf
 * printf www
 * setbuf(stdout, NULL);//non buff ring
 * Defined to use float
 * uC/C++ Buildv->[Settings]->[Tool Settings]->[MCU GCC Linker]->[Miscellaneous]
 * -u _printf_float
 * */
/*** defined to huartx ***/
#define PRINTF_USE_UART huart4
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* XUART_H_ */
