#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include <QMessageBox>
#include "UserService.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "SignupDlg.h"

extern UserInfo* currentUserInfo;

LoginDlg::LoginDlg(QWidget *parent) : QDialog(parent) , ui(new Ui::LoginDlg) {
    ui->setupUi(this);

    displayLogo(128, 100, 80);

    ui->instructionsTextEdit->setHtml(
        "<h3 style='text-align:center'>说明</h3>"
        "<p>1、输入你的账号密码登录，账号即你的用户ID；</p>"
        "<p>2、若<font color='red'>忘记密码</font>，请持有效证件到前台重置；</p>"
        "<p>3、使用自助注册功能创建的新用户默认为冻结状态，需管理员授权后方可使用，请持有效证件和用户ID到前台办理激活手续。</p>");

    // Qt Designer 自动连接的机制，不需要写connect了，按约定命名槽函数会自动查询
    // connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginDlg::on_loginPushButton_clicked); // 连接登录按钮的点击信号和自定义的槽函数
    // connect(ui->signupPushButton, &QPushButton::clicked, this, &LoginDlg::on_signupPushButton_clicked); // 连接注册按钮的点击信号和自定义的槽函数
}

LoginDlg::~LoginDlg() {
    delete ui;
}

void LoginDlg::displayLogo(int ax, int ay, int maxHeight) {
    QLabel *logoLabel = new QLabel(this);  // 创建 QLabel 用于显示 logo
    QPixmap pixmap(":/icon/logo.png");
    if (!pixmap.isNull()) {
        int width = pixmap.width() * maxHeight / pixmap.height();
        if (width > 100) {
            width = 100;
        }
        logoLabel->setFixedSize(width, maxHeight);
        // 使用平滑转换模式
        logoLabel->setPixmap(pixmap.scaled(logoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->move(ax, ay);
    } else {
        qDebug() << "Failed to load logo from resource.";
    }
}

void LoginDlg::on_loginPushButton_clicked(){
    // 获取用户名和输入框的内容
    QString userId = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    std::string msg;

    if( UserService::login(msg, userId.toStdString(), password.toStdString(), *currentUserInfo)){
        accept(); // 登录成功，关闭登录对话框
    }else{
        QMessageBox::warning(this, "登录失败", QString::fromStdString(msg)); // 登录失败，显示错误消息
    }
}

void LoginDlg::on_signupPushButton_clicked(){
    SignupDlg SignupDlg(nullptr);
    SignupDlg.exec();
}
