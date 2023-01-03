//
// Created by Luca on 03/01/2023.
//

#include "file_manager/FileManager.hpp"

#include <iostream>
#include <fstream>

#include "logger/logger.hpp"

namespace aes::fm {

// fstream no flag enabled, ifstream automatically std::ios::in enabled, ofstream automatically std::ios::out enabled.

[[nodiscard]] inline uintmax_t FileManager::get_file_size(const std::string& file_path)
{
    return std::filesystem::file_size(file_path);
}

unsigned int FileManager::file_size(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::ate);
    return file.tellg();
}

size_t FileManager::binary_file_size(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    return file.tellg();
}

std::vector<char*> FileManager::get_file_data(const std::string& file_path)
{
    std::vector<char*> file_data;
    std::vector<std::string> buffer;
    std::fstream file; //TODO: usare ifstream
    file.open(file_path, std::ios::in); //TODO: get_file_mode(FileModes::READ);

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            buffer.push_back(line);
            AES_DEBUG("line of file: {}", line)
            AES_DEBUG("buffer.size(): {}", buffer.size())
        }
    }

    file.close();

    /*for(int i = 0; i < buffer.size(); i++) { //TOOD: remove
        //file_data.push_back(buffer[i]);
        file_data.push_back(buffer.at(i).c_str());
    }*/

    for(const auto& line : buffer) {
        //file_data.push_back(line.data());
        file_data.push_back(const_cast<char*>(line.c_str()));
    }

    //TODO: strcpy(char, char*) per passare da char* a char.

    return file_data;
}

std::vector<std::string> FileManager::get_file_data2(const std::string& file_path)
{
    std::vector<std::string> buffer;
    std::fstream file; //(file_path, std::ios::in)
    file.open(file_path, get_file_mode(FileModes::READ)); //TODO: get_file_mode(FileModes::READ); std::ios::in

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            buffer.push_back(line);
            AES_DEBUG("line of file: {}", line)
            AES_DEBUG("buffer.size(): {}", buffer.size())
        }
    }

    file.close();

    return buffer;
}

void FileManager::write_file_data(const std::string &file_path, const std::vector<std::string> &data)
{
    std::ofstream file;
    file.open(file_path, get_file_mode(FileModes::APPEND));

    if(file.is_open()) {
        for(const auto& line : data) {
            file << line;
            AES_DEBUG("line: {}", line)
        }

        //file.close();
        //return true; //TODO: file.size() > 0
    }

    file.close();

    AES_INFO("file size: {}", get_file_size(file_path))

    //return false;
    //return get_file_size(file_path) > 0;
}

[[nodiscard]] inline bool FileManager::has_data(const std::string& file_path)
{
    return get_file_size(file_path) > 0;
}

}
