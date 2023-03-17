//
// Created by Luca on 03/01/2023.
//

#ifndef AES_FILEMANAGER_H
#define AES_FILEMANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <type_traits>

#include "logger/logger.hpp"

//TODO: forse cercare per il '\n' nel file?

namespace aes::fm {

/*enum class FM : std::_Ios_Openmode { //TODO: non funziona perché non è un int; remove
    READ = std::ios::in,
    WRITE = std::ios::out
};*/

// ATE = at end of file.

//TODO: enum class?
enum class FileModes {
    READ,
    WRITE,
    APPEND,
    ATE,
    BINARY
};

//TODO: rename in just FILE_MODES?
static const std::map<FileModes, std::_Ios_Openmode>& FILE_MODES_MAP = {
        {FileModes::READ, std::ios::in},
        {FileModes::WRITE, std::ios::out},
        {FileModes::APPEND, std::ios::app},
        {FileModes::ATE, std::ios::ate},
        {FileModes::BINARY, std::ios::binary}
};

//prima std::ios::openmode
static std::_Ios_Openmode get_file_mode(const FileModes& file_mode)
{
    return FILE_MODES_MAP.at(file_mode);
}

//TODO: rename in FileHandler? then rename namespace in aes::fh?
class FileManager {

public:
    // Deleting copy constructor.
    FileManager(const FileManager& file_manager) = delete;

    static FileManager& get_instance() //TODO: constexpr?
    {
        static FileManager instance;
        return instance;
    }

    [[nodiscard]] static std::string get_filename(const std::string_view& file_path); //TODO: inline dava errore!

    //TODO: std::string_view non andava come return type.
    static std::vector<char*> get_file_data(const std::string& file_path); //TODO: remove?
    static std::vector<std::string> get_file_data2(const std::string& file_path);
    static std::string get_file_data3(const std::string& file_path); //TODO: remove this or get_key() qui sotto.
    static std::string get_file_data4(const std::string& file_path); //TODO: remove this or get_key() qui sotto.
    static std::string get_key(const std::string& file_path); //TODO: rinominare in get_file_data_single_stream o qualcosa del genere. //TODO: remove?

    /*template<typename T, typename FP>
    static std::vector<T>& get_file_data(const FP& file_path) //TODO: remove
    {
        std::vector<T> buffer;
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

        // If T is a char* then do this.
        //TODO: update?
        if(std::is_same_v<T, char*>) { //TODO: decltype()?
            for(const auto& line : buffer) {
                buffer.push_back(const_cast<char*>(line.c_str()));
            }
        }

        file.close();

        return buffer;
    }*/

    template<typename T, typename FP>
    /*concept char_like = requires(T d) //TODO: doesn't work so remove
    {
        std::is_same_v<T, std::string>;
    };*/
    // Tolgo il & (reference) dal const FP& file_path perché altrimenti char[] non andrebbe.
    [[nodiscard]] static T get_file_data(const FP file_path) requires std::is_same_v<FP, std::string> || std::is_same_v<FP, char*> || std::is_same_v<FP, char[]>
    {

        std::ifstream file; //TODO: std::ifstream file(file_path);
        file.open(file_path);

        std::vector<T> buffer; //TODO: T buffer?
        std::stringstream ss; //TODO: T ss?

        if(file.is_open()) {
            std::string line;

            if(std::is_same_v<T, std::string>) {
                while(std::getline(file, line)) {
                    ss << line;
                }
            } else {
                while(std::getline(file, line)) {
                    buffer.push_back(line);
                    AES_DEBUG("line of file: {}", line)
                    AES_DEBUG("buffer.size(): {}", buffer.size())
                }
            }
        }

        if(std::is_same_v<T, std::string>) {
            AES_DEBUG("data: {}", ss.str())
        } else {
            AES_DEBUG("data: {}", std::string(buffer.cbegin(), buffer.cend()))
        }

        return std::is_same_v<T, std::string> ? (T)ss.str() : (T)buffer; //TODO: uncomment; prima c'era questa.
    }

    template<typename T, typename FP> //TODO: to fix.
    // Tolgo il & (reference) dal const FP& file_path perché altrimenti char[] non andrebbe.
    [[nodiscard]] static T get_file_data2(const FP file_path) requires std::is_same_v<FP, std::string> || std::is_same_v<FP, char*> || std::is_same_v<FP, char[]>
    {

        std::ifstream file; //TODO: std::ifstream file(file_path);
        file.open(file_path);

        if(std::is_same_v<T, std::string>) {
            std::stringstream ss; //TODO: T ss?
            std::string line;

            if(file.is_open()) {
                while(std::getline(file, line)) {
                    ss << line;
                }
            }

            return (T) ss.str();

        } else {
            std::vector<std::string> buffer; //TODO: T buffer?
            //const char* buffer;
            std::string line;

            if(file.is_open()) {
                while(std::getline(file, line)) {
                    buffer.push_back(line);
                    AES_DEBUG("line of file: {}", line)
                    AES_DEBUG("buffer.size(): {}", buffer.size())
                }
            }

            return (T) buffer;
        }
    }

    static void write_file_data(const std::string& file_path, const std::vector<std::string>& data); //TODO: remove?
    static void write_file_data(const std::string& file_path, const std::vector<uint8_t>& data); //TODO: remove?
    static void write_file_data(const std::string& file_path, const std::string& data); //TODO: remove?

    template<typename T, typename FP> //TODO: typename FP for file_path, typename T for data
    // Tolgo il & (reference) dal const FP& file_path perché altrimenti char[] non andrebbe.
    //TODO: prima era const T& data
    static void write_file_data(const FP file_path, const T data, const FileModes& file_mode = FileModes::APPEND) requires std::is_same_v<FP, std::string> || std::is_same_v<FP, char*> || std::is_same_v<FP, char[]>
    {
        std::ofstream file;
        file.open(file_path, get_file_mode(file_mode));

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

    [[nodiscard]] static inline bool has_data(const std::string_view& file_path);

    [[nodiscard]] static inline uintmax_t get_file_size(const std::string_view& file_path)
    {
        return std::filesystem::file_size(file_path);
    }

    [[nodiscard]] static bool file_exists(const std::string_view& file_path);
    static void delete_file(const std::string& file_path);

private:
    FileManager() = default;
    ~FileManager() = default;
};

} // namespace aes::fm

#endif //AES_FILEMANAGER_H
