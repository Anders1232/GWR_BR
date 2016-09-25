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

void MainWindow::on_SelectFileButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
													"/home",
													tr("data (*.csv *.gwr *.dota);; Any File(*)"));
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setText(fileName);
	driver.FileSelected(fileName);
}

void MainWindow::on_LoadButton_clicked()
{
	driver.LoadFileButtonCliked();
}

void MainWindow::on_TabRadioButton_clicked()
{
	driver.DelimiterSelected('\t');
}

void MainWindow::on_SemicolonRadioButton_clicked()
{
	driver.DelimiterSelected(';');
}

void MainWindow::on_DelimiterLineEdit_textEdited(const QString &arg1)
{
	std::string str= arg1.toStdString();
	char lastCharacter[2];
	lastCharacter[0]= str[str.size()-1];
	lastCharacter[1]= '\0';
	driver.DelimiterSelected(lastCharacter[0]);
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("DelimiterLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setText(lastCharacter);
	QRadioButton *otherRadioButton = this->findChild<QRadioButton*>("OtherRadioButton");
	Q_ASSERT(otherRadioButton);
	otherRadioButton->setChecked(true);
	otherRadioButton->setEnabled(true);
}
