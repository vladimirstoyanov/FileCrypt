#include "loadingwindow.h"
#include "ui_loadingwindow.h"

LoadingWindow::LoadingWindow(QWidget *parent) :
    QWidget(parent)
    , m_loadingGif (std::make_shared<QLabel>(this))
    , m_label (std::make_shared<QLabel>(this))
    , m_movie (std::make_shared<QMovie>("please_wait.gif"))
    , m_ui(std::make_shared<Ui::LoadingWindow> ())
{
    m_ui->setupUi(this);

    setWindowFlags( Qt::CustomizeWindowHint );
    setWindowFlags(windowFlags() | Qt::Tool);

    m_label->setGeometry(5,35,this->width()-10, m_label->height());

    //init waiting gif animation
    m_loadingGif->setMovie(m_movie.get());

    this->setStyleSheet("background-color: white;");

    m_movie->start();
    m_loadingGif->show();
}

LoadingWindow::~LoadingWindow()
{
}

void LoadingWindow::paintEvent( QPaintEvent* e )
{
    QPainter painter(this);
    QPen linepen(Qt::black);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(2);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(linepen);

    painter.drawRect(0,0,this->width(), this->height());
}

void LoadingWindow::on_setLable(const QString &label)
{
    m_label->setText(label);
}
