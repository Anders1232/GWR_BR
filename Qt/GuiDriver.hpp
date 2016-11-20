#ifndef GUI_DRIVER_HPP
#define GUI_DRIVER_HPP

#include<string>
#include<list>
#include"QString"

#define MODE_LAT_PLUS_LON (1)
#define MODE_DEPENDENT_TIMES_OFFSET (2)
#define MODE_DISTANCE_TO_ORIGIN (3)

extern "C"
{
	//C side
	int InitQtGui(int argc, char**argv);
#include "NamedColumnDoubleTable.h"
}
//C++ side
class GuiDriver
{
	public:
//		void FileSelected(QString fileName);
//		void DelimiterSelected(char separator);
		GuiDriver(void);
		NamedColumnDoubleTable* LoadFileButtonCliked(std::string fileName, char separator);
		std::string GetPreview(std::string filename, int linesInPreview, int charPerLineInPreview);
//		void SelectModelType(int);
		QString Calculate(std::string fileName, char separator,
							int modelType,
							std::list<std::string> const &variables,
							std::string const &identifier,
							std::string const &dependent,
							std::string const &latitude,
							std::string const &longitude,
							std::string const &offset,
							std::list<std::string> const &localVariables,
							std::list<std::string> const &globalVariables
						);
	private:
		NamedColumnDoubleTable *table;
//		int modelType;
};

#endif // GUI_DRIVER_HPP
