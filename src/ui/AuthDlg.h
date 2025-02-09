#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "UserService.h"

class AuthDlg : public QDialog {
public:
    AuthDlg(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("身份验证");
        setFixedSize(200, 120);

        // 垂直布局管理器
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 密码标签和输入框
        QHBoxLayout *passwordLayout = new QHBoxLayout();
        QLabel *passwordLabel = new QLabel("密码:", this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordLayout->addWidget(passwordLabel);
        passwordLayout->addWidget(passwordEdit);
        mainLayout->addLayout(passwordLayout);

        // 按钮布局
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton("确定", this);
        QPushButton *cancelButton = new QPushButton("取消", this);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);

        // 连接按钮的点击信号到槽函数
        connect(okButton, &QPushButton::clicked, this, &AuthDlg::onOkButtonClicked);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    }

private slots:
    void onOkButtonClicked() {
        std::string msg;
        std::string password = passwordEdit->text().toStdString();
        if (UserService::validateCurrentUser(msg, password)) {
            accept();
        } else {
            QMessageBox::warning(this, "验证失败", "密码错误");
        }
    }

private:
    QLineEdit *passwordEdit;
};

#endif // AUTHDIALOG_H
