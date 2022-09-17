#include "camera_data.h"

CameraData::CameraData() { id = 0; }

CameraData::CameraData(int id, QString cameraIP, QString login,
                       QString password) {
  this->id = id;
  this->cameraIP = cameraIP;
  this->login = login;
  this->password = password;
}
