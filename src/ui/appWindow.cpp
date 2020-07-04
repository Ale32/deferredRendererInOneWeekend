#include "appWindow.h"

#include "ui_appWindow.h"

AppWindow::AppWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::AppWindow)
{
	ui->setupUi(this);
}

AppWindow::~AppWindow()
{
	delete ui;
}
