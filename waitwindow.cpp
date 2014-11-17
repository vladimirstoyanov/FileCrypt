#include "waitwindow.h"
#include "ui_waitwindow.h"

WaitWindow::WaitWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitWindow)
{
    ui->setupUi(this);

    setWindowFlags( Qt::CustomizeWindowHint );
    setWindowFlags(windowFlags() | Qt::Tool);

    label = new QLabel(this);
    label->setGeometry(5,35,this->width()-10, label->height());
    //init waiting gif animation
    movie = new QMovie("please_wait.gif");
    l_waiting_gif = new QLabel(this);
    l_waiting_gif->setMovie(movie);

    this->setStyleSheet("background-color: white;");

    movie->start();
    l_waiting_gif->show();
}

void WaitWindow::paintEvent( QPaintEvent* e )
{
    QPainter painter(this);
    QPen linepen(Qt::black);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(2);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(linepen);

    painter.drawRect(0,0,this->width(), this->height());
}
WaitWindow::~WaitWindow()
{
    delete ui;
    delete l_waiting_gif;
    delete movie;
    delete label;
}

void WaitWindow::on_setLable(QString label_)
{
    label->setText(label_);
}
