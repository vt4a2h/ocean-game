#include "gameocean.h"

GameOcean::GameOcean(QGraphicsView *view, QObject *parent)
    : QObject(parent)
    , m_Status(NotInProcess)
    , m_View(view)
    , m_Timer(new QTimer(this))
    , m_Scene(new QGraphicsScene(this))
{
    m_View->setScene(m_Scene);
    addGrid();
    makeConnection();
    setupCounters();
}

void GameOcean::newGame()
{
    m_Timer->stop();
    m_Scene->clear();
    addGrid();

    m_Status = NotInProcess;
    m_Timer->setInterval(1000);

    qsrand(QTime::currentTime().msec());

    generateItems(ALGA_COUNT, Alga);
    generateItems(SHARK_COUNT, Shark);
    generateItems(FISH_COUNT,  Fish);

    setupCounters();
}

void GameOcean::startGame()
{
    if (m_Status == NotInProcess) {
        m_Status = InProccess;
        m_Timer->start();
    }
}

void GameOcean::makeAlga(QPointF pos, float margin)
{
    auto alga = m_Scene->addPixmap(QPixmap(":/img/img/alga.png")
                                   .scaled(QSize(CELL_WIDTH - margin, CELL_HEIGHT - margin),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation)
                                   );
    alga->setPos(pos);
    alga->setData(Name, Alga);
}

void GameOcean::makeShark(QPointF pos, float margin)
{
    auto shark = m_Scene->addPixmap(QPixmap(":/img/img/shark.png")
                                    .scaled(QSize(CELL_WIDTH - margin, CELL_HEIGHT - margin),
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation)
                                    );
    shark->setPos(pos);
    shark->setData(Name, Shark);
    shark->setData(Step, 0);
    shark->setData(VictimCount, 0);
}

void GameOcean::makeFish(QPointF pos, float margin)
{
    auto fish = m_Scene->addPixmap(QPixmap(QString(":/img/img/fish_%1.png").arg(QString::number(qrand() % FISH_PIC_COUNT)))
                                   .scaled(QSize(CELL_WIDTH - margin, CELL_HEIGHT - margin),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation)
                                   );
    fish->setPos(pos);
    fish->setData(Name, Fish);
    fish->setData(Step, 0);
}

void GameOcean::generateItems(size_t count, Items itemType)
{
    float margin = graphics_item::Grid::MARGIN;
    QPointF itemPos(m_Grid->boundingRect().topLeft());
    size_t currentCount(0);

    do {
        itemPos.rx() += margin + CELL_WIDTH * (qrand() % CELL_COUNT_H);
        itemPos.ry() += margin + CELL_HEIGHT * (qrand() % CELL_COUNT_V);

        if (m_Scene->itemAt(itemPos.x() + CELL_WIDTH / 2, itemPos.y() + CELL_HEIGHT / 2, m_View->transform()) == m_Grid) {
            switch (itemType) {
            case Fish:
                makeFish(itemPos, margin);
                break;
            case Shark:
                makeShark(itemPos, margin);
                break;
            case Alga:
                makeAlga(itemPos, margin);
                break;
            }
            ++currentCount;
        }

        itemPos = m_Grid->boundingRect().topLeft();
    } while(currentCount != count);
}

void GameOcean::addGrid()
{
    m_Grid = new graphics_item::Grid(CELL_COUNT_H, CELL_COUNT_V, {CELL_WIDTH, CELL_HEIGHT});
    m_Scene->addItem(m_Grid);
}

void GameOcean::oneGameCircle()
{
    fishStep();
    sharkStep();
    m_Scene->update();

    if (m_SharkCount == 0 || m_FishCount == 0) stopGame();
    ++m_Step;

    emit currentStep(m_Step);
}

void GameOcean::makeConnection()
{
    connect(m_Timer, &QTimer::timeout, this, &GameOcean::oneGameCircle);
}

void GameOcean::stopGame()
{
    m_Timer->stop();
    m_Scene->clear();
    m_Scene->addText(tr("Игра окончена! Миру не повезло..."));
    m_Status = NotInProcess;
}

void GameOcean::setupCounters()
{
    m_Step = 0;
    m_SharkCount = SHARK_COUNT;
    m_FishCount = FISH_COUNT;

    emit sharkCountChanged(m_SharkCount);
    emit fishCountChanged(m_FishCount);
    emit currentStep(m_Step);
}

GameOcean::MovePossibilities GameOcean::findPossibleStep(QGraphicsItem *item)
{
    QPointF pos;
    int direction(Left);
    MovePossibilities result;

    do {
        pos = getPos(item, static_cast<MoveDirections>(direction));
        if (pos.x() > m_Grid->boundingRect().left() && pos.y() > m_Grid->boundingRect().top() &&
            pos.x() < m_Grid->boundingRect().right() && pos.y() < m_Grid->boundingRect().bottom())
            if (item->data(Name).toInt() == Fish &&
                m_Scene->itemAt(pos.x() + CELL_WIDTH / 2, pos.y() + CELL_HEIGHT / 2, m_View->transform()) == m_Grid)
                result.append(pos);
            if (item->data(Name).toInt() == Shark)
                if(m_Scene->itemAt(pos.x() + CELL_WIDTH / 2, pos.y() + CELL_HEIGHT / 2, m_View->transform()) == m_Grid ||
                   m_Scene->itemAt(pos.x() + CELL_WIDTH / 2, pos.y() + CELL_HEIGHT / 2, m_View->transform())->data(Name).toInt() == Fish)
                   result.append(pos);
        ++direction;
    } while (direction != BottomRight);

    return result;
}

QPointF GameOcean::getPos(QGraphicsItem *item, const MoveDirections &direction)
{
    QPointF pos(item->pos());

    // TODO: refactore it
    switch (direction) {
    case Left:
        pos.rx() -= CELL_WIDTH;
        break;
    case Right:
        pos.rx() += CELL_WIDTH;
        break;
    case Top:
        pos.ry() -= CELL_HEIGHT;
        break;
    case Bottom:
        pos.ry() += CELL_HEIGHT;
        break;
    case TopLeft:
        pos.rx() -= CELL_WIDTH;
        pos.ry() -= CELL_HEIGHT;
        break;
    case TopRight:
        pos.rx() += CELL_WIDTH;
        pos.ry() -= CELL_HEIGHT;
        break;
    case BottomLeft:
        pos.rx() -= CELL_WIDTH;
        pos.ry() += CELL_HEIGHT;
        break;
    case BottomRight:
        pos.ry() += CELL_HEIGHT;
        pos.rx() += CELL_WIDTH;
    }

    return pos;
}

void GameOcean::fishStep()
{
    for(QGraphicsItem *item : m_Scene->items()) {
        if (item->data(Name).toInt() == Fish) {
            auto p = findPossibleStep(item);
            if (!p.isEmpty()) {
                item->setData(Step, item->data(Step).toInt() + 1);
                if (item->data(Step).toInt() == FISH_GENERATION_STEP) {
                    auto point = p.takeAt(qrand() % p.count());
                    makeFish(point, m_Grid->margin());
                    item->setData(Step, 0);
                    ++m_FishCount;
                    emit fishCountChanged(m_FishCount);
                }
                if (!p.isEmpty()) item->setPos(p[qrand() % p.count()]);
            }
        }
    }
}

void GameOcean::sharkStep()
{
    for(QGraphicsItem *item : m_Scene->items()) {
        if (item->data(Name).toInt() == Shark) {
            auto p = findPossibleStep(item);
            auto lastPos = item->pos();
            if (!p.isEmpty()) {
                auto posWithFish = std::find_if(p.begin(), p.end(), [this](QPointF pos){
                        return m_Scene->itemAt(pos.x() + CELL_WIDTH / 2, pos.y() + CELL_HEIGHT / 2, m_View->transform())->data(Name).toInt() == Fish;});
                if (posWithFish != p.end()) {
                    auto fish = m_Scene->itemAt(posWithFish->x() + CELL_WIDTH / 2, posWithFish->y() + CELL_HEIGHT / 2, m_View->transform());
                    m_Scene->removeItem(fish);
                    delete fish;
                    --m_FishCount;
                    emit fishCountChanged(m_FishCount);

                    item->setData(VictimCount, item->data(VictimCount).toInt() + 1);
                    item->setData(Step, 0);
                    item->setPos(*posWithFish);
                } else {
                    item->setPos(p[qrand() % p.count()]);
                }

            }

            if (item->data(VictimCount).toInt() == SHARK_GENERATION_STEP) {
                makeShark(lastPos, m_Grid->margin());
                item->setData(VictimCount, 0);
                ++m_SharkCount;
                emit sharkCountChanged(m_SharkCount);
            }

            item->setData(Step, item->data(Step).toInt() + 1);
            if (item->data(Step).toUInt() >= SHARK_DEATH_STEP) {
                m_Scene->removeItem(item);
                delete item;
                --m_SharkCount;
                emit sharkCountChanged(m_SharkCount);
            }
        }
    }
}
