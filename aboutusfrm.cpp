#include "aboutusfrm.h"
#include "ui_aboutusfrm.h"

aboutUsFrm::aboutUsFrm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutUsFrm)
{
    ui->setupUi(this);
}

aboutUsFrm::~aboutUsFrm()
{
    delete ui;
}
