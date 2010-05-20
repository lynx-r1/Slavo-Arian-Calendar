#ifndef KDGRAPHICSBUTTONGROUP_H
#define KDGRAPHICSBUTTONGROUP_H

#include <QGraphicsObject>
#include <QGraphicsWidget>

#include "sagraphicspushbutton.h"

class SAGraphicsButtonGroup : public QGraphicsWidget
{
    Q_OBJECT

public:
    SAGraphicsButtonGroup(QGraphicsItem *parent = 0);

    void addButton(SAGraphicsPushButton *button, int id = -1);
    void removeButton(SAGraphicsPushButton *button);

    SAGraphicsPushButton *checkedButton() const;
    int checkedId() const;

    bool exclusive() const;
    void setExclusive(bool);

    int id(SAGraphicsPushButton *button) const;
    void setId(SAGraphicsPushButton *button, int id);

    SAGraphicsPushButton *button(int id) const;
    QList<SAGraphicsPushButton *> buttons() const;

signals:
    void buttonClicked(SAGraphicsPushButton *button);
    void buttonClicked(int id);

private slots:
    void emitClicked(bool clicked);
    void buttonToggled(bool checked);

private:
    QMap<SAGraphicsPushButton *, int> mPushButtonMap;
    SAGraphicsPushButton *mCheckedButton;
    bool mExclusive;
};

#endif // KDGRAPHICSBUTTONGROUP_H
