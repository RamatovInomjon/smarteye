#ifndef ABOUTUSFRM_H
#define ABOUTUSFRM_H

#include <QDialog>

namespace Ui {
class aboutUsFrm;
}

class aboutUsFrm : public QDialog
{
    Q_OBJECT

public:
    explicit aboutUsFrm(QWidget *parent = nullptr);
    ~aboutUsFrm();

private:
    Ui::aboutUsFrm *ui;
};

#endif // ABOUTUSFRM_H
