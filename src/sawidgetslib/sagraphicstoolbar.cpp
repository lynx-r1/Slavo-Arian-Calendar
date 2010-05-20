#include <QGraphicsEffect>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "sagraphicstoolbar.h"

SAGraphicsToolBar::Separator::Separator(Qt::Orientation orientation, const QColor &color,
                                        SAGraphicsToolBar *parent)
    : QGraphicsWidget(parent)
    , mOrientation(orientation)
    , mColor(color)
{
    if (mOrientation == Qt::Vertical)
        setMaximumHeight(4);
    else
        setMaximumWidth(4);
}

void SAGraphicsToolBar::Separator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);

    painter->setPen(QPen(mColor, 1, Qt::SolidLine, Qt::RoundCap));
    if (mOrientation == Qt::Vertical) {
        int yCenter = rect().center().y();
        painter->drawLine(2, yCenter, rect().width() - 2, yCenter);
    } else {
        int xCenter = rect().center().x();
        painter->drawLine(xCenter, 2, xCenter, rect().height() - 2);
    }

    QGraphicsWidget::paint(painter, option, widget);
}


SAGraphicsToolBar::SAGraphicsToolBar(const QColor &color, QGraphicsWidget *parent)
    : QGraphicsWidget(parent)
    , mLayout(new QGraphicsLinearLayout(this))
    , mColor(color)
{
    mBlurEffect = new QGraphicsBlurEffect;
    mBlurEffect->setBlurRadius(0);

    setAcceptHoverEvents(true);
    setGraphicsEffect(mBlurEffect);
}

void SAGraphicsToolBar::addWidget(QGraphicsWidget *widget)
{
    mLayout->addItem(widget);
}

void SAGraphicsToolBar::addSeparator()
{
    mLayout->addItem(new Separator(mLayout->orientation()));
}

Qt::Orientation SAGraphicsToolBar::orientation() const
{
    return mLayout->orientation();
}

void SAGraphicsToolBar::setOrientation(Qt::Orientation orientation)
{
    mLayout->setOrientation(orientation);
}

void SAGraphicsToolBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    painter->setBrush(mColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rect(), 4, 4);

    QGraphicsWidget::paint(painter, option, widget);
}

void SAGraphicsToolBar::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (opacity() == 0.1) {
        int duration = 500;
        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        QPropertyAnimation *animOpcity = new QPropertyAnimation(this, "opacity", group);
        animOpcity->setDuration(duration);
        animOpcity->setEndValue(1);

        if (mBlurEffect->blurRadius() == 9) {
            QPropertyAnimation *animBlur = new QPropertyAnimation(mBlurEffect, "blurRadius", group);
            animBlur->setDuration(duration);
            animBlur->setEndValue(0);
        }

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

    QGraphicsWidget::hoverEnterEvent(event);
}

void SAGraphicsToolBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (opacity() == 1) {
        int duration = 500;
        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        QPropertyAnimation *animOpcity = new QPropertyAnimation(this, "opacity", group);
        animOpcity->setDuration(duration);
        animOpcity->setEndValue(0.1);

        QPropertyAnimation *animBlur = new QPropertyAnimation(mBlurEffect, "blurRadius", group);
        animBlur->setDuration(duration);
        animBlur->setEndValue(9);

        group->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QGraphicsWidget::hoverEnterEvent(event);
}
