#include "../Public/PoolAllocator.hpp"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>    
#include <algorithm> 
#include <stdio.h>
#include <string.h>   
#ifdef _DEBUG
#include <iostream>
#endif
using namespace ez;

PoolAllocator::PoolAllocator(PoolAllocator &al) :
    Allocator(al),
    _peak(al._peak),
    _chunksPerBlock(al._chunksPerBlock),
    _chunkSize(al._chunkSize),
    _freeList(al._freeList)
{
}

PoolAllocator::PoolAllocator(const std::size_t chunksPerBlock, const std::size_t chunkSize)  : _chunksPerBlock(chunksPerBlock), _chunkSize(chunkSize) {
    AllocateBlock();
}

PoolAllocator::~PoolAllocator() {
    free(_start);
}

bool PoolAllocator::InPool(void* data) noexcept
{
    if ((std::size_t)_start <= (std::size_t)data &&
         (std::size_t)_end >= (std::size_t)data) {
            Dellocate(data);
            return true;
    }
    return false;

}

void  *PoolAllocator::AllocateBlock() noexcept
{
    _size = _chunkSize * _chunksPerBlock;
    _start = malloc(_size);
    _end = (void*) ((uintptr_t)_start + _size * sizeof(uint8_t));
    this->Reset();
    return _start;
}

const void PoolAllocator::Reset() noexcept {
    _used = 0;
    _peak = 0;
    _free = _size;
    // Create a linked-list with all free positions
    for (int i = 0; i < _chunksPerBlock; ++i) {
        std::size_t address = (std::size_t) _start + i * _chunkSize;
        _freeList.push((Node *) address);
    }
}

bool PoolAllocator::isFull()
{
    if (_freeList.head == nullptr || _used == _size)
        return true;
    return false;
}

void *PoolAllocator::Allocate(const std::size_t sz) noexcept
{
    if (isFull()) {
        return nullptr;
    }
    void * freeChunk  = _freeList.pop();
    
    _used += _chunkSize;
    _free -= _chunkSize;
    return freeChunk; 
}

void PoolAllocator::Dellocate(void * ptr) noexcept {
    _used -= _chunkSize;
    _free += _chunkSize;
    Node *node = (Node *)ptr;
    _freeList.push(node);
}

const void PoolAllocator::Release() noexcept {
    free(_start);
}

void PoolAllocator::ZeroMem() noexcept {
    memset(_start, 0, _size);
}

