//Author: satanisticmicrowave

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <string>
#include <filesystem>
#include <map>
#include <sstream>
#include <thread>

#include <QColor>
#include <QPalette>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "io.h"
#include "dots.h"
#include "variables.hpp"

namespace fs = std::filesystem;
namespace pt = boost::property_tree;

/*
 * Получает массив с именами миров. Используется в выборе пользователем мира.
 */

std::array<std::string, 100> io::get_world_names()
{

    int i = 0;
    std::array<std::string, 100> names;

    for (auto& p : fs::directory_iterator(io::get_path((char*)"world")))
    {
        fs::path path = p;
        if (path.extension() == (fs::path)".dat" && i<=100)
        {
            names[i] = path.stem().fs::path::generic_string();
            i++;
        }
    }

    return names;
}

/*
 * Получает JSON строку с местами сохранения миров, а также с их именами.
 */

std::string io::get_worlds_file()
{
    return io::read_file(io::get_path((char*)"world") / "worlds.inf");
}

/*
 * Получает JSON строку самого сохранения мира.
 */

std::string io::get_world(std::string filename)
{
    return io::read_file(get_world_path(filename));
}

/*
 * Получает путь к сохранению через имя.
 */

fs::path io::get_world_path(std::string filename)
{
    fs::path path =  io::get_path((char*)"world") / (filename + (char*)".dat");

    return path;

}

/*
 * Пустой конструктор точки
 */

dots::dot::dot()
{
    //this is empty constructor for empty dot c:

}

/*
 * Конструктор точки из сохраненного мира.
 */

dots::dot::dot(std::string data)
{
    std::stringstream ss;
    pt::ptree dot;

    ss<<data;
    pt::read_json(ss, dot);

    this->x = dot.get<int>("x");
    this->y = dot.get<int>("y");
}

/*
 * Конструктор новой точки.
 */

dots::dot::dot(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
 * Создает строку-ключ точки, которую можно использовать для записи точки в мир.
 */

std::string dots::dot::key()
{
    return std::to_string(x) + "|" + std::to_string(y);
}

/*
 * Сериализатор точки в пригодную для сохранения информацию.
 */

std::string dots::dot::str()
{
    pt::ptree data;
    std::stringstream ss;

    data.put("x", this->x);
    data.put("y", this->y);

    pt::write_json(ss, data);

    return ss.str();
}

/*
 * Проверяет, пустой ли класс точки или нет.
 */

bool dots::dot::is_empty()
{
    if(&x == NULL && &y == NULL)
        return true;
    else
        return false;
}

/*
 * Конструктор нового мира.
 */

dots::world::world(std::string name)
{
    this->name = name;
}

/*
 * Сеттер имени мира
 */

void dots::world::set_name(std::string name)
{
    this->name = name;
}

/*
 * Сеттер списка точек
 */

void dots::world::set_dots(std::map<std::string, dots::dot> dots)
{
    this->dots = dots;
}

/*
 * Геттер списка точек
 */

std::map<std::string, dots::dot> dots::world::get_dots()
{
    return this->dots;
}

/*
 * Геттер имени мира
 */

std::string dots::world::get_name()
{
    return this->name;
}

/*
 * Получает определенную точку из мира с помощью строки-ключа (dots::key())
 */

dots::dot dots::world::get(std::string id)
{
    return is_alive(id) ? this->dots[id] : dots::dot();
}

/*
 * Получает соседа точки. Информация о соседях находится в variables.hpp
 */

dots::dot dots::world::get_nb(std::string id, int i)
{
    std::stringstream ss(id);
    std::string x_str, y_str;

    std::getline(ss, x_str, '|');
    std::getline(ss, y_str);

    int x = std::stoi(x_str);
    int y = std::stoi(y_str);

    switch(i)
    {
        case NB_POS_LD:
            return this->get(dots::dot(x-1, y-1).key());
            break;
        case NB_POS_LC:
            return this->get(dots::dot(x-1, y).key());
            break;
        case NB_POS_LU:
            return this->get(dots::dot(x-1, y+1).key());
            break;
        case NB_POS_UP:
            return this->get(dots::dot(x, y+1).key());
            break;
        case NB_POS_RU:
            return this->get(dots::dot(x+1, y+1).key());
            break;
        case NB_POS_RC:
            return this->get(dots::dot(x+1, y).key());
            break;
        case NB_POS_RD:
            return this->get(dots::dot(x+1, y-1).key());
            break;
        case NB_POS_DO:
            return this->get(dots::dot(x, y-1).key());
            break;
        default:
            return dots::dot();
            break;
    }
}

/*
 * Проверяет, жива ли точка в мире на данный момент.
 */

bool dots::world::is_alive(std::string id)
{
    if(dots.count(id))
        return true;
    return false;
}

/*
 * Считает количество живых соседей точки
 */

int dots::world::count_nb(std::string id)
{
    std::stringstream ss(id);
    std::string x_str, y_str;

    std::getline(ss, x_str, '|');
    std::getline(ss, y_str);

    int xd = std::stoi(x_str);
    int yd = std::stoi(y_str);

    int count = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            std::string nb_id = std::to_string(xd+x) + "|" + std::to_string(yd+y);
            if (is_alive(nb_id)) {
                count++;
            }
        }
    }
    return count;
}

/*
 * Добавляет точку в мир
 */

void dots::world::add(dots::dot dot)
{
    this->dots.insert(std::pair<std::string, dots::dot>(dot.key(), dot));
}

/*
 * Убивает точку в мире
 */

void dots::world::kill(std::string id)
{
    this->dots.erase(id);
}

/*
 * Превращает мир в JSON строку, пригодную для сохранения.
 */

std::string dots::world::str()
{
    pt::ptree world, dots;
    std::stringstream ss;
    world.put("name", this->name);

    for(const auto& [key, value] : this->dots)
    {
        pt::ptree dot_t;
        std::stringstream json;
        dots::dot dot = (dots::dot)value;
        json << dot.str();
        pt::read_json(json, dot_t);
        dots.put_child(key, dot_t);
    }
    world.put_child("dots", dots);

    pt::write_json(ss, world);

    return ss.str();
}

/*
 * Деструктор
 */

dots::handler::~handler()
{
    delete world;
}

/*
 * Загружает мир из сохраненной JSON строки
 */

bool dots::handler::load(std::string name)
{

    std::stringstream ss;
    pt::ptree world, dots;
    std::string path = io::get_world_path(name);
    std::string data = io::read_file(path);

    if (data == "")
    {
        return false;
    }

    ss << data;
    pt::read_json(ss, world);
    dots = world.get_child("dots");

    std::map<std::string, dots::dot> dots_m;

    BOOST_FOREACH(const pt::ptree::value_type &v, dots)
    {
        dots::dot dot(std::stoi(v.second.get<std::string>("x")),
        std::stoi(v.second.get<std::string>("y")));

        dots_m.insert(std::pair<std::string, dots::dot>(v.first, dot));
    }

    this->world = new dots::world(name);
    this->world->set_dots(dots_m);
    this->world->static_name = name;

    return true;
}

/*
 * Подготваливает обработчик к использованию
 */

dots::handler::handler()
{
    this->world = nullptr;
}

/*
 * Сохраняет и записывает мир в JSON строку на жестком диске.
 */

bool dots::handler::save()
{
    if(this->world == nullptr)
    {
        return false;
    }

    pt::ptree world, inf;
    std::stringstream ss;

    std::string path = io::get_world_path(this->world->get_name());

    ss.str(std::string());

    ss<<io::get_worlds_file();

    pt::read_json(ss, inf);

    inf.erase(this->world->static_name);
    inf.add(this->world->get_name(), path);

    ss.str(std::string());

    pt::write_json(ss, inf);

    io::save_file(ss.str(), io::get_path((char*)"world/worlds.inf"));
    io::remove(io::get_world_path(this->world->static_name));
    io::save_file(this->world->str(), (fs::path)path);

    return true;
}

inline dots::dot get_nbr(std::string id, int i)
{
    std::stringstream ss(id);
    std::string x_str, y_str;

    std::getline(ss, x_str, '|');
    std::getline(ss, y_str);

    int x = std::stoi(x_str);
    int y = std::stoi(y_str);

    switch(i)
    {
        case NB_POS_LD:
            return dots::dot(x-1, y-1);
            break;
        case NB_POS_LC:
            return dots::dot(x-1, y);
            break;
        case NB_POS_LU:
            return dots::dot(x-1, y+1);
            break;
        case NB_POS_UP:
            return dots::dot(x, y+1);
            break;
        case NB_POS_RU:
            return dots::dot(x+1, y+1);
            break;
        case NB_POS_RC:
            return dots::dot(x+1, y);
            break;
        case NB_POS_RD:
            return dots::dot(x+1, y-1);
            break;
        case NB_POS_DO:
            return dots::dot(x, y-1);
            break;
        default:
            return dots::dot();
            break;
    }
}

/*
 * Первый шаг обработки мира.
 */

void dots::handler::prepare()
{
    std::map<std::string, dots::dot> to_kill;
    std::map<std::string, dots::dot> to_create;

    for (auto& [id, dot] : world->get_dots())
    {

        int count = world->count_nb(id);

        if (count < 2 || count > 3)
            to_kill[id] = dot;


        for (int i = 0; i < 8; i++)
        {
            dots::dot nb = get_nbr(id, i);

            if (!world->is_alive(nb.key()))
            {

                int nb_count = world->count_nb(nb.key());

                if (nb_count == 3)
                    to_create[nb.key()] = nb;

            }
        }
    }

    todo["to_kill_dots"] = to_kill;
    todo["to_create_dots"] = to_create;
}

/*
 * Второй шаг обработки мира
 */

void dots::handler::apply()
{

    for (const auto& [id, dot] : todo["to_kill_dots"])
        world->kill(id);

    for (const auto& [id, dot] : todo["to_create_dots"])
        world->add(dot);

}

/*
 * Создает мир.
 */

void dots::create_world(std::string name)
{
    pt::ptree worlds, world, dots;
    std::stringstream ss;

    ss<<io::get_worlds_file();
    pt::read_json(ss, worlds);

    ss.str(std::string());

    worlds.put(name, io::get_world_path(name));
    pt::write_json(ss, worlds);

    io::save_file(ss.str(), io::get_path((char*)"world/worlds.inf"));

    world.put("name", name);
    world.put_child("dots", dots);

    ss.str(std::string());
    pt::write_json(ss, world);

    io::save_file(ss.str(), io::get_world_path(name));
}

/*
 * При первом запуске игры подготавливает и создает папки и файлы, отвечающие за корректную работу
 * сохранений
 */

void dots::initialize()
{
    io::mkdir(io::get_path((char*)"world"));
    io::create_file(io::get_path((char*)"world") / "worlds.inf");

    pt::ptree inf;
    inf.put("I love cookie?","yes");
    std::stringstream ss;
    pt::write_json(ss,inf);
    io::save_file(ss.str(),io::get_path((char*)"world") / "worlds.inf");
}

//Для оптимизации функции render()
inline bool edited = true;

/*
 * Конструктор класса pixels.
 */

dots::pixels::pixels::pixels(QWidget *parent)
    : QWidget(parent)
{
    handler = new dots::handler();
    pixmap = new QPixmap(GAME_WIDTH - EDITPANEL_WIDTH, GAME_HEIGHT);
    label = new QLabel(this);
    pixpainter = new QPainter;
    pixpainter->begin(pixmap);

    label->setFixedSize(pixmap->size());

    running = false;
    show_borders = true;

    scaling = 1;

    this->setGeometry(EDITPANEL_WIDTH, 0, GAME_WIDTH - EDITPANEL_WIDTH, GAME_HEIGHT);

    this->world_loaded = false;
    scale(1);
    render();

   timer = new QTimer(this);
   speed(10);

   connect(timer, &QTimer::timeout, this, &dots::pixels::pixels::start);

   timer->start();
}

/*
 * Во избежание утечки памяти
 */

dots::pixels::pixels::~pixels(){

    pixpainter->end();
    delete pixpainter;
    delete pixmap;
    delete handler;
    delete label;
    delete timer;
}

/*
 * Устанавливает скорость игры
 */

void dots::pixels::pixels::speed(float speed)
{
    speed_val = speed;
    timer->setInterval(2500/speed_val);
}

/*
 * Пожалуй, главная функция всей программы. Это игровой цикл.
 */

void dots::pixels::pixels::start()
{
    if (running && !handler->world->get_dots().empty())
    {
        handler->prepare();
        handler->apply();
        this->render();
    }
}

/*
 * Устанавливает масштаб
 */

void dots::pixels::pixels::scale(float scale)
{
    scaling = scale;
    edited = true;

    this->rows = (GAME_WIDTH-EDITPANEL_WIDTH) / (DOT_SIZE*scaling);
    this->columns = (GAME_HEIGHT) / (DOT_SIZE*scaling);

    pixmap->fill(QColor::fromRgb(22, 22, 22));

}

/*
 * Создает и удаляет точки.
 */

void dots::pixels::pixels::render()
{

    QPen pen(QColor(100, 100, 100, 200));
    pen.setWidth(1);
    pixpainter->setPen(pen);

    if(this->world_loaded == true)
    {

        if (edited == true)
        {
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < columns; ++j)
                {

                   if(handler->world->is_alive(dots::dot(i, j).key()))
                       this->add(i, j);
                   else
                       this->kill(i, j);
                }
            edited = false;
        }


        for (const auto& [id, dot] : handler->todo["to_kill_dots"])
            kill(dot.x, dot.y);

        for (const auto& [id, dot] : handler->todo["to_create_dots"])
            add(dot.x, dot.y);

        handler->todo.clear();

    }
    else
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
            {
                   this->kill(i, j);
            }
    }

    label->setPixmap(*pixmap);
}

/*
 * Создает точку на поле
 */

void dots::pixels::pixels::add(int x, int y)
{

    dots::dot dot = to_pixmap_coords(x, y);

    QBrush brush(Qt::white, Qt::SolidPattern);
    QPen pen(QColor::fromRgb(100, 100, 100, 200));
    pen.setWidth(1);
    pixpainter->setPen(pen);
    pixpainter->setBrush(brush);

    pixpainter->fillRect(dot.x, dot.y, DOT_SIZE*scaling, DOT_SIZE*scaling, brush);

    if (show_borders)
        pixpainter->drawRect(dot.x, dot.y, DOT_SIZE*scaling, DOT_SIZE*scaling);

    label->setPixmap(*pixmap);
}

/*
 * Удаляет точку на поле
 */

void dots::pixels::pixels::kill(int x, int y)
{

    dots::dot dot = to_pixmap_coords(x, y);

    QBrush brush(QColor::fromRgb(22, 22, 22), Qt::SolidPattern);
    QPen pen(QColor::fromRgb(100, 100, 100, 200));
    pen.setWidth(1);
    pixpainter->setPen(pen);
    pixpainter->setBrush(brush);

    pixpainter->fillRect(dot.x, dot.y, DOT_SIZE*scaling, DOT_SIZE*scaling, brush);

    if(show_borders)
        pixpainter->drawRect(dot.x, dot.y, DOT_SIZE*scaling, DOT_SIZE*scaling);

    label->setPixmap(*pixmap);
}

/*
 * Форсированно создает или удаляет точку, при вводе пользователя
 */

void dots::pixels::pixels::mousePressEvent(QMouseEvent *event)
{
    if (world_loaded && !running)
    {
        QPoint clickPos = event->pos();
        dots::dot dot = to_dotmap_coords(clickPos.x(), clickPos.y());
        if (event->button() == Qt::LeftButton)
        {
            if (!handler->world->is_alive(dot.key()))
            {
                handler->world->add(dot);
                add(dot.x, dot.y);
            }
        }
        else if(event->button() == Qt::RightButton)
        {
            if (handler->world->is_alive(dot.key()))
            {
                handler->world->kill(dot.key());
                kill(dot.x, dot.y);
            }
        }
    }
    else
    {

    }
}

/*
 * Сохраняет игру
 */

bool dots::pixels::pixels::save()
{
    if (world_loaded)
        return handler->save();
    else
        return false;
}

/*
 * Загружает игру
 */

bool dots::pixels::pixels::load(std::string name)
{
    if(handler->load(name))
    {
        world_loaded = true;
        render();
        return true;
    }
    else
        return false;
}

/*
 * Переводит координаты из системы координат на экране в мнимую обрабатываемую систему координат.
 */

dots::dot dots::pixels::pixels::to_dotmap_coords(int x, int y)
{
    return dots::dot((int)(x/(DOT_SIZE*scaling)), (int)(y/(DOT_SIZE*scaling)));
}

/*
 * Переводит из мнимой системы координат handler в понятную для qpixmap систему координат
 */

dots::dot dots::pixels::pixels::to_pixmap_coords(int x, int y)
{
    return dots::dot(x*DOT_SIZE*scaling, y*DOT_SIZE*scaling);
}
