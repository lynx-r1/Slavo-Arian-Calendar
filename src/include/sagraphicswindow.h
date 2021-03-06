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

#ifndef KDGRAPHICSWINDOW_H
#define KDGRAPHICSWINDOW_H

#include <QGraphicsWidget>
#include <QGraphicsEffect>

QT_FORWARD_DECLARE_CLASS(QState)
QT_FORWARD_DECLARE_CLASS(QStateMachine)
QT_FORWARD_DECLARE_CLASS(QGraphicsBlurEffect)
QT_FORWARD_DECLARE_CLASS(QGraphicsDropShadowEffect)
QT_FORWARD_DECLARE_CLASS(QGraphicsLinearLayout)
QT_FORWARD_DECLARE_CLASS(QGraphicsOpacityEffect)
QT_FORWARD_DECLARE_CLASS(QGraphicsProxyWidget)

QT_FORWARD_DECLARE_CLASS(SAGraphicsLabelItem)
QT_FORWARD_DECLARE_CLASS(SAGraphicsTitleBarButton)

class SAGraphicsWindowBase : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsWindowBase(const QString &title, QGraphicsItem *parent = 0);

    QColor titleColor() const { return mTitleColor; }
    void setTitleColor(QColor color) { mTitleColor = color; }

    QGraphicsWidget *centralWidget() const;
    void setCentralWidget(QGraphicsWidget *widget);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void addGraphicsEffect(QGraphicsEffect *eff);

    qreal opacityEffect() const;
    void setOpacityEffect(qreal opacity);

    qreal blurRadiusEffect() const;
    void setBlurRadiusEffect(qreal radius);

    QGraphicsWidget *widget() { return mPrevParent; }

public slots:
    void showMinimized();
    void showNormal();
    void showInfo(bool show, const QString &html);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
    void setInfoHtml();

private:
    void setInfoHtml(const QString &html);
    bool minimized() const;
    QGraphicsLayout *createTitleBar();

private:
    QStateMachine *mStateMachine;

    QGraphicsLinearLayout *mMainLayout;

    QGraphicsWidget *mCentralWidget;
    QGraphicsWidget *mPrevParent;
    QGraphicsWidget *mWidgetWithEffects;

    QGraphicsProxyWidget *mInfoWidget;

    QString mTitle;

    QColor mTitleColor;

    QGraphicsOpacityEffect mOpacityEffect;
    QGraphicsDropShadowEffect mDropShadowEffect;
    QGraphicsBlurEffect mBlurEffect;

    QString mInfoHtml;

    SAGraphicsTitleBarButton *mMinimizeTitleButton;
    SAGraphicsLabelItem *mWindowTitle;

    bool mFirstWidgetWithEffects;
};


// ------------------------------------------------------------------------- //
class SAGraphicsWindow : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsWindow(const QString &title, bool effects = true,
                     QGraphicsWidget *parent = 0);

    QColor titleColor() const { return mWindow->titleColor(); }
    void setTitleColor(QColor color) { mWindow->setTitleColor(color); }

    void setCentralWidget(QGraphicsWidget *widget);
    void createAnimatedStateSelected(QState *parent, QStateMachine *machine);

    void update(const QRectF &rect);
    void update(qreal x, qreal y, qreal width, qreal height);

    qreal opacityEffect() const { return mWindow->opacityEffect(); }
    void setOpacityEffect(qreal opacity) { mWindow->setOpacityEffect(opacity); }

    qreal blurRadiusEffect() const { return mWindow->blurRadiusEffect(); }
    void setBlurRadiusEffect(qreal radius) { mWindow->setBlurRadiusEffect(radius); }

public slots:
    void showInfo(bool show, const QString &html);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void pressed();

private:
    QGraphicsBlurEffect mBlurEffect;
    QGraphicsOpacityEffect mOpacityEffect;
    QGraphicsDropShadowEffect mDropShadowEffect;

    SAGraphicsWindowBase *mWindow;

    bool mEffects;
};

#endif // KDGRAPHICSWINDOW_H
