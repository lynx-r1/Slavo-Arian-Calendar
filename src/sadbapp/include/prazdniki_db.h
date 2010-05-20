#ifndef INITDB_H
#define INITDB_H

#include <QSqlDatabase>
#include <QSqlError>

namespace DBPrazdniki
{
    QSqlError openDB(const QString &fileName, const QString &connectionName);
    QSqlError createDB(QSqlDatabase db, bool svyashch = false);
    QSqlError populateDB(QSqlDatabase db, bool svyashch = false);
};

#endif // INITDB_H
