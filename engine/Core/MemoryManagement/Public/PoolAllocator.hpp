#include "Allocator.hpp"
#include "StackLinkedList.hpp"
#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H
namespace ez {

class PoolAllocator : public Allocator {
public:
 struct  DellocateHeader{
    };
private:
   
    using Node = StackLinkedList<DellocateHeader>::Node;
    StackLinkedList<DellocateHeader> _freeList;
    std::size_t _chunkSize;
    std::size_t _chunksPerBlock;

    std::size_t _peak;
    
public:
    std::size_t GetChunkSize() { return _chunkSize; }
    PoolAllocator(const std::size_t chunksPerBlock, const std::size_t chunkSize);
    PoolAllocator(PoolAllocator &al);
    ~PoolAllocator();
    bool isFull();
    bool InPool(void* data) noexcept;
    void *AllocateBlock() noexcept override;
    void* Allocate(const std::size_t sz)  noexcept override;
    void Dellocate(void*) noexcept override;
    const void Reset() noexcept override;
    void ZeroMem() noexcept override;
    // release (if exists) the buffer allocated from the OS at the initialiation of the allocator.
    const inline void Release() noexcept override;
private:

};
}
#endif 