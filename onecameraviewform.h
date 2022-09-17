#ifndef ONECAMERAVIEWFORM_H
#define ONECAMERAVIEWFORM_H

#include <HCNetSDK.h>
#include <QDialog>
#include <camera_data.h>
#include <imagerec.h>
#include <onecameraviewform.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include <plaympeg4.h>
#include "PlayM4.h"
//#include <unistd.h>
namespace Ui {
class OneCameraViewForm;
}

class OneCameraViewForm : public QDialog {
  Q_OBJECT

public:
  explicit OneCameraViewForm(CameraData camera, QWidget *parent = nullptr);
  ~OneCameraViewForm();

private:
  Ui::OneCameraViewForm *ui;
};

#endif // ONECAMERAVIEWFORM_H
