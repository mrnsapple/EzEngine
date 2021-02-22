/** 
 *  Base abstract class for allocate memory.
 */
#pragma once

#include <iostream>
#include <stdint.h>

#ifndef ALLOCATOR_H
#define ALLOCATOR_H
namespace ez {
    
class Allocator {
protected:
    void* _start = nullptr;
    void* _end = nullptr;
    std::size_t _size = 0;
    std::size_t _free = 0;
    std::size_t _used = 0;

    
public:
    Allocator() = default;
    Allocator(Allocator &);
    std::size_t GetFree() { return _free; }
    std::size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment) noexcept;

    // allocate
    virtual void* Allocate(const std::size_t sz) noexcept =0 ;
    
    // deallocate
    virtual void Dellocate(void*) noexcept =0;
    
    // reset allocator state
    const virtual void Reset() noexcept =0;
    
    virtual void  *AllocateBlock() noexcept = 0;
    // override memory region with zeroes
    virtual void ZeroMem() noexcept = 0;
    
    // Free the memory.
    const virtual void Release() noexcept = 0;

};
}
#endif 