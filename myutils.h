#ifndef MYUTILS_H
#define MYUTILS_H
#include <QDir>
#include <QString>
class MyUtils {
public:
  MyUtils();
  static bool clearFolder(QString path);
};

#endif // MYUTILS_H
