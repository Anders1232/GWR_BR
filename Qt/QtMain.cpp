#include "mainwindow.hpp"
#include <QApplication>
#include <QLocale>

#include "GuiDriver.hpp"

int InitQtGui(int argc, char *argv[])
{
//	QLocale loc = QLocale::system(); // current locale
//	loc.setNumberOptions(QLocale::RejectGroupSeparator);
//	loc.setNumberOptions(QLocale::NumberOption::UnitedStates); // borrow number options from the "C" locale
//	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates)); // set as default
	QLocale::setDefault(QLocale::C);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}

