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

namespace aes::fm {

/*enum class FM : std::_Ios_Openmode { //TODO: non funziona perché non è un int; remove
    READ = std::ios::in,
    WRITE = std::ios::out
};*/

// ATE = at end of file credo.

enum FileModes {
    READ,
    WRITE,
    APPEND,
    ATE,
    BINARY
};

static const std::map<FileModes, std::_Ios_Openmode> file_modes_map = { //std::ios::openmode
        {FileModes::READ, std::ios::in},
        {FileModes::WRITE, std::ios::out},
        {FileModes::APPEND, std::ios::app},
        {FileModes::ATE, std::ios::ate},
        {FileModes::BINARY, std::ios::binary}
};

//prima std::ios::openmode
static constexpr std::_Ios_Openmode get_file_mode(const FileModes& file_mode)
{
    return file_modes_map.at(file_mode);
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

    /*static std::unique_ptr<FileManager> get_instance() ///TODO: remove
    {
        file_manager = std::make_unique<FileManager>();
        return file_manager;
    }*/

    //TODO: std::string_view non andava.
    static std::vector<char*> get_file_data(const std::string& file_path);
    static std::vector<std::string> get_file_data2(const std::string& file_path);

    static void write_file_data(const std::string& file_path, const std::vector<std::string>& data);
    [[nodiscard]] static inline bool has_data(const std::string& file_path);

    [[nodiscard]] static inline uintmax_t get_file_size(const std::string& file_path);
    static unsigned int file_size(const std::string& file_path); //TODO: remove?
    static size_t binary_file_size(const std::string& file_path); //TODO: remove?

private:
    FileManager() = default;
    ~FileManager() = default;

    //static std::unique_ptr<FileManager> file_manager; //= std::make_unique<FileManager>(); //TODO: constexpr; remove
    //static FileManager file_manager; //TODO: remove
};

}

#endif //AES_FILEMANAGER_H
