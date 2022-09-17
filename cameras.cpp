#include "cameras.h"
#include "ui_cameras.h"

Cameras::Cameras(DataBase *db, CameraModel *cms, QWidget *parent)
    : QDialog(parent), ui(new Ui::Cameras) {
  ui->setupUi(this);
  this->cms = cms;
  this->db = db;
  refresh();
}

Cameras::~Cameras() { delete ui; }

void Cameras::on_btnAddCamera_clicked() {
  OneCameraForm *frm = new OneCameraForm(CameraData());
  connect(frm, SIGNAL(saveCameraData(CameraData)),
          SIGNAL(saveCameraData(CameraData)));
  connect(frm, SIGNAL(saveCameraData(CameraData)), SLOT(refresh()));
  frm->setAttribute(Qt::WA_DeleteOnClose);
  frm->setModal(true);
  frm->show();
}

void Cameras::on_btnEditCamera_clicked() {
  QModelIndex index = ui->tableView->currentIndex();
  if (index.row() >= 0) {
    int id = model->data(model->index(index.row(), 0)).toInt();
    for (auto c : cms->getCameras()) {
      qDebug() << "ID: " << id << " IDC: " << c.id;
      if (c.id == id) {
        OneCameraForm *frm = new OneCameraForm(c);
        connect(frm, SIGNAL(saveCameraData(CameraData)),
                SIGNAL(saveCameraData(CameraData)));
        connect(frm, SIGNAL(saveCameraData(CameraData)), SLOT(refresh()));
        frm->setAttribute(Qt::WA_DeleteOnClose);
        frm->setModal(true);
        frm->show();
        return;
      }
    }
  }
}

void Cameras::on_btnDeleteCamera_clicked() {
  QModelIndex index = ui->tableView->currentIndex();
  if (index.row() >= 0) {
    int id = model->data(model->index(index.row(), 0)).toInt();
    cms->deleteCamera(id);
    emit(deleteCamera());
    refresh();
  }
}

void Cameras::refresh() {
  model =
      db->QryModel("SELECT id, camera_name, ip, login FROM camera ORDER BY id");
  // ui->tableView->setColumnHidden(2, true);
  model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Название"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("IP"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Логин"));
  ui->tableView->setModel(model);
}
