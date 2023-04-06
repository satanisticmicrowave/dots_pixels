//Author: satanisticmicrowave

#include "io.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <stdio.h>

/*
 * Проверяет, есть ли в файле хоть что-то.
 * Вход: Путь к файлу
 * Выход: bool (Если файл пустой, то выводит true, если что-то есть - false
 */

bool io::check_empty(fs::path path)
{
    return fs::is_empty(path);
}

/*
 * Проверяет наличие файла по заданному пути.
 * Вход: Путь к файлу
 * Выход: bool (Если файл существует, то выведет true, в противоположнорм случае - false)
 */

bool io::check_exists(fs::path path)
{
    return fs::exists(path);
}

/*
 * Удаляет файл.
 * Вход: Путь к файлу
 * Выход: bool (Если при удалении произошла ошибка, выведет false)
 */

bool io::remove(fs::path path)
{
	return fs::remove(path);
}

/*
 * Создает папку.
 * Вход: Путь к директории
 * Выход: bool (Если при создании папки возникнет ошибка выведет false)
 */

bool io::mkdir(fs::path path)
{
    return fs::create_directories(path.c_str());
}

/*
 * Получает путь к файлу в папке игры. Например io::get_path((fs::path)"dickpic.img") выведет 
 * "/home/satanisticmicrowave/Documents/My Games/Life/dickpic.img". Нужен для удобства 
 * использования в связке с io::get_game_folder()
 * Вход: Имя файла
 * Выход: Путь к файлу
 */

fs::path io::get_path(char* filename)
{
    return get_game_folder() / filename;
}

/*
 * Сохраняет текст в файл.
 * Вход: Текст для сохранения, путь к файлу
 */

void io::save_file(std::string content, fs::path path)
{

    std::ofstream file;
	file.open(path);
	file<<content;
    file.close();
}

/*
 * Создает файл. По факту записывает пустоту в файл, тем самым создавая его.
 * Вход: Путь к файлу
 * Выход: bool (Если при сохранении произошла ошибка, выведет false)
 */

bool io::create_file(fs::path path)
{
	io::save_file((char*)"",path);
    return io::check_exists(path); 
}

/*
 * Читает данные из файла. 
 * Вход: Путь к файлу
 * Выход: Данные из файла
 */

std::string io::read_file(fs::path path)
{
	std::string data;
	std::ifstream file(path);
	std::string line;
	while(std::getline(file,line))
	{
		data += "\n";
		data += line;
	}
	return data;
}

/*
 * Определяет путь к локальной директории игры.
 */

fs::path io::get_game_folder()
{
	std::string path = getenv("HOME");
	path += "/Documents/My Games/life";
	return path;
}
