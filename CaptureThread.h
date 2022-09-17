#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QtGui>
// OpenCV header files
#include "HCNetSDK.h"
#include <camera_data.h>
#include <imagerec.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <photo_data.h>
using namespace cv;

class CaptureThread : public QThread {
  Q_OBJECT

public:
  CaptureThread(CameraData camera, int lUserID);
  ~CaptureThread() override;

signals:
  void addPhoto(PhotoData pht);

private:
  CameraData camera;
  Mat grabbedFrame;
  ImageRec imgRec;
  int lUserID;
  // std::string filename;

protected:
  void run() override;
};

#endif // CAPTURETHREAD_H
