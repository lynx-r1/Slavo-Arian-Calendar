#include <QGraphicsSceneHoverEvent>
#include <QToolTip>

#include "kd.h"

#include "dennedeli.h"

DenNedeli::DenNedeli()
    : KDGraphicsPushButton()
{
}

DenNedeli::DenNedeli(QString nazvanie, QString podskazka, QColor cvet, int mesyac, bool svyashch)
    : KDGraphicsPushButton(nazvanie, cvet)
    , mPodskazka(podskazka)
    , mCvet(cvet)
    , mMesyac(mesyac)
    , mSvyashch(svyashch)
{
}

void DenNedeli::vzyatInfo(int &mesyac, bool &svyashch)
{
    mesyac = mMesyac;
    svyashch = mSvyashch;
}

void DenNedeli::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QString letoTip = mSvyashch ? tr("Священное Лето") : tr("Простое Лето");
    QString toolTip(tr("<p style='white-space:pre; color :%1'>%2\n"
                       "%3\n%4")
                    .arg(mCvet.darker(200).name())
                    .arg(mPodskazka).arg(KDMesyaci[mMesyac]).arg(letoTip));
    QToolTip::showText(event->screenPos(), toolTip);
    QGraphicsProxyWidget::hoverEnterEvent(event);
}
