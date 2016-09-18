#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionManual_triggered()
{

}

void MainWindow::on_pushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
													"/home",
													tr("data (*.csv *.gwr *.dota);; Any File(*.*)"));
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setText(fileName);
	driver.FileSelected(fileName);
}

void MainWindow::on_pushButton_2_clicked()
{
	driver.LoadFileButtonCliked();
}
