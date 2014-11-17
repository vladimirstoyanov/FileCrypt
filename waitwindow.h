#ifndef WAITWINDOW_H
#define WAITWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QMovie>
#include <QLabel>
#include <QPainter>
#include <QPen>

namespace Ui {
class WaitWindow;
}

class WaitWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WaitWindow(QWidget *parent = 0);
    ~WaitWindow();
public slots:
    void on_setLable(QString label);
private:
    Ui::WaitWindow *ui;
    QMovie *movie;
    QLabel *l_waiting_gif;
    QLabel *label;

    void paintEvent( QPaintEvent* e );
};

#endif // WAITWINDOW_H
