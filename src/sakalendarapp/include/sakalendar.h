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

#ifndef KALYADYIDAR_H
#define KALYADYIDAR_H

#include <QDir>

#include "ui/ui_sakalendar.h"

QT_FORWARD_DECLARE_CLASS(QGraphicsItemGroup)
QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

QT_FORWARD_DECLARE_CLASS(SAGraphicsItemGroup)
QT_FORWARD_DECLARE_CLASS(SAGraphicsPushButton)
QT_FORWARD_DECLARE_CLASS(SAGraphicsToolBar)
QT_FORWARD_DECLARE_CLASS(SAKSettingsDialog)

class SAKalendar : public QMainWindow, private Ui::SAKalendar
{
    Q_OBJECT

public:
    SAKalendar(QWidget *parent = 0);
    ~SAKalendar();

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void showAbout();
    void toggleVisiblePlugin(bool visible);
    void updateStaticItemsPos(const QRectF &rect);
    void resetItemPos();
    void dertyWidgetPos();
    void cleanWidgetPos();

signals:
    void triggeredWidgetVisibility(QGraphicsWidget *widget, bool visible);

private:
    void createToolBar();
    void createSAKalendarView();
    void createSAKalendarLogo();
    void setupConnections();

    void readSettings();
    void writeSettings();

    void loadPlugins();
    void addPlugin(QGraphicsWidget *plugin, const QPixmap &pixmap, const QString &pluginName);

private:
    QGraphicsView *mSAKalendarView;
    QGraphicsScene *mSAKalendarScene;

    QDir mPluginsDir;
    QStringList mPluginNames;
    QList<QGraphicsWidget *> mPluginsList;

    SAGraphicsItemGroup *mKalyadnikLogo;

    QStateMachine *mStateMachine;
    QState *mGroupStates;

    SAGraphicsToolBar *mMainToolBar;
    SAGraphicsPushButton *mRangeWidgetButton;
    SAGraphicsPushButton *mSettingsButton;
    SAGraphicsPushButton *mAboutButton;
    SAGraphicsPushButton *mQuitButton;

    SAKSettingsDialog *mSettingsDialog;

    bool mDertyWidgetPos;
};

#endif // KALYADYIDAR_H
