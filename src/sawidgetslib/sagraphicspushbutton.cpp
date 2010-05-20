#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOptionGraphicsItem>

#include "sadefineslib.h"
#include "sagraphicspushbutton.h"

SAGraphicsPushButton::SAGraphicsPushButton()
    : QGraphicsWidget()
    , mPixmap(QSize(0, 0))
    , mBackgroundColor(QColor())
    , mText("")
    , mGroup(0)
    , mJump(1.4)
    , mCheckable(false)
    , mChecked(false)
    , mUnderline(false)
{
    setObjectName("theSAGraphicsPushButton");
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
}

SAGraphicsPushButton::SAGraphicsPushButton(const QString &text, const QColor &color,
                                           const QColor &textColor, QGraphicsItem *parent)
                                               : QGraphicsWidget(parent)
                                               , mPixmap(QSize(0, 0))
                                               , mBackgroundColor(color)
                                               , mText(text)
                                               , mGroup(0)
                                               , mJump(1.4)
                                               , mCheckable(false)
                                               , mChecked(false)
                                               , mUnderline(false)
{
    setObjectName("theSAGraphicsPushButton");
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);

    mColor = textColor;
    if (!mColor.isValid()) {
        if (color == Qt::white)
            mColor = Qt::darkRed;
        else if (color == Qt::black)
            mColor = QColor(240, 240, 240);
        else
            mColor =  color.darker(200);
    }
}

void SAGraphicsPushButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setClipRect(option->exposedRect);

    QColor bgColor(mBackgroundColor);
    bool black = bgColor == Qt::black;
    if (black)
        bgColor = QColor(30, 30, 30);

    QColor lighter1 = bgColor.lighter(black ? 400 : 180);
    QColor lighter2 = bgColor.lighter(black ? 340 : 140);
    QColor lighter3 = bgColor.lighter(black ? 200 : 110);
    QColor darker1 = bgColor.darker(black ? 200 : 120);

    // внешняя часть кнопки
    bool mouseOver = option->state & QStyle::State_MouseOver;
    int round = 3;
    QRectF r = option->rect;
    QLinearGradient grad(r.topLeft(), r.bottomLeft());
    grad.setColorAt(mChecked ? 1 : 0, mouseOver ? lighter1 : lighter2);
    grad.setColorAt(mChecked ? 0 : 1, mouseOver ? lighter3 : darker1);
    painter->setPen(darker1);
    painter->setBrush(grad);
    painter->drawRoundedRect(r, round, round);

    // эффект стекла
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.1);
    painter->drawRoundedRect(r.adjusted(0, 0, 0, -r.height() / 2 + 2), round, round);
    painter->setOpacity(1);

    // внутренняя часть кнопки
    grad.setColorAt(mChecked ? 1 : 0, mouseOver ? lighter3 : darker1);
    grad.setColorAt(mChecked ? 0 : 1, mouseOver ? lighter1 : lighter2);
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    if (mChecked)
        painter->translate(1, 1);
    painter->drawRoundedRect(r.adjusted(1.5, 1.5, -1.5, -1.5), round, round);

    // эффект стекла
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0.01));
    painter->setOpacity(0.1);
    painter->drawRoundedRect(r.adjusted(1.5, r.height() / 2 + 2, -3, 0), round, round);
    painter->setOpacity(1);

    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    if (!mPixmap.isNull())
        painter->drawPixmap(r.adjusted(1.5, 1.5, -1.5, -1.5), mPixmap, mPixmap.rect());

    QFont saFont(SAFONTFAMILYIZHITSAC);
    saFont.setUnderline(mUnderline);

    painter->setFont(saFont);
    painter->setPen(mColor);
#if defined(Q_OS_UNIX)
    painter->drawText(r.adjusted(3, 5, -2, -2), mText);
#elif defined(Q_OS_WIN)
    painter->drawText(r.adjusted(3, 3, -2, -2), mText);
#endif
}

void SAGraphicsPushButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (zValue() == 0 && mJump != 1) {
        // TODO: переместить в другое место, достаточно один раз установить
        setTransformOriginPoint(boundingRect().center());

        mScaleAnimation = new QPropertyAnimation(this, "scale");
        mScaleAnimation->setEndValue(mJump);

        mZValueAnimation = new QPropertyAnimation(this, "zValue");
        mZValueAnimation->setEndValue(100);

        mScaleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        mZValueAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    QGraphicsWidget::hoverEnterEvent(event);
}

void SAGraphicsPushButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (mJump != 1) {
        mScaleAnimation = new QPropertyAnimation(this, "scale");
        mScaleAnimation->setEndValue(1);

        mZValueAnimation = new QPropertyAnimation(this, "zValue");
        mZValueAnimation->setEndValue(0);

        mScaleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        mZValueAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        update();
    }

    QGraphicsWidget::hoverLeaveEvent(event);
}

void SAGraphicsPushButton::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if (!mCheckable)
        mChecked = true;

    update();
}

void SAGraphicsPushButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    mChecked = mCheckable ? !mChecked : false;

    emit toggled(mChecked);
    emit clicked(mChecked);
    emit clicked();
    update();
}
