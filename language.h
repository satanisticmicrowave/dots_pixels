//Author: satanisticmicrowave

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <filesystem>
#include <map>
#include <string>

namespace fs = std::filesystem;

/*
 * Пространство имен LANGUAGE управляет и хранит языки пользовательского интерфейса.
 */

namespace language
{
    /*
     * Данный класс является обьектом языка в котором хранится массив слов, а также методы управления им.
     */

    class language
    {
     public:
        language();
        language(std::string data);

        std::string str();

        void set_name(std::string name);
        void set_data(std::map<int, std::string> data);
        void set(int id, std::string text);

        std::string get_name();
        std::string get(int id);
        std::map<int, std::string> get_data();

     private:
        std::string name;
        std::map<int, std::string> data;
    };

    language* load_language(std::string name);
    bool create_language(language language);
    void create_default_languages();
    void initialize();

} //namespace LANGUAGE

/*
 * Продолжение пространства имен IO. Отвечает за управление файлами языковых пакетов и директории с ними.
 */

namespace io
{

    std::string get_langs_file();
    fs::path get_lang_path(std::string filename);
    std::string get_lang(std::string filename);

} //namespace IO

#endif // LANGUAGE_H
