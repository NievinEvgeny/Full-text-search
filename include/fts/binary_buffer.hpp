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

    template <class T>
    void write_to(T&& new_data, long offset)
    {
        data.insert(
            data.begin() + offset,
            reinterpret_cast<char*>(&new_data),
            reinterpret_cast<char*>(&new_data) + sizeof(new_data));
    }

    void serialize(const std::string& filename) const
    {
        const std::string& index_filename = filename;

        std::ofstream index_file;
        index_file.open(index_filename, std::ios::out | std::ios::binary);

        if (!index_file.is_open())
        {
            throw std::runtime_error{"Can't open file in BinIndexWriter::write function"};
        }

        index_file.write(data.data(), static_cast<long>(data.size()));
        index_file.close();
    }
};

}  // namespace fts