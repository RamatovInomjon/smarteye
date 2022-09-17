#ifndef PHOTOMODEL_H
#define PHOTOMODEL_H
#include "database.h"
#include "photo_data.h"

class PhotoModel {
public:
  PhotoModel();

  PhotoModel(DataBase *db);

  bool updateStatus(int key, int status);
  bool savePhoto(PhotoData photo);
  bool deleteOne(int photoId);
  bool deleteAll();
  int getFromDB(QString filter = "");

  QMap<int, PhotoData> getPhotos() const;
  void setPhotos(const QMap<int, PhotoData> &value);

private:
  QMap<int, PhotoData> photos;
  DataBase *db;
  bool updateData();
};

#endif // PHOTOMODEL_H
