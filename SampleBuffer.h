struct CircularBuffer {
	uint32_t* buffer;
	int size; //number of values that can be stored to the buffer array
	int head; //index of the oldest value in the array
	int tail; //index of the newest value in the array
	int isFull; //indicator if the buffer array is full or not
};

/* allocate an array of size 'size' into memory
 * set the size of the buffer
 * set head to the first index in the array
 * set tail to the first index in the array
 * set isFull to 0 (false)
 */
void cBufferInit(struct CircularBuffer* cBuffer, int size);

/* Write data to the head of the buffer
 * set tail to head, and increment head by 1
 */
void addToBuffer(struct CircularBuffer* cBuffer, uint32_t data);

//returns the value of the array stored at 'index' indices after the head of the buffer
uint32_t getValueAtIndex(struct CircularBuffer* cBuffer, int index);
