#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("FileOrganizer");
    resize(800, 600);
    
    label = new QLabel("Hello from Qt + fo_core!", this);
    setCentralWidget(label);
}

MainWindow::~MainWindow() {
}
