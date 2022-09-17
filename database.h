#ifndef DATABASE_H
#define DATABASE_H
#include <Crypter.h>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql/QtSql>
class DataBase : public QObject {
public:
  DataBase(QString connectionName = "mainConnection");
  ~DataBase();
  QSqlQuery getQuery(QString sQuery = "");
  QString GetConf(QString key);
  bool SetConf(QString key, QString val);
  QSqlTableModel *TableModel(QString tablename);
  QSqlQueryModel *QryModel(QString query);
  void clearHistory();

protected:
  QSqlQuery qry;
  QSqlDatabase db;
};

#endif // DATABASE_H
