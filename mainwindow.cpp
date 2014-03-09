#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameocean.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Game.reset(new game::GameOcean(ui->graphicsView, this));

    connect(ui->actionStart, &QAction::triggered,
            m_Game.data(), &game::GameOcean::startGame);
    connect(ui->actionNewGame, &QAction::triggered, [this]() {
            m_Game->newGame();
            ui->hs_Speed->setValue(ui->hs_Speed->minimum());
    });
    connect(ui->hs_Speed, &QSlider::valueChanged,
            m_Game.data(), &game::GameOcean::setUptime);

    connect(m_Game.data(), &game::GameOcean::sharkCountChanged, [this](size_t count){
        ui->lbl_sharkCount->setText(QString::number(count));
    });
    connect(m_Game.data(), &game::GameOcean::fishCountChanged, [this](size_t count){
        ui->lbl_VictimCount->setText(QString::number(count));
    });
    connect(m_Game.data(), &game::GameOcean::currentStep, [this](size_t step){
        ui->lbl_stepCount->setText(QString::number(step));
    });

    m_Game->newGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}
