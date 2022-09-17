#include "photo_model.h"

PhotoModel::PhotoModel() {}

PhotoModel::PhotoModel(DataBase *db) { this->db = db; }

bool PhotoModel::updateStatus(int key, int status) {
  if (photos.contains(key)) {
    QString sQuery = QString("UPDATE detected_photos SET status=%1 WHERE id=%2")
                         .arg(status)
                         .arg(key);
    QSqlQuery qry = db->getQuery(sQuery);
    if (qry.exec()) {
      photos[key].status = status;
      // PhotoData *photo=&photos.value(key);
      return true;
    }
  }
  return false;
}

bool PhotoModel::savePhoto(PhotoData photo) {
  bool res = false;
  QSqlQuery query, qry;
  if (photo.id == 0) {
    res = qry.prepare(
        " INSERT INTO detected_photos (photo_name, created_time, camera_id, "
        "is_smoke, is_fire, status, created) VALUES (:photo_name, "
        ":created_time, :camera_id, :is_smoke, :is_fire, :status, :created);");
  } else {
    res = qry.prepare(
        "UPDATE сamera SET photo_name=:photo_name, created_time=:created_time, "
        "camera_id=:camera_id, c=:is_smoke, is_fire=:is_fire, "
        "status=:status  WHERE id = :id;");
  }

  if (res) {
    qry.bindValue(":id", photo.id);
    qry.bindValue(":photo_name", photo.photoName);
    qry.bindValue(":created_time", photo.created);
    qry.bindValue(":camera_id", photo.cameraId);
    qry.bindValue(":is_smoke", photo.isSmoke == 1);
    qry.bindValue(":is_fire", photo.isFire == 1);
    qry.bindValue(":status", photo.status);
    qry.bindValue(":created", photo.created);
  }

  if (qry.exec()) {
    if (photo.id == 0 && qry.lastInsertId().isValid()) {
      photo.id = qry.lastInsertId().toInt();
      qDebug() << "Last INsert ID: " << photo.id;
      photos.insert(photo.id, photo);
    }
    return true;
  } else {
    qDebug() << qry.lastError();
    return false;
  };
}

bool PhotoModel::deleteOne(int photoId) {
  QSqlQuery query, qry;
  bool res = qry.prepare("DELETE FROM public.detected_photos WHERE id=:id;");
  if (res) {
    qry.bindValue(":id", photoId);
  }
  if (qry.exec()) {
    this->photos.remove(photoId);
    return true;
  } else {
    return false;
  }
}

bool PhotoModel::deleteAll() {
  QSqlQuery query, qry;
  qry.prepare("DELETE FROM public.detected_photos");
  if (qry.exec()) {
    this->photos.clear();
    return true;
  } else {
    return false;
  }
}

int PhotoModel::getFromDB(QString filter) {
  QSqlQuery qry = db->getQuery(
      QString("SELECT detected_photos.id, photo_name, created, camera_id, "
              "is_smoke, is_fire, detected_photos.status, created_time, "
              "camera_name  FROM detected_photos LEFT JOIN camera ON "
              "camera.id=detected_photos.camera_id %1")
          .arg(filter));
  PhotoData photo;
  int ret = 0;
  if (qry.exec()) {
    while (qry.next()) {
      photo.id = qry.value(0).toInt();
      photo.photoName = qry.value(1).toString();
      photo.created = qry.value(2).toDateTime();
      photo.cameraId = qry.value(3).toInt();
      photo.isSmoke = qry.value(4).toBool();
      photo.isFire = qry.value(5).toBool();
      photo.status = qry.value(6).toInt();
      photo.cameraName = qry.value(8).toString();
      photos.insert(photo.id, photo);
      ret = photo.id;
    }
  }
  qDebug() << "Return: " << ret;
  return ret;
}

QMap<int, PhotoData> PhotoModel::getPhotos() const { return photos; }

void PhotoModel::setPhotos(const QMap<int, PhotoData> &value) {
  photos = value;
}
