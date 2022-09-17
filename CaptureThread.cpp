#include "CaptureThread.h"

#include "opencv2/opencv.hpp"
//#include "unistd.h"

#include <QDebug>

CaptureThread::CaptureThread(CameraData camera, int lUserID) : QThread() {
  this->camera = camera;
  this->lUserID = lUserID;
  // filename = QString("D://%1.jpg").arg(camera.id).toStdString();

} // CaptureThread constructor

CaptureThread::~CaptureThread() { qDebug() << Q_FUNC_INFO << " Udalilos"; }

void CaptureThread::run() {
  forever {
    // qDebug() << "Hello " << camera.id << " : " <<
    // QDateTime::currentDateTime();
    QDateTime stTime = QDateTime::currentDateTime();
    LPNET_DVR_JPEGPARA JpegPara = new NET_DVR_JPEGPARA;
    JpegPara->wPicQuality = 0;
    JpegPara->wPicSize = 9;
    const DWORD len = 1024 * 1024;
    char *Jpeg = new char[len];

    LPDWORD Ret = 0;

    if (!NET_DVR_SetCapturePictureMode(BMP_MODE)) {
      qDebug() << "The error code is " << NET_DVR_GetLastError();
    }

    // bool capture = NET_DVR_CaptureJPEGPicture(lUserID,1,JpegPara,"1111");
    vector<char> data(len);

    bool capture =
        NET_DVR_CaptureJPEGPicture_NEW(lUserID, 1, JpegPara, Jpeg, len, Ret);
    if (!capture) {
      qDebug() << "Error: NET_DVR_CaptureJPEGPicture_NEW ="
               << NET_DVR_GetLastError();
      // return;
    } else {
      for (int i = 0; i < len; i++) {
        data[i] = Jpeg[i];
      }

      Mat img = imdecode(Mat(data), 1);
      // resize(img, img, Size(1024, 680), 0, 0, INTER_CUBIC);

      if (imgRec.calc(img)) {
        QString fname = QString("%1_%2.jpg")
                            .arg(camera.id)
                            .arg(QDateTime::currentDateTimeUtc().toString(
                                "ddMMyyyy_hhmmss"));
        QString path = QString("%1/pictures/%2")
                           .arg(QCoreApplication::applicationDirPath())
                           .arg(fname);

        imwrite(path.toStdString(), img);
        PhotoData pht;
        pht.photoName = fname;
        pht.cameraId = camera.id;
        pht.cameraName = camera.cameraName;
        pht.created = QDateTime::currentDateTime();
        pht.isFire = 80;
        pht.isSmoke = 70;
        pht.status = 0;
        pht.id = 0;
        emit(addPhoto(pht));
      };
      qDebug() << "C: " << camera.id
               << " Dif: " << stTime.msecsTo(QDateTime::currentDateTime())
               << " Time: "
               << QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
    }

    msleep(10);
    if (QThread::currentThread()->isInterruptionRequested()) {
      qDebug() << Q_FUNC_INFO << " terminated";
      return;
    }
  }
}
