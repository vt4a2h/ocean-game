#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
    class MainWindow;
}

namespace game {
    class GameOcean;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QScopedPointer<game::GameOcean> m_Game;
};

#endif // MAINWINDOW_H
