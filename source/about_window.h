#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QShowEvent>
#include <QWidget>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;

    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
    void setupGui();
};

#endif // ABOUT_WINDOW_H
