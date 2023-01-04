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

// ATE = at end of file.

enum FileModes {
    READ,
    WRITE,
    APPEND,
    ATE,
    BINARY
};

static const std::map<FileModes, std::_Ios_Openmode> file_modes_map = {
        {FileModes::READ, std::ios::in},
        {FileModes::WRITE, std::ios::out},
        {FileModes::APPEND, std::ios::app},
        {FileModes::ATE, std::ios::ate},
        {FileModes::BINARY, std::ios::binary}
};

//prima std::ios::openmode
static std::_Ios_Openmode get_file_mode(const FileModes& file_mode)
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

    [[nodiscard]] static std::string get_filename(const std::string_view& file_path); //TODO: inline dava errore!

    //TODO: std::string_view non andava come return type.
    static std::vector<char*> get_file_data(const std::string& file_path);
    static std::vector<std::string> get_file_data2(const std::string& file_path);
    static std::string get_key(const std::string& file_path); //TODO: rinominare in get_file_data_single_stream o qualcosa del genere.

    static void write_file_data(const std::string& file_path, const std::vector<std::string>& data);
    [[nodiscard]] static inline bool has_data(const std::string_view& file_path);

    [[nodiscard]] static inline uintmax_t get_file_size(const std::string_view& file_path);

    [[nodiscard]] static bool file_exists(const std::string_view& file_path);
    static void delete_file(const std::string& file_path);

private:
    FileManager() = default;
    ~FileManager() = default;
};

}

#endif //AES_FILEMANAGER_H
