#include "onecameraframe.h"
#include "ui_onecameraframe.h"
LONG nPort = 0;
volatile int gbHandling = 3;
unsigned int cnt = 1;

OneCameraFrame::OneCameraFrame(CameraData camera, DataBase *db, QWidget *parent)
    : QFrame(parent), ui(new Ui::OneCameraFrame) {
  ui->setupUi(this);
  this->db = db;
  this->camera = camera;
  ui->camFeed3GroupBox->setTitle(camera.cameraName);
  ui->cam3RemoveMotionRegionsToolButton->setVisible(false);

  on_cam3ConnectToolButton_clicked();
}

void OneCameraFrame::captureRun(QLabel *label, QString filename,
                                QString camid) {}

OneCameraFrame::~OneCameraFrame() {
  delete ui;
  qDebug() << "Delte Thread";
  if (cThread != 0 && cThread->isRunning()) {
    cThread->requestInterruption();
    cThread->quit();
    cThread->wait();
  }

  if (lUserID >= 0) {
    lUserID = -1;
    if (lRealPlayHandle >= 0) {
      NET_DVR_StopRealPlay(lRealPlayHandle);
      lRealPlayHandle = -1;
    }
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
  }
}

void OneCameraFrame::on_cam3SettingsToolButton_clicked() {
  OneCameraForm *frm = new OneCameraForm(this->camera);
  connect(frm, SIGNAL(saveCameraData(CameraData)),
          SIGNAL(saveCameraData(CameraData)));
  frm->setAttribute(Qt::WA_DeleteOnClose);
  frm->setModal(true);
  frm->show();
}

void OneCameraFrame::on_cam3ImageToolButton_clicked() {

  //  //---------------------------------------

  //  LPNET_DVR_JPEGPARA JpegPara = new NET_DVR_JPEGPARA;
  //  JpegPara->wPicQuality = 0;
  //  JpegPara->wPicSize = 9;

  //  char *Jpeg = new char[1024 * 1024];
  //  DWORD len = 1024 * 1024;
  //  LPDWORD Ret = 0;

  //  if (!NET_DVR_SetCapturePictureMode(BMP_MODE)) {
  //    cout << "Set Capture Picture Mode error!" << endl;
  //    cout << "The error code is " << NET_DVR_GetLastError() << endl;
  //  }

  //  // bool capture = NET_DVR_CaptureJPEGPicture(lUserID,1,JpegPara,"1111");
  //  vector<char> data(len);
  //  // while (1) {
  //  bool capture =
  //      NET_DVR_CaptureJPEGPicture_NEW(lUserID, 1, JpegPara, Jpeg, len, Ret);
  //  if (!capture) {
  //    printf("Error: NET_DVR_CaptureJPEGPicture_NEW = %d",
  //           NET_DVR_GetLastError());
  //    return;
  //  }

  //  for (int i = 0; i < len; i++)
  //    data[i] = Jpeg[i];

  //  Mat img = imdecode(Mat(data), 1);
  //  imwrite("D://ttt.jpg", img);
  //  imshow("camera", img);
}

void OneCameraFrame::on_cam3ConnectToolButton_clicked() {
  ui->cam3ConnectToolButton->setChecked(false);
  if (lUserID >= 0) {
    lUserID = -1;
    if (lRealPlayHandle >= 0) {
      NET_DVR_StopSaveRealData(lRealPlayHandle);
      NET_DVR_StopRealPlay(lRealPlayHandle);
      lRealPlayHandle = -1;
    }
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();

  } else {
    bool isok = NET_DVR_Init();
    if (isok) {
      qDebug() << "Инициализация SDK прошла успешно";
      // Устанавливаем время подключения и время переподключения
      NET_DVR_SetConnectTime(2000, 1);
      NET_DVR_SetReconnect(10000, true);
      NET_DVR_DEVICEINFO_V30 struDeviceInfo;
      lUserID = NET_DVR_Login_V30(
          const_cast<char *>(this->camera.cameraIP.toLocal8Bit().data()), 8000,
          const_cast<char *>(this->camera.login.toLocal8Bit().data()),
          const_cast<char *>(this->camera.password.toLocal8Bit().data()),
          &struDeviceInfo);

      if (lUserID < 0) {
        qDebug() << "Ошибка входа:" << NET_DVR_GetLastError();
        NET_DVR_Cleanup(); // Освободить ресурсы SDK
        return;
      } else {

        HWND hWnd =
            (HWND)ui->videoFrameLabel->winId(); // Получить дескриптор окна
        NET_DVR_PREVIEWINFO struPlayInfo;
        struPlayInfo.hPlayWnd = hWnd; // NULL;
        //  hWnd; // Дескриптор должен быть установлен на допустимое значение,
        // когда SDK декодируется, и его можно установить в null, когда
        // поток только извлекается, но не декодируется
        struPlayInfo.lChannel = 1; // Предварительный просмотр номера канала
        struPlayInfo.dwStreamType = 0; // 0-основной поток, 1-дополнительный
                                       // поток, 2-поток 3, 3-поток 4 и т. д.
        struPlayInfo.dwLinkMode = 0; // 0- режим TCP, 1- режим UDP, 2- режим
                                     // многоадресной рассылки, 3-
        // режим RTP, 4-RTP / RTSP, 5-RSTP / HTTP
        struPlayInfo.bBlocked =
            1; // 0- неблокирующий поток выборки, 1- блокирующий поток выборки
        lRealPlayHandle =
            NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, nullptr, nullptr);
        qDebug() << "Войти успешно userId: " << lUserID
                 << " playHandler: " << lRealPlayHandle;

        if (lRealPlayHandle < 0) {
          qDebug() << "Ошибка предварительного просмотра:"
                   << NET_DVR_GetLastError();

          NET_DVR_Logout(lUserID);
          NET_DVR_Cleanup();
          return;
        } else {

          cThread = new CaptureThread(this->camera, lUserID);
          connect(cThread, SIGNAL(addPhoto(PhotoData)), this,
                  SIGNAL(addPhoto(PhotoData)));
          ui->cam3ConnectToolButton->setChecked(true);
        }
      }

    } else {
      qDebug() << "Ошибка инициализации SDK:" << NET_DVR_GetLastError();
    }
  }
}

void OneCameraFrame::on_cam3ExpandToolButton_clicked() {
  OneCameraViewForm *cam = new OneCameraViewForm(this->camera);
  cam->setAttribute(Qt::WA_DeleteOnClose);
  cam->setModal(true);
  cam->showMaximized();
}

void OneCameraFrame::on_cam3MotionRegionsToolButton_clicked() {}

void OneCameraFrame::on_cam3MotionRegionsToolButton_clicked(bool checked) {
  if (checked) {
    if (cThread != 0 && cThread->isRunning())
      return;

    // connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
    // QThread::msleep(1000);
    cThread->start();
    //}
  } else {
    if (cThread != 0 && cThread->isRunning()) {
      cThread->requestInterruption();
      // cThread->wait();
    }
  }
}

void OneCameraFrame::on_cam3RecordToolButton_clicked(bool checked) {

  if (checked && lRealPlayHandle >= 0) {
    QString fname =
        QString("%1_%2.mp4")
            .arg(camera.id)
            .arg(QDateTime::currentDateTimeUtc().toString("ddMMyyyy_hhmmss"));
    QString path = QString("%1/videos/%2")
                       .arg(QCoreApplication::applicationDirPath())
                       .arg(fname);

    if (!NET_DVR_SaveRealData(lRealPlayHandle, path.toUtf8().data())) {
      ui->cam3RecordToolButton->setChecked(false);
    };
  } else {

    NET_DVR_StopSaveRealData(lRealPlayHandle);
  }
}
