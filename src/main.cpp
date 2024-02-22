#include <iostream>

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow* window = new MainWindow();
    window->show();

    std::cout << "It's alive!" << std::endl;
    return app.exec();
}

