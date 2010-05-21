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

#include <QAction>
#include <QApplication>
#include <QBitmap>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QLabel>
#include <QLayout>

#include "sachasyappsettingsdialog.h"
#include "sadefines.h"
#include "sadate_cpp.h"
#include "sachasy.h"

#include "sachasyapp.h"

SAChasyApp::SAChasyApp(QWidget *parent)
    : QGraphicsView(parent)
{
    setWindowIcon(QIcon(":/SAKalendar/SAChasyApp/icons/sachasy.png"));

//    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setWindowTitle(tr("Часы"));
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

//    QPalette p = viewport()->palette();
//    p.setColor(QPalette::Base, Qt::transparent);
//    viewport()->setPalette(p);

    createSAChasy();
    createActions();
    readSettings();
}

void SAChasyApp::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        move(e->globalPos() - mDragPosition);
        e->accept();
    }

    QGraphicsView::mouseMoveEvent(e);
}

void SAChasyApp::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mDragPosition = e->globalPos() - frameGeometry().topLeft();
        e->accept();
    }

    QGraphicsView::mouseMoveEvent(e);
}

void SAChasyApp::resizeEvent(QResizeEvent *e)
{
    int side = qMin(width(), height());
    QBitmap mask(side, side);
    mask.fill(Qt::color0);

    QPainter p(&mask);
    p.setRenderHints(p.renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    p.setPen(Qt::color1);
    p.setBrush(Qt::color1);
    p.drawEllipse(QPointF(side / 2, side / 2), side / 2 - 3, side / 2 - 3);

    setMask(mask);

    QGraphicsView::resizeEvent(e);
}

void SAChasyApp::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

void SAChasyApp::showAbout()
{
    QString about = tr("<b>Славяно-Арийские часы (Версия %1)</b><br>"
                       "Программа показывает Славяно-Арийское время и дату(опц.).<br><br>"
                       "Программа написана на Qt %2.<br><br>"
                       "<b>Разработчик</b><br>"
                       "<span style='white-space:pre'>    Рысь (lynx_r)</span>")
            .arg(QCoreApplication::applicationVersion()).arg(qVersion());

    QMessageBox::about(this, tr("О программе Славяно-Арийские часы"), about);
}

void SAChasyApp::readSettings()
{
    QSettings s;
    QPoint screenCenter = qApp->desktop()->screenGeometry().center() - rect().center();
    move(s.value("/SAKalendar/SAChasyApp/pos", screenCenter).toPoint());
}

void SAChasyApp::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SAChasyApp/pos", pos());
}

void SAChasyApp::createActions()
{
    mSettingsAction = new QAction(tr("Настройки"), this);
    mSettingsAction->setShortcut(tr("Ctrl+P"));
    connect(mSettingsAction, SIGNAL(triggered()), mSettingsDialog, SLOT(show()));
    addAction(mSettingsAction);

    mAboutAction = new QAction(tr("О программе Славяно-Арийские часы"), this);
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    addAction(mAboutAction);

    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);

    mQuitAction = new QAction(tr("Выход"), this);
    mQuitAction->setShortcut(tr("Ctrl+Q"));
    connect(mQuitAction, SIGNAL(triggered()), this, SLOT(close()));
    addAction(mQuitAction);
}

void SAChasyApp::createSAChasy()
{
    QSettings s;
    s.beginGroup("/SAKalendar/SAChasyApp");
    bool pravSutKrug = s.value("PravSutKrug", true).toBool();
    bool showDoly = s.value("ShowDoly", true).toBool();
    bool ignoreDST = s.value("IgnoreDST", true).toBool();
    bool simpleView = s.value("SimpleView", false).toBool();
    bool showDate = s.value("ShowDate", false).toBool();
    bool stayOnTop = s.value("StayOnTop", false).toBool();

    int updateInterval = s.value("UpdateInterval", 16).toInt();
    int sideSize = s.value("SideSize", 270).toInt();

    QColor bgColor = s.value("BackgroundColor", QColor(10, 120, 10, 200)).value<QColor>();
    QColor shadowColor = s.value("ShadowColor", QColor(Qt::blue)).value<QColor>();
    s.endGroup();

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setColor(shadowColor);
    shadow->setOffset(2);
    shadow->setBlurRadius(12);

    mSAChasy = new SAChasy;
    mSAChasy->setPravSutKrug(pravSutKrug);
    mSAChasy->setFixedSize(QSizeF(sideSize, sideSize));
    mSAChasy->setDolevayaStrelka(showDoly);
    mSAChasy->setIgnoreDST(ignoreDST);
    mSAChasy->setSimpleView(simpleView);
    mSAChasy->setShowDate(showDate);
    mSAChasy->setUpdateInterval(updateInterval);
    mSAChasy->setBackgroundColor(bgColor);
    mSAChasy->setGraphicsEffect(shadow);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->addItem(mSAChasy);

    setScene(scene);
    setWindowFlags(stayOnTop ? windowFlags() | Qt::WindowStaysOnTopHint
        : windowFlags() ^ Qt::WindowStaysOnTopHint);

    mSettingsDialog = new SAChasyAppSettingsDialog(mSAChasy, shadow, this);
    resize(sideSize, sideSize);

    setToolTip(mSAChasy->toolTip());
}
