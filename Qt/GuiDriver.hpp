#ifndef GUI_DRIVER_HPP
#define GUI_DRIVER_HPP

#include<string>
#include"QString"
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
		void FileSelected(QString fileName);
		void DelimiterSelected(char separator);
		static GuiDriver& GetReference(void);
		GuiDriver(void);
		void LoadFileButtonCliked(void);
	private:
		std::string fileName;
		char separator;
		NamedColumnDoubleTable *table;
		void UpdatePreview();
};

#endif // GUI_DRIVER_HPP
