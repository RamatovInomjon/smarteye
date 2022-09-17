#include "video_model.h"

VideoModel::VideoModel() {}

VideoModel::VideoModel(DataBase *db) { this->db = db; }

bool VideoModel::updateStatus(int key, bool status) {
  if (videos.contains(key)) {
    QString sQuery =
        QString("UPDATE videos SET status=%1 WHERE id=%2").arg(status).arg(key);
    QSqlQuery qry = db->getQuery(sQuery);
    if (qry.exec()) {
      videos[key].status = status;
      // VideoData *Video=&Videos.value(key);
      return true;
    }
  }
  return false;
}

bool VideoModel::saveVideo(VideoData video) {
  bool res = false;
  QSqlQuery query, qry;
  if (video.id == 0) {
    res = qry.prepare(
        " INSERT INTO videos (camera_id, start_time, end_time, status) VALUES ("
        ":camera_id, :start_time, :end_time, :status);");
  } else {
    res = qry.prepare(
        "UPDATE сamera SET camera_id=:camera_id, start_time=:start_time, "
        "end_time=:end_time, status=:status WHERE id = :id;");
  }

  if (res) {
    qry.bindValue(":id", video.id);
    qry.bindValue(":camera_id", video.cameraId);
    qry.bindValue(":start_time", video.startTime);
    qry.bindValue(":end_time", video.endTime);
    qry.bindValue(":status", video.status);
  }

  if (qry.exec()) {
    if (video.id == 0 && qry.lastInsertId().isValid()) {
      video.id = qry.lastInsertId().toInt();
      qDebug() << "Last INsert ID: " << video.id;
      videos.insert(video.id, video);
    }
    return true;
  } else {
    qDebug() << qry.lastError();
    return false;
  };
}

bool VideoModel::deleteOne(int videoId) {
  QSqlQuery query, qry;
  bool res = qry.prepare("DELETE FROM public.videos WHERE id=:id;");
  if (res) {
    qry.bindValue(":id", videoId);
  }
  if (qry.exec()) {
    this->videos.remove(videoId);
    return true;
  } else {
    return false;
  }
}

bool VideoModel::deleteAll() {
  QSqlQuery query, qry;
  qry.prepare("DELETE FROM public.videos");
  if (qry.exec()) {
    this->videos.clear();
    return true;
  } else {
    return false;
  }
}

int VideoModel::getFromDB(QString filter) {
  QSqlQuery qry =
      db->getQuery(QString("SELECT videos.id, start_time, end_time, camera_id, "
                           " videos.status, camera_name  FROM videos LEFT JOIN "
                           "camera ON camera.id=videos.camera_id %1")
                       .arg(filter));
  VideoData video;
  int ret = 0;
  if (qry.exec()) {
    while (qry.next()) {
      video.id = qry.value(0).toInt();
      video.startTime = qry.value(1).toDateTime();
      video.endTime = qry.value(2).toDateTime();
      video.cameraId = qry.value(3).toInt();
      video.status = qry.value(5).toBool();
      videos.insert(video.id, video);
      ret = video.id;
    }
  }
  qDebug() << "Return: " << ret;
  return ret;
}

QMap<int, VideoData> VideoModel::getVideos() const { return videos; }

void VideoModel::setVideos(const QMap<int, VideoData> &value) {
  videos = value;
}
