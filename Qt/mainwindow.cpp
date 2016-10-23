#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "QFileDialog"
//#include "QTableWidget"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
/*	QTableWidget *table= this->findChild<QTableWidget*>("tableWidget");
	Q_ASSERT(table);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);*/
	QSpinBox *linesSpinBox = this->findChild<QSpinBox *>("previewLines");
	Q_ASSERT(linesSpinBox);
	this->linesInPreview= linesSpinBox->value();
	QSpinBox *charsSpinBox = this->findChild<QSpinBox *>("previewCharsPerLine");
	Q_ASSERT(charsSpinBox);
	this->charsPerLineInPreview= charsSpinBox->value();

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
//	driver.Load(fileName, linesInPreview, charsPerLineInPreview);
	UpdatePreview();
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
	tabWidget->setCurrentIndex(2);
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
	bool sourceSelectable= !(sourceList->selectionMode() == QAbstractItemView::NoSelection);
	int itemToTake= sourceSelectable? sourceList->currentRow() : 0;
	targetList->addItem(sourceList->takeItem(itemToTake));
	if(1 < targetList->count() && onlyOneElementOnTarget)
	{
		sourceList->addItem(targetList->takeItem(0));
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
	tabWidget->setCurrentIndex(3);
}

void MainWindow::on_offsetVariableInButton_clicked()
{
	MoveBetweenQListWidgets("variablesList", "offsetVariable", true);
}

void MainWindow::on_offsetVariableOutButton_clicked()
{
	MoveBetweenQListWidgets("offsetVariable", "variablesList", false);
}

void MainWindow::on_identifierInButton_clicked()
{
	MoveBetweenQListWidgets("variablesList", "identifierVariable", true);
}

void MainWindow::on_identifierOutButton_clicked()
{
	MoveBetweenQListWidgets("identifierVariable", "variablesList", false);
}

void MainWindow::on_longitudeInButton_clicked()
{
	MoveBetweenQListWidgets("variablesList", "longitudeVariable", true);
}

void MainWindow::on_longitudeOutButton_clicked()
{
	MoveBetweenQListWidgets("longitudeVariable", "variablesList", false);
}

void MainWindow::on_latitudeInButton_clicked()
{
	MoveBetweenQListWidgets("variablesList", "latitudeVariable", true);
}

void MainWindow::on_latitudeOutButton_clicked()
{
	MoveBetweenQListWidgets("latitudeVariable", "variablesList", false);
}

void MainWindow::on_localVariablesOut_clicked()
{
	MoveBetweenQListWidgets("localVariablesList", "variablesList", false);
}

void MainWindow::on_localVariablesIn_clicked()
{
	MoveBetweenQListWidgets("variablesList", "localVariablesList", false);
}

void MainWindow::on_globalVariablesOut_clicked()
{
	MoveBetweenQListWidgets("globalVariablesList", "variablesList", false);
}

void MainWindow::on_globalVariablesIn_clicked()
{
	MoveBetweenQListWidgets("variablesList", "globalVariablesList", false);
}

void MainWindow::on_pushButton_clicked()
{
	QTabWidget *tabWidget = this->findChild<QTabWidget *>("tabWidget");
	Q_ASSERT(tabWidget);
	tabWidget->setCurrentIndex(1);
}

void MainWindow::UpdatePreview(void)
{
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	if("" != lineEdit->text())
	{
		QString preview = driver.GetPreview(linesInPreview, charsPerLineInPreview);
		QTextEdit *previewArea = this->findChild<QTextEdit *>("previewText");
		Q_ASSERT(previewArea);
		previewArea->setText(preview);

//		GuiDriver.FileSelected();
	}
}

/*void MainWindow::on_previewLines_editingFinished()
{
	QSpinBox *linesSpinBox = this->findChild<QSpinBox *>("previewLines");
	Q_ASSERT(linesSpinBox);
	this->linesInPreview= linesSpinBox->value();
	UpdatePreview();
}*/

/*void MainWindow::on_previewCharsPerLine_editingFinished()
{
	QSpinBox *linesSpinBox = this->findChild<QSpinBox *>("previewCharsPerLine");
	Q_ASSERT(linesSpinBox);
	this->charsPerLineInPreview= linesSpinBox->value();
	UpdatePreview();
}*/

void MainWindow::on_previewLines_valueChanged(int arg1)
{
	this->linesInPreview= arg1;
	UpdatePreview();
}

void MainWindow::on_previewCharsPerLine_valueChanged(int arg1)
{
	this->charsPerLineInPreview= arg1;
	UpdatePreview();
}
