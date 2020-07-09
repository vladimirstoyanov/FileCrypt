#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
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

private:
    void closeEvent(QCloseEvent *); // Overriding the window's close event
    void showEvent(QShowEvent *);
    void setupGui();
};

#endif // ABOUTWINDOW_H
