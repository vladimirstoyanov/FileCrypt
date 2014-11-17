#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initModelTableView();
    initActions();

    //this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    settings_destination_path = "";
    add_file_last_dir = "";
    settings_remove_original_files = "false";
    loadSettings();
    current_file = "";

    //init wait_window
    wait_window = new WaitWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete wait_window;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    qDebug()<<ui->menuBar->y();
    qDebug()<<ui->menuBar->height();
    ui->tableView->setGeometry(5,5,this->width()-10, this->height()-(15 + ui->encryptButton->height() + ui->menuBar->height()));
    ui->addButton->setGeometry(5,ui->tableView->y() + ui->tableView->height() + 5, ui->addButton->width(), ui->addButton->height());
    ui->decryptButton->setGeometry(this->width()-(ui->decryptButton->width()+5), ui->addButton->y(), ui->decryptButton->width(), ui->decryptButton->height());
    ui->encryptButton->setGeometry(ui->decryptButton->x() - (ui->encryptButton->width()+5), ui->addButton->y(), ui->decryptButton->width(), ui->decryptButton->height());
    ui->removeButton->setGeometry(ui->addButton->x() + ui->addButton->width() + 5, ui->addButton->y(), ui->removeButton->width(), ui->removeButton->height());
    //if (ui->tableView->)
    ui->removeAllButton->setGeometry(ui->removeButton->x() + ui->removeButton->width() + 5, ui->addButton->y(), ui->removeAllButton->width(), ui->removeAllButton->height());
    ui->tableView->setColumnWidth(0,ui->tableView->width());
}

void MainWindow::initModelTableView()
{
    model = new QStandardItemModel(0,1,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("File path")));
    ui->tableView->setModel(model);
}

void MainWindow::initActions()
{
    ui->actionOpen->setShortcut(QKeySequence::New);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    ui->actionExit->setShortcut(QKeySequence::Quit);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(menu_open()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(menu_save()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(menu_saveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(menu_exit()));
    connect(ui->actionSet_Destination, SIGNAL(triggered()), this, SLOT(menu_setDestination()));
    connect(ui->actionRemove_original_files_after_encrypt, SIGNAL(toggled(bool)), this, SLOT(menu_removeOriginalFilesAfterEncrypt(bool)));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(menu_about()));
}

void MainWindow::menu_open()
{
      QString filepath = QFileDialog::getOpenFileName(this, tr("List of Encripted File(s)"),"",tr("*.lef (*.lef)"));

      QFile file(filepath);
      if(!file.open(QIODevice::ReadOnly))
          return;

      QTextStream in(&file);

      if (in.atEnd())
      {
          file.close();
          return;
      }
      current_file = filepath;
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
    if (current_file == "")
        saveDialog();
    else
    {
        QFile file(current_file);
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );

            QModelIndex mi;
            QVariant v;
            for (int i=0; i<model->rowCount(); i++)
            {
                mi = model->index(i,0);
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
                                                             settings_destination_path,
                                                               QFileDialog::DontResolveSymlinks | QFileDialog::ReadOnly);
    if (path!="")
    {
        settings_destination_path = path;
        saveSettings();
    }
}

void MainWindow::menu_removeOriginalFilesAfterEncrypt(bool checked)
{
    if (checked)
        settings_remove_original_files = "true";
    else
        settings_remove_original_files = "false";

    saveSettings();

}

void MainWindow::menu_about()
{
    QMessageBox::critical(NULL,"","about");
}

void MainWindow::on_addButton_clicked()
{
    QStringList l_path = QFileDialog::getOpenFileNames(this, tr("Add file(s)"),add_file_last_dir,tr("Files (*.*)"));
    if (l_path.size() == 0)
        return;

    QString afld_tmp = "";
    if (!getDirNameFromPath(l_path[0], afld_tmp))
        add_file_last_dir = afld_tmp;

    for (int i=0; i<l_path.size(); i++)
        addDataInTableView(l_path[i]);
}

void MainWindow::on_removeButton_clicked()
{
    //get selected rows
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();

    qSort(indexes.begin(), indexes.end());

    //remove last index in list
    while (!indexes.isEmpty())
    {
        model->removeRows(indexes.last().row(), 1);
        indexes.removeLast();
    }
}


//encrypt button
void MainWindow::on_encryptButton_clicked()
{
    bool ok=1;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    QString text =  inputDialog->getText(NULL ,"Encrypt",
                                              "Password:", QLineEdit::Password,
                                              "", &ok);
    if (!ok)
        return;

    if (text.isEmpty())
    {
        QMessageBox::information(0,"","The password field cannot be empty!");
        on_encryptButton_clicked();
        return;
    }

    if (model->rowCount()==0)
    {
                QMessageBox::information(0,"", "Please add some file(s) to encrypt!");
                return;
    }

    QModelIndex mi;
    QVariant v;

    deleteEncryptionFileNameList();

    //check for exist path
    for (int i=0; i<model->rowCount(); i++)
    {
        mi = model->index(i,0);
        v=mi.data();
        QString file_name;
        getFileNameFromPath(v.toString(),file_name);
        QString dest_path = settings_destination_path + "/" + file_name;

        if (dest_path == v.toString())
            dest_path += "_en";


        l_source_files.push_back(new QString(v.toString()));
        l_destination_files.push_back(new QString (dest_path));
        //encryptFile(v.toString().toLatin1().data(),dest_path.toLatin1().data(),text.toLatin1().data());
    }
    newThread(text,true);
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
        return;
    if (text.isEmpty())
    {
        QMessageBox::information(0,"","The password field cannot be empty!");
        on_decryptButton_clicked();
        return;
    }
    if (model->rowCount()==0)
    {
                QMessageBox::information(0,"", "Please add some file(s) to dectypt!");
                return;
    }

    QModelIndex mi;
    QVariant v;

    deleteEncryptionFileNameList();

    //check for exist path
    for (int i=0; i<model->rowCount(); i++)
    {
        mi = model->index(i,0);
        v=mi.data();
        QString file_name;
        getFileNameFromPath(v.toString(),file_name);
        QString dest_path = settings_destination_path + "/" + file_name;
        if (dest_path == v.toString())
            dest_path += "_de";

        l_source_files.push_back(new QString(v.toString()));
        l_destination_files.push_back(new QString(dest_path));
        //decryptFile(v.toString().toLatin1().data(),dest_path.toLatin1().data(),text.toLatin1().data());
    }
    newThread(text, false);
}

//add data to tableView widget
void MainWindow::addDataInTableView(QString file_name)
{

    //check for existing file_name in tableView
    QModelIndex mi;
    QVariant v;
    for (int i=0; i<model->rowCount(); i++)
    {
        mi = model->index(i,0);
        v=mi.data();
        if (v.toString() == file_name)
            return;
    }

    //Add file_name to tableView
    model->setRowCount(model->rowCount()+1);
    model->setData(model->index(model->rowCount()-1,0),file_name);

    //set data not editable
    model->item(model->rowCount()-1,0)->setEditable(false);

}

void MainWindow::loadSettings()
{
    QFile file("settings");
    if(!file.open(QIODevice::ReadOnly))
        return;

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
    settings_destination_path = sl.at(1);
    settings_destination_path = settings_destination_path.replace("\n","");

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
        settings_remove_original_files = sl.at(1);
        //settings_remove_original_files = settings_remove_original_files.replace("\n","");
    }

    if (settings_remove_original_files == "true")
        ui->actionRemove_original_files_after_encrypt->setChecked(1);
    else
        ui->actionRemove_original_files_after_encrypt->setChecked(0);

    file.close();
}

void MainWindow::saveSettings()
{
    QFile file( "settings" );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream<<"dist_path:" + settings_destination_path + "\n";
        if (settings_remove_original_files == "true")
            stream<<"remove_original_files:true\n";
        else
            stream<<"remove_original_files:false\n";
    }
    file.close();
}

void MainWindow::encryptFile(const char *in,const char *out,const char *passPhrase)
{

    CryptoPP::FileSource f(in, true, new CryptoPP::DefaultEncryptorWithMAC(passPhrase,
   new CryptoPP::FileSink(out)));
}

void MainWindow::decryptFile(const char *in,const char *out,const char *passPhrase)
{

    CryptoPP::FileSource f(in, true,
   new CryptoPP::DefaultDecryptorWithMAC(passPhrase, new CryptoPP::FileSink(out)));
}

int MainWindow::getDirNameFromPath(QString path, QString &dirname)
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

void MainWindow::newThread(QString password, bool encryption)
{
    thread = new Thread();
    connect(thread, SIGNAL(processFinished(bool)),this, SLOT(on_processFinished(bool)));
    connect(thread, SIGNAL(setLabel(QString)),wait_window,SLOT(on_setLable(QString)));
    thread->setSourceDestionationFiles(l_source_files, l_destination_files);
    thread->setPassword(password);
    thread->isEncrypt(encryption);
    thread->start();
    //wait_window->show();

    wait_window->move(((this->geometry().x() + this->width()/2) - wait_window->width()/2),
                ((this->geometry().y() + this->height()/2) - wait_window->height()/2));
    //wait_window->setParent(this);
    this->setEnabled(false);
    wait_window->show();
}

void MainWindow::on_processFinished(bool encrypt)
{
    this->setEnabled(true);
    wait_window->close();
    thread->deleteLater();
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

                for (int i=0; i<l_destination_files.size(); i++)
                    stream<<*l_destination_files[i] + "\n";
            }
            file.close();
        }
    }
    else
        QMessageBox::information(this,"","Decryption has been completed successfully.");
}


//'remove all' button
void MainWindow::on_removeAllButton_clicked()
{
    model->removeRows(0,model->rowCount());
}

void MainWindow::saveDialog()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save list of file(s)"),"",tr("*.lef (*.lef)"));

    current_file = file_name;
    QFile file(file_name + ".lef");
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        QModelIndex mi;
        QVariant v;
        for (int i=0; i<model->rowCount(); i++)
        {
            mi = model->index(i,0);
            v=mi.data();

            stream<<v.toString() + "\n";
        }
        deleteEncryptionFileNameList();
    }
    file.close();
}

void MainWindow::deleteEncryptionFileNameList()
{
    if (l_source_files.size() >0)
    {
        qDeleteAll(l_source_files);
        l_source_files.clear();
    }
    if (l_destination_files.size() >0)
    {
        qDeleteAll(l_destination_files);
        l_destination_files.clear();
    }
}

int MainWindow::getFileNameFromPath(QString path, QString &filename)
{
    QString separator, filename_tmp ="";
    separator = "/";

    if (separator.length()<1)
        return 1;

    for (int i=path.length()-1; i>=0; i--)
    {

        if (path[i] == separator[0])
            break;
        if (i == 0) //if not found separator, return 1, mean error
            return 1;
        filename_tmp = path[i] + filename_tmp;
    }
    filename = filename_tmp;
    return 0;
}
