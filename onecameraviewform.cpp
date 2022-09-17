#include "onecameraviewform.h"
#include "ui_onecameraviewform.h"

OneCameraViewForm::OneCameraViewForm(CameraData camera, QWidget *parent)
    : QDialog(parent), ui(new Ui::OneCameraViewForm) {
  ui->setupUi(this);
  QWidget::setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint |
                          Qt::WindowMaximizeButtonHint);

  bool isok = NET_DVR_Init();
  if (isok) {
//    qDebug() << "Инициализация SDK прошла успешно";
    // Устанавливаем время подключения и время переподключения
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    NET_DVR_DEVICEINFO_V30 struDeviceInfo;

    int lUserID = NET_DVR_Login_V30(
        const_cast<char *>(camera.cameraIP.toLocal8Bit().data()), 8000,
        const_cast<char *>(camera.login.toLocal8Bit().data()),
        const_cast<char *>(camera.password.toLocal8Bit().data()),
        &struDeviceInfo);

    if (lUserID < 0) {
//      qDebug() << "Ошибка входа:" << NET_DVR_GetLastError();
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
      int lRealPlayHandle =
          NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, nullptr, nullptr);
//      qDebug() << "Войти успешно userId: " << lUserID
//               << " playHandler: " << lRealPlayHandle;
      // fRealDataCallBack

      if (lRealPlayHandle < 0) {
//        qDebug() << "Ошибка предварительного просмотра:"
//                 << NET_DVR_GetLastError();

        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return;
      } else {
      }
    }

  } else {
      printf("error\n");
//    qDebug() << "Ошибка инициализации SDK:" << NET_DVR_GetLastError();
  }
}

OneCameraViewForm::~OneCameraViewForm() { delete ui; }
