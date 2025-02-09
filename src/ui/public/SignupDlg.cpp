#include "SignupDlg.h"
#include "ui_SignupDlg.h"
#include "entity.h"
#include <QRegularExpression>
#include <QString>
#include <QMessageBox>
#include "UserService.h"
#include <string>

extern UserInfo* currentUserInfo;

SignupDlg::SignupDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupDlg)
{
    ui->setupUi(this);
}

SignupDlg::~SignupDlg(){
    delete ui;
}


void SignupDlg::on_registerPushButton_clicked(){
    // 实际开发中用户输入的数据还要防注入攻击，这里简化就不实现了
    UserInfo userInfo;
    // 1、获取用户基本信息（姓名、性别）
    QString yourName = ui->nameLineEdit->text();
    if(yourName.length() >= 2 && yourName.length() <= 10){ // 其实UI最长可输入长度也有限制，这里最长限制可以删除
        userInfo.username = yourName.toStdString();
    }else{
        QMessageBox::warning(ui->nameLineEdit, "不合法输入", "请将姓名限制在2-10个字", QMessageBox::Ok);
        return;
    }
    if(ui->maleRadioButton->isChecked()){ // UI界面就限制了性别只能输入男和女
        userInfo.gender = true;
    }else{
        userInfo.gender = false;
    }
    // 2、获取用户联系方式等信息（邮箱、手机号、通讯住址），初步合法性校验（邮箱格式、手机号格式、通讯地址长度）
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"); // 邮箱格式
    QString yourEmail = ui->emailLineEdit->text();
    if(emailRegex.match(yourEmail).hasMatch()){
        userInfo.email = yourEmail.toStdString();
    }else{
        QMessageBox::warning(ui->emailLineEdit, "不合法输入", "请输入合法的电子邮件地址", QMessageBox::Ok);
        return;
    }
    QRegularExpression phoneRegex("^1\\d{10}$"); // 手机号格式：以1开头、11位长度的纯数字字符串
    QString yourPhone = ui->telephoneLineEdit->text();
    if (phoneRegex.match(yourPhone).hasMatch()) {
        userInfo.telephone = yourPhone.toStdString();
    } else {
        QMessageBox::warning(ui->telephoneLineEdit, "不合法输入", "请输入合法的手机号（以 1 开头的 11 位纯数字）", QMessageBox::Ok);
        return;
    }
    QString yourAddress = ui->addressPlainTextEdit->toPlainText(); // 除省份名称外的地址
    if(yourAddress.length() < 30){
        userInfo.address = (ui->provinceComboBox->currentText() + yourAddress).toStdString();
    }else{
        QMessageBox::warning(ui->addressPlainTextEdit, "不合法输入", "通讯地址长度最长不得超过30字（不含省份名称）", QMessageBox::Ok);
        return;
    }

    // 3、获取用户账号等信息（用户ID、密码、确定密码），简单合法性校验（用户ID格式、两次输入密码是否一致以及长度和复杂程度）
    QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 用户ID：3-6位长度，只允许数字、小写字母、下划线_
    QString yourId = ui->userIdLineEdit->text();
    if(userIdRegex.match(yourId).hasMatch()){
        userInfo.id = yourId.toStdString();
    }else{
        QMessageBox::warning(ui->userIdLineEdit, "不合法输入", "用户ID只允许数字、小写字母和下划线_，3到6位长度", QMessageBox::Ok);
        return;
    }

    QString yourPasswd1 = ui->passwordLineEdit->text();
    QString yourPasswd2 = ui->confirmPasswordLineEdit->text(); // 这里可以改用->textEdited()实时比较两个密码是否相同并提醒用户（当然不是写在这个函数中）
    if(yourPasswd1 == yourPasswd2){
        if(yourPasswd1.length() >= 6){
            userInfo.password = yourPasswd1.toStdString();
        }else{
            QMessageBox::warning(ui->passwordLineEdit, "不合法输入", "你输入的密码不够复杂（至少6位）", QMessageBox::Ok);
            return;
        }
    }else{
        QMessageBox::warning(ui->confirmPasswordLineEdit, "输入错误", "你两次输入的密码不一致", QMessageBox::Ok);
        return;
    }

    // 4、其他不需要用户输入的数据
    // userId其实亦可以不由用户输入而是自动生成，这里简化了，让用户自定义，后端只检测是否已存在
    // userInfo.userType = "DS"; // 为了验证用户真实身份，使用自助服务注册的账号默认处于冻结状态，后续需要管理员激活

    // 5、使用MainService服务执行注册操作并使用弹窗显示注册结果（用户ID是否存在）
    std::string msg; // 注册反馈信息
    bool result = UserService::registerUser(msg, userInfo, "DS");
    if(result){
        QMessageBox::information(nullptr, "注册账号成功", QString::fromStdString(msg));
    }else{
        QMessageBox::critical(nullptr, "注册账号失败", QString::fromStdString(msg));
    }
}
