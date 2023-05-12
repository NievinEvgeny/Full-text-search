#pragma once
#include <cstring>

namespace fts {

class BinaryReader
{
    const char* current;

   public:
    explicit BinaryReader(const char* buf) : current(buf)
    {
    }

    const char* get() const noexcept
    {
        return current;
    }

    void ptr_shift(std::size_t shift) noexcept
    {
        current += shift;
    }

    template <class T>
    void read(T* dest, std::size_t size) noexcept
    {
        std::memcpy(dest, current, size);
        ptr_shift(size);
    }
};

}  // namespace fts