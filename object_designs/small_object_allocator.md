

* If we want polymorphic behavior, we have to store small objects on heap
* If we just use `new` and `delete`, its performance is bad
  * Traditionally, we have bookkeeping memory for extra bytes (4-32), which would be a great overhead for small objects
    * Something look like this
    ```C++
    struct MemControlBlock 
    { 
        bool available_; 
        MemControlBlock* prev_; 
        MemControlBlock* next_;
    }; 
    ``` 
  * Also, the implemented algorithm for finding an okay position for memory is not optimized for small objects. 


Now, we try for a small-object allocator. We have four layers.

# Summary

## Chunk
Each object of type Chunk contains and manages a chunk of memory consisting of an integral number of fixed size blocks. Chunk contains logic that allows you to allocate and deallocate memory blocks.
* A chunk = n blocks, with size predefined


## Fixed Allocators
It has an array of Chunks. Create new chunk and add to the array when it needs more space. One FixedAllocator only allocates one single size.

## SmallObjAllocator
It has several FixedAllocator objects, each specialized for allocating objects of one size.

## SmallObject
it uses FixedAllocator. It overloads operator new and delete. In the overload functions it pass calls to SmallObjAllocator

# Chunk in details
```C
struct Chunk 
{ 
 void Init(std::size_t blockSize, unsigned char blocks); 
 void* Allocate(std::size_t blockSize); 
 void Deallocate(void* p, std::size_t blockSize); 
 unsigned char* pData_; 
 unsigned char 
 firstAvailableBlock_, 
 blocksAvailable_; 
}; 
```
firstAvailableBlock_, which holds the **index** of the first block available in this chunk. Starts with 0.
blocksAvailable_, the number of blocks available in this chunk
Chunk doesn't keep a copy of `blockSize`. That will waste too much memory. The higher level will keep track of this info.
Being unsigned char, firstAvailableBlock_ takes one byte, 8 bits, which supports 255 blocks.

```
The first byte of an unused block holds the index of the next unused block
```
With the optimization here, we don't need a separate structure to maintain the free_list. We simply dereference the current head, and the value would be the new head (with multiplication and addition).

### Init function
```C++
void Chunk::Init(std::size_t blockSize, unsigned char blocks) 
{ 
 pData_ = new unsigned char[blockSize * blocks]; 
 firstAvailableBlock_ = 0; 
 blocksAvailable_ = blocks; 
 unsigned char i = 0; 
 unsigned char* p = pData_; 
 for (; i != blocks; p += blockSize) 
 { 
 *p = ++i; 
 } 
}
```
The block indices start with 0. A chunk holds an array, which is consecutive order.
In the for loop, the first byte of each block would store the value of the indices of the next block, starting with 1.
It explains why we have to have `firstAvailableBlock_` defined as unsigned char
* We cannot allocate blocks smaller than sizeof(unsigned short), or what ever others. Because we have to save enough space for the indices
* If we make an allocator for 5-byte blocks. When we want to read the value stored, we make want to cast the 5-byte pointer to unsigned int/short, which might cause alignment error.
  * In contrast, a char type has size 1. We directly dereference it and it will not bring any alignment problem

### Allocate
```C
void* Chunk::Allocate(std::size_t blockSize) 
{ 
 if (!blocksAvailable_) return 0; 
 unsigned char* pResult = 
 pData_ + (firstAvailableBlock_ * blockSize); 
 // Update firstAvailableBlock_ to point to the next block 
 firstAvailableBlock_ = *pResult; 
 --blocksAvailable_; 
 return pResult; 
} 
```
I had some problem understanding this line
`firstAvailableBlock_ = *pResult; `
This is correct because the first byte in each chunk is initialized as a number. pResult is returned, which is a pointer, or address to the memory block. But the value there (pResult but dereferenced) is the number block. For example, firstAvailableBlock starts 0, pResult == pData. But the value stored in pResult is 1.

### Deallocate

```C
void Chunk::Deallocate(void* p, std::size_t blockSize) 
{ 
 assert(p >= pData_); 
 unsigned char* toRelease = static_cast<unsigned char*>(p); 
 // Alignment check 
 assert((toRelease - pData_) % blockSize == 0); 
 *toRelease = firstAvailableBlock_; 
 firstAvailableBlock_ = static_cast<unsigned char>( 
 (toRelease - pData_) / blockSize); 
 // Truncation check 
 assert(firstAvailableBlock_ == 
 (toRelease - pData_) / blockSize); 
 ++blocksAvailable_; 
}
```
toRelease is the address to the to be released memory block. We dereference it and set the first byte to the current available block, aka pointing it to. Then update the head of the free list. 