#ifndef PHOTODATA_H
#define PHOTODATA_H
#include <QDateTime>
#include <QString>

class PhotoData {
public:
  PhotoData();
  int id;
  QString photoName;
  QString cameraName;
  QDateTime created;
  int cameraId;
  int isSmoke;
  int isFire;
  int status;
};

#endif // PHOTODATA_H
