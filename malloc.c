#define NULL 0

/** Malloc code: An attempt at a memory management system.
 * There should never be more than 1 consecutive free block.
 * The algorithm is first fit.
 */

//extern char *heap;
char heap_mem[0x80000]; //this gets allocated in .bss
void *heap = heap_mem;
#define HEAP_SIZE 0x80000 //8mb, defined in startup.S

#define MIN_ALLOC 32 //32 bytes... 24 + control block
#define ALLOC_INCR 32 //alloc in 32 byte blocks. Makes management easier.

#define MEM_INUSE 0x1
typedef struct memblock {
	unsigned long flags;
	unsigned long size; //block + mem
} memblock;

void *heap_end;

void *malloc(unsigned long size) {
	memblock *mem = (memblock *)heap;
	void *ret = NULL;
	unsigned long oldsize;
	size += sizeof(struct memblock);
	if (size < MIN_ALLOC)
		size = MIN_ALLOC;
	if (size % ALLOC_INCR != 0)
		size += ALLOC_INCR - (size % ALLOC_INCR); //round up

	while (mem < (memblock *)heap_end) {
		if (!mem->flags & MEM_INUSE) {
			if (mem->size >= size) {
				oldsize = mem->size;
				mem->flags |= MEM_INUSE;
				mem->size = size;
				ret = mem + sizeof(struct memblock);
				if (oldsize == size)
					break;
				//there's space left, so block it.
				mem += size;
				mem->size = oldsize - size;
				mem->flags  &= ~MEM_INUSE; //just in case
				break;
			}
		}
		//mem + mem->size should never be beyond heap_end
		if ((void *)(mem + mem->size) >= heap_end) {
			//won't fit.
			return NULL;
		}
		mem += mem->size;
	}
	return ret;
}

void free(void *alloced) {
	memblock *mem = (memblock *)alloced - sizeof(struct memblock),
		*ptr = (memblock *)heap;
	mem->flags &= ~MEM_INUSE;
	while (ptr + ptr->size != mem) ptr += ptr->size;
	if (!ptr->flags & MEM_INUSE) {
		ptr->size += mem->size;
		mem = ptr;
	}
	//is the next one free as well?
	ptr += ptr->size;
	if (!ptr->flags & MEM_INUSE)
		mem->size += ptr->size;
	//there should be no reason to loop this. It could be done, though.
	return;
}

void malloc_init() {
	heap_end = heap + HEAP_SIZE;
	memblock *mem = heap;
	mem->size = 128;
	mem->flags = 0;
	return;
}

