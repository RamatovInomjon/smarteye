#ifndef SETTINGFRM_H
#define SETTINGFRM_H

#include <QDialog>
#include "database.h"
namespace Ui {
class SettingFrm;
}

class SettingFrm : public QDialog
{
    Q_OBJECT

public:
    QStringList comlist;
    explicit SettingFrm(QWidget *parent = 0, DataBase *mdb=0);
    ~SettingFrm();

private slots:
    void on_btnSetport_clicked();

private:
    Ui::SettingFrm *ui;
    DataBase *db;
};

#endif // SETTINGFRM_H
