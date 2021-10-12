#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , m_aboutWindow             (std::make_shared <AboutWindow>())
    , m_currentFile             ("")
    , m_cryptographicThread      (std::make_shared<CryptographicThread> ())
    , m_fileDir                 ("")
    , m_loadingWindow           (std::make_shared <LoadingWindow>())
    , m_model                   (std::make_shared<QStandardItemModel> (0,1,this))
    , m_modelFilePathColumnId   (0)
    , m_settings                (std::make_shared<Settings> ())
    , m_ui                      (std::make_shared<Ui::MainWindow> ())
    , m_widgetOffset            (5)
{
    m_ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    initializeModelTableView();
    initializeActions();
    setCryptographicThreadConnections();
}

MainWindow::~MainWindow()
{
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

void MainWindow::initializeModelTableView()
{
    m_model->setHorizontalHeaderItem(m_modelFilePathColumnId, new QStandardItem(QString("File path")));
    m_ui->tableView->setModel(m_model.get());
}

void MainWindow::initializeActions()
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

void MainWindow::setCryptographicThreadConnections()
{
    connect(m_cryptographicThread.get(), SIGNAL(processFinished()),this, SLOT(on_processFinished()));
}

//Menu: File->Open
void MainWindow::menu_open()
{
      QString filepath = QFileDialog::getOpenFileName(this, tr("List of Encripted File(s)"),"",tr("*.lef (*.lef)"));

      m_currentFile = filepath;
      std::vector<QString> lines = m_fileOperations.readFileLines(filepath);

      for (auto &item : lines)
      {
        if (m_fileOperations.isFileExist(item))
        {
            addDataToTableView(item);
        }
      }
}

//Menu: File->Save
void MainWindow::menu_save()
{
    if ("" == m_currentFile)
    {
        saveDialog();
    }
    else
    {
        QModelIndex mi;
        QVariant v;
        std::vector<QString> lines;
        for (int i=0; i<m_model->rowCount(); ++i)
        {
            mi = m_model->index(i,m_modelFilePathColumnId);
            v=mi.data();

            lines.push_back(v.toString());
        }

        m_fileOperations.writeFileLines(m_currentFile, lines);
    }
}

//Menu: File->Save As
void MainWindow::menu_saveAs()
{
    saveDialog();
}

//Menu: File->Exit
void MainWindow::menu_exit()
{
    this->close();
}

//Menu: Settings->Destination Path
void MainWindow::menu_setDestination()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                     tr("Set A Directory"),
                                                     m_settings->getDestinationPath(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path!="" && path.length()>0)
    {
        if (path[path.length()-1] != "/")
        {
            path+="/";
        }
        m_settings->setDestinationPath(path);
    }
}

//Menu: Help->About
void MainWindow::menu_about()
{
    m_aboutWindow->show();
}

//"Add a file" button
void MainWindow::on_addButton_clicked()
{
    QStringList pathList = QFileDialog::getOpenFileNames(this, tr("Add file(s)"),m_fileDir, tr("Files (*)"));
    if (0 == pathList.size())
    {
        return;
    }

    m_fileDir = m_path.getDirectoryNameByPath(pathList[0]);

    for (auto &item : pathList)
    {
        addDataToTableView(item);
    }
}

//"Clear" button
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


std::vector<File> MainWindow::getFiles ()
{
    QModelIndex mi;
    QVariant v;
    std::vector<File> files;
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_modelFilePathColumnId);
        v=mi.data();
        QString fileName=m_path.getFileNameByPath(v.toString());

        File file(fileName, m_path.getDirectoryNameByPath(fileName), v.toString());

        files.push_back(file);
    }

    return files;
}

bool MainWindow::prepareCryptography (QString& password)
{
    if (0==m_model->rowCount())
    {
        QMessageBox::information(0, "", "Please add some file(s)!");
        return false;
    }

    if (!passwordDialogHandle("Password: ", password))
    {
        return false;
    }

    return true;
}

void MainWindow::startCryptographicThread (std::shared_ptr<ICryptography> cryptography)
{
     m_cryptographicThread->setCryptography(cryptography);
     m_cryptographicThread->start();
     showLoadingWindow();
}

//encrypt button
void MainWindow::on_encryptButton_clicked()
{
    QString password = "";
    if (!prepareCryptography(password))
    {
        return;
    }

    startCryptographicThread(std::make_shared<CBCEncrypt> (getFiles(),
                                                           m_settings->getDestinationPath(),
                                                           password,
                                                           m_loadingWindow));
}

//decrypt button
void MainWindow::on_decryptButton_clicked()
{
    QString password = "";
    if (!prepareCryptography(password))
    {
        return;
    }

    startCryptographicThread(std::make_shared<CBCDecrypt> (getFiles(),
                                                           m_settings->getDestinationPath(),
                                                           password,
                                                           m_loadingWindow));
}

//add data to tableView widget
void MainWindow::addDataToTableView(const QString &fileName)
{
    //check if fileName exist
    QModelIndex mi;
    QVariant v;
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_modelFilePathColumnId);
        v=mi.data();
        if (v.toString() == fileName)
        {
            return;
        }
    }

    //Add fileName to tableView widget
    m_model->setRowCount(m_model->rowCount()+1);
    m_model->setData(m_model->index(m_model->rowCount()-1,m_modelFilePathColumnId),fileName);

    //set data to be not editable
    m_model->item(m_model->rowCount()-1,m_modelFilePathColumnId)->setEditable(false);
}



void MainWindow::showLoadingWindow()
{
    m_loadingWindow->move(((this->geometry().x() + this->width()/2) - m_loadingWindow->width()/2),
                ((this->geometry().y() + this->height()/2) - m_loadingWindow->height()/2));
    this->setEnabled(false);
    m_loadingWindow->show();
}

void MainWindow::on_processFinished()
{
    this->setEnabled(true);
    m_loadingWindow->close();
    m_cryptographicThread->terminate();

    QMessageBox::information(this,"","Operation has been finished.");
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

    QModelIndex mi;
    QVariant v;
    std::vector<QString> lines;
    for (int i=0; i<m_model->rowCount(); ++i)
    {
        mi = m_model->index(i,m_modelFilePathColumnId);
        v=mi.data();

        lines.push_back(v.toString());
    }

    m_fileOperations.writeFileLines(file_name + ".lef", lines);
}
