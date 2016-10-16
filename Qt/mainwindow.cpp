#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QTableWidget *table= this->findChild<QTableWidget*>("tableWidget");
	Q_ASSERT(table);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
													tr("data (*.csv *.gwr);; Any File(*)"));
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setText(fileName);
	driver.FileSelected(fileName);
}

void MainWindow::on_LoadButton_clicked()
{
	NamedColumnDoubleTable *loadedTable= driver.LoadFileButtonCliked();
	FILE *tempFile= tmpfile ();
	Q_ASSERT(tempFile);
	NamedColumnDoubleTable_PrintAll(loadedTable, tempFile, "%s\t\t", "%lf\t\t", NEW_LINE);
	rewind(tempFile);
	QString str= "";
	char aux;
	while( EOF != (aux = getc(tempFile) ) )
	{
		str+= aux;
	}
	fclose(tempFile);
	QTextEdit *output= this->findChild<QTextEdit*>("OutputTextEdit");
	Q_ASSERT(output);
	output->setText(str);
	int numberOfColumns= loadedTable->matrix->columns;
	QListWidget *list= this->findChild<QListWidget*>("variablesList");
	Q_ASSERT(list);
	list->clear();
	for(int cont=0; cont < numberOfColumns;cont++)
	{
		list->addItem(QString(loadedTable->columnsName[cont] ) );
	}
	QTabWidget *tabWidget = this->findChild<QTabWidget *>("tabWidget");
	Q_ASSERT(tabWidget);
	tabWidget->setCurrentIndex(1);
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

void MainWindow::MoveBetweenQListWidgets(QString const &source, QString const &target, bool onlyOneElementOnTarget)
{
	QListWidget *sourceList = this->findChild<QListWidget*>(source);
	Q_ASSERT(sourceList);
	QListWidget *targetList = this->findChild<QListWidget*>(target);
	Q_ASSERT(targetList);
	//verificar casos especiais (por exemplo se já tem algo na dependentVariable)
	if(0 < targetList->count() && onlyOneElementOnTarget)
	{
		sourceList->addItem(targetList->takeItem(0));
		targetList->addItem(sourceList->takeItem(sourceList->currentRow()));
	}
	else
	{
		targetList->addItem(sourceList->takeItem(0));
	}
}

void MainWindow::on_dependentVariableInButton_clicked()
{
	MoveBetweenQListWidgets("variablesList", "dependentVariable", true);
}

void MainWindow::on_dependentVariableOutButton_clicked()
{
	MoveBetweenQListWidgets("dependentVariable", "variablesList", false);
}

void MainWindow::on_modelReadyButton_clicked()
{
	QTabWidget *tabWidget = this->findChild<QTabWidget *>("tabWidget");
	Q_ASSERT(tabWidget);
	tabWidget->setCurrentIndex(2);
}
