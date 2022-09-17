#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "qttelegrambot.h"
#include <HCNetSDK.h>
#include <QDebug>
#include <QListWidgetItem>
#include <QMainWindow>
#include <aboutusfrm.h>
#include <camera_data.h>
#include <camera_model.h>
#include <cameras.h>
#include <database.h>
#include <imagerec.h>
#include <myutils.h>
#include <onecameraform.h>
#include <onecameraframe.h>
#include <opencv2/opencv.hpp>
#include <photo_data.h>
#include <photo_model.h>
//#include <plaympeg4.h>
#include "PlayM4.h"
#include <popup.h>
#include <rerportfrm.h>
#include <settingfrm.h>
//#include <windows.h>

#define TOKEN "917022622:AAHZCH96bL7eDs1Xl3Zg_Vm9hOjKBdVpo9A"
#define CHATID "-1001388857661"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  //==========Manual slots===========
  void placeCameras(short colLimit = 0, short playType = 0);
  void saveCameraData(CameraData camera);
  void addPhoto(PhotoData pht);

  //======Generated slots========
  void on_btnSendMessage_clicked();
  void on_btnSendPhoto_clicked();
  void on_btnRefresh_clicked();
  void on_photosListWidget_itemClicked(QListWidgetItem *item);

  //======Menu slots========
  void on_actionOneCol_triggered();

  void on_actionTwoCol_triggered();

  void on_actionThreeCol_triggered();

  void on_actionFourCol_triggered();

  void on_actionFiveCol_triggered();

  void on_actionSystemSettings_triggered();

  void on_actionAboutUs_triggered();

  void on_actionExit_triggered();

  void on_actionHelp_triggered();

  void on_actionAddCamera_triggered();

  void on_actionManageCameras_triggered();

  void on_photosListWidget_itemDoubleClicked(QListWidgetItem *item);

  void on_actionClearPhotos_triggered();

  void on_checkBox_toggled(bool checked);

  void on_actionHistory_triggered();

private:
  bool SendPhoto(PhotoData data);
  bool UpdateList();
//  Telegram::Bot *bot;
  Ui::MainWindow *ui;
  QVector<OneCameraFrame *> cameras;
  DataBase db;
  ImageRec img;
  int photoLastId = 0;
  CameraModel *cms;
  int columnsLimit = 3;
  PhotoModel *photos;
  QString chatId;
  QString botToken;
  QString imgDirectory = "";
  PopUp *errorPopUp;
  PopUp *succesPopUp;
};
#endif // MAINWINDOW_H
