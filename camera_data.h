#ifndef CAMERADATA_H
#define CAMERADATA_H
#include <QString>
#include <QVariant>

class CameraData {
public:
  CameraData();
  CameraData(int id, QString cameraIP, QString login, QString password);
  int id;
  QString cameraIP;
  QString login;
  QString password;
  QString cid;
  QString cameraName;
  QString sourcePath;
  QString imagePath;
  bool isExpanded = false;
  bool status;
  int orderPosition = 0;
  int smokePercentage;
  int firePercentage;
};

#endif // CAMERADATA_H
