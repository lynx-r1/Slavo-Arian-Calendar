#include <QPainter>

#include "sacolorbutton.h"

SAColorButton::SAColorButton(QWidget *parent)
    : QPushButton(parent)
{
}

SAColorButton::SAColorButton(QString text, QWidget *parent)
    : QPushButton(text, parent)
{
}

void SAColorButton::setColor(const QColor &color)
{
    mColor = color;

    QPixmap pixmap(iconSize());
    pixmap.fill(QColor(0, 0, 0, 0));

    QRect rect(1, 1, iconSize().width() - 2, iconSize().height() - 2);

    // draw a slightly rounded rectangle
    QPainter p(&pixmap);
    p.setPen(Qt::NoPen);
    p.setRenderHints(p.renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setBrush(color);
    p.drawRoundedRect(rect, 4, 4);
    p.end();

    // set this pixmap as icon
    setIcon(QIcon(pixmap));
}
