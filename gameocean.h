#ifndef GAMEOCEAN_H
#define GAMEOCEAN_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QList>

#include "grid.h"

class GameOcean : public QObject
{
    Q_OBJECT
public:
    enum ModellingStatus {InProccess, NotInProcess};

    explicit GameOcean(QGraphicsView *view, QObject *parent = 0);

signals:
    void fishCountChanged(size_t count);
    void sharkCountChanged(size_t count);
    void currentStep(size_t stepNumber);
    void stopModeling();

public slots:
    void newGame();
    void startGame();

private:
    // NOTE: mb stringlist
    enum Items {Alga, Shark, Fish};
    enum Data {Name, Step, VictimCount};

    void makeAlga(QPointF pos, float margin);
    void makeShark(QPointF pos, float margin);
    void makeFish(QPointF pos, float margin);

    void generateItems(size_t count, Items itemType);
    void addGrid();

    void oneGameCircle();
    void makeConnection();
    void stopGame();
    void setupCounters();

    // NOTE: mb dict
    enum MoveDirections {Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight};
    using MovePossibilities = QList<QPointF>;
    MovePossibilities findPossibleStep(QGraphicsItem *item);
    QPointF getPos(QGraphicsItem *item, const MoveDirections& direction);
    void fishStep();
    void sharkStep();

    static constexpr size_t ALGA_COUNT       = 50;
    static constexpr size_t SHARK_COUNT      = 30;
    static constexpr size_t FISH_COUNT       = 30;
    static constexpr size_t FISH_PIC_COUNT   = 5;

    static constexpr size_t FISH_GENERATION_STEP  = 1;
    static constexpr size_t SHARK_GENERATION_STEP = 2;
    static constexpr size_t SHARK_DEATH_STEP      = 2;

    static constexpr size_t CELL_COUNT_H = 20;
    static constexpr size_t CELL_COUNT_V = 20;
    static constexpr float  CELL_HEIGHT  = 64;
    static constexpr float  CELL_WIDTH   = 64;

    ModellingStatus m_Status;
    size_t m_Step;
    size_t m_FishCount;
    size_t m_SharkCount;

    QGraphicsView       *m_View;
    QTimer              *m_Timer;
    QGraphicsScene      *m_Scene;
    graphics_item::Grid *m_Grid;
};

#endif // GAMEOCEAN_H
