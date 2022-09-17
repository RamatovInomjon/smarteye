#include "settingfrm.h"
#include "ui_settingfrm.h"
//#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>

SettingFrm::SettingFrm(QWidget *parent, DataBase *mdb)
    : QDialog(parent), ui(new Ui::SettingFrm) {
  ui->setupUi(this);
  db = mdb;

  ui->edtPhotoDir->setText(db->GetConf("imgDirectory"));
  ui->edtChatId->setText(db->GetConf("chatId"));
  ui->edtBotToken->setText(db->GetConf("botToken"));
}

SettingFrm::~SettingFrm() { delete ui; }

void SettingFrm::on_btnSetport_clicked() {
  // comlist.clear();

  // for(int i=0;i<ui->comList->count(); i++){
  //    QListWidgetItem * item=ui->comList->item(i);
  //     if(item->checkState()==Qt::Checked) comlist<<item->text();
  // }

  //// foreach(QListWidgetItem * item, ui->comList->)
  //// {
  ////     if(item->checkState()==Qt::Checked) comlist<<item->text();
  //// }

  // if(db->SetConf("port", comlist.join(",") )){//ui->cbPortname->currentText()
  //     db->SetConf("comtimeout", QString::number(ui->sbSpeed->value()));
  //     QMessageBox::information(0,"Информация", "Настройки COM порта успешно
  //     сохранились"); this->hide();
  // }else
  //  QMessageBox::warning(0,"Информация", "Настройки COM порта не сохранились
  //  обратитесь к админстратору.");
}
