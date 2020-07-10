#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <memory>

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QPen>
#include <QWidget>

namespace Ui
{
    class LoadingWindow;
}

class LoadingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingWindow(QWidget *parent = 0);
    virtual ~LoadingWindow();
public slots:
    void on_setLable(const QString &label);
    void on_percentageUpdated (const int percentage);
private:
    std::shared_ptr<QLabel>             m_loadingGif;
    std::shared_ptr<QLabel>             m_filenameLabel;
    std::shared_ptr<QMovie>             m_movie;
    std::shared_ptr<QLabel>             m_percentageLabel;
    std::shared_ptr<Ui::LoadingWindow>  m_ui;

    void paintEvent( QPaintEvent* e );
};

#endif // LOADINGWINDOW_H
