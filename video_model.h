#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H
#include "database.h"
#include "video_data.h"

class VideoModel {
public:
  VideoModel();

  VideoModel(DataBase *db);

  bool updateStatus(int key, bool status);
  bool saveVideo(VideoData video);
  bool deleteOne(int videoId);
  bool deleteAll();
  int getFromDB(QString filter = "");

  QMap<int, VideoData> getVideos() const;
  void setVideos(const QMap<int, VideoData> &value);

private:
  QMap<int, VideoData> videos;
  DataBase *db;
  bool updateData();
};

#endif // PHOTOMODEL_H
