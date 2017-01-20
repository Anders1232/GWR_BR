#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
//#include "QTableWidget"

static std::string RemovePath(std::string const &str)
{
	std::string ret= "";
	char temp[2];
	temp[1]= '\0';
	for(int cont =str.size()-1; cont >= 0; cont--)
	{
		temp[0]= str[cont];
		if( (temp[0] == '\\' ) ||  ( temp[0] == '/' ))
		{
			break;
		}
		ret.insert(0, temp);
	}
	return ret;
}

static std::string RemoveExtension(std::string const &str)
{
	std::string ret= "";
	unsigned int count=0;
	while(count < str.size())
	{
		if('.' == str[count])
		{
			break;
		}
		ret+= str[count];
		count++;
	}
	return ret;
}

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

	QRadioButton *latPlusLon = this->findChild<QRadioButton *>("LatPlusLonOperation");
	Q_ASSERT(latPlusLon);
	latPlusLon->click();
//	this->charsPerLineInPreview= charsSpinBox->value();
	coordsMode= PROJECTED;
	QRadioButton *coordProjected = this->findChild<QRadioButton *>("modelProjected");
	Q_ASSERT(coordProjected);
	coordProjected->click();

	//setting kernel radio buttons
	QRadioButton *radioButton = this->findChild<QRadioButton *>("kernelFixedGausButton");
	Q_ASSERT(radioButton);
	radioButton->click();
	radioButton = this->findChild<QRadioButton *>("kernelAICc");
	Q_ASSERT(radioButton);
	radioButton->click();

	tabWidget = this->findChild<QTabWidget *>("tabWidget");
	Q_ASSERT(tabWidget);
	tabWidget->setCurrentIndex(0);

	outputFileLineEdit= this->findChild<QLineEdit *>("OutputFileLineEdit");
	Q_ASSERT(outputFileLineEdit);

	goldenRadioButton= this->findChild<QRadioButton *>("KernelGoldenRadioButton");
	Q_ASSERT(goldenRadioButton);
	singleRadioButton= this->findChild<QRadioButton *>("KernelSingleRadioButton");
	Q_ASSERT(singleRadioButton);
	intervalSearchRadioButton= this->findChild<QRadioButton *>("KernelIntervalRadioButton");
	Q_ASSERT(intervalSearchRadioButton);

	QLineEdit *lineEdit= this->findChild<QLineEdit *>("KernelGoldenMinLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setValidator(new QDoubleValidator());

	lineEdit= this->findChild<QLineEdit *>("KernelGoldenMaxLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setValidator(new QDoubleValidator());

	lineEdit= this->findChild<QLineEdit *>("KernelIntervalMinLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setValidator(new QDoubleValidator());

	lineEdit= this->findChild<QLineEdit *>("KernelIntervalMaxLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setValidator(new QDoubleValidator());

	lineEdit= this->findChild<QLineEdit *>("KernelIntervalLineEdit");
	Q_ASSERT(lineEdit);
	lineEdit->setValidator(new QDoubleValidator());

	separator = '\t';
	fileName= "";
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
	this->fileName= fileName.toLocal8Bit().data();
//	driver.Load(fileName, linesInPreview, charsPerLineInPreview);
	UpdatePreview();
}

void MainWindow::ShowErrorMessage(QString errorMessage)
{
	QMessageBox msgBox;
	msgBox.setText(errorMessage);
	msgBox.exec();
	return;
}

void MainWindow::on_LoadButton_clicked()
{
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	if("" ==lineEdit->text())
	{
		ShowErrorMessage("No file selected!");
		return;
	}

	QLabel *auxLabel= this->findChild<QLabel*>("executeFileLabel");
	Q_ASSERT(auxLabel);
	auxLabel->setText(QString("File: ") + lineEdit->text());

	auxLabel= this->findChild<QLabel*>("executeDelimiterLabel");
	Q_ASSERT(auxLabel);

	if('\t' == separator)
	{
		auxLabel->setText(QString("Delimiter: \\t") );
	}
	else
	{
		auxLabel->setText(QString("Delimiter: ") + separator);
	}

	NamedColumnDoubleTable *loadedTable= driver.LoadFileButtonCliked(fileName.toStdString(), separator);
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
	ClearLists();
	QListWidget *list= this->findChild<QListWidget*>("variablesList");
	Q_ASSERT(list);
	for(int cont=0; cont < numberOfColumns;cont++)
	{
		list->addItem(QString(loadedTable->columnsName[cont] ) );
	}


	outputFileLineEdit->setText(QString::fromStdString( RemoveExtension(RemovePath(fileName.toStdString()) ) + "_result.txt") );

	tabWidget->setCurrentIndex(2);
}

void MainWindow::ClearLists(void)
{
/*	QWidget *tab2= this->findChild<QWidget*>("Model");
	Q_ASSERT(tab2);
	QList<QListWidget *> list= tab2->findChildren<QListWidget*>(QRegularExpression("*"));
	for(int count=0; count < list.count(); count++)
	{
		list[count]->clear();
	}
*/
	QListWidget *list;
	list= this->findChild<QListWidget*>("variablesList");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("identifierVariable");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("latitudeVariable");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("longitudeVariable");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("offsetVariable");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("localVariablesList");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("globalVariablesList");
	Q_ASSERT(list);
	list->clear();
	list= this->findChild<QListWidget*>("dependentVariable");
	Q_ASSERT(list);
	list->clear();

}

void MainWindow::on_TabRadioButton_clicked()
{
	separator= '\t';
}

void MainWindow::on_SemicolonRadioButton_clicked()
{
	separator= ';';
}

void MainWindow::on_DelimiterLineEdit_textEdited(const QString &arg1)
{
	std::string str= arg1.toStdString();
	char lastCharacter[2];
	lastCharacter[0]= str[str.size()-1];
	lastCharacter[1]= '\0';
	separator= lastCharacter[0];
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
	QListWidget *variablesList= this->findChild<QListWidget*>("variablesList");
	Q_ASSERT(variablesList);
	std::list<std::string> *vList= GetStringList(variablesList);

	QListWidget *identifierVariableList= this->findChild<QListWidget*>("identifierVariable");
	Q_ASSERT(identifierVariableList);
	std::string identifer;
	if(0 == identifierVariableList->count())
	{
		identifer= "";
	}
	else
	{
		identifer= identifierVariableList->item(0)->text().toStdString();
	}

	QLabel* aux= this->findChild<QLabel *>("executeIDLabel");
	Q_ASSERT(aux);
	aux->setText(QString("ID column: ") + identifer.c_str());

	aux= this->findChild<QLabel *>("executeModelTypeLabel");
	Q_ASSERT(aux);
	QRadioButton *radioButton= this->findChild<QRadioButton *>("LatPlusLonOperation");
	Q_ASSERT(radioButton);
	if(true == radioButton->isChecked())
	{
		aux->setText(QString("Model type: Lat + Lon"));
	}
	else
	{
		QRadioButton *radioButton= this->findChild<QRadioButton *>("dependentTimesOffsetOperation");
		Q_ASSERT(radioButton);
		if(true == radioButton->isChecked())
		{
			aux->setText(QString("Model type: Dependent * Offset"));
		}
		else
		{
			QRadioButton *radioButton= this->findChild<QRadioButton *>("distanceToOrigin");
			Q_ASSERT(radioButton);
			if(true == radioButton->isChecked())
			{
				aux->setText(QString("Model type: Distance to origin from (Lat, Lon)"));
			}
			else
			{
				fprintf(stderr,"[ERROR] Operation not Selected!\t\t%s:%d" NEW_LINE, __FILE__, __LINE__);
			}
		}
	}

	QListWidget *latitudeVariableList= this->findChild<QListWidget*>("latitudeVariable");
	Q_ASSERT(latitudeVariableList);
	std::string latitude;
	if(0 == latitudeVariableList->count())
	{
		latitude= "";
	}
	else
	{
		latitude = latitudeVariableList->item(0)->text().toStdString();
	}

	QListWidget *longitudeVariableList= this->findChild<QListWidget*>("longitudeVariable");
	Q_ASSERT(longitudeVariableList);
	std::string longitude;
	if(0 == longitudeVariableList->count())
	{
		longitude= "";
	}
	else
	{
		longitude= longitudeVariableList->item(0)->text().toStdString();
	}


	QListWidget *offsetVariableList= this->findChild<QListWidget*>("offsetVariable");
	Q_ASSERT(offsetVariableList);
	std::string offset;
	if(0 == offsetVariableList->count())
	{
		offset= "";
	}
	else
	{
		offset= offsetVariableList->item(0)->text().toStdString();
	}

	QListWidget *localVariablesList= this->findChild<QListWidget*>("localVariablesList");
	Q_ASSERT(localVariablesList);
	std::list<std::string> *lvList= GetStringList(localVariablesList);

	QListWidget *globalVariablesList= this->findChild<QListWidget*>("globalVariablesList");
	Q_ASSERT(globalVariablesList);
	std::list<std::string> *gvList= GetStringList(globalVariablesList);

	QListWidget *dependentVariableList= this->findChild<QListWidget*>("dependentVariable");
	Q_ASSERT(dependentVariableList);
	std::string dependent;
	if(0 == dependentVariableList->count())
	{
		dependent= "";
	}
	else
	{
		dependent= dependentVariableList->item(0)->text().toStdString();
	}

	if(MODE_DEPENDENT_TIMES_OFFSET == modelType && 0 == dependentVariableList->count())
	{
		ShowErrorMessage("Dependent variable not set.");
		return;
	}
	if(MODE_DEPENDENT_TIMES_OFFSET == modelType && 0 == offsetVariableList->count())
	{
		ShowErrorMessage("Offset variable not set.");
		return;
	}
	if( (MODE_DISTANCE_TO_ORIGIN == modelType || MODE_LAT_PLUS_LON == modelType ) && 0 == latitudeVariableList->count())
	{
		ShowErrorMessage("Latitude not set.");
		return;
	}
	if( (MODE_DISTANCE_TO_ORIGIN == modelType || MODE_LAT_PLUS_LON == modelType ) && 0 == longitudeVariableList->count())
	{
		ShowErrorMessage("Longitude not set.");
		return;
	}

	tabWidget->setCurrentIndex(3);

	delete vList;
	delete lvList;
	delete gvList;
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

void MainWindow::on_ModelChooseFileButton_clicked()
{
	tabWidget->setCurrentIndex(1);
}

void MainWindow::UpdatePreview(void)
{
	QLineEdit *lineEdit = this->findChild<QLineEdit*>("fileNameLineEdit");
	Q_ASSERT(lineEdit);
	if("" != lineEdit->text())
	{
		QString preview = QString(driver.GetPreview(fileName.toStdString(), linesInPreview, charsPerLineInPreview).c_str() );
//		QString preview = QString(driver.GetPreview(fileName.toLocal8Bit().toStdString() linesInPreview, charsPerLineInPreview).c_str() );
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

void MainWindow::on_LatPlusLonOperation_clicked()
{
	modelType= MODE_LAT_PLUS_LON;
}

void MainWindow::on_dependentTimesOffsetOperation_clicked()
{
	modelType= MODE_DEPENDENT_TIMES_OFFSET;
}

void MainWindow::on_distanceToOrigin_clicked()
{
	modelType= MODE_DISTANCE_TO_ORIGIN;
}

std::list<std::string> *MainWindow::GetStringList(QListWidget *widgetList)
{
	std::list<std::string> *ret= new std::list<std::string>();
	for(int count =0; count < widgetList->count(); count++)
	{
		ret->push_back(widgetList->item(count)->text().toStdString());
	}
	return ret;
}

void MainWindow::on_welcomeNextButton_clicked()
{
	tabWidget->setCurrentIndex(1);
}


void MainWindow::on_modelProjected_clicked()
{
	this->coordsMode= PROJECTED;
	QLabel *coordsTypelabel= this->findChild<QLabel *>("executeLocationalTypeLabel");
	Q_ASSERT(coordsTypelabel);
	coordsTypelabel->setText(QString("Coordinates type: Projected"));
}

void MainWindow::on_modelSpherical_clicked()
{
	this->coordsMode= SPHERICAL;
	QLabel *coordsTypelabel= this->findChild<QLabel *>("executeLocationalTypeLabel");
	Q_ASSERT(coordsTypelabel);
	coordsTypelabel->setText(QString("Coordinates type: Spherical"));
}

void MainWindow::on_executeStartComputingButton_clicked()
{
	QListWidget *variablesList= this->findChild<QListWidget*>("variablesList");
	Q_ASSERT(variablesList);
	std::list<std::string> *vList= GetStringList(variablesList);

	QListWidget *identifierVariableList= this->findChild<QListWidget*>("identifierVariable");
	Q_ASSERT(identifierVariableList);
	std::string identifer;
	if(0 == identifierVariableList->count())
	{
		identifer= "";
	}
	else
	{
		identifer= identifierVariableList->item(0)->text().toStdString();
	}

	QListWidget *latitudeVariableList= this->findChild<QListWidget*>("latitudeVariable");
	Q_ASSERT(latitudeVariableList);
	std::string latitude;
	if(0 == latitudeVariableList->count())
	{
		latitude= "";
	}
	else
	{
		latitude = latitudeVariableList->item(0)->text().toStdString();
	}

	QListWidget *longitudeVariableList= this->findChild<QListWidget*>("longitudeVariable");
	Q_ASSERT(longitudeVariableList);
	std::string longitude;
	if(0 == longitudeVariableList->count())
	{
		longitude= "";
	}
	else
	{
		longitude= longitudeVariableList->item(0)->text().toStdString();
	}


	QListWidget *offsetVariableList= this->findChild<QListWidget*>("offsetVariable");
	Q_ASSERT(offsetVariableList);
	std::string offset;
	if(0 == offsetVariableList->count())
	{
		offset= "";
	}
	else
	{
		offset= offsetVariableList->item(0)->text().toStdString();
	}

	QListWidget *localVariablesList= this->findChild<QListWidget*>("localVariablesList");
	Q_ASSERT(localVariablesList);
	std::list<std::string> *lvList= GetStringList(localVariablesList);

	QListWidget *globalVariablesList= this->findChild<QListWidget*>("globalVariablesList");
	Q_ASSERT(globalVariablesList);
	std::list<std::string> *gvList= GetStringList(globalVariablesList);

	QListWidget *dependentVariableList= this->findChild<QListWidget*>("dependentVariable");
	Q_ASSERT(dependentVariableList);
	std::string dependent;
	if(0 == dependentVariableList->count())
	{
		dependent= "";
	}
	else
	{
		dependent= dependentVariableList->item(0)->text().toStdString();
	}

	if(MODE_DEPENDENT_TIMES_OFFSET == modelType && 0 == dependentVariableList->count())
	{
		ShowErrorMessage("Dependent variable not set.");
		return;
	}
	if(MODE_DEPENDENT_TIMES_OFFSET == modelType && 0 == offsetVariableList->count())
	{
		ShowErrorMessage("Offset variable not set.");
		return;
	}
	if( (MODE_DISTANCE_TO_ORIGIN == modelType || MODE_LAT_PLUS_LON == modelType ) && 0 == latitudeVariableList->count())
	{
		ShowErrorMessage("Latitude not set.");
		return;
	}
	if( (MODE_DISTANCE_TO_ORIGIN == modelType || MODE_LAT_PLUS_LON == modelType ) && 0 == longitudeVariableList->count())
	{
		ShowErrorMessage("Longitude not set.");
		return;
	}

	std::string outputfileName= outputFileLineEdit->text().toStdString();

	QString result = driver.Calculate(fileName.toStdString(), separator, outputfileName, modelType, *vList, identifer, dependent, latitude, longitude, offset, *lvList, *gvList);
//	QString result = driver.Calculate(fileName.toLocal8Bit().toStdString(), separator, modelType, *vList, identifer, dependent, latitude, longitude, offset, *lvList, *gvList);

	QTextEdit *resultArea= this->findChild<QTextEdit*>("OutputTextEdit");
	Q_ASSERT(resultArea);
	resultArea->setText(result);
	delete vList;
	delete lvList;
	delete gvList;
}

void MainWindow::on_kernelFixedGausButton_clicked()
{
	kernelType= FIXED_G;
}

void MainWindow::on_kernelAdaptativeGausButton_clicked()
{
	kernelType= ADAPTIVE_N;
}

void MainWindow::on_kernelFixedBiSquaButton_clicked()
{
	kernelType= FIXED_BSQ;
}

void MainWindow::on_kernelAdapatativeBiSquaButton_clicked()
{
	kernelType= ADAPTIVE_BSQ;
}

void MainWindow::on_kernelAICc_clicked()
{
	selectionCriteria= AICc;
}

void MainWindow::on_kernelAIC_clicked()
{
	selectionCriteria= AIC;
}

void MainWindow::on_kernelBIC_MDL_clicked()
{
	selectionCriteria= BIC_MDL;
}

void MainWindow::on_kernelCV_clicked()
{
	selectionCriteria= CV;
}

void MainWindow::on_OutputFileSelectButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
													"/home",
													tr("Text File (*.txt);; Any File(*)"));
	if(!fileName.endsWith(".txt"))
	{
		outputFileLineEdit->setText(fileName + ".txt");
	}
	//fazer mais coisas aq
}

void MainWindow::on_OutputKernelButton_clicked()
{
	tabWidget->setCurrentIndex(3);
}

void MainWindow::on_OutputExecuteButton_clicked()
{
	tabWidget->setCurrentIndex(5);
}

void MainWindow::on_KernelModelButton_clicked()
{
	tabWidget->setCurrentIndex(2);
}

void MainWindow::on_KernelOutputButton_clicked()
{
	tabWidget->setCurrentIndex(4);
}

void MainWindow::on_ExecuteAboutButton_clicked()
{
	tabWidget->setCurrentIndex(6);
}

/*
static std::string RemoverExtensao(std::string &str) const
{
	std::string ret= "";
	for(int cont=0; str[cont] != '.' && cont < str.size(); cont++)
	{
		ret+= str[cont];
	}
	return ret;
}
*/

void MainWindow::on_KernelGoldenRadioButton_clicked()
{
	goldenRadioButton->setChecked(true);
	singleRadioButton->setChecked(false);
	intervalSearchRadioButton->setChecked(false);
}

void MainWindow::on_KernelSingleRadioButton_clicked()
{
	singleRadioButton->setChecked(true);
	goldenRadioButton->setChecked(false);
	intervalSearchRadioButton->setChecked(false);
}

void MainWindow::on_KernelIntervalRadioButton_clicked()
{
	intervalSearchRadioButton->setChecked(true);
	goldenRadioButton->setChecked(false);
	singleRadioButton->setChecked(false);
}

void MainWindow::on_KernelGoldenManualCheckBox_clicked(bool checked)
{
	QLineEdit *goldenMinLineEdit = this->findChild<QLineEdit *>("KernelGoldenMinLineEdit");
	Q_ASSERT(goldenMinLineEdit);
	QLineEdit *goldenMaxLineEdit = this->findChild<QLineEdit *>("KernelGoldenMaxLineEdit");
	Q_ASSERT(goldenMaxLineEdit);
	if(checked)
	{
		goldenRadioButton->click();
		goldenMinLineEdit->setDisabled(false);
		goldenMaxLineEdit->setDisabled(false);
	}
	else
	{
		goldenMinLineEdit->setDisabled(true);
		goldenMaxLineEdit->setDisabled(true);
	}
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
	tabWidget->setCurrentIndex(6);
}

void MainWindow::on_distanceBetweenPointsOperation_clicked()
{
	modelType= MODE_DISTANCE_BETWEEN_POITS;
}
