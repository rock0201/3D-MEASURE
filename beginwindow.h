#ifndef BEGINWINDOW_H
#define BEGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
namespace Ui {
class beginWindow;
}
using namespace std;
class beginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit beginWindow(QWidget *parent = 0);
    ~beginWindow();
signals:
    void goMainwindow(string cName,string iName);
private slots:
    void on_pushButton_clicked();

    void on_cail_button_clicked();

    void on_img_button_clicked();
public slots:
    void backWindow();

private:
    Ui::beginWindow *ui;
    QString calibrateName;
    std::string caliname;
    QString fileName;
    std::string fname;
};

#endif // BEGINWINDOW_H
