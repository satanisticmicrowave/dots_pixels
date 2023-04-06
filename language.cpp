//Author: satanisticmicrowave

#include <string>
#include <sstream>
#include <filesystem>
#include <map>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "io.h"
#include "language.h"
#include "variables.hpp"

namespace pt = boost::property_tree;
namespace fs = std::filesystem;

/*
 * Выводит JSON строку со списком всех доступных языков пользовательского интерфейса. Заполняется 
 * автоматически.
 */

std::string io::get_langs_file()
{
	return io::read_file(io::get_path((char*)"lang") / "languages.inf");
}

/*
 * Выводит JSON строку языка пользовательского интерфейса. 
 */

std::string io::get_lang(std::string filename)
{
	return io::read_file(io::get_path((char*)"lang") / (filename + (char*)".dat"));
}

/*
 * Является форком io::get_path() только ведет не в папку игры, а в папку языков.
 */

fs::path io::get_lang_path(std::string filename)
{
	return io::get_path((char*)"lang") / filename;
}

/*
 * Создает папку с языком и записывает первоначальный JSON файл для дальнейшего хранения информации о
 * языках.
 */

void language::initialize()
{
	
	io::mkdir(io::get_path((char*)"lang"));
	io::create_file(io::get_path((char*)"lang") / "languages.inf");
	
	pt::ptree inf;
	inf.put("I've met my end here?", "yes");
	std::stringstream ss;
	pt::write_json(ss,inf);
	io::save_file(ss.str(),io::get_path((char*)"lang") / "languages.inf");

}

/*
 * Получает на вход имя языка, сам находит язык и передает в конструктор. На выходе готовый эксземпляр языка.
 */

language::language* language::load_language(std::string name)
{
    pt::ptree languages;
    std::stringstream json(io::get_langs_file());
    pt::read_json(json, languages);

	return new language(io::read_file(languages.get<std::string>(name)));
}

/* 
 * Создает файл языка из самого языка.
 */

bool language::create_language(language language)
{
	fs::path path = io::get_lang_path(language.get_name() + "_lc.dat");

	pt::ptree inf;
	pt::read_json(io::get_path((char*)"lang") / "languages.inf",inf);
	inf.put(language.get_name(),(std::string)path);
	std::stringstream ss;
	pt::write_json(ss,inf);

	io::save_file(language.str(), path);
    io::save_file(ss.str(), io::get_path((char*)"lang") / "languages.inf");
    return io::check_exists(path);
}

/*
 * Конструктор пустого класса языка.
 */

language::language::language()
{
	this->set_name("unnamed");
}

/* 
 * Конструктор сохраненного языка. Парсит строку в обьект.
 */

language::language::language(std::string data){

    pt::ptree language, data_lang;
    std::stringstream language_json(data);
    
    pt::read_json(language_json, language);
    data_lang = language.get_child("data");

	this->set_name(language.get<std::string>("name"));
	int words = std::stoi(language.get<std::string>("words"));

	for(int i = 0; i <= words; i++)
	{
		this->data[i] = data_lang.get<std::string>(std::to_string(i));	
	}
}

/*
 * Превращает обьект языка в JSON строку для хранения на диске.
 */

std::string language::language::str()
{
	pt::ptree data, lang;
	std::stringstream ss;

	data.put("name", this->name);
	data.put("words", this->data.size()-1);

	for (auto& [key, value] : this->data)
	{lang.put(std::to_string((int)key), value);}

	data.put_child("data", lang);
	
	pt::write_json(ss, data);

	return ss.str();
}
 

//Геттеры и сеттеры

std::map<int, std::string> language::language::get_data()
{
	return this->data;
}

std::string language::language::get_name()
{
	return this->name;
}

/* 
 * Получает слово из массива
 */

std::string language::language::get(int id)
{
	return this->data[id];
}

void language::language::set_name(std::string name)
{
	this->name = name;
}

void language::language::set_data(std::map<int, std::string> data)
{
	this->data = data;
}

/*
 * Помещает фразу в массив
 */

void language::language::set(int id, std::string text)
{
	this->data[id] = text;
}

/*
 * Создает базовые языки пользовательского интерфейса. Учавствует в 
 * configuration::create_default_properties();
 */

void language::create_default_languages()
{

	initialize();	
    language en, ru;

    en.set_name("en");

    en.set(LANG_GAME_NAME, "dots::pixels");

    en.set(LANG_WORLD, "World");
    en.set(LANG_HELP, "Help");
    en.set(LANG_FILE, "File");

    en.set(LANG_WORLD_CREATE, "New");
    en.set(LANG_WORLD_CREATED, "World created");
    en.set(LANG_WORLD_CREATE_DESC, "Create a new world");

    en.set(LANG_WORLD_LOAD, "Load");
    en.set(LANG_WORLD_LOADED, "World loaded");
    en.set(LANG_WORLD_LOAD_DESC, "Load world");

    en.set(LANG_WORLD_SAVE, "Save");
    en.set(LANG_WORLD_SAVED, "World saved");
    en.set(LANG_WORLD_SAVE_DESC, "Save world");

    en.set(LANG_GAME_QUIT, "Quit");
    en.set(LANG_GAME_QUIT_DESC, "Quit game");

    en.set(LANG_HELP_ABOUT_QT, "About qt");

    en.set(LANG_APPLY_BTN, "Apply");
    en.set(LANG_START_BTN, "Start");
    en.set(LANG_STOP_BTN, "Stop");

    en.set(LANG_WORLD_NAME_LABEL, "World name: ");
    en.set(LANG_SCALING_LABEL, "Scale");
    en.set(LANG_SPEED_LABEL, "Speed");

    en.set(LANG_DOTS_LABEL, "Dots: ");
    en.set(LANG_SPEED_TEXT_LABEL, "Speed: ");

    en.set(LANG_SWITCH_FIELD, "Show field");

    create_language(en);

    ru.set_name("ru");

    ru.set(LANG_GAME_NAME, "dots::pixels");

    ru.set(LANG_WORLD, "Мир");
    ru.set(LANG_HELP, "Помощь");
    ru.set(LANG_FILE, "Файл");

    ru.set(LANG_WORLD_CREATE, "Новый");
    ru.set(LANG_WORLD_CREATED, "Мир создан");
    ru.set(LANG_WORLD_CREATE_DESC, "Создать новый мир");

    ru.set(LANG_WORLD_LOAD, "Открыть");
    ru.set(LANG_WORLD_LOADED, "Мир загружен");
    ru.set(LANG_WORLD_LOAD_DESC, "Открыть мир");

    ru.set(LANG_WORLD_SAVE, "Сохранить");
    ru.set(LANG_WORLD_SAVED, "Сохранено");
    ru.set(LANG_WORLD_SAVE_DESC, "Сохранить мир");

    ru.set(LANG_GAME_QUIT, "Выйти");
    ru.set(LANG_GAME_QUIT_DESC, "Выйти из игры");

    ru.set(LANG_HELP_ABOUT_QT, "О Qt");

    ru.set(LANG_APPLY_BTN, "Применить");
    ru.set(LANG_START_BTN, "Старт");
    ru.set(LANG_STOP_BTN, "Стоп");

    ru.set(LANG_WORLD_NAME_LABEL, "Имя мира ");
    ru.set(LANG_SCALING_LABEL, "Масштаб");
    ru.set(LANG_SPEED_LABEL, "Скорость");

    ru.set(LANG_DOTS_LABEL, "Точки: ");
    ru.set(LANG_SPEED_TEXT_LABEL, "Скорость: ");

    ru.set(LANG_SWITCH_FIELD, "Показывать сетку");

    create_language(ru);

}
