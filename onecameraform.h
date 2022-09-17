#ifndef ONECAMERAFORM_H
#define ONECAMERAFORM_H

#include <QDialog>
#include <camera_data.h>

namespace Ui {
class OneCameraForm;
}

class OneCameraForm : public QDialog {
  Q_OBJECT

public:
  explicit OneCameraForm(CameraData camera, QWidget *parent = nullptr);
  ~OneCameraForm();
signals:
  void saveCameraData(CameraData camera);

private slots:
  void on_buttonBox_accepted();

private:
  Ui::OneCameraForm *ui;
  CameraData camera;
};

#endif // ONECAMERAFORM_H
