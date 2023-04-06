//Author: satanisticmicrowave

#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QKeySequence>
#include <QString>
#include <QMessageBox>
#include <QWidget>

#include "dots.h"
#include "configuration.h"
#include "variables.hpp"
#include "language.h"

QT_BEGIN_NAMESPACE
namespace Ui { class window; }
QT_END_NAMESPACE

/*
 * Класс WINDOW отображает на экран окно с игрой.
 */

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();

private slots:
    void newWorld();
    void quit();

    void load();
    void save();

    void aboutQt();

    void on_start_btn_clicked();
    void on_stop_btn_clicked();
    void on_apply_btn_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_english_lang_clicked();

    void on_russian_lang_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    void createActions();
    void createMenus();

    void text();

    QMenu *fileMenu;
    QMenu *worldMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *quitAct;

    QAction *loadAct;
    QAction *saveAct;

    QAction *aboutQtAct;

    dots::pixels::pixels *pixels;
    configuration::configuration config;
    language::language *language;

    Ui::window *ui;
};
#endif // WINDOW_H
