#include "tape.h"
#include "tape-utils.h"

#include <cstddef>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>

tape::tape(const std::size_t capacity, const std::size_t buffer_capacity)
    : tape(capacity, buffer_capacity, get_tmp_file_name(),
           std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc) {}

tape::tape(const std::size_t capacity, const std::size_t buffer_capacity, const std::string &path)
    : tape(capacity, buffer_capacity, path, std::ios::binary | std::ios::in | std::ios::out) {}

tape::tape(const std::size_t capacity, const std::size_t buffer_capacity, const std::string &path, const std::ios::openmode mode)
    : _file(path, mode), _file_path(path), _data(new int[buffer_capacity]), _i(0),
      _buffer_capacity(buffer_capacity), _capacity(capacity) {
    if (!_file.is_open()) {
        throw tape_exception("Error while creating tape's file ot path: '" + file_path() + "'.");
    }
}

void tape::set_config_file(const std::string& file) {
    _config = tape_config(file);
}

std::string tape::get_tmp_file_name() {
    return TMP_PATH + std::to_string(NEXT_TMP_FILE_NAME++) + ".bin";
}

void tape::load_buffer() {
    _file.seekg(buffer_start_byte(_i));
    if (file_size(_file_path) - _file.tellg() < buffer_byte_size()) {
        return;
    }
    if (!_file.read(reinterpret_cast<char *>(_data.get()), buffer_byte_size())) {
        throw tape_exception("Error: unable to read from file: '" + file_path() + "'!");
    }
}

void tape::update_buffer() {
    _file.seekp(buffer_start_byte(_i));
    if (!_file.write(reinterpret_cast<char *>(_data.get()), buffer_byte_size())) {
        throw tape_exception("Error: unable to write in file: '" + file_path() + "'!");
    }
}


void tape::rshift() {
    std::this_thread::sleep_for(std::chrono::microseconds(_config.SHIFT_DELAY));
    if (_i == capacity()) {
        throw tape_exception("Error: tape head out of bounds!");
    }
    if ((_i + 1) % buffer_capacity() == 0) {
        update_buffer();
        ++_i;
        load_buffer();
    } else {
        ++_i;
    }
}

void tape::lshift() {
    std::this_thread::sleep_for(std::chrono::microseconds(_config.SHIFT_DELAY));
    if (_i == 0) {
        throw tape_exception("Error: tape head out of bounds!");
    }
    if (_i % buffer_capacity() == 0) {
        update_buffer();
        --_i;
        load_buffer();
    } else {
        --_i;
    }
}

int tape::read() const {
    std::this_thread::sleep_for(std::chrono::microseconds(_config.READ_DELAY));
    return _data.get()[_i % buffer_capacity()];
}

void tape::write(int value) {
    std::this_thread::sleep_for(std::chrono::microseconds(_config.WRITE_DELAY));
    _data.get()[_i % buffer_capacity()] = value;
}

std::size_t tape::buffer_capacity() const {
    return _buffer_capacity;
}

std::size_t tape::capacity() const {
    return _capacity;
}

std::size_t tape::buffer_byte_size() const {
    return sizeof(int) * buffer_capacity();
}

std::size_t tape::buffer_start_byte(const std::size_t i) const {
    return (i - i % buffer_capacity()) * sizeof(int);
}

std::string tape::file_path() const {
    return _file_path.string();
}

std::size_t tape::head() const {
    return _i;
}

tape_config::tape_config(const std::string &config_file) {
    cfg_parser parser(config_file);
    std::string r = parser.get_field("READ_DELAY");
    std::string w = parser.get_field("WRITE_DELAY");
    std::string s = parser.get_field("SHIFT_DELAY");
    try {
        READ_DELAY = std::stoull(r);
        WRITE_DELAY = std::stoull(w);
        SHIFT_DELAY = std::stoull(s);
    } catch (...) {
        throw tape_exception("Error: incorrect configuration file '" + config_file + "'!");
    }
}

tape_config::tape_config() : READ_DELAY(0), WRITE_DELAY(0), SHIFT_DELAY(0) {}
