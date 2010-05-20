#ifndef DK4LINEEDIT_H
#define DK4LINEEDIT_H

#include <QLineEdit>


class DK4KZhizniLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    DK4KZhizniLineEdit(QWidget *parent = 0);

    QColor textColor() const {
        return palette().color(QPalette::Text);
    }
    void setTextColor(const QColor &textColor) {
        QPalette p = palette();
        p.setColor(QPalette::Text, textColor);
        setPalette(p);
    }
    // эта часть нужна для того, чтобы исправить фичу из-за которой в Linux шрифт
    // рисуется не по центру
#if defined(Q_OS_UNIX)
    void paintEvent(QPaintEvent *);

protected:
    void timerEvent(QTimerEvent *);
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private:
    bool mShowCursor;
    bool mHasFocus;
#endif
};

#endif // DK4LINEEDIT_H
