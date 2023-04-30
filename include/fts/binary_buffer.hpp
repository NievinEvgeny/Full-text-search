#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>

namespace fts {

class BinaryBuffer
{
    std::vector<char> data;

   public:
    BinaryBuffer() = default;

    explicit BinaryBuffer(std::size_t min_size)
    {
        data.reserve(min_size);
    }

    const std::vector<char>& get_data() const
    {
        return data;
    }

    void write(const std::string& new_data)
    {
        std::move(new_data.begin(), new_data.end(), std::back_inserter(data));
    }

    void write(char new_data)
    {
        data.emplace_back(new_data);
    }

    template <class T>
    void write(T&& new_data)
    {
        std::move(
            reinterpret_cast<char*>(&new_data),
            reinterpret_cast<char*>(&new_data) + sizeof(new_data),
            std::back_inserter(data));
    }

    void write_to(const std::string& new_data, long offset)
    {
        data.insert(data.begin() + offset, new_data.begin(), new_data.end());
    }

    void write_to(char new_data, long offset)
    {
        data.emplace(data.begin() + offset, new_data);
    }

    template <class T>
    void write_to(T&& new_data, long offset)
    {
        data.insert(
            data.begin() + offset,
            reinterpret_cast<char*>(&new_data),
            reinterpret_cast<char*>(&new_data) + sizeof(new_data));
    }

    void serialize(std::ostream& file) const
    {
        file.write(data.data(), static_cast<long>(data.size()));
    }
};

}  // namespace fts