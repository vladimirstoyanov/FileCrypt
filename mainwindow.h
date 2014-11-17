#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QDialog>
#include <math.h>
#include "thread.h"

/*
#include <iostream>
#include <cryptopp/files.h>
#define CRYPTOPP_DEFAULT_NO_DLL
#include <cryptopp/dll.h>
#include <cryptopp/default.h>
#ifdef CRYPTOPP_WIN32_AVAILABLE
#include <windows.h>
#endif
*/

#include "waitwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QString settings_destination_path;
    QString settings_remove_original_files;
    QString add_file_last_dir;
    bool first_save;
    QString current_file;
    WaitWindow *wait_window;
    Thread *thread;
    QList<QString*> l_source_files;
    QList<QString*> l_destination_files;

    void initModelTableView();
    void initActions();
    void addDataInTableView(QString path);
    void loadSettings();
    void saveSettings();
    void saveDialog();
    void deleteEncryptionFileNameList();
    //void getFileNameFromPath(QString path, QString &filename);
    void encryptFile(const char *in,const char *out,const char *passPhrase);
    void decryptFile(const char *in,const char *out,const char *passPhrase);

    int getFileNameFromPath(QString path, QString &filename);
    int getDirNameFromPath(QString path, QString &dirname);

    void newThread(QString password, bool encryption);

public slots:
    void on_processFinished(bool encrypt);

private slots:
    void menu_open();
    void menu_save();
    void menu_saveAs();
    void menu_exit();
    void menu_setDestination();
    void menu_removeOriginalFilesAfterEncrypt(bool);
    void menu_about();
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();
    void on_removeAllButton_clicked();
};

#endif // MAINWINDOW_H
