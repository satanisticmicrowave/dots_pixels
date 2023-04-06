//Author: satanisticmicrowave

#include "window.h"
#include "io.h"
#include "qevent.h"
#include "ui_window.h"
#include "dots.h"

#include <QFileDialog>

inline bool world_saved = false;
inline bool world_loaded = false;

/*
 * Конструктор класса WINDOW.
 */

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
{
    configuration::load_config(&config);
    language = language::load_language(config.language);

    ui->setupUi(this);

    ui->world_name_edit->setEnabled(false);
    ui->scaling_edit->setEnabled(false);
    ui->speed_edit->setEnabled(false);
    ui->start_btn->setEnabled(false);
    ui->stop_btn->setEnabled(false);

    ui->speed_edit->setValue(config.speed);
    ui->scaling_edit->setValue(config.scaling);

    setWindowTitle(QString::fromStdString(GAME_NAME));

    pixels = new dots::pixels::pixels(ui->centralwidget);

    createActions();
    createMenus();

    float speed, scale;
    scale = (float)ui->scaling_edit->value();
    scale = scale / 100;
    speed = (float)ui->speed_edit->value()/2;
    pixels->scale(scale);
    pixels->speed(speed);
    ui->checkBox->setChecked(config.show_borders);
    text();

}

/*
 * Во избежание утечки памяти
 */

window::~window()
{
    delete ui;

    if(world_loaded)
        this->pixels->save();

    delete pixels;
    delete language;
    delete newAct;
    delete loadAct;
    delete saveAct;
    delete fileMenu;
    delete worldMenu;
    delete helpMenu;
    delete aboutQtAct;
    delete quitAct;
}

/*
 * Устанавливает текст на все нужные части окна
 */

void window::text()
{
    ui->stop_btn->setText(QString::fromStdString(language->get(LANG_STOP_BTN)));
    ui->apply_btn->setText(QString::fromStdString(language->get(LANG_APPLY_BTN)));
    ui->start_btn->setText(QString::fromStdString(language->get(LANG_START_BTN)));
    ui->dots_label->setText(QString::fromStdString(language->get(LANG_DOTS_LABEL)));
    ui->speed_text_label->setText(QString::fromStdString(language->get(LANG_SPEED_TEXT_LABEL)));
    ui->speed_label->setText(QString::fromStdString(language->get(LANG_SPEED_LABEL)));
    ui->scaling_label->setText(QString::fromStdString(language->get(LANG_SCALING_LABEL)));
    ui->world_name_label->setText(QString::fromStdString(language->get(LANG_WORLD_NAME_LABEL)));
    ui->dots_label->setText(QString::fromStdString(language->get(LANG_DOTS_LABEL)));
    ui->checkBox->setText(QString::fromStdString(language->get(LANG_SWITCH_FIELD)));
    newAct->setText(tr(language->get(LANG_WORLD_CREATE).c_str()));
    loadAct->setText(tr(language->get(LANG_WORLD_LOAD).c_str()));
    saveAct->setText(tr(language->get(LANG_WORLD_SAVE).c_str()));
    quitAct->setText(tr(language->get(LANG_GAME_QUIT).c_str()));
    aboutQtAct->setText(tr(language->get(LANG_HELP_ABOUT_QT).c_str()));
    fileMenu->setTitle(tr(language->get(LANG_FILE).c_str()));
    worldMenu->setTitle(tr(language->get(LANG_WORLD).c_str()));
    helpMenu->setTitle(tr(language->get(LANG_HELP).c_str()));
    newAct->setStatusTip(language->get(LANG_WORLD_CREATE_DESC).c_str());
    loadAct->setStatusTip(language->get(LANG_WORLD_LOAD_DESC).c_str());
    saveAct->setStatusTip(language->get(LANG_WORLD_SAVE_DESC).c_str());
    aboutQtAct->setStatusTip(language->get(LANG_HELP_ABOUT_QT).c_str());
}

void window::createActions()
{
    newAct = new QAction(tr(language->get(LANG_WORLD_CREATE).c_str()), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(language->get(LANG_WORLD_CREATE_DESC).c_str());
    connect(newAct, &QAction::triggered, this, &window::newWorld);

    loadAct = new QAction(tr(language->get(LANG_WORLD_LOAD).c_str()), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(language->get(LANG_WORLD_LOAD_DESC).c_str());
    connect(loadAct, &QAction::triggered, this, &window::load);

    saveAct = new QAction(tr(language->get(LANG_WORLD_SAVE).c_str()), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(language->get(LANG_WORLD_SAVE_DESC).c_str());
    connect(saveAct, &QAction::triggered, this, &window::save);

    quitAct = new QAction(tr(language->get(LANG_GAME_QUIT).c_str()), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(language->get(LANG_GAME_QUIT_DESC).c_str());
    connect(quitAct, &QAction::triggered, this, &window::quit);

    aboutQtAct = new QAction(tr(language->get(LANG_HELP_ABOUT_QT).c_str()), this);
    aboutQtAct->setStatusTip(language->get(LANG_HELP_ABOUT_QT).c_str());
    connect(aboutQtAct, &QAction::triggered, this, &window::aboutQt);

}

void window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr(language->get(LANG_FILE).c_str()));
    fileMenu->addAction(newAct);
    fileMenu->addAction(quitAct);

    worldMenu = menuBar()->addMenu(tr(language->get(LANG_WORLD).c_str()));
    worldMenu->addAction(loadAct);
    worldMenu->addAction(saveAct);

    helpMenu = menuBar()->addMenu(tr(language->get(LANG_HELP).c_str()));
    helpMenu->addAction(aboutQtAct);
}

void window::newWorld()
{
    dots::create_world("New world");
    if(pixels->load("New world"))
    {
        world_loaded = true;
        this->setWindowTitle(QString::fromStdString(GAME_NAME + " - " + pixels->handler->world->get_name()));
        ui->world_name_edit->setEnabled(true);
        ui->world_name_edit->setText(QString::fromStdString(pixels->handler->world->get_name()));
        ui->scaling_edit->setEnabled(true);
        ui->speed_edit->setEnabled(true);

        ui->apply_btn->setEnabled(true);
        ui->start_btn->setEnabled(true);
        setStatusTip(language->get(LANG_WORLD_CREATED).c_str());
    }
    else
        world_loaded = false;
}

void window::quit()
{
    if(world_loaded)
        pixels->save();
    configuration::save(&config);
    QMainWindow::close();

}

void window::load()
{
    QFileDialog dialog;
    dialog.setNameFilter("World file (*.dat)");
    dialog.setDirectory(io::get_path((char*)"world/"));
    if (dialog.exec()) {

        fs::path path =  dialog.selectedFiles().first().toStdString();

        if(pixels->load(path.stem()))
        {
            world_loaded = true;

            setStatusTip(language->get(LANG_WORLD_LOADED).c_str());

            ui->world_name_edit->setEnabled(true);
            ui->world_name_edit->setText(QString::fromStdString(pixels->handler->world->get_name()));
            ui->scaling_edit->setEnabled(true);
            ui->speed_edit->setEnabled(true);

            ui->apply_btn->setEnabled(true);
            ui->start_btn->setEnabled(true);

        }
        else
            world_loaded = false;

    }
}

void window::save()
{
    if(pixels->save())
    {
        world_saved = true;
        setStatusTip(language->get(LANG_WORLD_SAVED).c_str());
    }
}

void window::aboutQt()
{
    QMessageBox::aboutQt(this, QString::fromStdString(language->get(LANG_HELP_ABOUT_QT).c_str()));
}


void window::on_start_btn_clicked()
{
    pixels->running = true;
    ui->start_btn->setEnabled(false);
    ui->stop_btn->setEnabled(true);
}


void window::on_stop_btn_clicked()
{
    pixels->running = false;

    ui->dots_value_label->setText(QString::fromStdString(std::to_string(pixels->handler->world->get_dots().size())));

    ui->start_btn->setEnabled(true);
    ui->stop_btn->setEnabled(false);
}

void window::keyPressEvent(QKeyEvent *event)
{
    if (world_loaded)
    {
        if (event->key() == Qt::Key_Space)
        {
            if (pixels->running)
                on_stop_btn_clicked();
            else
                on_start_btn_clicked();
        }
        else if (event->key() == Qt::Key_C)
        {
            std::map<std::string, dots::dot> dots;
            pixels->handler->world->set_dots(dots);
            float scale = (float)ui->scaling_edit->value();
            scale = scale / 100;
            pixels->scale(scale);
            pixels->render();
        }
    }
}

void window::on_apply_btn_clicked()
{
    float speed, scale;
    scale = (float)ui->scaling_edit->value();
    scale = scale / 100;
    speed = (float)ui->speed_edit->value()/2;
    pixels->scale(scale);
    pixels->speed(speed);
    ui->speed_value_label->setText(QString::fromStdString(std::to_string(speed)));
    pixels->handler->world->set_name(ui->world_name_edit->text().toStdString());
    this->setWindowTitle(QString::fromStdString(GAME_NAME + " - " + pixels->handler->world->get_name()));
    pixels->render();

    config.scaling = ui->scaling_edit->value();
    config.speed = ui->speed_edit->value();
    config.show_borders = ui->checkBox->isChecked();

}


void window::on_checkBox_stateChanged(int arg1)
{
    float scale;
    scale = (float)ui->scaling_edit->value();
    scale = scale / 100;
    pixels->show_borders = (bool)arg1;
    pixels->scale(scale);
    pixels->render();
    config.show_borders = !config.show_borders;
}


void window::on_english_lang_clicked()
{
    config.language = "en";
    language = language::load_language("en");
    text();
}


void window::on_russian_lang_clicked()
{
    config.language = "ru";
    language = language::load_language("ru");
    text();
}

