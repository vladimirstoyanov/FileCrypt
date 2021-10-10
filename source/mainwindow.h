#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>
#include <memory>

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDesktopWidget>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStringList>

#include "aes.h"
#include "aboutwindow.h"
#include "file.h"
#include "fileoperations.h"
#include "loadingwindow.h"
#include "path.h"
#include "thread.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_processFinished(const bool isEncrypt);

private:
    std::shared_ptr<QAESEncryption>         m_aes;
    std::shared_ptr<AboutWindow>            m_aboutWindow;
    QString                                 m_currentFile;
    std::vector<QString>                    m_destinationFiles;
    QString                                 m_destinationPath;
    QString                                 m_fileDir;
    FileOperations                          m_fileOperations;
    std::shared_ptr<LoadingWindow>          m_loadingWindow;
    std::shared_ptr<QStandardItemModel>     m_model;
    int                                     m_modelFilePathColumnId;
    std::vector<File>                       m_sourceFiles;
    Path                                    m_path;
    std::shared_ptr<Thread>                 m_thread;
    std::shared_ptr<Ui::MainWindow>         m_ui;
    int                                     m_widgetOffset;

    void addDataToTableView(const QString &path);
    void createThread(const QString &password, const bool isDecrypted);
    void encryptDecryptHandle (const QString& dialogMessage,const bool isDecrypted);
    void deleteEncryptionFileNameList();

    void initializeModelTableView();
    void initializeActions();
    void initializeThread ();
    void loadSettings();
    bool passwordDialogHandle (const QString &dialogText, QString &password);
    void resizeEvent(QResizeEvent *event);
    void saveDialog();
    void saveSettings();

private slots:
    void menu_about();
    void menu_exit();
    void menu_open();
    void menu_save();
    void menu_saveAs();
    void menu_setDestination();

    void on_addButton_clicked();
    void on_decryptButton_clicked();
    void on_encryptButton_clicked();
    void on_removeButton_clicked();
    void on_removeAllButton_clicked();
};

#endif // MAINWINDOW_H
