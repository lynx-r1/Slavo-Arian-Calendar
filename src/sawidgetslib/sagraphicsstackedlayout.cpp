#include <QGraphicsOpacityEffect>
#include <QGraphicsScale>
#include <QGraphicsWidget>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "sagraphicsstackedlayout.h"

SAGraphicsStackedLayout::SAGraphicsStackedLayout(QGraphicsLayoutItem *parent)
    : QGraphicsLayout(parent)
    , mSwitchEasingCurve(QEasingCurve::OutBounce)
    , mSwitchDirection(Top2Bottom)
    , mSwitchDuration(3000)
    , mCurrentWidgetIndex(-1)
    , mSilentSwitch(false)
{
}

SAGraphicsStackedLayout::~SAGraphicsStackedLayout()
{
    qDeleteAll(mItems);
}

void SAGraphicsStackedLayout::setGeometry(const QRectF &rect)
{
    QGraphicsLayout::setGeometry(rect);

    for (int i = 0; i < mItems.size(); ++i) {
        itemAt(i)->setGeometry(rect);
    }
}

QSizeF SAGraphicsStackedLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    QSizeF currentWidgetSize;
    if (mCurrentWidgetIndex != -1)
        currentWidgetSize = itemAt(mCurrentWidgetIndex)->effectiveSizeHint(which, constraint);

    return QSizeF(left + right + currentWidgetSize.width(),
                  right + bottom + currentWidgetSize.height());
}

QGraphicsLayoutItem *SAGraphicsStackedLayout::itemAt(int i) const
{
    if (i < 0 || i >= mItems.size())
        return 0;
    return mItems.at(i);
}

void SAGraphicsStackedLayout::insertItem(QGraphicsLayoutItem *item, int pos)
{
    if(!pos && (mCurrentWidgetIndex == -1)) {
        mCurrentWidgetIndex = 0;
    } else {
        item->graphicsItem()->setOpacity(0.0);
    }

    if (pos <= mCurrentWidgetIndex && mCurrentWidgetIndex < mItems.size() - 1)
        mCurrentWidgetIndex++;

    mItems.insert(pos, item);
    invalidate();
    activate();
}

void SAGraphicsStackedLayout::addItem(QGraphicsLayoutItem *item)
{
    insertItem(item, mItems.size());
}

void SAGraphicsStackedLayout::removeAt(int index)
{
    mItems.removeAt(index);

    if (mCurrentWidgetIndex >= mItems.size())
        mCurrentWidgetIndex = mItems.size() - 1;
}

void SAGraphicsStackedLayout::setCurrentIndex(int index)
{
    QGraphicsItem *currentItem = itemAt(mCurrentWidgetIndex)->graphicsItem();
    QGraphicsItem *hidenItem = itemAt(index)->graphicsItem();

    if (currentItem->opacity() != 1) {
        emit currentIndexChanged(-1);
        return;
    }

    QGraphicsWidget *currentWidget, *hidenWidget;
    if (index != mCurrentWidgetIndex) {
        if ((currentWidget = dynamic_cast<QGraphicsWidget *>(currentItem))
            && (hidenWidget = dynamic_cast<QGraphicsWidget *>(hidenItem))) {
            if (mSilentSwitch) {
                int duration = 100;

                QParallelAnimationGroup *group = new QParallelAnimationGroup;
                QPropertyAnimation *curAnim = new QPropertyAnimation(currentWidget, "opacity", group);
                curAnim->setDuration(duration);
                curAnim->setStartValue(1);
                curAnim->setEndValue(0);

                QPropertyAnimation *hidAnim = new QPropertyAnimation(hidenWidget, "opacity", group);
                hidAnim->setDuration(duration);
                hidAnim->setStartValue(0);
                hidAnim->setEndValue(1);

                connect(group, SIGNAL(finished()), this, SLOT(groupAnimationFinished()));
                group->start(QAbstractAnimation::KeepWhenStopped);
            } else {
                currentWidget->setZValue(0);
                hidenWidget->setZValue(10);
                QParallelAnimationGroup *group = new QParallelAnimationGroup;

                QPropertyAnimation *opacAnimHid = new QPropertyAnimation(hidenWidget, "opacity", group);
                opacAnimHid->setDuration(mSwitchDuration);
                opacAnimHid->setEasingCurve(QEasingCurve::OutExpo);
                opacAnimHid->setStartValue(0);
                opacAnimHid->setEndValue(1);

                QPropertyAnimation *opacAnimCur = new QPropertyAnimation(currentWidget, "opacity", group);
                opacAnimCur->setDuration(mSwitchDuration);
                opacAnimCur->setEasingCurve(QEasingCurve::InExpo);
                opacAnimCur->setStartValue(1);
                opacAnimCur->setEndValue(0);

                QGraphicsScale *curScale = new QGraphicsScale(group);
                currentWidget->setTransformations(QList<QGraphicsTransform *>() << curScale);
                QGraphicsScale *hidScale = new QGraphicsScale(group);
                hidenWidget->setTransformations(QList<QGraphicsTransform *>() << hidScale);

                QPropertyAnimation *scaleCurAnim;
                QPropertyAnimation *scaleHidAnim;
                switch (mSwitchDirection) {
                case Top2Bottom:
                    curScale->setOrigin(QVector3D(currentWidget->boundingRect().bottomLeft()));
                    scaleCurAnim = new QPropertyAnimation(curScale, "yScale", group);
                    scaleHidAnim = new QPropertyAnimation(hidScale, "yScale", group);
                    break;
                case Bottom2Top:
                    hidScale->setOrigin(QVector3D(hidenWidget->boundingRect().bottomLeft()));
                    scaleCurAnim = new QPropertyAnimation(curScale, "yScale", group);
                    scaleHidAnim = new QPropertyAnimation(hidScale, "yScale", group);
                    break;
                case Left2Right:
                    curScale->setOrigin(QVector3D(currentWidget->boundingRect().topRight()));
                    scaleCurAnim = new QPropertyAnimation(curScale, "xScale", group);
                    scaleHidAnim = new QPropertyAnimation(hidScale, "xScale", group);
                    break;
                case Right2Left:
                    hidScale->setOrigin(QVector3D(hidenWidget->boundingRect().topRight()));
                    scaleCurAnim = new QPropertyAnimation(curScale, "xScale", group);
                    scaleHidAnim = new QPropertyAnimation(hidScale, "xScale", group);
                    break;
                default:
                    scaleCurAnim = new QPropertyAnimation();
                    scaleHidAnim = new QPropertyAnimation();
                }

                scaleCurAnim->setDuration(mSwitchDuration);
                scaleCurAnim->setEasingCurve(mSwitchEasingCurve);
                scaleCurAnim->setStartValue(1);
                scaleCurAnim->setEndValue(0);

                scaleHidAnim->setDuration(mSwitchDuration);
                scaleHidAnim->setEasingCurve(mSwitchEasingCurve);
                scaleHidAnim->setStartValue(0);
                scaleHidAnim->setEndValue(1);

                connect(group, SIGNAL(finished()), this, SLOT(groupAnimationFinished()));
                group->start(QAbstractAnimation::DeleteWhenStopped);
            }
        } else {
            currentItem->setOpacity(0.0);
            hidenItem->setOpacity(1.0);
        }

        mCurrentWidgetIndex = index;
        emit currentIndexChanged(index);
    }
}

void SAGraphicsStackedLayout::groupAnimationFinished()
{
    emit currentIndexChanged(mCurrentWidgetIndex);
}
