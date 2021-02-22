
#include "PoolAllocator.hpp"
#include "StackLinkedList.hpp"
#include <list>
#ifndef POOLMANGER_H
#define POOLMANGER_H
namespace ez {
class PoolManager
{
private:
    using Node = StackLinkedList<PoolAllocator*>::Node;

private:
    static PoolManager *instance;
    PoolManager(std::size_t chunksPerBlock = 8);

    //Get pool of given size
    Node * GetPool(const std::size_t sz);
    //Create a pool

    Node * CreatePool(const std::size_t chunkSize) ;
public:
    static PoolManager *getInstance(std::size_t chunksPerBlock = 8);
    static PoolManager *getNewInstance(std::size_t chunksPerBlock = 8);
 
    std::size_t getPoolNumber() { return _amountPools; }

    void *Allocate(const std::size_t sz) noexcept;
    void Dellocate(void*) noexcept;

private:
    StackLinkedList<PoolAllocator*> _pools;
    std::size_t _chunksPerBlock;
    std::size_t _amountPools =0;
};
}

#endif
