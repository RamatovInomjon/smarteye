#ifndef VIDEODATA_H
#define VIDEODATA_H
#include <QDateTime>
#include <QString>

class VideoData {
public:
  VideoData();
  int id;
  int cameraId;
  QDateTime startTime;
  QDateTime endTime;
  bool status;
};

#endif // VIDEODATA_H
