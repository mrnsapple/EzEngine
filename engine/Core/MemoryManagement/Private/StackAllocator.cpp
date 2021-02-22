#include "../Public/StackAllocator.hpp"

using namespace ez;
StackAllocator::StackAllocator(size_t sz) :  _preAlloc(false), _initialized(false) 
{
    _size = sz;
    _free = sz;
    _used = 0;
    if (sz > 0)
        AllocateBlock();
}

void *StackAllocator::AllocateBlock() noexcept
{
    _start = (uint8_t*)malloc(_size * sizeof(uint8_t));
    _end = (void*) ((uintptr_t)_start + _size * sizeof(uint8_t));
    _cur = _start;
    _initialized = true;
    return _start;
}

StackAllocator::~StackAllocator() noexcept 
{
    try {
        free(_start);
    } catch(...) {

    }
    //_start = nullptr;

}

void* StackAllocator::Allocate(const std::size_t sz) noexcept {
    if (!_initialized)
        return nullptr;
    std::size_t padding = this->CalculatePadding((std::size_t)_cur, sz);
    if ((std::size_t)_cur + padding + sz > (std::size_t)_end) {
        return nullptr;
    }
    const std::size_t nextAddress  = (std::size_t)_cur + padding;

    _cur = (void*)( (std::size_t)_cur + sz + padding) ;

    _free = (std::size_t)_end - (std::size_t)_cur;
    _used = (std::size_t)_cur - (std::size_t)_start;
    return (void*)(nextAddress);
}

void StackAllocator::Dellocate(void* ptr) noexcept {
    if (!ptr)
        return;

    AllocHeader* header = (AllocHeader*) ((uintptr_t)ptr - sizeof(AllocHeader));
    _cur = (void*) ((uintptr_t)ptr - header->padding);
    ptr = NULL;
}

const void StackAllocator::Release() noexcept {
    //if (_preAlloc)
    //    return;
    try {
        free(_start);
    } catch(...) {
        
    }
    _initialized = false;
}

const void StackAllocator::Reset() noexcept {
    _cur = (void *)_start;
}

void StackAllocator::ZeroMem() noexcept {
    memset(_start, 0, _size);
}
