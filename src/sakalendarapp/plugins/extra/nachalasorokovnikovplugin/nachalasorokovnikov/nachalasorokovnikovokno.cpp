#include <QGraphicsLinearLayout>

#include "kdgraphicswindow.h"
#include "nachalasorokovnikov.h"

#include "nachalasorokovnikovokno.h"

NachalaSorokovnikovOkno::NachalaSorokovnikovOkno(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    setObjectName("NachalaSorokovnikovOkno");

    KDGraphicsWindow *frame = new KDGraphicsWindow(tr("Начала Сороковников"));
    frame->setCentralWidget(new NachalaSorokovnikov);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->setSpacing(0);
    layout->addItem(frame);
    setLayout(layout);
}
