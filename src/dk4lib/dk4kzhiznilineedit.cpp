#include <QCommonStyle>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QValidator>

#include "sadefines.h"
#include "dk4kzhiznilineedit.h"

DK4KZhizniLineEdit::DK4KZhizniLineEdit(QWidget *parent)
    : QLineEdit(parent)
#if defined(Q_OS_UNIX)
    , mShowCursor(false)
#endif
{
    QRegExp rx("(180|1[0-7]\\d|\\d\\d?)");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setMaximumWidth(50);
    setFrame(false);
    setValidator(validator);
    setStyle(new QCommonStyle);
    setStyleSheet("background: transparent");
    setFont(QFont(SAFONTFAMILYIZHITSAC, 16));
#if defined(Q_OS_UNIX)
    setTextMargins(0, 6, 0, 0);
#endif

    setCursorPosition(-1);

    startTimer(500);
}

#if defined(Q_OS_UNIX)
void DK4KZhizniLineEdit::paintEvent(QPaintEvent *pe)
{
    QLineEdit::paintEvent(pe);

    QPainter p(this);
    if (mHasFocus && mShowCursor) {
        p.setPen(QPen(textColor(), 1));
        p.drawLine(cursorRect().topRight() - QPoint(4, 7), cursorRect().bottomRight() - QPoint(4, 2));
    }
}

void DK4KZhizniLineEdit::timerEvent(QTimerEvent *)
{
    if (mHasFocus) {
        mShowCursor = !mShowCursor;
        update();
    }
}

void DK4KZhizniLineEdit::focusInEvent(QFocusEvent *)
{
    mHasFocus = true;
    update();
}

void DK4KZhizniLineEdit::focusOutEvent(QFocusEvent *)
{
    mHasFocus = false;
    update();

    if (isModified())
        emit editingFinished();
}
#endif
