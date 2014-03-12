#pragma once

#include <QObject>
#include <QList>
#include <QPointF>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsItem;
class QTime;
class QTimer;

namespace graphics_item {
    class Grid;
}

namespace game {

    class GameOcean : public QObject
    {
        Q_OBJECT
    public:
        explicit GameOcean(QGraphicsView *view, QObject *parent = 0);

    signals:
        void fishCountChanged(size_t count);
        void sharkCountChanged(size_t count);
        void currentStep(size_t stepNumber);

    public slots:
        void newGame();
        void startGame();
        void setUpTime(float percent);

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
        enum MoveDirections {Left, Right, Top, Bottom,
                             TopLeft, TopRight, BottomLeft, BottomRight};
        using MovePossibilities = QList<QPointF>;
        MovePossibilities findPossibleStep(QGraphicsItem *item);
        QPointF getPos(QGraphicsItem *item, const MoveDirections& direction);
        void fishStep();
        void sharkStep();

        size_t m_Step;
        size_t m_FishCount;
        size_t m_SharkCount;

        QGraphicsView       *m_View;
        QTimer              *m_Timer;
        QGraphicsScene      *m_Scene;
        graphics_item::Grid *m_Grid;
    };

} // namespace game
