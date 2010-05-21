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

#include <QDataWidgetMapper>
#include <QDebug>
#include <QGraphicsEffect>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSettings>

#include "sadefines.h"

#include "prazdniki_db.h"
#include "opisanielet_db.h"

#include "sadate_cpp.h"

#include "sorokovnikdendb.h"
#include "sorokovnik.h"

#include "dk4letodb.h"
#include "dk4.h"

#include "sadbgui.h"

SADBGui::SADBGui(QWidget *parent) :
        QMainWindow(parent)
        , mKZhizni(-1)
        , mDen(-1)
        , mSorok(-1)
        , mRangDnya(0)
        , mMessageTimeout(4000)
{
    setupUi(this);
    readSettings();
    setWindowIcon(QIcon(":/SAKalendar/SADBApp/icons/sadb.png"));

    init();
    createSorokovnik();
    createDK4();
    setupConnections();
}

void SADBGui::closeEvent(QCloseEvent *e)
{
    writeSettings();
    e->accept();
}

// ----------------------------- private slots ----------------------------- //
void SADBGui::daySelected(int den, int sorokovnik)
{
    if (den == -1) {
        mDen = -1;
        mSorok = -1;
        resetInfoWidgets();
        return;
    }

    QSqlDatabase prazdnikiDB = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    if (!prazdnikiDB.isOpen()) {
        mStatusBar->showMessage(tr("Откройте базу данных"), mMessageTimeout);
        return;
    }

    mDen = den;
    mSorok = sorokovnik;

    QSqlQuery q(prazdnikiDB);
    q.prepare(QString("SELECT count FROM %1 WHERE id=?").arg(mTables.at(sorokovnik - 1)));
    q.addBindValue(den);
    q.exec();
    q.next();
    if (q.isValid()) {
        int holidId = q.value(0).toInt();
        if (holidId != 0) {
            q.prepare("SELECT name, descr, rang FROM slavo_arian_holid WHERE id=?");
            q.addBindValue(holidId);
            q.exec();
            q.next();

            QString name = q.value(0).toString();
            QString description = q.value(1).toString();
            int rang = q.value(2).toInt();

            mSrkTitleLineEdit->setText(name);
            mSrkDescriptionTextEdit->setText(description);
            setRangDnya(rang);

            mSrkVesshchieSnyCheckBox->setChecked(rang & 1);
            mSrkRoditelskieDniCheckBox->setChecked(rang & 2);
            mSrkDniPamyatiCheckBox->setChecked(rang & 4);
            mSrkVazhniePrazdnikiCheckBox->setChecked(rang & 8);

            mSrkSimpleDayRadioButton->setChecked(!(rang & 16 && rang & 32 && rang & 64
                                                   && rang & 128 && rang & 256));
            mSrkSvyatiiPostRadioButton->setChecked(rang & 16);
            mSrkDazhdbogPostRadioButton->setChecked(rang & 32);
            mSrkChistyiPostRadioButton->setChecked(rang & 64);
            mSrkVelikiiPostRadioButton->setChecked(rang & 128);
            mSrkSvetlyiPostRadioButton->setChecked(rang & 256);
        } else {
            resetInfoWidgets();
        }
    } else {
        mStatusBar->showMessage(tr("Ошибка: %1").arg(q.lastError().text()), mMessageTimeout);
    }
}

void SADBGui::kzhizniSelected(int kzhizni)
{
    QSqlDatabase opisanieLetDB = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    if (!opisanieLetDB.isOpen()) {
        mStatusBar->showMessage(tr("Откройте базу данных"), mMessageTimeout);
        return;
    }

    if (kzhizni == -1) {
        mDK4TitleLineEdit->clear();
        mDK4TitleLineEdit->setStyleSheet(QString());
        mDK4DescriptionTextEdit->clear();
        return;
    }

    mKZhizni = kzhizni;

    QSqlQuery q(opisanieLetDB);
    q.prepare(QString("SELECT name, descr, symbol, power, color FROM opisanie_let WHERE id=?"));
    q.addBindValue(kzhizni);
    q.exec();
    q.next();
    if (q.isValid()) {
        QString name = q.value(0).toString();
        QString description = q.value(1).toString();
        QString symbol = q.value(2).toString();
        QString power = q.value(3).toString();
        QString color = q.value(4).toString();

        if (color == "#000000") {
            mDK4TitleLineEdit->setStyleSheet(QString("background-color: %1; color: %2")
                                             .arg(color).arg(QColor(240, 240, 240).name()));
        } else {
            mDK4TitleLineEdit->setStyleSheet(QString("background-color: %1; color: %2")
                                             .arg(color).arg(QColor(color).darker(200).name()));
        }

        mDK4PowerLabel->setText(power);
        mDK4SymbolLabel->setText(symbol);
        mDK4TitleLineEdit->setText(name);
        mDK4DescriptionTextEdit->setText(description);
    }
}

void SADBGui::showAbout()
{
    QString about = tr("<b>Редактор баз данных (Версия %1)</b><br>"
                       "Программа предназначена для создания и редактирования базы данных "
                       "Славяно-Арийских праздников и базы данных описаний лет Круга Жизни.<br><br>"
                       "Программа написана на Qt %2.<br><br>"
                       "<b>Разработчик</b><br>"
                       "<span style='white-space:pre'>    Рысь (lynx_r)</span>")
            .arg(QCoreApplication::applicationVersion()).arg(qVersion());

    QMessageBox::about(this, tr("О программе Сороковник"), about);
}

void SADBGui::setLeto()
{
    mSorokovnik->setLeto(mSrkLetoSpinBox->value(), false);
#if defined(Q_OS_UNIX)
    mSrkDBNameLineEdit->setText(QString("../share/databases/%1_%2.db").arg(SADBPRAZDNIKINAME).arg(mSorokovnik->leto()));
#elif defined(Q_OS_WIN)
    mSrkDBNameLineEdit->setText(QString("databases/%1_%2.db").arg(SADBPRAZDNIKINAME).arg(mSorokovnik->leto()));
#endif

    setRangDnya(0);
}

// --------------------------- auto connect -------------------------------- //
void SADBGui::on_mSrkEditPushButton_clicked(bool edit)
{
    QSqlDatabase prazdnikiDB = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    if (prazdnikiDB.isOpen()) {
        mSrkTitleLineEdit->setReadOnly(!edit);
        mSrkDescriptionTextEdit->setReadOnly(!edit);
        mSrkPrazdnikiGroupBox->setEnabled(edit);
        mSrkPostyGroupBox->setEnabled(edit);

        mStatusBar->showMessage(edit ? tr("Редактирование") : tr("Просмотр"), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("Откройте базу данных"), mMessageTimeout);
    }
}

void SADBGui::on_mSrkSubmitPushButton_clicked()
{
    QSqlDatabase prazdnikiDB = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    if (prazdnikiDB.isOpen()) {
        if (!mSrkEditPushButton->isChecked()) {
            mStatusBar->showMessage(tr("Нажмите кнопку \"%1\"").arg(mSrkEditPushButton->text()),
                                    mMessageTimeout);
            return;
        }

        if (mDen == -1) {
            mStatusBar->showMessage(tr("Выбирете день"), mMessageTimeout);
            return;
        }

        QSqlQuery q(prazdnikiDB);
        q.prepare(QString("SELECT count FROM %1 WHERE id=?").arg(mTables.at(mSorok - 1)));
        q.addBindValue(mDen);
        q.exec();
        q.next();
        int holidId = q.value(0).toInt();
        if (holidId != 0) {
            q.prepare(QString("UPDATE slavo_arian_holid SET name=?, descr=?, rang=? WHERE id=?"));
            q.addBindValue(mSrkTitleLineEdit->text());
            q.addBindValue(mSrkDescriptionTextEdit->toPlainText());
            q.addBindValue(mRangDnya);
            q.addBindValue(holidId);
            q.exec();

            mSorokovnik->updateCheckedDen(mSrkTitleLineEdit->text(),
                                          mSrkDescriptionTextEdit->toPlainText(),
                                          mRangDnya);

            mStatusBar->showMessage(tr("Изменения внесены"), mMessageTimeout);
        }
    } else {
        mStatusBar->showMessage(tr("Откройте базу данных"), mMessageTimeout);
    }
}

void SADBGui::on_mSrkOpenDBPushButton_clicked()
{
    QSqlDatabase::removeDatabase(mPrazdnikiDbConnectionName);

    QSqlDatabase prazdnikiDB = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    prazdnikiDB = mSorokovnik->openPrazdnikiDb(mSrkDBNameLineEdit->text(), mPrazdnikiDbConnectionName);
    if (prazdnikiDB.isOpen()) {
        mStatusBar->showMessage(tr("База данных %1 открыта").arg(mSrkDBNameLineEdit->text()), mMessageTimeout);
        mSorokovnik->populatePrazdnikami();
    }
}

void SADBGui::on_mSrkOpenDBToolButton_clicked()
{
    QSettings s;
    QString dir = s.value("/SAKalendar/SADBApp/prazdniki_db", ".").toString();
    QString dbFileName = QFileDialog::getOpenFileName(this, tr("Открыть базу данных праздников"), dir, "(*.db)");
    QFileInfo fi(dbFileName);
    s.setValue("/SAKalendar/SADBApp/prazdniki_db", fi.path());

    mSrkDBNameLineEdit->setText(dbFileName);
}

void SADBGui::on_mSrkCreateDBPushButton_clicked()
{
    QSqlDatabase::removeDatabase(mPrazdnikiDbConnectionName);
    QDir dir;
    dir.remove(mSrkDBNameLineEdit->text());
    DBPrazdniki::openDB(mSrkDBNameLineEdit->text(), mPrazdnikiDbConnectionName);
    QSqlDatabase db = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    if (db.isOpen()) {
        DBPrazdniki::createDB(db);
        mStatusBar->showMessage(tr("База данных %1 создана").arg(mSrkDBNameLineEdit->text()), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("База данных %1 не создана").arg(mSrkDBNameLineEdit->text(), mMessageTimeout));
    }
}

void SADBGui::on_mSrkPopulateDBPushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database(mPrazdnikiDbConnectionName);
    QSqlError err = DBPrazdniki::populateDB(db);
    if (err.type() == QSqlError::NoError) {
        mStatusBar->showMessage(tr("База данных заполнена основными Славяно-Арийским праздниками"), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("Не удалось заполнить базу данных: %1").arg(err.text()), mMessageTimeout);
    }
}

void SADBGui::on_mSrkVesshchieSnyCheckBox_toggled(bool toggled)
{
    resetRangDnya(toggled, 1, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkRoditelskieDniCheckBox_toggled(bool toggled)
{
    resetRangDnya(toggled, 2, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkDniPamyatiCheckBox_toggled(bool toggled)
{
    resetRangDnya(toggled, 4, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkVazhniePrazdnikiCheckBox_toggled(bool toggled)
{
    resetRangDnya(toggled, 8, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkSimpleDayRadioButton_toggled(bool)
{
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkSvyatiiPostRadioButton_toggled(bool toggled)
{
    resetRangDnya(toggled, 16, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkDazhdbogPostRadioButton_toggled(bool toggled)
{
    resetRangDnya(toggled, 32, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkChistyiPostRadioButton_toggled(bool toggled)
{
    resetRangDnya(toggled, 64, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkVelikiiPostRadioButton_toggled(bool toggled)
{
    resetRangDnya(toggled, 128, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}

void SADBGui::on_mSrkSvetlyiPostRadioButton_toggled(bool toggled)
{
    resetRangDnya(toggled, 256, mRangDnya);
    setRangDnyaSorokovnika(mRangDnya);
    setRangDnya(mRangDnya);
}


void SADBGui::on_mDK4EditPushButton_clicked(bool edit)
{
    QSqlDatabase opisanieLetDB = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    if (opisanieLetDB.isOpen()) {
        mDK4TitleLineEdit->setReadOnly(!edit);
        mDK4DescriptionTextEdit->setReadOnly(!edit);
        mSrkPrazdnikiGroupBox->setEnabled(edit);
        mSrkPostyGroupBox->setEnabled(edit);

        mStatusBar->showMessage(edit ? tr("Редактирование") : tr("Просмотр"), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("Откройте базу данных"), mMessageTimeout);
    }
}

void SADBGui::on_mDK4SubmitPushButton_clicked()
{
    QSqlDatabase opisanieLetDB = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    if (opisanieLetDB.isOpen()) {
        if (!mDK4EditPushButton->isChecked()) {
            mStatusBar->showMessage(tr("Нажмите кнопку \"%1\"").arg(mDK4EditPushButton->text()),
                                    mMessageTimeout);
            return;
        }

        if (mKZhizni == -1) {
            mStatusBar->showMessage(tr("Выбирете день"), mMessageTimeout);
            return;
        }

        QSqlQuery q(opisanieLetDB);
        q.prepare(QString("UPDATE opisanie_let SET name=?, descr=? WHERE id=?"));
        q.addBindValue(mDK4TitleLineEdit->text());
        q.addBindValue(mDK4DescriptionTextEdit->toPlainText());
        q.addBindValue(mKZhizni);
        q.exec();

        mStatusBar->showMessage(tr("Изменения внесены"), mMessageTimeout);
    }
}

void SADBGui::on_mDK4OpenDBPushButton_clicked()
{
    QSqlDatabase::removeDatabase(mOpisanieLetDbConnectionName);

    QSqlDatabase opisanieLetDB = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    opisanieLetDB = mDK4->openOpisanieLetDb(mDK4DBNameLineEdit->text(), mOpisanieLetDbConnectionName);
    if (opisanieLetDB.isOpen()) {
        mStatusBar->showMessage(tr("База данных %1 открыта").arg(mDK4DBNameLineEdit->text()), mMessageTimeout);
    }
}

void SADBGui::on_mDK4OpenDBToolButton_clicked()
{
    QSettings s;
    QString dir = s.value("/SAKalendar/SADBApp/opisanielet_db", ".").toString();
    QString dbFileName = QFileDialog::getOpenFileName(this, tr("Открыть базу данных описания лет"), dir, "(*.db)");
    QFileInfo fi(dbFileName);
    s.setValue("/SAKalendar/SADBApp/opisanielet_db", fi.path());

    mDK4DBNameLineEdit->setText(dbFileName);
}

void SADBGui::on_mDK4CreateDBPushButton_clicked()
{
    QSqlDatabase::removeDatabase(mOpisanieLetDbConnectionName);
    QDir dir;
    dir.remove(mDK4DBNameLineEdit->text());
    DBOpisanieLet::openDB(mDK4DBNameLineEdit->text(), mOpisanieLetDbConnectionName);
    QSqlDatabase db = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    if (db.isOpen()) {
        DBOpisanieLet::createDB(db);
        mStatusBar->showMessage(tr("База данных %1 создана").arg(mDK4DBNameLineEdit->text()), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("База данных %1 не создана").arg(mDK4DBNameLineEdit->text(), mMessageTimeout));
    }
}

void SADBGui::on_mDK4PopulateDBPushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database(mOpisanieLetDbConnectionName);
    QSqlError err = DBOpisanieLet::populateDB(db);
    if (err.type() == QSqlError::NoError) {
        mStatusBar->showMessage(tr("База данных заполнена описанием лет"), mMessageTimeout);
    } else {
        mStatusBar->showMessage(tr("Не удалось заполнить базу данных: %1").arg(err.text()), mMessageTimeout);
    }
}

// -------------------------------- private -------------------------------- //
void SADBGui::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("/SAKalendar/SADBApp/Geometry").toByteArray());
    mTabWidget->setCurrentIndex(s.value("/SAKalendar/SADBApp/tab").toInt());
}

void SADBGui::writeSettings()
{
    QSettings s;
    s.setValue("/SAKalendar/SADBApp/Geometry", saveGeometry());
    s.setValue("/SAKalendar/SADBApp/tab", mTabWidget->currentIndex());
}

void SADBGui::createSorokovnik()
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setXOffset(0);
    shadow->setYOffset(2);
    shadow->setBlurRadius(12);

    mSorokovnik = new SorokovnikDB(0, false);
    mSorokovnik->setDenPrototype(new SorokovnikDenDB);
    mSorokovnik->createSorokovnik();
    mSorokovnik->setGraphicsEffect(shadow);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(palette().color(backgroundRole()));
    scene->addItem(mSorokovnik);

    mSorokovnikGraphicsView->setScene(scene);
    mSorokovnikGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSorokovnikGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSorokovnikGraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void SADBGui::createDK4()
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setOffset(0);
    shadow->setBlurRadius(12);

    mDK4 = new DK4DB(0, false);
    mDK4->setLetoPrototype(new DK4LetoDB);
    mDK4->createDK4();
    mDK4->setGraphicsEffect(shadow);

    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(palette().color(backgroundRole()));
    scene->addItem(mDK4);
    mDK4->scale(0.9, 0.9);

    mDK4GraphicsView->setScene(scene);
    mDK4GraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mDK4GraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mDK4GraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void SADBGui::setupConnections()
{
    connect(mSrkPredSorokToolButton, SIGNAL(clicked()), mSorokovnik, SLOT(predSorokovnik()));
    connect(mSrkSledSorokToolButton, SIGNAL(clicked()), mSorokovnik, SLOT(sledSorokovnik()));

    connect(mSrkLetoSpinBox, SIGNAL(editingFinished()), this, SLOT(setLeto()));
    connect(mSorokovnik, SIGNAL(selected(int,int,int)), this, SLOT(daySelected(int, int)));
    connect(mDK4, SIGNAL(selected(int)), this, SLOT(kzhizniSelected(int)));

    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(mActionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

void SADBGui::init()
{
    mTables = QStringList() << QString("ramkhato") << QString("aileto")
              << QString("beileto") << QString("geileto") << QString("daileto")
              << QString("eileto") << QString("veileto") << QString("heileto")
              << QString("taileto");

    int kzhizniFromSMZH = SADate::currentDate().kzhizniFromSMZH();
#if defined(Q_OS_UNIX)
    mDK4DBNameLineEdit->setText(QString("../share/databases/%1_%2.db").arg(SADBOPISANIELETNAME)
                                .arg(kzhizniFromSMZH));
#elif defined(Q_OS_WIN)
    mDK4DBNameLineEdit->setText(QString("databases/%1_%2.db").arg(SADBOPISANIELETNAME)
                                .arg(kzhizniFromSMZH));
#endif

    mOpisanieLetDbConnectionName = QString("%1_%2.db").arg(SADBOPISANIELETNAME)
                                   .arg(kzhizniFromSMZH);

    int leto = SADate::currentDate().leto();
#if defined(Q_OS_UNIX)
    mSrkDBNameLineEdit->setText(QString("../share/databases/%1_%2.db").arg(SADBPRAZDNIKINAME)
                                .arg(leto));
#elif defined(Q_OS_WIN)
    mSrkDBNameLineEdit->setText(QString("databases/%1_%2.db").arg(SADBPRAZDNIKINAME)
                                .arg(leto));
#endif
    mSrkLetoSpinBox->setValue(leto);
    mPrazdnikiDbConnectionName = QString("%1_%2.db").arg(SADBPRAZDNIKINAME)
                                 .arg(leto);

    QFont saFont(SAFONTFAMILYIZHITSAC, 12);
    mSrkTitleLineEdit->setFont(saFont);
    mSrkDescriptionTextEdit->setFont(saFont);
    mDK4TitleLineEdit->setFont(saFont);
    mDK4DescriptionTextEdit->setFont(saFont);
}

void SADBGui::setRangDnya(int rang)
{
    mSrkDecRangLineEdit->setText(QString::number(rang));

    QString rangStr = QString::number(rang, 2);
    if (rangStr.length() == 1)
        rangStr.prepend("00000000");
    else if (rangStr.length() == 2)
        rangStr.prepend("0000000");
    else if (rangStr.length() == 3)
        rangStr.prepend("000000");
    else if (rangStr.length() == 4)
        rangStr.prepend("00000");
    else if (rangStr.length() == 5)
        rangStr.prepend("0000");
    else if (rangStr.length() == 6)
        rangStr.prepend("000");
    else if (rangStr.length() == 7)
        rangStr.prepend("00");
    else if (rangStr.length() == 8)
        rangStr.prepend("0");

    mSrkBinRangLineEdit->setText(rangStr);
}

void SADBGui::setRangDnyaSorokovnika(int rang)
{
    mSorokovnik->updateCheckedDen(QString(), QString(), rang);
}

void SADBGui::resetRangDnya(bool set, int setRang, int &rang)
{
    if (set) {
        rang |= setRang;
    } else {
        rang ^= setRang;
    }
}

void SADBGui::resetInfoWidgets()
{
    mSrkTitleLineEdit->clear();
    mSrkDescriptionTextEdit->clear();
    setRangDnya(0);
    setRangDnyaSorokovnika(0);

    mSrkVesshchieSnyCheckBox->setChecked(false);
    mSrkRoditelskieDniCheckBox->setChecked(false);
    mSrkDniPamyatiCheckBox->setChecked(false);
    mSrkVazhniePrazdnikiCheckBox->setChecked(false);

    mSrkSimpleDayRadioButton->setChecked(true);
}
