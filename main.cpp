#include "mainwindow.h"
#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    Controller controller;

    // Connect HTTP requests to Controller
    QObject::connect(&window, &MainWindow::processRequest,
                    &controller, &Controller::processRequest);
    
    // Connect Controller to MainWindow UI
    QObject::connect(&controller, &Controller::updateUI,
                    &window, &MainWindow::updateUI);

    window.show();
    return app.exec();
}