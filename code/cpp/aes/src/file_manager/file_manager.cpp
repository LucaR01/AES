//
// Created by Luca on 03/01/2023.
//

#include "file_manager/file_manager.hpp"

#include <iostream>
#include <sstream>

#include "logger/logger.hpp"

namespace aes::fm {

// fstream no flag enabled, ifstream automatically std::ios::in enabled, ofstream automatically std::ios::out enabled.

[[nodiscard]] inline uintmax_t FileManager::get_file_size(const std::string_view& file_path)
{
    return std::filesystem::file_size(file_path);
}

std::vector<char*> FileManager::get_file_data(const std::string& file_path)
{
    std::vector<char*> file_data;
    std::vector<std::string> buffer;
    std::ifstream file;
    file.open(file_path);

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            buffer.push_back(line);
            AES_DEBUG("line of file: {}", line)
            AES_DEBUG("buffer.size(): {}", buffer.size())
        }
    }

    file.close();

    for(const auto& line : buffer) {
        file_data.push_back(const_cast<char*>(line.c_str()));
    }

    //TODO: strcpy(char, char*) per passare da char* a char.

    return file_data;
}

std::vector<std::string> FileManager::get_file_data2(const std::string& file_path)
{
    std::vector<std::string> buffer;
    std::fstream file; //(file_path, std::ios::in) //TODO: ifstream
    file.open(file_path, get_file_mode(FileModes::READ));

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

std::string FileManager::get_file_data3(const std::string& file_path) // Same code as get_key();
{
    std::vector<std::string> buffer;
    std::stringstream ss;

    std::ifstream file; //TODO: std::ifstream file(file_path);
    file.open(file_path);

    //TODO: while(getline(file, line)) { ss << line }

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            buffer.push_back(line);
            AES_DEBUG("line of file: {}", line)
            AES_DEBUG("buffer.size(): {}", buffer.size())
        }
    }

    for(const auto& line : buffer) {
        ss << line;
    }

    AES_DEBUG("data: {}", ss.str())

    return ss.str();
}

std::string FileManager::get_key(const std::string& file_path)
{
    std::vector<std::string> buffer;
    std::stringstream ss;

    std::ifstream file;
    file.open(file_path);

    //TODO: while(getline(file, line)) { ss << line }

    if(file.is_open()) {
        std::string line;
        while(std::getline(file, line)) {
            buffer.push_back(line);
            AES_DEBUG("line of file: {}", line)
            AES_DEBUG("buffer.size(): {}", buffer.size())
        }
    }

    for(const auto& line : buffer) {
        ss << line;
    }

    AES_DEBUG("key: {}", ss.str())

    return ss.str();
}

void FileManager::write_file_data(const std::string& file_path, const std::vector<std::string>& data)
{
    std::ofstream file;
    file.open(file_path, get_file_mode(FileModes::APPEND));

    if(file.is_open()) {
        for(const auto& line : data) {
            file << line;
            AES_DEBUG("line: {}", line)
        }
    } else {
        std::cout << "File doesn't exist or is not open." << std::endl;
        //TODO: throw error?
    }

    file.close();

    AES_INFO("file size: {}", get_file_size(file_path))
}

void FileManager::write_file_data(const std::string& file_path, const std::vector<uint8_t>& data)
{
    std::ofstream file;
    file.open(file_path, get_file_mode(FileModes::APPEND));

    if(file.is_open()) {
        for(const auto& line : data) {
            file << line;
            AES_DEBUG("line: {}", line)
        }
    } else {
        std::cout << "File doesn't exist or is not open." << std::endl; //TODO: remove?
        throw std::runtime_error("File not found or not open");
    }

    file.close();

    AES_INFO("file size: {}", get_file_size(file_path))
}

void FileManager::write_file_data(const std::string& file_path, const std::string& data)
{
    std::ofstream file;
    file.open(file_path, get_file_mode(FileModes::APPEND));

    if(file.is_open()) {
        for(const auto& line : data) {
            file << line;
            AES_DEBUG("line: {}", line)
        }
    } else {
        throw std::runtime_error("File not found or not open");
    }

    file.close();

    AES_INFO("file size: {}", get_file_size(file_path))
}

[[nodiscard]] inline bool FileManager::has_data(const std::string_view& file_path)
{
    return get_file_size(file_path) > 0;
}

[[nodiscard]] bool FileManager::file_exists(const std::string_view& file_path)
{
    return std::filesystem::is_regular_file(file_path);
}

[[nodiscard]] std::string FileManager::get_filename(const std::string_view& file_path)
{
    return std::filesystem::path(file_path).filename().string();
}

void FileManager::delete_file(const std::string& file_path)
{
    std::filesystem::remove(file_path);
}

} // namespace aes::fm
