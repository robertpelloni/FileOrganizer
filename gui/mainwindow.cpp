#include "mainwindow.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("FileOrganizer");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Top bar: Path input and Browse button
    QHBoxLayout *topLayout = new QHBoxLayout();
    pathInput = new QLineEdit(this);
    pathInput->setPlaceholderText("Select directory to scan...");
    browseBtn = new QPushButton("Browse", this);
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::selectDirectory);

    topLayout->addWidget(pathInput);
    topLayout->addWidget(browseBtn);
    mainLayout->addLayout(topLayout);

    // Scan button
    scanBtn = new QPushButton("Start Scan", this);
    connect(scanBtn, &QPushButton::clicked, this, &MainWindow::startScan);
    mainLayout->addWidget(scanBtn);

    // Log area
    logArea = new QTextEdit(this);
    logArea->setReadOnly(true);
    mainLayout->addWidget(logArea);
}

MainWindow::~MainWindow() {
}

void MainWindow::selectDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory",
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        pathInput->setText(dir);
    }
}

void MainWindow::startScan() {
    QString dir = pathInput->text();
    if (dir.isEmpty()) {
        logArea->append("Please select a directory first.");
        return;
    }
    logArea->append("Scanning: " + dir);
    // TODO: Integrate with fo_core Engine
}
