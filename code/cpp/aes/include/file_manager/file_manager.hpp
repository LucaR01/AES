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

namespace aes::fm {

/**
 * @enum FileModes
 */
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
/**
 * @param file_mode: the file_mode in which we want to open the file.
 * @return : a std::_Ios_Openmode
 */
static std::_Ios_Openmode get_file_mode(const FileModes& file_mode)
{
    return FILE_MODES_MAP.at(file_mode);
}

class FileManager {

public:
    // Deleting copy constructor.
    FileManager(const FileManager& file_manager) = delete;

    static FileManager& get_instance() //TODO: constexpr?
    {
        static FileManager instance;
        return instance;
    }

    /**
     * @brief This function returns the name of the file.
     * @param file_path: the file we want to get the name from.
     * @return the name of the file as a std::string.
     */
    [[nodiscard]] static inline std::string get_filename(const std::string_view& file_path) //TODO: inline dava errore!
    {
        return std::filesystem::path(file_path).filename().string();
    }

    //TODO: std::string_view non andava come return type.
    static std::vector<char*> get_file_data(const std::string& file_path); //TODO: remove?
    static std::vector<std::string> get_file_data2(const std::string& file_path);
    static std::string get_file_data3(const std::string& file_path); //TODO: remove this or get_key() qui sotto.
    static std::string get_file_data4(const std::string& file_path); //TODO: remove this or get_key() qui sotto.
    static std::string get_file_data5(const std::string& file_path);
    static std::vector<uint8_t> get_file_data6(const std::string& file_path);
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

    /**
     *
     * @tparam T: the return type.
     * @tparam FP: the type of the file path
     * @param file_path: the file we want to get the data from.
     * @return the data of type T.
     */
    template<typename T, typename FP> //FIXME
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

    /*static void write_file_data(const std::string& file_path, const std::vector<std::string>& data); //TODO: remove?
    static void write_file_data(const std::string& file_path, const std::vector<uint8_t>& data); //TODO: remove?
    static void write_file_data(const std::string& file_path, const std::string& data); //TODO: remove?*/

    /**
     * @brief This function writes data in the file specified by file_path.
     * @tparam T: the type of the data to write in the file.
     * @tparam FP: the type of the file_path which could be either a std::string, char* or char[].
     * @param file_path: the path of the file in which we want to write the data.
     * @param data: the data that we want to write to the file.
     * @param file_mode: the mode in which we want to open the file.
     */
    template<typename T, typename FP>
    // Tolgo il & (reference) dal const FP& file_path perché altrimenti char[] non andrebbe.
    static void write_file_data(const FP file_path, const T data, const FileModes& file_mode = FileModes::APPEND) requires std::is_same_v<FP, std::string> || std::is_same_v<FP, char*> || std::is_same_v<FP, char[]>
    {
        std::ofstream file;
        file.open(file_path, get_file_mode(file_mode));

        if(file.is_open()) {
            for(const auto& line : data) {
                file <<  line; //TODO: std::noskipws <<?
                AES_DEBUG("line: {}", line)
            }
        } else {
            throw std::runtime_error("File not found or not open");
        }

        file.close();

        AES_INFO("file size: {}", get_file_size(file_path))
    }

    /**
     * @brief This function returns the size of a file specified by the param file_path.
     * @param file_path: the path of the file we want to get the file size.
     * @return file size as uintmax_t which is a unsigned long long int.
     */
    [[nodiscard]] static inline uintmax_t get_file_size(const std::string_view& file_path)
    {
        return std::filesystem::file_size(file_path);
    }

    /**
     * @brief This function returns true if the file size is greater than 0, false otherwise.
     * @param file_path: the file we want to check if has data.
     * @return true if file size > 0, false otherwise.
     */
    [[nodiscard]] static inline bool has_data(const std::string_view& file_path)
    {
        return get_file_size(file_path) > 0;
    }

    /**
     * @brief This function checks whether a file exists or not.
     * @param file_path: the file we want to check.
     * @return true if it exists, false otherwise.
     */
    [[nodiscard]] static inline bool file_exists(const std::string_view& file_path)
    {
        return std::filesystem::is_regular_file(file_path);
    }

    /**
     * @brief This function deletes the file passed as argument.
     * @param file_path: the file we want to delete.
     */
    static inline void delete_file(const std::string& file_path)
    {
        std::filesystem::remove(file_path);
    }

private:
    FileManager() = default;
    ~FileManager() = default;
};

} // namespace aes::fm

#endif //AES_FILEMANAGER_H
