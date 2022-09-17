#include "database.h"

DataBase::DataBase(QString connectionName) {
  QString mpassword = "123qazwsxedc4";
  QString filename = "passw.inf";
  //  QFile file(filename);
  //  if (file.open(QIODevice::ReadOnly)) {
  //    QTextStream in(&file);
  //    QString encrpassword = in.readLine();
  //    // mpassword=Crypter::decryptString(encrpassword);
  //  }
  //  file.close();
  qDebug() << "Password  fdtest: " << mpassword;
  db = QSqlDatabase(QSqlDatabase::addDatabase("QPSQL")); //, connectionName
  //  db = QSqlDatabase::addDatabase("QPSQL", connectionName);
  db.setHostName("127.0.0.1");
  db.setDatabaseName("smarteye");
  db.setUserName("postgres");
  db.setPassword(mpassword);
  if (!db.open()) {
    QString password = QInputDialog::getText(
        0, "Ошибка при открытии базы данных", db.lastError().text());

    //    QString cryptpass = Crypter::cryptString(password);
    //    QFile file(filename);
    //    if (file.open(QIODevice::ReadWrite)) {
    //      QTextStream stream(&file);
    //      stream << cryptpass;
    //    }
    //    file.close();
  } else {
    qDebug() << "BD Uspeshno otrilas!!!" << db.lastError().text();
  }
}

DataBase::~DataBase() {
  if (db.isOpen())
    db.close();
}

QSqlQuery DataBase::getQuery(QString sQuery) {
  QSqlQuery query;
  query.prepare(sQuery);
  // query.exec(sQuery);
  return query;
}

QString DataBase::GetConf(QString key) {
//    return "jjj";
  QSqlQuery query;
//  query.prepare("INSERT INTO public.\"Config\"(\"Val\", \"Key\") VALUES(654, 12234)");
//  qDebug()<<query.lastInsertId();
//  return "";
  query.prepare("SELECT val FROM config WHERE key = :mkey");
  query.bindValue(":mkey", key);

  if (!query.exec()) {
    qDebug() << "Oshibka db conf: " << query.lastError();
    return "";
  }
  if (query.next())
    return query.value(0).toString();
  else {
    QSqlQuery qry;
    if (qry.prepare("INSERT INTO config (key, val) VALUES (:key, :val)")) {
      qry.bindValue(":key", key);
      qry.bindValue(":val", "0");
    }
    qry.exec();
  }
  return "";
}

bool DataBase::SetConf(QString key, QString val) {
  QSqlQuery query, qry;
  if (qry.prepare("UPDATE config SET val = :mval WHERE key = :mkey")) {
    qry.bindValue(":mval", val);
    qry.bindValue(":mkey", key);
  }
  return qry.exec();
}

QSqlTableModel *DataBase::TableModel(QString tablename) {
  QSqlTableModel *model = new QSqlTableModel(0, db);
  model->setTable(tablename);
  model->select();
  return model;
}

QSqlQueryModel *DataBase::QryModel(QString query) {
  QSqlQueryModel *model = new QSqlQueryModel();
  model->setQuery(query, db);
  return model;
}

void DataBase::clearHistory() {
  QSqlQuery query;
  query.exec("DELETE FROM tabmaindata");
  query.exec("DELETE FROM tabchange");
  query.exec("DELETE FROM tabsummcounters");
  query.exec("delete from sqlite_sequence where name='tabmaindata'");
  query.exec("delete from sqlite_sequence where name='tabchange'");
  query.exec("delete from sqlite_sequence where name='tabsummcounters'");
}
