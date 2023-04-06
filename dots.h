//Author: satanisticmicrowave

#ifndef DOTS_H
#define DOTS_H

#include <string>
#include <filesystem>
#include <map>
#include <thread>

#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include <QLabel>

/*
 * В компьютерной реализации игры "Жизнь" есть два этапа обработки игры.
 *
 * Первый этап проверяет все живые точки в мире на соблюдение условий жизни. Если эти условия
 * нарушены, клетку записывают в "список смерти". Также проверяют всех точек-соседей в мире. Если
 * У пустой клетки есть все условия для жизни, ее координаты записывают в "список жизни".
 *
 * Второй этап берет оба списка смерти и жизни и по списку убивает и рождает новые точки в мире.
 */

namespace fs = std::filesystem;

/*
 * Пространство имен DOTS отвечает за создание модели клетки в сетке самой игры "жизнь",
 * самой сетки и класса обработчика событий для игры. Оно манипулирует точками, сохранением и
 * запуском игры, а также обработкой и настраиванием правил самой игры.
 */

namespace dots
{

    /*
     * Класс DOT представляет из себя модель клетки.
     */

    class dot
    {
     public:
        dot();
        dot(std::string data);
        dot(int x, int y);
        std::string str();
        std::string key();
        bool is_empty();
        int x,y;
    };

    /*
     * Класс WORLD представляет из себя бесконечное поле точек. Манипулирует сохранением и
     * загрузкой игры. Имеет информацию о всех точках и может их как создавать, так убивать.
     */

    class world
    {
     public:
        world(std::string name);

        void set_name(std::string name);
        void set_dots(std::map<std::string, dots::dot> dots);
        void add(dots::dot dot);
        void kill(std::string id);

        std::map<std::string, dots::dot> get_dots();
        std::string get_name();
        std::string str();
        dots::dot get(std::string id);
        dots::dot get_nb(std::string id, int i);

        bool is_alive(std::string id);

        int count_nb(std::string id);

        std::string static_name;

     private:
        std::map<std::string, dots::dot> dots;
        std::string name;
    };

    /*
     * Класс HANDLER отвечает за обработку событий игры. Он приказывает классу WORLD создавать или
     * убивать точки. Иными словами - это правила игры.
     */

    class handler
    {
     public:
        handler();
        ~handler();

        bool load(std::string name);
        bool save();

        void prepare();
        void apply();

        dots::world *world;
        std::map<std::string, std::map<std::string, dots::dot>> todo;
    };

    void initialize();
    void create_world(std::string name);

    namespace pixels
    {

        class pixels : public QWidget
        {
            Q_OBJECT

        public:
            explicit pixels(QWidget *parent = nullptr);
            ~pixels();

            void scale(float scale);
            void render();

            void speed(float speed);

            bool save();
            bool load(std::string name);

            dots::handler *handler;

            QLabel *label;
            bool running;
            bool show_borders;

            int speed_val;

        protected:
            void mousePressEvent(QMouseEvent *event) override;

        private slots:
            void start();
        private:

            void add(int x, int y);
            void kill(int x, int y);

            dots::dot to_pixmap_coords(int x, int y);
            dots::dot to_dotmap_coords(int x, int y);

            QPixmap *pixmap;
            QPainter *pixpainter;

            QTimer* timer;

            int rows;
            int columns;
            float scaling;

            bool world_loaded;

        };

    } //namespace PIXELS

}//namespace DOTS

/*
 * Пространство имен IO отвечает за запись JSON сохранений на жесткий диск, а также получения
 * информации о сохранинях.
 */

namespace io
{
    std::string get_worlds_file();
    fs::path get_world_path(std::string filename);
    std::string get_world(std::string filename);
    std::array<std::string, 100> get_world_names();
}//namespace IO


#endif // DOTS_H
