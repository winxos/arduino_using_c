/*
 * ring_buffer.c
 *
 * Created: 2016-12-16 17:08:29
 *  Author: winxos
 */ 
#include "ring_buffer.h"

struct ring_buffer ring_buffer_out;
struct ring_buffer ring_buffer_in;

uint8_t out_buffer[BUFFER_SIZE]={0};
uint8_t in_buffer[BUFFER_SIZE]={0};
/**
* \brief Function to get the next offset in a ring buffer.
*
* \param cur_offset the current offset in the ring buffer
* \param size       the size of the ring buffer in bytes
*
* \returns next offset or 0 if we are wrapping
*/
uint8_t get_next(uint8_t cur_offset, uint8_t size)
{
	return (cur_offset == (size - 1) ? 0 : cur_offset + 1);
}

/**
* \brief Function to get the next write offset in a ring buffer.
*
* \param ring pointer to a struct of type ring_buffer
*
* \returns the next write offset in the ring buffer
*/
uint8_t ring_buffer_get_next_write(const struct ring_buffer *ring)
{
	return get_next(ring->write_offset, ring->size);
}

/**
* \brief Function to get the next read offset in a ring buffer.
*
* \param ring pointer to a struct of type ring_buffer
*
* \returns the next read offset in the ring buffer
*/
uint8_t ring_buffer_get_next_read(const struct ring_buffer *ring)
{
	return get_next(ring->read_offset, ring->size);
}

/**
* \brief Function for checking if the ring buffer is full
*
* \param ring pointer to a struct of type ring_buffer
*
* \retval true  if the buffer is full
* \retval false if there is space available in the ring buffer
*/
bool ring_buffer_is_full(const struct ring_buffer *ring)
{
	return (ring->read_offset == ring_buffer_get_next_write(ring));
}

/**
* \brief Function for checking if the ring buffer is empty
*
* \param ring pointer to a struct of type ring_buffer
*
* \retval true   if the buffer is empty
* \retval false  if there is still data in the buffer
*/
bool ring_buffer_is_empty(const struct ring_buffer *ring)
{
	return (ring->read_offset == ring->write_offset);
}

/**
* \brief Function for initializing a ring buffer
*
* \param buffer pointer to the buffer to use as a ring buffer
* \param size   the size of the ring buffer
*
* \retval struct ring_buffer a struct containing the ring buffer
*/
struct ring_buffer ring_buffer_init(uint8_t *buffer, uint8_t size)
{
	struct ring_buffer ring;
	ring.write_offset = 0;
	ring.read_offset = 0;
	ring.size = size;
	ring.buffer = buffer;
	return ring;
}

/**
* \brief Function for getting one byte from the ring buffer
*
* Call this function to get a byte of data from the ring buffer.
* Make sure buffer is not empty (using \ref ring_buffer_is_empty)
* before calling this function.
*
* \param ring pointer to a struct of type ring_buffer
*
* \returns next data byte in buffer
*/
uint8_t ring_buffer_get(struct ring_buffer *ring)
{
	Assert(!ring_buffer_is_empty(ring));
	uint8_t data = ring->buffer[ring->read_offset];
	ring->read_offset = ring_buffer_get_next_read(ring);
	return data;
}

/**
* \brief Function for putting a data byte in the ring buffer
*
* Call this function to put a byte of data in the ring buffer.
* Make sure buffer is not full (using \ref ring_buffer_is_full)
* before calling this function.
*
* \param ring pointer to a struct of type ring_buffer
* \param data the byte to put to the buffer
*
*/
void ring_buffer_put(struct ring_buffer *ring, uint8_t data)
{
	Assert(!ring_buffer_is_full(ring));
	ring->buffer[ring->write_offset] = data;
	ring->write_offset = ring_buffer_get_next_write(ring);
}