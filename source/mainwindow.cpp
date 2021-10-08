#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , m_aes                     (std::make_shared <QAESEncryption>(QAESEncryption::AES_256, QAESEncryption::CBC))
    , m_aboutWindow             (std::make_shared <AboutWindow>())
    , m_currentFile             ("")
    , m_destinationPath         ("")
    , m_fileDir                 ("")
    , m_loadingWindow           (std::make_shared <LoadingWindow>())
    , m_model                   (std::make_shared<QStandardItemModel> (0,1,this))
    , m_modelFilePathColumnId   (0)
    , m_thread                  (std::make_shared<Thread> (m_aes))
    , m_ui                      (std::make_shared<Ui::MainWindow> ())
    , m_widgetOffset            (5)
{
    m_ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    initModelTableView();
    initActions();
    initThread();
    loadSettings();
}

MainWindow::~MainWindow()
{
    m_thread->deleteLater();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    m_ui->tableView->setGeometry(m_widgetOffset,
                                 m_widgetOffset,
                                 this->width()-m_widgetOffset*2,
                                 this->height()-(m_widgetOffset*3 + m_ui->encryptButton->height() + m_ui->menuBar->height()));

    m_ui->addButton->setGeometry(m_ui->tableView->x(),
                                 m_ui->tableView->y() + m_ui->tableView->height() + m_widgetOffset,
                                 m_ui->addButton->width(),
                                 m_ui->addButton->height());

    m_ui->decryptButton->setGeometry(this->width()-(m_ui->decryptButton->width()+m_widgetOffset),
                                     m_ui->addButton->y(),
                                     m_ui->decryptButton->width(),
                                     m_ui->decryptButton->height());

    m_ui->encryptButton->setGeometry(m_ui->decryptButton->x() - (m_ui->encryptButton->width()+m_widgetOffset),
                                     m_ui->addButton->y(),
                                     m_ui->decryptButton->width(),
                                     m_ui->decryptButton->height());

    m_ui->removeButton->setGeometry(m_ui->addButton->x() + m_ui->addButton->width() + m_widgetOffset,
                                    m_ui->addButton->y(),
                                    m_ui->removeButton->width(),
                                    m_ui->removeButton->height());

    m_ui->removeAllButton->setGeometry(m_ui->removeButton->x() + m_ui->removeButton->width() + m_widgetOffset,
                                       m_ui->addButton->y(),
                                       m_ui->removeAllButton->width(),
                                       m_ui->removeAllButton->height());

    m_ui->tableView->setColumnWidth(0,m_ui->tableView->width());
}

void MainWindow::initModelTableView()
{
    m_model->setHorizontalHeaderItem(m_modelFilePathColumnId, new QStandardItem(QString("File path")));
    m_ui->tableView->setModel(m_model.get());
}

void MainWindow::initActions()
{
    m_ui->actionOpen->setShortcut(QKeySequence::New);
    m_ui->actionSave->setShortcut(QKeySequence::Save);
    m_ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    m_ui->actionExit->setShortcut(QKeySequence::Quit);

    connect(m_ui->actionOpen, SIGNAL(triggered()), this, SLOT(menu_open()));
    connect(m_ui->actionSave, SIGNAL(triggered()), this, SLOT(menu_save()));
    connect(m_ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(menu_saveAs()));
    connect(m_ui->actionExit, SIGNAL(triggered()), this, SLOT(menu_exit()));
    connect(m_ui->actionSet_Destination, SIGNAL(triggered()), this, SLOT(menu_setDestination()));
    connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(menu_about()));
}

void MainWindow::initThread()
{
    connect(m_thread.get(), SIGNAL(processFinished(const bool)),this, SLOT(on_processFinished(const bool)));
    connect(m_thread.get(), SIGNAL(setLable(QString)),m_loadingWindow.get(),SLOT(on_setLable(const QString&)));
    connect(m_aes.get(), SIGNAL(percentageUpdated(const int)),m_loadingWindow.get(),SLOT(on_percentageUpdated(const int)));
}

//ToDo: split the implementation of menu_open method
void MainWindow::menu_open()
{
      QString filepath = QFileDialog::getOpenFileName(this, tr("List of Encripted File(s)"),"",tr("*.lef (*.lef)"));

      QFile file(filepath);
      if(!file.open(QIODevice::ReadOnly))
      {
          return;
      }

      QTextStream in(&file);

      if (in.atEnd())
      {
          file.close();
          return;
      }
      m_currentFile = filepath;
      while(!in.atEnd())
      {
        QString line = in.readLine();
        QFile file1(line);
        if(!file1.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error!", line + " not exist.");
            continue;
        }
        file1.close();
        addDataInTableView(line);
      }
      file.close();
}

void MainWindow::menu_save()
{
    if ("" == m_currentFile)
    {
        saveDialog();
    }
    else
    {
        QFile file(m_currentFile);
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );

            QModelIndex mi;
            QVariant v;
            for (int i=0; i<m_model->rowCount(); ++i)
            {
                mi = m_model->index(i,m_modelFilePathColumnId);
                v=mi.data();

                stream<<v.toString() + "\n";
            }
        }
        file.close();
    }
}

void MainWindow::menu_saveAs()
{
    saveDialog();
}

void MainWindow::menu_exit()
{
    this->close();
}

void MainWindow::menu_setDestination()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Set A Directory"),
                                                             m_destinationPath,
                                                             QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);
    if (path!="")
    {
        m_destinationPath = path;
        saveSettings();
    }
}

void MainWindow::menu_about()
{
    m_aboutWindow->show();
}

void MainWindow::on_addButton_clicked()
{
    QStringList l_path = QFileDialog::getOpenFileNames(this, tr("Add file(s)"),m_fileDir, tr("Files (*)"));
    if (0 == l_path.size())
    {
        return;
    }

    QString afld_tmp = "";
    if (!getDirNameByPath(l_path[0], afld_tmp))
    {
        m_fileDir = afld_tmp;
    }

    for (int i=0; i<l_path.size(); ++i)
    {
        addDataInTableView(l_path[i]);
    }
}

void MainWindow::on_removeButton_clicked()
{
    //get selected rows
    QModelIndexList indexes = m_ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    //remove the tableView's last index
    while (!indexes.isEmpty())
    {
        m_model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}

bool MainWindow::passwordDialogHandle (const QString &dialogText, QString &password)
{
    bool okDialogButton = true;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    password =  inputDialog->getText(NULL ,dialogText,
                                              "Password:", QLineEdit::Password,
                                              "", &okDialogButton);
    if (!okDialogButton)
    {
        return false;
    }

    if (password.isEmpty())
    {
        QMessageBox::information(0,"","The password field cannot be empty!");
        return false;
    }

    return true;
}

//ToDo: split the below method to two methods
void MainWindow::encryptDecryptHandle (const QString& dialogMessage, const bool isDecrypted)
{
    QString password = "";

    if (!passwordDialogHandle(dialogMessage, password))
    {
        return;
    }

    if (0==m_model->rowCount())
    {
                QMessageBox::information(0, "", "Please add some file(s)!");
                return;
    }

    QModelIndex mi;
    QVariant v;

    deleteEncryptionFileNameList();

    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_modelFilePathColumnId);
        v=mi.data();
        QString fileName;
        getFileNameByPath(v.toString(),fileName);
        QString destPath = m_destinationPath + fileName;

        m_sourceFiles.push_back(std::make_shared<QString>(v.toString()));
        m_destinationFiles.push_back(std::make_shared<QString> (destPath));
    }

    createThread(password, isDecrypted);
}

//encrypt button
void MainWindow::on_encryptButton_clicked()
{
    encryptDecryptHandle("Encrypt", true);
}

void MainWindow::on_decryptButton_clicked()
{
    encryptDecryptHandle("Decrypt", false);
}

//add data to tableView widget
void MainWindow::addDataInTableView(const QString &fileName)
{
    //check is fileName exist
    QModelIndex mi;
    QVariant v;
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_modelFilePathColumnId);
        v=mi.data();
        if (v.toString() == fileName)
            return;
    }

    //Add fileName to tableView widget
    m_model->setRowCount(m_model->rowCount()+1);
    m_model->setData(m_model->index(m_model->rowCount()-1,m_modelFilePathColumnId),fileName);

    //set data to be not editable
    m_model->item(m_model->rowCount()-1,m_modelFilePathColumnId)->setEditable(false);
}

void MainWindow::loadSettings()
{
    QFile file("settings");
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream in(&file);

    if (in.atEnd())
    {
        file.close();
        return;
    }
    QString line = in.readLine();
    line.contains("dist_path:");
    QStringList sl = line.split(":");
    if (sl.size()<2)
    {
        file.close();
        return;
    }
    if (sl.at(0) != "dist_path")
    {
        file.close();
        return;
    }
    m_destinationPath = sl.at(1);

    m_destinationPath = m_destinationPath.replace("\n","");

    file.close();
}

void MainWindow::saveSettings()
{
    QFile file( "settings" );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream<<"dist_path:" + m_destinationPath + "\n";
    }
    file.close();
}

int MainWindow::getDirNameByPath(const QString &path, QString &dirname)
{
    QString separator;
    separator = "/";

    int index= -1;
    for (int i=path.length()-1; i>=0; --i)
    {

        if (path[i] == separator[0])
        {
            index = i;
            break;
        }
    }
    if (-1 == index)
    {
        return 1;
    }

    for (int i=0; i<=index; ++i)
    {
        dirname+=path[i];
    }

    return 0;
}

void MainWindow::createThread(const QString &password, const bool isDecrypted)
{
    m_thread->setSourceDestionationFiles(m_sourceFiles, m_destinationFiles);
    m_thread->setPassword(password);
    m_thread->setIsDecrypted(isDecrypted);
    m_thread->start();

    m_loadingWindow->move(((this->geometry().x() + this->width()/2) - m_loadingWindow->width()/2),
                ((this->geometry().y() + this->height()/2) - m_loadingWindow->height()/2));

    this->setEnabled(false);
    m_loadingWindow->show();
}

void MainWindow::on_processFinished(const bool isEncrypt)
{
    this->setEnabled(true);
    m_loadingWindow->close();
    m_thread->terminate();
    if (isEncrypt)
    {
        QMessageBox::information(this,"","Encryption has been completed successfully.");
    }
    else
    {
        QMessageBox::information(this,"","Decryption has been completed successfully.");
    }
}


//"clear all" button
void MainWindow::on_removeAllButton_clicked()
{
    m_model->removeRows(0,m_model->rowCount());
}

void MainWindow::saveDialog()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save list of file(s)"),"",tr("*.lef (*.lef)"));

    m_currentFile = file_name;
    QFile file(file_name + ".lef");
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        QModelIndex mi;
        QVariant v;
        for (int i=0; i<m_model->rowCount(); ++i)
        {
            mi = m_model->index(i,m_modelFilePathColumnId);
            v=mi.data();

            stream<<v.toString() + "\n";
        }
        deleteEncryptionFileNameList();
    }
    file.close();
}

void MainWindow::deleteEncryptionFileNameList()
{
    if (m_sourceFiles.size() >0)
    {
        m_sourceFiles.clear();
    }
    if (m_destinationFiles.size() >0)
    {
        m_destinationFiles.clear();
    }
}

int MainWindow::getFileNameByPath(const QString &path, QString &filename)
{
    QString separator, filename_tmp = "";
    separator = "/";

    if (separator.length()<1)
    {
        return 1;
    }

    for (int i=path.length()-1; i>=0; --i)
    {
        if (path[i] == separator[0])
        {
            break;
        }
        if (0==i) //if a separator hasn't been found then it will return 1. It's meant that an error has been occured.
        {
            return 1;
        }
        filename_tmp = path[i] + filename_tmp;
    }
    filename = filename_tmp;
    return 0;
}
