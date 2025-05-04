#pragma once

#include <exception>
#include <string>
#include <map>
#include <fstream>

class tmp_path_creator {
public:
    static constexpr auto TMP_PATH = "./tmp/";

    tmp_path_creator();

    ~tmp_path_creator();

};

class tape_exception : public std::exception {
public:
    tape_exception(std::string msg);

    const char *what() const noexcept override;
private:
    std::string _msg;
};

class cfg_parser {
    static constexpr char DELIM = '=';
public:
    cfg_parser(const std::string& path);

    const std::map<std::string, std::string>& data() const;

    std::string get_field(const std::string& field);

    std::string filename() const;
private:
    std::string read_word();
    void read_delim();
    void read_ws();

    std::ifstream _file;
    std::map<std::string, std::string> _data;
    std::string _filename;
};