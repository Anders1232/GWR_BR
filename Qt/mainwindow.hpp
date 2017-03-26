#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QRadioButton>
#include <QCheckBox>
#include "GuiDriver.hpp"

enum CoordsMode
{
	PROJECTED,
	SPHERICAL
};
/*
enum KernelType
{
	FIXED_G,
	FIXED_BS,

}
*/

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_actionManual_triggered();

		void on_SelectFileButton_clicked();

		void on_LoadButton_clicked();

		void on_TabRadioButton_clicked();

		void on_SemicolonRadioButton_clicked();

		void on_DelimiterLineEdit_textEdited(const QString &arg1);

		void on_dependentVariableInButton_clicked();

		void on_dependentVariableOutButton_clicked();

		void on_modelReadyButton_clicked();

		void on_offsetVariableInButton_clicked();

		void on_offsetVariableOutButton_clicked();

		void on_identifierInButton_clicked();

		void on_identifierOutButton_clicked();

		void on_longitudeInButton_clicked();

		void on_longitudeOutButton_clicked();

		void on_latitudeInButton_clicked();

		void on_latitudeOutButton_clicked();

		void on_localVariablesOut_clicked();

		void on_localVariablesIn_clicked();

		void on_globalVariablesOut_clicked();

		void on_globalVariablesIn_clicked();

		void on_ModelChooseFileButton_clicked();

//		void on_previewLines_editingFinished();

//		void on_previewCharsPerLine_editingFinished();

		void on_previewLines_valueChanged(int arg1);

		void on_previewCharsPerLine_valueChanged(int arg1);

		void on_LatPlusLonOperation_clicked();

		void on_dependentTimesOffsetOperation_clicked();

		void on_welcomeNextButton_clicked();

		void on_distanceToOrigin_clicked();

		void on_modelProjected_clicked();

		void on_modelSpherical_clicked();

		void on_executeStartComputingButton_clicked();

		void on_kernelFixedGausButton_clicked();

		void on_kernelAdaptativeGausButton_clicked();

		void on_kernelFixedBiSquaButton_clicked();

		void on_kernelAdapatativeBiSquaButton_clicked();

		void on_kernelAICc_clicked();

		void on_kernelAIC_clicked();

		void on_kernelBIC_MDL_clicked();

		void on_kernelCV_clicked();

		void on_OutputFileSelectButton_clicked();

		void on_OutputKernelButton_clicked();

		void on_OutputExecuteButton_clicked();

		void on_KernelModelButton_clicked();

		void on_KernelOutputButton_clicked();

		void on_ExecuteAboutButton_clicked();

		void on_KernelGoldenRadioButton_clicked();

		void on_KernelSingleRadioButton_clicked();

		void on_KernelIntervalRadioButton_clicked();

		void on_KernelGoldenManualCheckBox_clicked(bool checked);

		void on_actionQuit_triggered();

		void on_actionAbout_triggered();

		void on_distanceBetweenPointsOperation_clicked();

		void on_OutputBetweenPointsFileCheckbox_clicked(bool checked);

		void on_OutputBetweenPointsSelectButton_clicked();

	private:
		Ui::MainWindow *ui;
		GuiDriver driver;
		void MoveBetweenQListWidgets(QString const &source, QString const &target, bool onlyOneElementOnTarget);
		int linesInPreview, charsPerLineInPreview;
		void UpdatePreview(void);
		void ClearLists(void);
		std::list<std::string>* GetStringList(QListWidget *widgetList);
		int modelType;
		void ShowErrorMessage(QString errorMessage);
		CoordsMode coordsMode;
		QString fileName;
		char separator;
		KernelType kernelType;
		SelectionCriteria selectionCriteria;
		BandwidthSelectionMethod bandwidthSelectionMethod;
		//commonly used variables, so it is not necessary needed to findChild
		QTabWidget *tabWidget;
		QLineEdit *outputFileLineEdit;
		QCheckBox *outputDistancesCheckBox;
		QLineEdit *outputDistancesFileLineEdit;
		QWidget *outputDistancesWidget;
		//Implementinf myself the radioButton mechanics for radioButton on different QWidgets
		QRadioButton *goldenRadioButton;
		QRadioButton *singleRadioButton;
		QRadioButton *intervalSearchRadioButton;

};

#endif // MAINWINDOW_H
