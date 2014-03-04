#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Game = new GameOcean(ui->graphicsView, this);

    connect(m_Game, &GameOcean::sharkCountChanged, [this](size_t count){ ui->lbl_sharkCount->setText(QString::number(count));});
    connect(m_Game, &GameOcean::fishCountChanged, [this](size_t count){ ui->lbl_VictimCount->setText(QString::number(count));});
    connect(m_Game, &GameOcean::currentStep, [this](size_t step){ ui->lbl_stepCount->setText(QString::number(step));});

    m_Game->newGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionStart_triggered()
{
    m_Game->startGame();
}

void MainWindow::on_actionNewGame_triggered()
{
    m_Game->newGame();
}
