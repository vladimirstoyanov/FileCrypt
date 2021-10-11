#include "loadingwindow.h"
#include "ui_loadingwindow.h"

LoadingWindow::LoadingWindow(QWidget *parent) :
    QWidget(parent)
    , m_loadingGif (std::make_shared<QLabel>(this))
    , m_filenameLabel (std::make_shared<QLabel>(this))
    , m_filenameLabelHeightRatio (0.07)
    , m_movie (std::make_shared<QMovie>("../resources/loading.gif"))
    , m_percentageLabel (std::make_shared<QLabel>(this))
    , m_percentageLabelWidthRatio(0.25)
    , m_ui(std::make_shared<Ui::LoadingWindow> ())
    , m_widgetOffset (10)

{
    initializeWidgets();
}

LoadingWindow::~LoadingWindow()
{
}

void LoadingWindow::setCurrentFile (const QString & currentFile)
{
    m_filenameLabel->setText(currentFile);
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

void LoadingWindow::on_percentageUpdated(const int percentage)
{
    m_percentageLabel->setText(QString::number(percentage) + "%");
}


void LoadingWindow::initializeWidgets ()
{
    m_ui->setupUi(this);

    setWindowFlags( Qt::CustomizeWindowHint );
    setWindowFlags(windowFlags() | Qt::Tool);

    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    m_filenameLabel->setGeometry(m_widgetOffset,
                                 this->height()*m_filenameLabelHeightRatio,
                                 this->width()-m_widgetOffset*2,
                                 m_filenameLabel->height());


    m_percentageLabel->setGeometry(m_filenameLabel->x(),
                                   m_filenameLabel->y() + m_filenameLabel->height() + m_widgetOffset,
                                   this->width()*m_percentageLabelWidthRatio,
                                   m_percentageLabel->height());

    m_filenameLabel->setStyleSheet("QLabel { background-color : white; color : black; }");
    m_percentageLabel->setStyleSheet("QLabel { background-color : white; color : black; }");

    //initialize the gif animation
    m_loadingGif->setMovie(m_movie.get());
    m_loadingGif->setAttribute( Qt::WA_TranslucentBackground, true );

    this->setStyleSheet("background-color: white;");

    m_movie->start();
    m_loadingGif->show();
}
