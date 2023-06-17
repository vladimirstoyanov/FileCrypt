#include "about_window.h"
#include "ui_about_window.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    setupGui();
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::closeEvent(QCloseEvent *)
{
    this->hide();
}

void AboutWindow::showEvent(QShowEvent *)
{

}

void AboutWindow::setupGui ()
{
    ui->setupUi(this);

    this->move(this->screen()->availableGeometry().center() - this->rect().center());

    ui->textBrowser->setGeometry(0,0,this->width(), this->height());

    ui->textBrowser->setHtml("FileCrypt"
                             "<p>Version: 0.2 </p>"
                             "<p>Author: Vladimir Stoyanov </p>"
                             "<p>e-mail: <a href=\"mailto:vlado_stoyanov@yahoo.com\">vlado_stoyanov@yahoo.com</p>");

    ui->textBrowser->setOpenLinks(1);
    ui->textBrowser->setOpenExternalLinks(1);
}
