#include "tape-utils.h"

#include <filesystem>

#include "tape.h"

tape_exception::tape_exception(std::string msg) : _msg(std::move(msg)) {
}

const char *tape_exception::what() const noexcept {
    return _msg.c_str();
}

tmp_path_creator::tmp_path_creator() {
    if (!std::filesystem::exists(TMP_PATH)) {
        std::filesystem::create_directories(TMP_PATH);
    }
}

tmp_path_creator::~tmp_path_creator() {
    if (std::filesystem::exists(TMP_PATH)) {
        std::filesystem::remove_all(TMP_PATH);
    }
}

cfg_parser::cfg_parser(const std::string &path) : _file(path), _filename(path) {
    if (!_file.is_open()) {
        throw tape_exception("Error: unable to process configuration file '" + filename() + "'!");
    }
    while (_file.peek() != EOF) {
        std::string key = read_word();
        read_delim();
        std::string value = read_word();
        read_ws();
        if (_data.find(key) != _data.end()) {
            throw tape_exception("Error: configuration file '" + filename() + "' has incorrect structure!");
        }
        _data[key] = value;
    }
}

const std::map<std::string, std::string> &cfg_parser::data() const {
    return _data;
}

void cfg_parser::read_delim() {
    if (_file.get() != DELIM) {
        throw tape_exception("Error: unable to parse configuration file '" + filename() + "'!");
    }
}

std::string cfg_parser::read_word() {
    std::string word;
    while (_file.peek() != EOF && !isspace(_file.peek()) && _file.peek() != DELIM) {
        word.push_back(_file.get());
    }
    if (word.empty()) {
        throw tape_exception("Error: unable to parse configuration file '" + filename() + "'!");
    }
    return word;
}

void cfg_parser::read_ws() {
    while (_file.peek() != EOF && isspace(_file.peek())) {
        _file.get();
    }
}

std::string cfg_parser::filename() const {
    return _filename;
}

std::string cfg_parser::get_field(const std::string &field) {
    if (_data.find(field) == _data.end()) {
        return "";
    }
    return _data[field];
}
