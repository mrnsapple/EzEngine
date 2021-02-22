#pragma once

#include "Allocator.hpp"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory>

#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H
namespace ez {

class StackAllocator : public Allocator {
public:
    StackAllocator(size_t sz=0);

    ~StackAllocator();

    void *AllocateBlock() noexcept override;

    void* Allocate(const std::size_t sz) noexcept override ;
    inline void Dellocate(void*) noexcept override;
    const void Release() noexcept override;
    const inline void Reset() noexcept override;
    inline void ZeroMem() noexcept override;

protected:

    struct AllocHeader {
        uint8_t padding;
    };

private:
    
    void* _cur = nullptr;
    bool _initialized;
    bool _preAlloc;
};
}
#endif