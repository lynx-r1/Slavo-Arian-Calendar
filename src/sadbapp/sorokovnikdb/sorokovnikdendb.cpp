#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "sadefines.h"
#include "sorokovnikdendb.h"

SorokovnikDenDB::SorokovnikDenDB()
{
    setJump(1);
}

SorokovnikDenDB::SorokovnikDenDB(const QString &nazvanie, const QString &podskazka, const QColor &cvet)
    : SorokovnikDen(nazvanie, podskazka, cvet)
{
    setJump(1);
}

void SorokovnikDenDB::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

#if defined(Q_OS_UNIX)
    int textAlignFlags = Qt::AlignHCenter | Qt::AlignBottom;
#elif defined(Q_OS_WIN)
    int textAlignFlags = Qt::AlignCenter;
#endif

    QFont saFont(SAFONTFAMILYIZHITSAC, 10);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect);
    painter->setFont(saFont);

    QColor bgColor(backgroundColor());
    bool black = bgColor == Qt::black;
    if (black)
        bgColor = QColor(30, 30, 30);

    QColor lighter1 = bgColor.lighter(black ? 400 : 130);
    QColor lighter2 = bgColor.lighter(black ? 320 : 120);
    QColor darker1 = bgColor.darker(115);
    QColor darker2 = bgColor.darker(125);

    QRectF r = option->rect;
    painter->setPen(bgColor);

    bool mouseOver = option->state & QStyle::State_MouseOver;

    QLinearGradient lgr(r.topLeft(), r.bottomLeft());
    lgr.setColorAt(0, checked() ? mouseOver ? darker1 : darker2 :
                   mouseOver ? lighter1 : lighter2);
    lgr.setColorAt(0.4, checked() ? mouseOver ? lighter1 : lighter2 :
                   mouseOver ? darker1 : darker2);
    lgr.setColorAt(0.6, checked() ? mouseOver ? lighter1 : lighter2 :
                   mouseOver ? darker1 : darker2);
    lgr.setColorAt(1, checked() ? mouseOver ? darker1 : darker2 :
                   mouseOver ? lighter1 : lighter2);

    painter->setBrush(lgr);
    painter->drawRoundedRect(r, 3, 3);

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.2);
    if (!checked())
        painter->drawRoundedRect(r.x(), r.y(), r.width(), r.height() / 2 - 2, 3, 3);

    painter->setOpacity(1);
    painter->setPen(color());
    painter->setBrush(lgr);
    painter->drawText(r.adjusted(3, 5, -2, -2), text());

    // рисуем обозначения дня: Вещие сны, Родительские дни, Дни памяти и почитания предков
    int x = 2;
    QString text;
    if (isVazhnyiPrazdnik()) {
        QColor red(255, 80, 80, 120);
        painter->setPen(Qt::NoPen);
        painter->setBrush(red);
        painter->drawRoundedRect(r.adjusted(2, 2, -2, -2), 2, 2);
    }

    if (isVeshchiiSon()) {
        painter->setPen(QColor(88, 79, 161));
        text = tr("с");
        QRectF rt(QRectF(x, r.height() / 2 + 1, 12, 12));
        painter->drawEllipse(rt);
        painter->drawText(rt, textAlignFlags, text);
        x += 14;
    }
    if (isRoditelskiiDen()) {
        painter->setPen(QColor(1, 169, 103));
        text = tr("р");
        QRectF rt(QRectF(x, r.height() / 2 + 1, 12, 12));
        painter->drawEllipse(rt);
        painter->drawText(rt, textAlignFlags, text);
        x += 14;
    }
    if (isDenPochitaniyaPredkov()) {
        painter->setPen(QColor(180, 30, 30));
        text = tr("п");
        QRectF rt(QRectF(x, r.height() / 2 + 1, 12, 12));
        painter->drawEllipse(rt);
        painter->drawText(rt, textAlignFlags, text);
    }
}

SorokovnikDen *SorokovnikDenDB::clone(const QString &nazvanie, const QString &podskazka,
                                     const QColor &cvet)
{
    return new SorokovnikDenDB(nazvanie, podskazka, cvet);
}
