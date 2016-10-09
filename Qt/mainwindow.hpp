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

	private:
		Ui::MainWindow *ui;
		GuiDriver driver;
};

#endif // MAINWINDOW_H
