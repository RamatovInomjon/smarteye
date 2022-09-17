#ifndef RERPORTFRM_H
#define RERPORTFRM_H

#include "database.h";
#include <QDesktopServices>
#include <QDialog>
#include <QTextDocument>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace Ui {
class RerportFrm;
}

class RerportFrm : public QDialog {
  Q_OBJECT

public:
  explicit RerportFrm(QWidget *parent = 0, DataBase *mdb = 0, int changeid = 0);
  ~RerportFrm();
private slots:
  void on_btnPrepare_clicked();

  void on_btnPrint_clicked();

  void on_tableView_doubleClicked(const QModelIndex &index);

signals:
  void getSmetLitrs();

private:
  bool createHtmlTableFromModel();
  Ui::RerportFrm *ui;
  DataBase *db;
  QString refTitleName;
  QSqlQueryModel *model;
};

#endif // RERPORTFRM_H
