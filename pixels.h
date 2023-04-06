//Author: satanisticmicrowave

#ifndef PIXELS_H
#define PIXELS_H

#include <QWidget>

#include "variables.hpp"
#include "dots.h"

#define SIZE DOT_SIZE*dot_scale

namespace pixels{

    class pixels : public QWidget
    {
        Q_OBJECT
    public:
        explicit pixels(QWidget *parent = nullptr);
        virtual ~pixels();

        void kill(dots::dot dot);
        void create(dots::dot dot);

        void scale(int scale);
        void redraw();

        void move_camera(int x, int y);

    private:
        int dot_scale = 1;

    };

}// namespace PIXELS

#endif // PIXELS_H
