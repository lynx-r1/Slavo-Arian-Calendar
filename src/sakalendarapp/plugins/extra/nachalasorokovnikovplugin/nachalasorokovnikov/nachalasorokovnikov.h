#ifndef NOVOLETIYA_H
#define NOVOLETIYA_H

#include <QGraphicsWidget>
#include <QButtonGroup>

class QGraphicsGridLayout;

class NachalaSorokovnikov : public QGraphicsWidget
{
    Q_OBJECT

public:
    NachalaSorokovnikov(QGraphicsItem *parent = 0);

    void vzyatInfo(int &mesyac, int &nachDen, bool &svyashchLeto);
    void ustanovitNovoletie(int novoletie);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

public slots:
    void vyibratNovoletie(int denNedeliNovoletiya);

signals:
    void vyibranoNovoletie(int mesyac, int pervDen, bool svyashch);

private slots:
    void test(int, int, bool);

private:
    QGraphicsLayout *shemaNachalaSorokovnikov();
    QGraphicsWidget *dobavitDenNedeli(QSizeF size, QString den, int nom,
                                      int mesyac, bool svyashch = false);

    QButtonGroup mDniNachMesButtonGroup;

    int mNachSorok;
};

#endif // NOVOLETIYA_H
