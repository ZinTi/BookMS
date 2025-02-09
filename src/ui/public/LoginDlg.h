#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginDlg;
}
QT_END_NAMESPACE

class LoginDlg : public QDialog {
    Q_OBJECT

public:
    // 构造函数，接收父窗口指针
    LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

private slots:
    void on_loginPushButton_clicked(); // 处理登录按钮点击事件的槽函数
    void on_signupPushButton_clicked(); // 处理注册按钮点击事件的槽函数

private:
    Ui::LoginDlg *ui;
    void displayLogo(int ax, int ay, int maxHeight); // 显示logo函数
};
#endif // LOGINDIALOG_H
