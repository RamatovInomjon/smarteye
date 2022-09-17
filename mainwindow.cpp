#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  succesPopUp = new PopUp(false);
  errorPopUp = new PopUp(true);
  ui->checkBox->setChecked(db.GetConf("autosend") == "true");
  chatId = CHATID;
  botToken = TOKEN;
  chatId = db.GetConf("chatId");
  botToken = db.GetConf("botToken");
  columnsLimit = db.GetConf("colLimit").toInt();
//  bot = new Telegram::Bot(botToken, true, 500, 4);
  cms = new CameraModel(&db);
  placeCameras(columnsLimit);
  photos = new PhotoModel(&db);

  photos->getFromDB("ORDER BY id ASC LIMIT 100");
  UpdateList();
}

MainWindow::~MainWindow() { delete ui; }

bool MainWindow::SendPhoto(PhotoData data) {
  QString photo =
      QString(QCoreApplication::applicationDirPath() + "/pictures/%1")
          .arg(data.photoName);
  QFile file(photo);
  file.open(QFile::ReadOnly);
  QString title = QString("%1: %2")
                      .arg(data.cameraName)
                      .arg(data.created.toString("yy.MM.dd hh:mm:ss"));
//  bool res = bot->sendPhoto(chatId, &file, title);
  bool res=true;
  if (res) {
    data.status = 1;
    photos->savePhoto(data);
    succesPopUp->setPopupText("Фото успешно отправленно!");
    succesPopUp->show();
    UpdateList();
    // return photos->updateStatus(data.id, 1);
    // change status in database
  } else {
    errorPopUp->setPopupText("Ошибка при отправке сообщения!");
    errorPopUp->show();
  }
  return res;
}

void MainWindow::on_btnSendMessage_clicked() {
  ui->btnSendMessage->setEnabled(false);
//  if (bot->sendMessage(CHATID, ui->edtMsg->toPlainText())) {
//    ui->edtMsg->clear();
//  } else {
//    QMessageBox msgBox;
//    errorPopUp->setPopupText("Ошибка при отправке сообщения! ");
//    errorPopUp->show();
//  }
  ui->btnSendMessage->setEnabled(true);
}

void MainWindow::on_btnSendPhoto_clicked() {
  ui->btnSendPhoto->setEnabled(false);
  if (!photos->getPhotos().contains(photoLastId)) {
    QMessageBox msgBox;
    errorPopUp->setPopupText("Выберите объект!");
    errorPopUp->show();
  } else {
    PhotoData photo = photos->getPhotos().value(photoLastId);
    SendPhoto(photo);
  }
  ui->btnSendPhoto->setEnabled(true);
}

bool MainWindow::UpdateList() {
  ui->photosListWidget->clear();
  ui->photosListWidget->setViewMode(QListWidget::IconMode);
  ui->photosListWidget->setIconSize(QSize(200, 150));
  ui->photosListWidget->setResizeMode(QListWidget::Adjust);

  for (auto &photo : photos->getPhotos()) {
    // QString phname=cms->getCameras()
    QString path =
        QString(QCoreApplication::applicationDirPath() + "/pictures/%1")
            .arg(photo.photoName);
    QString title = QString("%1: %2")
                        .arg(photo.cameraName)
                        .arg(photo.created.toString("yy.MM.dd hh:mm:ss"));

    QListWidgetItem *item = new QListWidgetItem(QIcon(path), title);

    item->setForeground(
        QBrush((photo.status) ? QColor(Qt::blue) : QColor(Qt::red)));
    item->setToolTip(QString::number(photo.id));
    ui->photosListWidget->insertItem(0, item);
  }
  return true;
}

void MainWindow::on_btnRefresh_clicked() {
  int maxId = photos->getFromDB("ORDER BY id ASC LIMIT 100");
  if (photoLastId != maxId) {
    photoLastId = maxId;
    UpdateList();
  }
  QApplication::beep();
}

void MainWindow::on_photosListWidget_itemClicked(QListWidgetItem *item) {
  photoLastId = item->toolTip().toInt(0);
}

void MainWindow::placeCameras(short colLimit, short playType) {
  if (columnsLimit != colLimit) {
    columnsLimit =
        (columnsLimit < 1) ? 1 : ((columnsLimit > 5) ? 5 : columnsLimit);
    colLimit = (colLimit < 1) ? columnsLimit
                              : ((colLimit > 5) ? columnsLimit : colLimit);
    columnsLimit = colLimit;
    db.SetConf("colLimit", QString::number(columnsLimit));
  }

  int c = 0;
  int r = 0;
  QGridLayout *grid = ui->cameraGrid;
  //  if (grid != NULL) {
  //    QLayoutItem *item;
  //    while ((item = grid->takeAt(0)) != NULL) {
  //      delete item->widget();
  //      delete item;
  //    }
  //    // delete grid->layout();
  //  }

  while (QLayoutItem *item = grid->takeAt(0)) {
    if (QWidget *widget = item->widget())
      widget->deleteLater();
    delete item;
  }
  //    photos = new PhotoModel(&db);
  //    photos->getFromDB();
  //    OneCameraFrame *cm = new OneCameraFrame(cms->getCameras()[0], this);
  //    cameras.append(cm);
  //    ui->cameraGrid->addWidget(cm, 0, 0, 2, 2);

  //    cm = new OneCameraFrame(cms->getCameras()[0], this);
  //    cameras.append(cm);
  //    ui->cameraGrid->addWidget(cm, 0, 2);

  //    cm = new OneCameraFrame(cms->getCameras()[0], this);
  //    cameras.append(cm);
  //    ui->cameraGrid->addWidget(cm, 1, 2);

  if (cms->getFromDB("ORDER BY id ASC LIMIT 100")) {
    c = 0;
    for (auto &camera : cms->getCameras()) {
      OneCameraFrame *cm = new OneCameraFrame(camera, &db, this);
      connect(cm, SIGNAL(addPhoto(PhotoData)), this, SLOT(addPhoto(PhotoData)));

      connect(cm, SIGNAL(saveCameraData(CameraData)), this,
              SLOT(saveCameraData(CameraData)));
      cameras.append(cm);
      if (c >= columnsLimit) {
        r++;
        c = 0;
      }
      if (camera.isExpanded) {
        ui->cameraGrid->addWidget(cm, r, c++);
      } else {
        ui->cameraGrid->addWidget(cm, r, c++);
      }
    }
  }
}

void MainWindow::saveCameraData(CameraData camera) {
  cms->saveCamera(camera);
  placeCameras(columnsLimit);
}

void MainWindow::addPhoto(PhotoData pht) {
  qDebug() << "Get new Photo: " << pht.photoName;
  photos->savePhoto(pht);
  if (ui->checkBox->isChecked())
    SendPhoto(pht);
  // on_btnRefresh_clicked();
  UpdateList();
}

void MainWindow::on_actionOneCol_triggered() { placeCameras(1); }

void MainWindow::on_actionTwoCol_triggered() { placeCameras(2); }

void MainWindow::on_actionThreeCol_triggered() { placeCameras(3); }

void MainWindow::on_actionFourCol_triggered() { placeCameras(4); }

void MainWindow::on_actionFiveCol_triggered() { placeCameras(5); }

void MainWindow::on_actionSystemSettings_triggered() {
  SettingFrm *cam = new SettingFrm(this, &db);
  cam->setAttribute(Qt::WA_DeleteOnClose);
  cam->setModal(true);
  cam->show();
}

void MainWindow::on_actionAboutUs_triggered() {
  QDir directory("D:/SmartEye/smart_eye/"
                 "build-SmartEye-Desktop_Qt_6_3_0_MSVC2019_64bit-Release/"
                 "release/pictures");
  QStringList images = directory.entryList(QStringList() << "*.jpg"
                                                         << "*.JPG",
                                           QDir::Files);

  foreach (QString filename, images) {
    qDebug() << filename;
    PhotoData ph;
    ph.photoName = filename;
    ph.id = 0;
    ph.isFire = false;
    ph.isSmoke = false;
    ph.status = 0;
    ph.created = QDateTime::currentDateTime();
    photos->savePhoto(ph);
  }
  //  aboutUsFrm *frm = new aboutUsFrm();
  //  frm->setAttribute(Qt::WA_DeleteOnClose);
  //  frm->setModal(true);
  //  frm->show();
}

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_actionHelp_triggered() { photos->deleteAll(); }

void MainWindow::on_actionAddCamera_triggered() {
  OneCameraForm *frm = new OneCameraForm(CameraData());
  connect(frm, SIGNAL(saveCameraData(CameraData)), this,
          SLOT(saveCameraData(CameraData)));

  frm->setAttribute(Qt::WA_DeleteOnClose);
  frm->setModal(true);
  frm->show();
}

void MainWindow::on_actionManageCameras_triggered() {
  Cameras *frm = new Cameras(&db, this->cms);
  connect(frm, SIGNAL(saveCameraData(CameraData)), this,
          SLOT(saveCameraData(CameraData)));
  connect(frm, SIGNAL(deleteCamera()), this, SLOT(placeCameras()));
  frm->setAttribute(Qt::WA_DeleteOnClose);
  frm->setModal(true);
  frm->show();
}

void MainWindow::on_photosListWidget_itemDoubleClicked(QListWidgetItem *item) {
  PhotoData data = photos->getPhotos().value(photoLastId);
  QString photo =
      QString(QCoreApplication::applicationDirPath() + "/pictures/%1")
          .arg(data.photoName);

  //"file:///home/fstigre/fileName.pdf"
  QDesktopServices::openUrl(QUrl(photo));

  //  Mat img = imread(photo.toStdString());
  //  imshow(QString("F: %1").arg(data.photoName).toStdString(), img);
  //  waitKey(0);
}

void MainWindow::on_actionClearPhotos_triggered() {
  photos->deleteAll();
  MyUtils::clearFolder(
      QString(QCoreApplication::applicationDirPath() + "/pictures"));
}

void MainWindow::on_checkBox_toggled(bool checked) {
  db.SetConf("autosend", (checked) ? "true" : "false");
}

void MainWindow::on_actionHistory_triggered() {
  RerportFrm *frm = new RerportFrm(this, &db);
  frm->setAttribute(Qt::WA_DeleteOnClose);
  frm->setModal(true);
  frm->show();
}
