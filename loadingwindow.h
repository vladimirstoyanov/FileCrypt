#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <memory>

#include <QWidget>
#include <QDebug>
#include <QMovie>
#include <QLabel>
#include <QPainter>
#include <QPen>

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
private:
    std::shared_ptr<QLabel>             m_loadingGif;
    std::shared_ptr<QLabel>             m_label;
    std::shared_ptr<QMovie>             m_movie;
    std::shared_ptr<Ui::LoadingWindow>  m_ui;

    void paintEvent( QPaintEvent* e );
};

#endif // LOADINGWINDOW_H
