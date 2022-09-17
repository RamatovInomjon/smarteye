#ifndef ONECAMERAFRAME_H
#define ONECAMERAFRAME_H

#include "camera_data.h"
//#include "getvideo.h"
#include <CaptureThread.h>
#include <HCNetSDK.h>
#include <QDateTime>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <database.h>
#include <imagerec.h>
#include <math.h>
#include <onecameraform.h>
#include <onecameraviewform.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <photo_data.h>
//#include <plaympeg4.h>
#include "PlayM4.h"
#include <video_data.h>
#include <video_model.h>
//#include <windows.h>

// Namespaces.
using namespace std;
using namespace cv;
using namespace cv::dnn;

using namespace std;
// using namespace cv;
namespace Ui {
class OneCameraFrame;
}

class OneCameraFrame : public QFrame {
  Q_OBJECT

public:
  explicit OneCameraFrame(CameraData camera, DataBase *db,
                          QWidget *parent = nullptr);
  ~OneCameraFrame();

signals:
  void saveCameraData(CameraData camera);
  void addPhoto(PhotoData pht);
public slots:
  void on_cam3SettingsToolButton_clicked();
  static void captureRun(QLabel *label, QString filename, QString camid);

private slots:
  void on_cam3ImageToolButton_clicked();

  void on_cam3ConnectToolButton_clicked();

  void on_cam3ExpandToolButton_clicked();

  void on_cam3MotionRegionsToolButton_clicked();

  void on_cam3MotionRegionsToolButton_clicked(bool checked);

  void on_cam3RecordToolButton_clicked(bool checked);

private:
  Ui::OneCameraFrame *ui;
  CaptureThread *cThread;
  CameraData camera;
  ImageRec img;
  DataBase *db;
  VideoModel *videos;

  int lUserID = -1;
  int lRealPlayHandle = -1;
};

#endif // ONECAMERAFRAME_H
