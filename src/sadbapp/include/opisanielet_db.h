#ifndef OPISANIE_LET_H
#define OPISANIE_LET_H

#include <QSqlDatabase>
#include <QSqlError>

namespace DBOpisanieLet
{
    QSqlError openDB(const QString &fileName, const QString &connectionName);
    QSqlError createDB(QSqlDatabase db);
    QSqlError populateDB(QSqlDatabase db);
};

#endif // OPISANIE_LET_H
