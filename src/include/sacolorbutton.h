#ifndef SACOLORBUTTON_H
#define SACOLORBUTTON_H

#include <QPushButton>

class SAColorButton : public QPushButton
{
    Q_OBJECT

public:
    SAColorButton(QWidget *parent = 0);
    SAColorButton(QString text, QWidget *parent = 0);

    void setColor(const QColor &color);
    QColor color() const { return mColor; }

private:
    QColor mColor;
};

#endif // SACOLORBUTTON_H
