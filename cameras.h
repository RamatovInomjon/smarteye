#ifndef CAMERAS_H
#define CAMERAS_H

#include <QDialog>
#include <camera_model.h>
#include <database.h>
#include <onecameraform.h>
namespace Ui {
class Cameras;
}

class Cameras : public QDialog {
  Q_OBJECT

public:
  explicit Cameras(DataBase *db, CameraModel *cms, QWidget *parent = nullptr);
  ~Cameras();
signals:
  void saveCameraData(CameraData camera);
  void deleteCamera();
private slots:
  void refresh();

  void on_btnAddCamera_clicked();

  void on_btnEditCamera_clicked();

  void on_btnDeleteCamera_clicked();

private:
  Ui::Cameras *ui;
  CameraModel *cms;
  DataBase *db;
  QSqlQueryModel *model;
};

#endif // CAMERAS_H
