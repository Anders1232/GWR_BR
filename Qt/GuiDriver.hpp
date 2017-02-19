#ifndef GUI_DRIVER_HPP
#define GUI_DRIVER_HPP

#include<string>
#include<list>
#include"QString"

#define MODE_LAT_PLUS_LON (1)
#define MODE_DEPENDENT_TIMES_OFFSET (2)
#define MODE_DISTANCE_TO_ORIGIN (3)
#define MODE_DISTANCE_BETWEEN_POITS (4)

extern "C"
{
	//C side
	int InitQtGui(int argc, char**argv);
#include "NamedColumnDoubleTable.h"
#include "gwr-lib.h"
//#include
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
		QString Calculate(
							std::string fileName,
							char separator,
							std::string outputFileName,
							std::string outputDistanceBetweenPointsFileName,
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
		QString CalculateDistanceBetweenPoints(
				std::string fileName,
				char separator,
				std::string outputFileName,
				const std::string &latitude,
				const std::string &longitude
		);

//		int modelType;
};

#endif // GUI_DRIVER_HPP
