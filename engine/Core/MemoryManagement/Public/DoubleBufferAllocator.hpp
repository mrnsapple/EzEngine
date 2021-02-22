#include "StackAllocator.hpp"
#include "PoolAllocator.hpp"
#ifndef DOUBLEBUFFERALLOCATOR_H
#define DOUBLEBUFFERALLOCATOR_H
namespace ez {
    
class DoubleBufferedAllocator : public Allocator
{
private:
    StackAllocator *_stackAllocator[2];
    int _curStack = 0;
public:
    ~DoubleBufferedAllocator() {} 
    DoubleBufferedAllocator(size_t sz=0);
    void SwapBuffers() noexcept;
    
    void *AllocateBlock() noexcept override;
    void* Allocate(const std::size_t sz) noexcept override ;
    void Dellocate(void*) noexcept override;
    const void Release() noexcept override;
    const inline void Reset() noexcept override;
    inline void ZeroMem() noexcept override;

};
}
#endif