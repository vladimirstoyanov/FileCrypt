#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <math.h>
#include <memory>

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStringList>

#include "thread.h"
#include "loadingwindow.h"

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

    void resizeEvent(QResizeEvent *event);

private:
    QString                                 m_currentFile;
    QList<std::shared_ptr<QString>>         m_destinationFiles;
    QString                                 m_destinationPath;
    QString                                 m_fileDir;
    std::shared_ptr<LoadingWindow>          m_loadingWindow;
    std::shared_ptr<QStandardItemModel>     m_model;
    QString                                 m_removeOriginalFiles;
    QList<std::shared_ptr<QString>>         m_sourceFiles;
    std::shared_ptr<Thread>                 m_thread;
    std::shared_ptr<Ui::MainWindow>         m_ui;


private:
    void addDataInTableView(const QString &path);
    void decryptFile(const char *in,const char *out, const char *passPhrase);
    void deleteEncryptionFileNameList();
    int  getDirNameFromPath  (const QString &path, QString &dirname);
    int  getFileNameFromPath (const QString &path, QString &filename);
    void encryptFile(const char *in,const char *out, const char *passPhrase);
    void initModelTableView();
    void initActions();
    void loadSettings();
    void newThread(const QString &password, const bool isDecrypted);
    void saveDialog();
    void saveSettings();

public slots:
    void on_processFinished(const bool encrypt);

private slots:
    void menu_about();
    void menu_exit();
    void menu_open();
    void menu_removeOriginalFilesAfterEncrypt(const bool);
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
