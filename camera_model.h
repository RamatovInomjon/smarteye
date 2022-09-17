#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H
#include "camera_data.h"
#include "database.h"
#include <QString>
#include <QVector>
#include <QtSql/QSqlQuery>

class CameraModel {
public:
  CameraModel(DataBase *db);

  //    static void registerMe(const std::string& moduleName);
  //    int rowCount(const QModelIndex& parent = {}) const override;
  //    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole)
  //    const override; QHash<int, QByteArray> roleNames() const override;

  QVector<CameraData> getCameras();
  bool setCameraExpanded(int id, bool status);
  void setCameras(const QVector<CameraData> &value);
  bool getFromDB(QString filter = "");
  bool saveCamera(CameraData camera);
  bool deleteCamera(int id);

private:
  QVector<CameraData> cameras;
  DataBase *db;
  bool updateData();
};

#endif // CAMERAMODEL_H
