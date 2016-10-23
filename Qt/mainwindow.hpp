#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GuiDriver.hpp"

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

		void on_pushButton_clicked();

//		void on_previewLines_editingFinished();

//		void on_previewCharsPerLine_editingFinished();

		void on_previewLines_valueChanged(int arg1);

		void on_previewCharsPerLine_valueChanged(int arg1);

	private:
		Ui::MainWindow *ui;
		GuiDriver driver;
		void MoveBetweenQListWidgets(QString const &source, QString const &target, bool onlyOneElementOnTarget);
		int linesInPreview, charsPerLineInPreview;
		void UpdatePreview(void);
		void ClearLists(void);
};

#endif // MAINWINDOW_H
