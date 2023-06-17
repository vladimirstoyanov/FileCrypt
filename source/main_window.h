#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <math.h>
#include <memory>

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QScreen>
#include <QStandardItemModel>
#include <QStringList>

#include "about_window.h"
#include "cbc_encrypt.h"
#include "cbc_decrypt.h"
#include "cryptography.h"
#include "cryptographic_thread.h"
#include "file.h"
#include "file_operations.h"
#include "loading_window.h"
#include "path.h"
#include "settings.h"


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
    void on_processFinished();

private:
    std::shared_ptr<AboutWindow>            m_aboutWindow;
    QString                                 m_currentFile;
    std::shared_ptr<CryptographicThread>    m_cryptographicThread;
    QString                                 m_fileDir;
    FileOperations                          m_fileOperations;
    std::shared_ptr<LoadingWindow>          m_loadingWindow;
    std::shared_ptr<QStandardItemModel>     m_model;
    int                                     m_modelFilePathColumnId;
    Path                                    m_path;
    std::shared_ptr<Settings>               m_settings;
    std::shared_ptr<Ui::MainWindow>         m_ui;
    int                                     m_widgetOffset;

    void addDataToTableView(const QString &path);
    std::vector<File> getFiles ();
    void initializeModelTableView();
    void initializeActions();
    bool prepareCryptography (QString& password);
    bool passwordDialogHandle (const QString &dialogText, QString &password);
    void resizeEvent(QResizeEvent *event);
    void saveDialog();
    void saveFileNamesList ();
    void setCryptographicThreadConnections ();
    void showLoadingWindow();
    void startCryptographicThread (std::shared_ptr<ICryptography> cryptography);

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

#endif // MAIN_WINDOW_H
