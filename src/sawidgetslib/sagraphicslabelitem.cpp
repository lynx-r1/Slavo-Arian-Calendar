#include <QCoreApplication>
#include <QPainter>

#include "sagraphicslabelitem.h"

SAGraphicsLabelItem::SAGraphicsLabelItem(const QString &nazvanie, const QFont &shrift,
                                         int vyiravn, const QString &cvet, bool fone)
    : mPodpis(nazvanie)
    , mShrift(shrift)
    , mVyiravn(vyiravn)
    , mCvet(cvet)
    , mFone(fone)
{
    setObjectName("theSAGraphicsLabelItem");
}

void SAGraphicsLabelItem::setFixedWidth(qreal width)
{
    setMinimumWidth(width);
    setMaximumWidth(width);
}

void SAGraphicsLabelItem::setFixedHeight(qreal height)
{
    setMinimumHeight(height);
    setMaximumHeight(height);
}

void SAGraphicsLabelItem::setFixedSize(const QSizeF &size)
{
    setMinimumSize(size);
    setMaximumSize(size);
}

void SAGraphicsLabelItem::setFixedSize(qreal width, qreal height)
{
    setFixedSize(QSizeF(width, height));
}

void SAGraphicsLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setFont(mShrift);

    QRectF r = rect();
    r.setWidth(r.width() - 1);

    if (!mFone) {
        painter->setPen(mCvet);
#if defined(Q_OS_UNIX)
        painter->drawText(r.adjusted(0, 5, 0, 0), mVyiravn, mPodpis);
#elif defined(Q_OS_WIN)
        painter->drawText(r.adjusted(0, 0, 0, 0), mVyiravn, mPodpis);
#endif
        return;
    }

    QColor bgColor(mCvet);
    bool black = bgColor == Qt::black;
    if (black)
        bgColor = QColor(30, 30, 30);

    QColor lighter1 = bgColor.lighter(black ? 400 : 200);
    QColor lighter2 = bgColor.lighter(black ? 320 : 120);
    QColor darker1 = bgColor.darker(115);

    QLinearGradient lgr(r.topLeft(), r.bottomLeft());
    lgr.setColorAt(0, lighter1);
    lgr.setColorAt(0.4, darker1);
    lgr.setColorAt(0.6, darker1);
    lgr.setColorAt(1, lighter2);

    painter->setPen(bgColor);
    painter->setBrush(QBrush(lgr));
    painter->drawRoundedRect(r, 3, 3);

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.3);
    painter->drawRoundedRect(r.x(), r.y(), r.width(), r.height() / 2 - 2, 3, 3);

    painter->setPen(mCvet);
    painter->setBrush(QBrush(QColor(mCvet)));
    painter->setOpacity(1);

    QColor textColor(bgColor);
    if (mCvet == "#ffffff")
        textColor = Qt::darkRed;
    else if (mCvet == "#000000")
        textColor = QColor(240, 240, 240);
    else
        textColor =  QColor(mCvet).darker(200);

    painter->setPen(textColor);
#if defined(Q_OS_UNIX)
    painter->drawText(r.adjusted(0, 5, 0, 0), mVyiravn, mPodpis);
#elif defined(Q_OS_WIN)
    painter->drawText(r.adjusted(0, 0, 0, 0), mVyiravn, mPodpis);
#endif
}

void SAGraphicsLabelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
    emit clicked();
}
