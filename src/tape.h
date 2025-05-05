#pragma once

#include <filesystem>
#include <string>
#include <fstream>

#include "tape-utils.h"

class tape_config {
public:
    tape_config();

    tape_config(const std::string& config_file);

    tape_config(const tape_config& other) = default;
    tape_config(tape_config&& other) = default;
    tape_config& operator=(const tape_config& other) = default;
    tape_config& operator=(tape_config&& other) = default;

    std::size_t READ_DELAY;
    std::size_t WRITE_DELAY;
    std::size_t SHIFT_DELAY;
};

class tape {
    static constexpr auto TMP_PATH = tmp_path_creator::TMP_PATH;
    inline static std::size_t NEXT_TMP_FILE_NAME = 0;
    inline static tmp_path_creator tmp_creator = {};
public:
    tape(std::size_t capacity, std::size_t buffer_capacity);

    tape(std::size_t capacity, std::size_t buffer_capacity, const std::string& path);

    tape(const tape& other) = delete;
    tape(tape&& other)  noexcept = default;
    tape& operator=(const tape&) = delete;
    tape& operator=(tape&&) noexcept = default;
    ~tape() = default;

    void write(int value);

    int read() const;

    void lshift();

    void rshift();

    void set_config_file(const std::string& file);

    std::size_t buffer_byte_size() const;
    std::size_t buffer_capacity() const;
    std::size_t capacity() const;
    std::size_t head() const;

    std::string file_path() const;

private:
    tape(std::size_t capacity, std::size_t buffer_capacity, const std::string& path, std::ios::openmode mode);

    static std::string get_tmp_file_name();
    void load_buffer();
    void update_buffer();
    std::size_t buffer_start_byte(std::size_t i) const;

    std::fstream _file;
    tape_config _config;
    std::filesystem::path _file_path;
    std::unique_ptr<int> _data;
    std::size_t _i;
    std::size_t _buffer_capacity;
    std::size_t _capacity;
};

