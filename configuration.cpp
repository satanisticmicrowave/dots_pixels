//Author: satanisticmicrowave

#include <string>
#include <cstdlib>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "io.h"
#include "dots.h"
#include "configuration.h"
#include "language.h"

/*
 * Получает текстовую информацию о конфигурации игры. Если ее нет, создает папки и записывает значения 
 * по умолчанию в конфигурацию.
 * Выход: JSON-строка с конфигурацей игры
 */

std::string configuration::get_config()
{

    if(!io::check_exists(config_path))
    {
        io::mkdir(io::get_game_folder());
        io::create_file(config_path);
    }

    if(io::check_empty(config_path))
    {
        set_default_properties();
    }
    //set_default_properties();

    return io::read_file(config_path);
}

/*
 * Парсит и подгружает конфигурацию в экземпляр класса configuration.
 */

void configuration::load_config(configuration* config)
{	
	
    std::stringstream json(get_config());
    boost::property_tree::ptree root, dot, graphics;

    boost::property_tree::read_json(json, root);

    dot = root.get_child("dots");
    graphics = root.get_child("graphics");

    config->language = root.get<std::string>("language");
    config->scaling = graphics.get<int>("scale");
    config->speed = dot.get<int>("speed");
    config->show_borders = (bool)graphics.get<int>("show_borders");

}


/*
 * Сохраняет конфигурацию.
 */
void configuration::save(configuration* config)
{

    boost::property_tree::ptree data, graphics, dot;
    std::stringstream json;

    graphics.put("show_borders", (int)config->show_borders);
    graphics.put("scale", config->scaling);
    data.put_child("graphics", graphics);

    dot.put("speed", config->speed);
    data.put_child("dots", dot);

    data.put("language", config->language);

    boost::property_tree::write_json(json, data);

    io::save_file(json.str(),config_path);
}

/*
 * Устанавливает значения конфигурации по умолчанию.
 */
void configuration::set_default_properties()
{

    boost::property_tree::ptree data, graphics, dot;
	std::stringstream json;

    graphics.put("show_borders", 1);
    graphics.put("scale", 100);
	data.put_child("graphics", graphics);
	
    dot.put("speed", 85);
	data.put_child("dots", dot);

    data.put("language", "en");

	boost::property_tree::write_json(json, data);

	io::save_file(json.str(),config_path);

    dots::initialize();
    language::create_default_languages();

}
