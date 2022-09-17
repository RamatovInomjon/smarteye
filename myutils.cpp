#include "myutils.h"

MyUtils::MyUtils() {}

bool MyUtils::clearFolder(QString path) {
  QDir dir(path);
  dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
  foreach (QString dirItem, dir.entryList())
    dir.remove(dirItem);

  dir.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
  foreach (QString dirItem, dir.entryList()) {
    QDir subDir(dir.absoluteFilePath(dirItem));
    subDir.removeRecursively();
  }
  return true;
}
