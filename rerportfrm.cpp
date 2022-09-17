#include "rerportfrm.h"
#include "ui_rerportfrm.h"

RerportFrm::RerportFrm(QWidget *parent, DataBase *mdb, int changeid)
    : QDialog(parent), ui(new Ui::RerportFrm) {
  ui->setupUi(this);
  db = mdb;
  ui->dtbegin->setDateTime(
      QDateTime::currentDateTime()); // dateTime(QDateTime::currentDateTime());
  ui->dtend->setDateTime(QDateTime::currentDateTime());
  on_btnPrepare_clicked();
  //  QTimer::singleShot(4000, this, SLOT(showrepbtn()));
}

RerportFrm::~RerportFrm() { delete ui; }

void RerportFrm::on_btnPrepare_clicked() {

  // emit getSmetLitrs();

  QString qry = "SELECT detected_photos.id, photo_name, created, camera_id, "
                "is_smoke, is_fire, detected_photos.status, created_time, "
                "camera_name  FROM detected_photos LEFT JOIN camera ON "
                "camera.id=detected_photos.camera_id %1";

  //  QString qry =
  //      "SELECT \"ChangeID\" AS \"СМЕНА\", \"PistoletID\" AS \"ПИСТОЛЕТ\",
  //      "
  //      "(\"Liters\"/100.0) AS \"ЛИТРЫ\", (\"OrderLiters\"/100.0) AS
  //      \"ЗАЯВКА " "ЛИТРЫ\", to_char(\"MoneyCash\", 'FM99999999.99') AS
  //      \"СУММА\", " "to_char(\"OrderMoney\"/100, 'FM99999999.99') AS
  //      \"ЗАЯВКА СУММА\", "
  //      "\"Price\" AS \"ЦЕНА ЗА 1 ЛИТР\", \"DateTime\" AS \"ДАТА\" FROM "
  //      "tabmaindata  %1 ORDER BY \"ChangeID\", \"DataID\" DESC";

  //======Esli summarniy Pistolet otmechen==========
  //  if (ui->chbSumPis->isChecked()) {
  //    if (ui->sbSmena->value() > 0)
  //      qry = "SELECT \"PistoletID\" AS \"ПИСТОЛЕТ\", "
  //            "to_char(Sum(\"Liters\")/100.0, 'FM99999999.99') AS
  //            \"ЛИТРЫ\", " "to_char(SUM(\"MoneyCash\"), 'FM99999999.99')
  //            AS \"СУММА\", " "to_char(((SELECT \"SumLitr\" FROM tabtrk
  //            WHERE "
  //            "\"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\")/100.0),
  //            "
  //            "'FM99999999.99') AS \"ИТОГ С КОЛОНКИ\", to_char(((SELECT "
  //            "\"SmenaLitr\" FROM tabtrk WHERE "
  //            "\"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\")/100.0),
  //            "
  //            "'FM99999999.99') AS \"СМЕНА С КОЛОНКИ\" FROM tabmaindata %1
  //            GROUP " "BY \"PistoletID\" ORDER BY \"PistoletID\"";
  //    else
  //      qry = "SELECT \"ChangeID\" AS \"СМЕНА\", \"PistoletID\" AS
  //      \"ПИСТОЛЕТ\", "
  //            "to_char(Sum(\"Liters\")/100.0, 'FM99999999.99') AS
  //            \"ЛИТРЫ\", " "to_char(SUM(\"MoneyCash\"), 'FM99999999.99')
  //            AS \"СУММА\", " "to_char(((SELECT \"SumLitr\" FROM tabtrk
  //            WHERE "
  //            "\"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\")/100.0),
  //            "
  //            "'FM99999999.99') AS \"ИТОГ С КОЛОНКИ\", to_char(((SELECT "
  //            "\"SmenaLitr\" FROM tabtrk WHERE "
  //            "\"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\")/100.0),
  //            "
  //            "'FM99999999.99') AS \"СМЕНА С КОЛОНКИ\"  FROM tabmaindata
  //            %1 " "GROUP BY \"PistoletID\", \"ChangeID\"  ORDER BY
  //            \"ChangeID\" " "DESC, \"PistoletID\"";
  //  }

  //  //====== Esli summarniy po vidu topliva otmechen ==========
  //  if (ui->chbSumVT->isChecked())
  //    qry = "SELECT (SELECT \"GasName\" FROM tabgastype WHERE
  //    \"Num\"=\"TRKID\") "
  //          "AS \"ВИД ТОПЛИВА\", to_char(Sum(\"Liters\")/100.0,
  //          'FM99999999.99') " "AS \"ЛИТРЫ\",  to_char(SUM(\"MoneyCash\"),
  //          'FM99999999.99') AS "
  //          "\"СУММА\", to_char((SUM(\"tabtrk\".\"SumLitr\")/100.0), "
  //          "'FM99999999.99') AS \"ИТОГ С КОЛОНКИ\", to_char(((SELECT "
  //          "\"SmenaLitr\" FROM tabtrk WHERE "
  //          "\"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\")/100.0),
  //          "
  //          "'FM99999999.99') AS \"СМЕНА С КОЛОНКИ\"  FROM tabmaindata
  //          LEFT JOIN " "tabtrk ON
  //          \"tabtrk\".\"PistoletID\"=\"tabmaindata\".\"PistoletID\" "
  //          "%1 GROUP BY \"TRKID\"";

  //  //======== Ustanavlivaem filtr po date =================
  QString condition = " WHERE true ";
  if (ui->chbIsDate->isChecked()) {
    QString begindate = ui->dtbegin->text();
    QString enddate = ui->dtend->text();
    qDebug() << "Filtr po date: " << begindate;

    condition += QString(" AND created BETWEEN '%1' AND '%2'")
                     .arg(begindate)
                     .arg(enddate);
  }

  //========Esli vibranna opredelyonnay smena=================
  //  if (ui->sbSmena->value()>0){
  //   condition+=QString(" AND \"ChangeID\"=%1").arg(ui->sbSmena->text());
  //  }

  model = db->QryModel(QString(qry).arg(condition));

  //  QSqlQuery query;
  //  bool isexec = query.exec(QString(tot).arg(condition));
  //  ui->btnPrint->setEnabled(isexec);
  //  if (isexec) {
  //    if (query.first()) {
  //      // return query.value(0).toInt();
  //      qDebug() << "Otchyot";
  //      ui->lblLitrSum->setText(
  //          "Итого объем: " + QString::number(query.value(0).toFloat(), 'f',
  //          1));
  //      ui->lblSumTot->setText("Итого сумма: " +
  //                             QString::number(query.value(1).toFloat(), 'f',
  //                             1));
  //    }
  //  }
  //  qDebug() << "Osibhka otchyot: " << query.lastError();

  ui->tableView->setModel(model);
  ui->tableView->show();
  // Vivod ITOGO
}

void RerportFrm::on_btnPrint_clicked() {
  const QString htmlFileName =
      QString("%1/%2").arg(qApp->applicationDirPath()).arg("myTable.html");
  if (createHtmlTableFromModel())
    // QDesktopServives::openUrl( QUrl::fromLocalFile(htmlFileName));
    QDesktopServices::openUrl(QUrl::fromLocalFile(htmlFileName));
  /*#ifndef QT_NO_PRINTER
      QTextBrowser *editor = new QTextBrowser(); //static_cast<QTextBrowser*
  >(textBrowser); QPrinter printer; QPrintDialog *dialog = new
  QPrintDialog(&printer, this); dialog->setWindowTitle(tr("Print Document")); if
  (editor->textCursor().hasSelection())
          dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
      if (dialog->exec() != QDialog::Accepted)
          return;

      editor->print(&printer);
  #endif*/
}

bool RerportFrm::createHtmlTableFromModel() {
  // make a html-dump of table view

  QTableView *tableView = ui->tableView;
  if (tableView) {

    const QString htmlFileName =
        QString("%1/%2").arg(qApp->applicationDirPath()).arg("myTable.html");

    QFile file(htmlFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      // MSG(QString("Can`t create file %1").arg(htmlFileName));
      return false;
    }

    QTextStream out(&file);

    const long long int rowCount = tableView->model()->rowCount();
    const long long int columnCount = tableView->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg(refTitleName)
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<tr bgcolor=#f0f0f0>";
    for (long long int column = 0; column < columnCount; column++)
      if (!tableView->isColumnHidden(column))
        out << QString("<th>%1</th>")
                   .arg(tableView->model()
                            ->headerData(column, Qt::Horizontal)
                            .toString());
    out << "</tr>\n";
    file.flush();

    // data table
    for (long long int row = 0; row < rowCount; row++) {
      out << "<tr>";
      for (long long int column = 0; column < columnCount; column++) {
        if (!tableView->isColumnHidden(column)) {
          QString data = tableView->model()
                             ->data(tableView->model()->index(row, column))
                             .toString()
                             .simplified();
          out << QString("<td bkcolor=0>%1</td>")
                     .arg((!data.isEmpty()) ? data : QString("&nbsp;"));
        }
      }
      out << "</tr>\n";
    }
    out << "</table>\n"
           "</body>\n"
           "</html>\n";

    file.close();
  }

  return true;
}

void RerportFrm::on_tableView_doubleClicked(const QModelIndex &index) {

  if (index.row() >= 0) {
    QString photoName = model->data(model->index(index.row(), 1)).toString();
    QString photo =
        QString(QCoreApplication::applicationDirPath() + "/pictures/%1")
            .arg(photoName);
    QDesktopServices::openUrl(QUrl(photo));
    //    Mat img = imread(photo.toStdString());
    //    imshow(QString("F: %1").arg(photoName).toStdString(), img);
    //    waitKey(0);

    //    for (auto c : cms->getCameras()) {
    //      qDebug() << "ID: " << id << " IDC: " << c.id;
    //      if (c.id == id) {
    //        OneCameraForm *frm = new OneCameraForm(c);
    //        connect(frm, SIGNAL(saveCameraData(CameraData)),
    //                SIGNAL(saveCameraData(CameraData)));
    //        connect(frm, SIGNAL(saveCameraData(CameraData)), SLOT(refresh()));
    //        frm->setAttribute(Qt::WA_DeleteOnClose);
    //        frm->setModal(true);
    //        frm->show();
    //        return;
    //      }
    //    }
  }
}
