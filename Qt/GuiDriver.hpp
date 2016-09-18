#ifndef GUI_DRIVER_HPP
#define GUI_DRIVER_HPP

#include<string>
#include"QString"
//C side
int InitQtGui(int argc, char**argv);

//C++ side
class GuiDriver
{
	public:
		void FileSelected(QString fileName);
		void SeparatorSelected(char separator);
		static GuiDriver& GetReference(void);
		GuiDriver(void);
		void LoadFileButtonCliked(void);
	private:
		std::string fileName;
		char separator;
};

#endif // GUI_DRIVER_HPP
