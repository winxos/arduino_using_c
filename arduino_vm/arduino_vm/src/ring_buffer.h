/*
* ring_buffer.h
*
* Created: 2016-12-6 21:55:14
*  Author: winxos
*/

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "public.h"

struct ring_buffer {
	volatile uint8_t write_offset;
	volatile uint8_t read_offset;
	uint8_t size;
	uint8_t *buffer;
};
#define BUFFER_SIZE 100

extern uint8_t out_buffer[BUFFER_SIZE];
extern uint8_t in_buffer[BUFFER_SIZE];

uint8_t get_next(uint8_t cur_offset, uint8_t size);
uint8_t ring_buffer_get_next_write(const struct ring_buffer *ring);
uint8_t ring_buffer_get_next_read(const struct ring_buffer *ring);
bool ring_buffer_is_full(const struct ring_buffer *ring);
bool ring_buffer_is_empty(const struct ring_buffer *ring);
struct ring_buffer ring_buffer_init(uint8_t *buffer, uint8_t size);
uint8_t ring_buffer_get(struct ring_buffer *ring);
void ring_buffer_put(struct ring_buffer *ring, uint8_t data);

#endif /* RING_BUFFER_H_ */

