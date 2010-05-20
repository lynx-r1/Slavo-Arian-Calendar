#include <QDebug>
#include <QCompleter>
#include <QRegExpValidator>

#include "sorokovniklineedit.h"

SorokovnikLineEdit::SorokovnikLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , mGreating(true)
{
    mGreating = tr("Перейти на дату");
    setText(mGreating);
    setToolTip(tr("Введите дату. Например: 22 Айлетъ 7519"));
//    setPlaceholderText(mGreating);

    mSorokovniki[tr("Рамхатъ")] = 1;
    mSorokovniki[tr("Айлетъ")] = 2;
    mSorokovniki[tr("Бэйлетъ")] = 3;
    mSorokovniki[tr("Гэйлетъ")] = 4;
    mSorokovniki[tr("Дайлетъ")] = 5;
    mSorokovniki[tr("Эйлетъ")] = 6;
    mSorokovniki[tr("Вэйлетъ")] = 7;
    mSorokovniki[tr("Хэйлетъ")] = 8;
    mSorokovniki[tr("Тайлетъ")] = 9;

    QRegExp rx("(([0-9]|[0-3][0-9]|4[01])\\s(Рамхатъ|Айлетъ|Бэйлетъ|Гэйлетъ|Дайлетъ|"
               "Эйлетъ|Вэйлетъ|Хэйлетъ|Тайлетъ)\\s(\\d{1,4}(\\sлет[ао])?))");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    setValidator(validator);

    connect(this, SIGNAL(returnPressed()), this, SLOT(parseDate()));
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(highlite(QString)));
}

void SorokovnikLineEdit::getDate(int &den, int &sorokovnik, int &leto)
{
    den = mDen;
    sorokovnik = mSorokovnik;
    leto = mLeto;
}

void SorokovnikLineEdit::focusInEvent(QFocusEvent *)
{
    if (text() == mGreating)
        clear();
}

void SorokovnikLineEdit::focusOutEvent(QFocusEvent *)
{
    if (text().isEmpty()) {
        setText(mGreating);
        setCursorPosition(0);
        setStyleSheet("background-color: white");
    }
}

void SorokovnikLineEdit::parseDate()
{
    QString t = text();
    QStringList list = t.split(" ");
    if (list.isEmpty() || list.count() < 3)
        return;

    mDen = list.at(0).toInt();
    mSorokovnik = mSorokovniki[list.at(1)];
    mLeto = list.at(2).toInt();
}

void SorokovnikLineEdit::highlite(const QString &text)
{
    QString t = text;
    int pos = 0;
    QValidator::State state = validator()->validate(t, pos);
    if (state == QValidator::Acceptable) {
        setStyleSheet("background-color: #bbbbff");
    } else {
        setStyleSheet("background-color: #ffbbbb");
    }
}
