#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->listView->setModel(new QStringListModel(QStringList()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
  QStringList sl;
  sl.append(ui->lineEdit->text());
  ((QStringListModel*) ui->listView->model())->setStringList(sl);
  /*
model->insertRow(model->rowCount());
QModelIndex index = model->index(model->rowCount()-1)
model->setData(index, str);
   */
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //emit setText(arg1);

}
