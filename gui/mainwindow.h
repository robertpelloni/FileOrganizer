#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTableWidget>
#include <QTabWidget>
#include <QProgressBar>
#include <QComboBox>
#include <QCheckBox>
#include <QThread>
#include <memory>
#include "fo/core/engine.hpp"

class ScanWorker;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void scanRequested(const QString& path, const QStringList& extensions);

private slots:
    void selectDirectory();
    void startScan();
    void findDuplicates();
    void exportResults();
    void onScanProgress(int current, int total);
    void onScanComplete(int fileCount);
    void onFileFound(const QString& path, qint64 size);
    void onDuplicatesFound(int groupCount, qint64 wastedSpace);

private:
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void log(const QString& message);

    // UI Elements
    QLineEdit *pathInput;
    QPushButton *browseBtn;
    QPushButton *scanBtn;
    QPushButton *dupBtn;
    QPushButton *exportBtn;
    QTextEdit *logArea;
    QTableWidget *fileTable;
    QTableWidget *dupTable;
    QTabWidget *tabWidget;
    QProgressBar *progressBar;
    QComboBox *scannerCombo;
    QComboBox *hasherCombo;
    QCheckBox *followSymlinks;
    QLabel *statusLabel;

    // Engine
    std::unique_ptr<fo::core::Engine> engine;
    std::vector<fo::core::FileInfo> scannedFiles;
    std::vector<fo::core::DuplicateGroup> duplicateGroups;
};
