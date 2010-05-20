#ifndef KDGRAPHICSLABELITEM_H
#define KDGRAPHICSLABELITEM_H

#include <QGraphicsWidget>

class SAGraphicsLabelItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsLabelItem(const QString &podpis, const QFont &shrift = QFont(),
                        int vyiravn = Qt::AlignCenter, const QString &cvet = "#000000",
                        bool fone = false);

    void setText(const QString &text) { mPodpis = text; }

    void setColor(const char *color) { mCvet = color; }
    void setColor(const QColor &color) { mCvet = color.name(); }

    void setFixedWidth(qreal width);
    void setFixedHeight(qreal height);
    void setFixedSize(const QSizeF &size);
    void setFixedSize(qreal width, qreal height);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();

private:
    QString mPodpis;
    QFont mShrift;
    int mVyiravn;
    QString mCvet;
    bool mFone;
};

#endif // KDGRAPHICSLABELITEM_H
