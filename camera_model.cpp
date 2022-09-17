#include "camera_model.h"

CameraModel::CameraModel(DataBase *db) { this->db = db; }

QVector<CameraData> CameraModel::getCameras() { return cameras; }

bool CameraModel::setCameraExpanded(int id, bool status) {
  bool res = false;
  for (auto &camera : cameras) {
    if (camera.id == id) {
      camera.isExpanded = status;
      res = true;
    } else {
      camera.isExpanded = false;
    }
  }
  return res;
}

void CameraModel::setCameras(const QVector<CameraData> &value) {
  cameras = value;
}

bool CameraModel::getFromDB(QString filter) {
  QSqlQuery qry =
      db->getQuery("SELECT id, camera_name, source_path, status, "
                   "smoke_percentage, fire_percentage, image_path, ip, login, "
                   "password FROM camera ORDER BY id");
  cameras.clear();
  CameraData camera;
  if (qry.exec()) {
    while (qry.next()) {
      camera.id = qry.value(0).toInt();
      camera.cameraName = qry.value(1).toString();
      camera.sourcePath = qry.value(2).toString();
      camera.status = qry.value(3).toBool();
      camera.smokePercentage = qry.value(4).toInt();
      camera.firePercentage = qry.value(5).toInt();
      camera.imagePath = qry.value(6).toString();
      camera.cameraIP = qry.value(7).toString();
      camera.login = qry.value(8).toString();
      camera.password = qry.value(9).toStringList()[0];
      cameras.push_back(camera);
    }
    return true;
  }
  return false;
}

bool CameraModel::saveCamera(CameraData camera) {
  bool res = false;
  QSqlQuery query, qry;
  if (camera.id == 0) {
    //     INSERT INTO public.camera (camera_name, source_path, status,
    //     smoke_percentage, fire_percentage,  image_path, ip, login, password,
    //     order_position) VALUES ('Camera 1',
    //     'http://admin:123qazwsx@192.168.1.64/ISAPI/Streaming/channels/102/httpPreview',
    //     true, 50, 50,
    //     'http://admin:123qazwsx@192.168.1.64/ISAPI/Streaming/channels/102/httpPreview',
    //     '192.168.1.64', 'admin', '123qazwsx', 0);

    res = qry.prepare(
        "INSERT INTO public.camera (camera_name, source_path, status, "
        "smoke_percentage, fire_percentage,  image_path, ip, login, password, "
        "order_position) VALUES (    :camera_name, :source_path, :status, "
        ":smoke_percentage, :fire_percentage, :image_path, :ip, :login, "
        ":password, :order_position);");

    //    res = qry.prepare(
    //        "INSERT INTO public.сamera (camera_name, source_path, status, "
    //        "smoke_percentage, fire_percentage, image_path, ip, login, "
    //        "password, order_position) VALUES (:camera_name, :source_path,
    //        :status, "
    //        ":smoke_percentage, :fire_percentage, :image_path, :ip, "
    //        ":login, :password, 0);");
  } else {

    res = qry.prepare(
        "UPDATE public.camera SET "
        "camera_name = :camera_name, source_path = :source_path, status = "
        ":status, smoke_percentage = :smoke_percentage, fire_percentage = "
        ":fire_percentage, image_path = :image_path, ip = :ip, login = :login, "
        "password = :password, order_position = :order_position "
        "WHERE id = :id;");
  }
  qDebug() << "Camera saved " << qry.lastQuery();

  if (res) {
    qry.bindValue(":id", camera.id);
    qry.bindValue(":camera_name", camera.cameraName);
    qry.bindValue(":source_path", camera.sourcePath);
    qry.bindValue(":status", camera.status);
    qry.bindValue(":smoke_percentage", camera.smokePercentage);
    qry.bindValue(":fire_percentage", camera.firePercentage);
    qry.bindValue(":image_path", camera.imagePath);
    qry.bindValue(":ip", camera.cameraIP);
    qry.bindValue(":login", camera.login);
    qry.bindValue(":password", camera.password);
    qry.bindValue(":order_position", camera.orderPosition);
  }
  bool b = qry.exec();
  qDebug() << res << " : " << qry.lastError();
  return b;
}

bool CameraModel::deleteCamera(int id) {
  QSqlQuery query, qry;
  bool res = qry.prepare("DELETE FROM public.camera WHERE id=:id;");
  if (res) {
    qry.bindValue(":id", id);
  }
  if (qry.exec()) {
    // this->cameras.remove(id);
    return true;
  } else {
    return false;
  }
}
