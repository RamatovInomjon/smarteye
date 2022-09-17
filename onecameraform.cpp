#include "onecameraform.h"
#include "ui_onecameraform.h"

OneCameraForm::OneCameraForm(CameraData camera, QWidget *parent)
    : QDialog(parent), ui(new Ui::OneCameraForm) {
  ui->setupUi(this);
  this->camera = camera;
  ui->edtCameraIP->setText(camera.cameraIP);
  ui->edtCameraName->setText(camera.cameraName);
  ui->edtLogin->setText(camera.login);
  ui->edtPassword->setText(camera.password);
  ui->sbFirePerc->setValue(camera.firePercentage);
  ui->sbSmokePerc->setValue(camera.firePercentage);
}

OneCameraForm::~OneCameraForm() { delete ui; }

void OneCameraForm::on_buttonBox_accepted() {
  this->camera.cameraIP = ui->edtCameraIP->text();
  this->camera.cameraName = ui->edtCameraName->text();
  this->camera.login = ui->edtLogin->text();
  this->camera.password = ui->edtPassword->text();
  this->camera.cameraIP = ui->edtCameraIP->text();
  this->camera.firePercentage = ui->sbFirePerc->value();
  this->camera.smokePercentage = ui->sbSmokePerc->value();
  this->camera.status = ui->cbStatus->currentIndex();
//  qDebug() << "Try to Save";
  emit(saveCameraData(camera));
}
