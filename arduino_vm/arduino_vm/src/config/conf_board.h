/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define F_CPU 16000000UL

#define BAUD 115200
#define BAUD_TOL 2
#define UART0_DATA_EMPTY_IRQ USART_UDRE_vect
#define UART0_RX_IRQ USART_RX_vect

#endif // CONF_BOARD_H
