#include "../Public/Allocator.hpp"
#include <assert.h>    

using namespace ez;

std::size_t Allocator::CalculatePadding(const std::size_t baseAddress, const std::size_t alignment) noexcept
{
    if (baseAddress % alignment == 0)
        return 0;
    return (((baseAddress / alignment)+1) * alignment) - baseAddress;  
}


Allocator::Allocator(Allocator &al) :
    _start(al._start), _end(al._end), _size(al._size), _free(al._free), _used(al._used)
{
}
