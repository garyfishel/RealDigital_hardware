#include "SampleBuffer.h"


//initialize the buffer array with size size
void cBufferInit(struct CircularBuffer* cBuffer, int size) {
	cBuffer->buffer = (uint32_t*) malloc(size*sizeof(uint32_t));
	cBuffer->size = (uint32_t) size;
	cBuffer->head = 0;
	cBuffer->tail = 0;
}

void addToBuffer(struct CircularBuffer* cBuffer, uint32_t data) {
	cBuffer->buffer[cBuffer->head] = data;
	cBuffer->tail = cBuffer->head;
	(cBuffer->head)++;
	if (cBuffer->head>=cBuffer->size)
		cBuffer->head = 0; //reset to start of array if end is met
}

uint32_t getValueAtIndex(struct CircularBuffer* cBuffer, int index) {
	return cBuffer->buffer[(cBuffer->head + index) % cBuffer->size];
}
