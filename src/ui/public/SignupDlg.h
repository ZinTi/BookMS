#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignupDlg;
}

class SignupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SignupDlg(QWidget *parent = nullptr);
    ~SignupDlg();

private slots:
    void on_registerPushButton_clicked(); // 处理registerPushButton点击事件的槽函数

private:
    Ui::SignupDlg *ui;

};

#endif // SIGNUPDIALOG_H
