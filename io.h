//Author: satanisticmicrowave

#ifndef IO_H
#define IO_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/*
 * Пространство имен IO отвечает за работу с файловой системой без обращения к std::filesystem
 * с базовыми функциями. Функции внутри пространства имен используются для построения
 * и достроения функций внутри данного пространства имен.
 */

namespace io
{

    fs::path get_path(char* filename);
    fs::path get_game_folder();

    std::string read_file(fs::path path);
    void save_file(std::string content, fs::path path);

    bool check_empty(fs::path path);
    bool check_exists(fs::path path);

    bool mkdir(fs::path path);
    bool remove(fs::path path);
    bool create_file(fs::path path);

}// namespace IO


#endif // IO_H
