#include <fts/binary_buffer.hpp>
#include <cstdint>
#include <string>

namespace fts {

void BinaryBuffer::write(uint32_t new_data)
{
    std::move(
        reinterpret_cast<char*>(&new_data),
        reinterpret_cast<char*>(&new_data) + sizeof(new_data),
        std::back_inserter(data));
}

void BinaryBuffer::write(uint8_t new_data)
{
    std::move(
        reinterpret_cast<char*>(&new_data),
        reinterpret_cast<char*>(&new_data) + sizeof(new_data),
        std::back_inserter(data));
}

void BinaryBuffer::write(const std::string& new_data)
{
    std::move(new_data.begin(), new_data.end(), std::back_inserter(data));
}

void BinaryBuffer::write_to(uint32_t new_data, long offset)
{
    data.insert(
        data.begin() + offset,
        reinterpret_cast<char*>(&new_data),
        reinterpret_cast<char*>(&new_data) + sizeof(new_data));
}

void BinaryBuffer::write_to(uint8_t new_data, long offset)
{
    data.insert(
        data.begin() + offset,
        reinterpret_cast<char*>(&new_data),
        reinterpret_cast<char*>(&new_data) + sizeof(new_data));
}

void BinaryBuffer::write_to(const std::string& new_data, long offset)
{
    data.insert(data.begin() + offset, new_data.begin(), new_data.end());
}

}  // namespace fts