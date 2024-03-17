//
// Created by anass on 3/16/24.
//

#include <iostream>
#include <QApplication>
#include "mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("DPIR");
    window.show();
    return app.exec();
}