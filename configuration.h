//Author: satanisticmicrowave

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define config_path io::get_path((char*)".lifeconfig")

#include <string>

/*
 * Пространство имен CONFIGURATION отвечает за хранение и управление конфигурацией/настройками
 * всей игры. Данное пространство имен определено один раз и не предполгает его продолжение.
 */

namespace configuration
{

    /*
     * Данный класс содержит информацию о конфигурации/настройках всей игры. Создается в главном потоке
     * и передается как configuration::configuration*
     */

    class configuration
    {
     public:
        std::string language;
        bool show_borders;
        float scaling, speed;
    };

    std::string get_config();
    void set_default_properties();
    void load_config(configuration* config);
    void save(configuration* config);

}//namespace CONFIGURATION

#endif // CONFIGURATION_H
