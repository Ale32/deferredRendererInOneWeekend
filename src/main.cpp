#include "appWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AppWindow window;
	window.show();

	return app.exec();
}
