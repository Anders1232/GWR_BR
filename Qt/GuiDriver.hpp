#ifndef GUI_DRIVER_HPP
#define GUI_DRIVER_HPP

#include<string>
#include<list>
#include"QString"
#include "QTextEdit"

#define MODE_LAT_PLUS_LON (1)
#define MODE_DEPENDENT_TIMES_OFFSET (2)
#define MODE_DISTANCE_TO_ORIGIN (3)
#define MODE_DISTANCE_BETWEEN_POITS (4)

enum bandwidthSelectionMethod
{
	GOLDEN,
	SINGLE,
	INTERVAL
};
typedef enum bandwidthSelectionMethod BandwidthSelectionMethod;

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
		QString Calculate(std::string fileName,
							char separator,
							std::string outputFileName,
							std::string outputDistanceBetweenPointsFileName,
							int modelType,
							KernelType kernelType,
							SelectionCriteria selectionCriteria,
							std::list<std::string> const &variables,
							std::string const &identifier,
							std::string const &dependent,
							std::string const &latitude,
							std::string const &longitude,
							std::string const &offset,
							std::list<std::string> const &localVariables,
							std::list<std::string> const &globalVariables,
							BandwidthSelectionMethod bandSelectionMethod,
							QTextEdit &textArea,
							bool distanceInKm);
	private:
		NamedColumnDoubleTable *table;
		QString CalculateDistanceBetweenPoints(
				std::string fileName,
				char separator,
				std::string outputFileName,
				const std::string &latitude,
				const std::string &longitude
		);
		void CalculateGolden(
				QTextEdit &textArea,
				FILE* outputFile,
				FILE *outputDistancesBetweenPoints,
				KernelType kernelType,
				DoubleMatrix *data,
				bool distanceInKm,
				int independentVariable,
				int *dependentLocalVariables,
				int *dependentGlobalVariables,
				int latitude,
				int longitude
			);

//		int modelType;
};

#endif // GUI_DRIVER_HPP
