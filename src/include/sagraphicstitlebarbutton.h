#ifndef KDGRAPHICSTITLEBARBUTTON_H
#define KDGRAPHICSTITLEBARBUTTON_H

#include <QGraphicsWidget>

class SAGraphicsTitleBarButton : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsTitleBarButton(const QColor &color, QGraphicsItem *parent = 0);

    bool checked() const;
    void setChecked(bool checked);

    void setFixedWidth(qreal width);
    void setFixedHeight(qreal height);
    void setFixedSize(const QSizeF &size);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    QPainterPath shape() const;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked(bool);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
    QSize mSize;
    QColor mColor;
    QColor mBackgroundColor;

    bool mChecked;
};

#endif // KDGRAPHICSTITLEBARBUTTON_H
