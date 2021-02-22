#include "../Public/DoubleBufferAllocator.hpp"
using namespace ez;

DoubleBufferedAllocator::DoubleBufferedAllocator(size_t sz)
{
    _stackAllocator[0] = new StackAllocator(sz);
    _stackAllocator[1] = new StackAllocator(sz);
}

void *DoubleBufferedAllocator::AllocateBlock() noexcept
{
    return nullptr;
}

void DoubleBufferedAllocator::Dellocate(void * ptr) noexcept {
    _stackAllocator[_curStack]->Dellocate(ptr);
}

void DoubleBufferedAllocator::SwapBuffers() noexcept
{
    _curStack = (_curStack == 0) ? 1 : 0;
}
const void DoubleBufferedAllocator::Reset() noexcept
{

    _stackAllocator[_curStack]->Reset();
}
const void DoubleBufferedAllocator::Release()noexcept
{
    _stackAllocator[_curStack]->Release();
}

void DoubleBufferedAllocator::ZeroMem() noexcept
{
    _stackAllocator[_curStack]->ZeroMem();
}


void* DoubleBufferedAllocator::Allocate(const std::size_t sz) noexcept  
{
    void* result = result = _stackAllocator[_curStack]->Allocate(sz);
    return result;
}
