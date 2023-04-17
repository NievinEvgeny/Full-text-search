#pragma once
#include <vector>
#include <variant>
#include <cstdint>
#include <string>

namespace fts {

class BinaryBuffer
{
    std::vector<char> data;

   public:
    // reserve??

    const std::vector<char>& get_data() const
    {
        return data;
    }

    void write(uint32_t new_data);

    void write(uint8_t new_data);

    void write(const std::string& new_data);

    void write_to(uint32_t new_data, long offset);

    void write_to(uint8_t new_data, long offset);

    void write_to(const std::string& new_data, long offset);
};

}  // namespace fts