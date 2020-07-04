#pragma once

#include <QtWidgets/QMainWindow>

namespace Ui {
	class AppWindow;
}

class AppWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit AppWindow(QWidget* parent = 0);

	~AppWindow();

private:
	Ui::AppWindow* ui;
};
