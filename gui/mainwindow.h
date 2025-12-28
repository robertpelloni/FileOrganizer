#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectDirectory();
    void startScan();

private:
    QLineEdit *pathInput;
    QPushButton *browseBtn;
    QPushButton *scanBtn;
    QTextEdit *logArea;
};
