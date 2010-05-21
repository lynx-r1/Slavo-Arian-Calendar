/*****************************************************************************
 *   Copyright (C) 2009-2010  Jack R  <lynx21.12.12@yandex.ru>               *
 *                                                                           *
 *   This file is part of SAKalendar.                                        *
 *                                                                           *
 *   SAKalendar is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

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
