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
    , m_removeOriginalFiles     ("false")
    , m_thread                  (std::make_shared<Thread> (m_aes))
    , m_ui                      (std::make_shared<Ui::MainWindow> ())
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
    m_ui->tableView->setGeometry(5,5,this->width()-10, this->height()-(15 + m_ui->encryptButton->height() + m_ui->menuBar->height()));
    m_ui->addButton->setGeometry(5,m_ui->tableView->y() + m_ui->tableView->height() + 5, m_ui->addButton->width(), m_ui->addButton->height());
    m_ui->decryptButton->setGeometry(this->width()-(m_ui->decryptButton->width()+5), m_ui->addButton->y(), m_ui->decryptButton->width(), m_ui->decryptButton->height());
    m_ui->encryptButton->setGeometry(m_ui->decryptButton->x() - (m_ui->encryptButton->width()+5), m_ui->addButton->y(), m_ui->decryptButton->width(), m_ui->decryptButton->height());
    m_ui->removeButton->setGeometry(m_ui->addButton->x() + m_ui->addButton->width() + 5, m_ui->addButton->y(), m_ui->removeButton->width(), m_ui->removeButton->height());
    m_ui->removeAllButton->setGeometry(m_ui->removeButton->x() + m_ui->removeButton->width() + 5, m_ui->addButton->y(), m_ui->removeAllButton->width(), m_ui->removeAllButton->height());
    m_ui->tableView->setColumnWidth(0,m_ui->tableView->width());
}

void MainWindow::initModelTableView()
{
    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("File path")));
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
    connect(m_ui->actionRemove_original_files_after_encrypt, SIGNAL(toggled(bool)), this, SLOT(menu_removeOriginalFilesAfterEncrypt(const bool)));
    connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(menu_about()));
}

void MainWindow::initThread()
{
    connect(m_thread.get(), SIGNAL(processFinished(const bool)),this, SLOT(on_processFinished(const bool)));
    connect(m_thread.get(), SIGNAL(setLabel(QString)),m_loadingWindow.get(),SLOT(on_setLable(const QString&)));
    connect(m_aes.get(), SIGNAL(percentageUpdated(const int)),m_loadingWindow.get(),SLOT(on_percentageUpdated(const int)));
}

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
    if (m_currentFile == "")
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
            for (int i=0; i<m_model->rowCount(); i++)
            {
                mi = m_model->index(i,0);
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
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose Or Create Directory"),
                                                             m_destinationPath,
                                                               QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);
    if (path!="")
    {
        m_destinationPath = path;
        saveSettings();
    }
}

void MainWindow::menu_removeOriginalFilesAfterEncrypt(const bool checked)
{
    if (checked)
    {
        m_removeOriginalFiles = "true";
    }
    else
    {
        m_removeOriginalFiles = "false";
    }

    saveSettings();

}

void MainWindow::menu_about()
{
    m_aboutWindow->show();
}

void MainWindow::on_addButton_clicked()
{
    QStringList l_path = QFileDialog::getOpenFileNames(this, tr("Add file(s)"),m_fileDir, tr("Files (*)"));
    if (l_path.size() == 0)
        return;

    QString afld_tmp = "";
    if (!getDirNameByPath(l_path[0], afld_tmp))
        m_fileDir = afld_tmp;

    for (int i=0; i<l_path.size(); i++)
        addDataInTableView(l_path[i]);
}

void MainWindow::on_removeButton_clicked()
{
    //get selected rows
    QModelIndexList indexes = m_ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    //remove last index in list
    while (!indexes.isEmpty())
    {
        m_model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}


//encrypt button
void MainWindow::on_encryptButton_clicked()
{
    bool okDialogButton=true;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    QString textDialog =  inputDialog->getText(NULL ,"Encrypt",
                                              "Password:", QLineEdit::Password,
                                              "", &okDialogButton);
    if (false == okDialogButton)
    {
        return;
    }

    if (textDialog.isEmpty())
    {
        QMessageBox::information(0,"","The password field cannot be empty!");
        on_encryptButton_clicked();
        return;
    }

    if (m_model->rowCount()==0)
    {
                QMessageBox::information(0,"", "Please add some file(s) to encrypt!");
                return;
    }

    QModelIndex mi;
    QVariant v;

    deleteEncryptionFileNameList();

    //check for exist path
    for (int i=0; i<m_model->rowCount(); i++)
    {
        mi = m_model->index(i,0);
        v=mi.data();
        QString fileName;
        getFileNameByPath(v.toString(),fileName);
        QString dest_path = m_destinationPath + fileName;

        if (dest_path == v.toString())
            dest_path += "_en";


        m_sourceFiles.push_back(std::make_shared<QString>(v.toString()));
        m_destinationFiles.push_back(std::make_shared<QString> (dest_path));
    }
    createThread(textDialog,true);
}

void MainWindow::on_decryptButton_clicked()
{
    bool ok;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    QString text =  inputDialog->getText(NULL ,"Decrypt",
                                              "Password:", QLineEdit::Password,
                                              "", &ok);
    if (!ok)
    {
        return;
    }

    if (text.isEmpty())
    {
        QMessageBox::information(0,"","The password field cannot be empty!");
        on_decryptButton_clicked();
        return;
    }

    if (m_model->rowCount()==0)
    {
        QMessageBox::information(0,"", "Please add some file(s) to dectypt!");
        return;
    }

    QModelIndex mi;
    QVariant v;

    deleteEncryptionFileNameList();

    //check for exist path
    for (int i=0; i<m_model->rowCount(); i++)
    {
        mi = m_model->index(i,0);
        v=mi.data();
        QString file_name;
        getFileNameByPath(v.toString(),file_name);
        QString dest_path = m_destinationPath + file_name;
        if (dest_path == v.toString())
            dest_path += "_de";

        m_sourceFiles.push_back(std::make_shared<QString>(v.toString()));
        m_destinationFiles.push_back(std::make_shared<QString>(dest_path));
        //decryptFile(v.toString().toLatin1().data(),dest_path.toLatin1().data(),text.toLatin1().data());
    }
    createThread(text, false);
}

//add data to tableView widget
void MainWindow::addDataInTableView(const QString &fileName)
{
    //check for existing file_name in tableView
    QModelIndex mi;
    QVariant v;
    for (int i=0; i<m_model->rowCount(); i++)
    {
        mi = m_model->index(i,0);
        v=mi.data();
        if (v.toString() == fileName)
            return;
    }

    //Add file_name to tableView
    m_model->setRowCount(m_model->rowCount()+1);
    m_model->setData(m_model->index(m_model->rowCount()-1,0),fileName);

    //set data not editable
    m_model->item(m_model->rowCount()-1,0)->setEditable(false);
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

    if (in.atEnd())
    {
        file.close();
        return;
    }

    line = in.readLine();
    sl = line.split(":");

    if (sl.size()<2)
    {
        file.close();
        return;
    }

    if (sl.at(0) != "remove_original_files")
    {
        file.close();
        return;
    }

    if (sl.at(1)=="false" || sl.at(1) == "true")
    {
        m_removeOriginalFiles = sl.at(1);
        //m_removeOriginalFiles = m_removeOriginalFiles.replace("\n","");
    }

    if (m_removeOriginalFiles == "true")
    {
        m_ui->actionRemove_original_files_after_encrypt->setChecked(1);
    }
    else
    {
        m_ui->actionRemove_original_files_after_encrypt->setChecked(0);
    }

    file.close();
}

void MainWindow::saveSettings()
{
    QFile file( "settings" );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream<<"dist_path:" + m_destinationPath + "\n";
        if (m_removeOriginalFiles == "true")
        {
            stream<<"remove_original_files:true\n";
        }
        else
        {
            stream<<"remove_original_files:false\n";
        }
    }
    file.close();
}

int MainWindow::getDirNameByPath(const QString &path, QString &dirname)
{
    QString separator;
    separator = "/";

    int index=-1;
    for (int i=path.length()-1; i>=0; i--)
    {

        if (path[i] == separator[0])
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return 1;

    for (int i=0; i<=index; i++)
        dirname+=path[i];

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

void MainWindow::on_processFinished(const bool encrypt)
{
    this->setEnabled(true);
    m_loadingWindow->close();
    m_thread->terminate();
    if (encrypt)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "", "Encryption has been completed. Do you want to save a list of encrypted file(s) path(s)?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QString file_name = QFileDialog::getSaveFileName(this, tr("Save list of file(s)"),"",tr("*.lef (*.lef)"));

            QFile file(file_name + ".lef");
            if ( file.open(QIODevice::ReadWrite) )
            {
                QTextStream stream(&file);

                for (int i=0; i<m_destinationFiles.size(); i++)
                    stream<<*m_destinationFiles[i] + "\n";
            }
            file.close();
        }
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
        for (int i=0; i<m_model->rowCount(); i++)
        {
            mi = m_model->index(i,0);
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
    QString separator, filename_tmp ="";
    separator = "/";

    if (separator.length()<1)
    {
        return 1;
    }

    for (int i=path.length()-1; i>=0; i--)
    {
        if (path[i] == separator[0])
        {
            break;
        }
        if (i == 0) //if a separator hasn't been found then it will return 1. It's meant that an error has been occured.
        {
            return 1;
        }
        filename_tmp = path[i] + filename_tmp;
    }
    filename = filename_tmp;
    return 0;
}
