#include "UserPermissionForm.h"
#include "ui_UserPermissionForm.h"
#include <QMessageBox>
#include "AuthDlg.h"

UserPermissionForm::UserPermissionForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserPermissionForm)
{
    ui->setupUi(this);
}

UserPermissionForm::~UserPermissionForm()
{
    delete ui;
}

void UserPermissionForm::on_pBtnAllocateAllocatePermissionsUserType_clicked(){ // 处理用户管理-权限分配-修改用户类型按钮pBtnAllocateAllocatePermissionsUserType的点击事件的槽函数
    // 二次认证
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; }
    std::string userId;
    std::string newUserType;
    // 1、获取lineEditTabAllocatePermissionsUserId输入并校验格式
    QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 用户ID：3-6位长度，只允许数字、小写字母、下划线_
    QString inUserId = ui->lineEditTabAllocatePermissionsUserId->text();
    if(userIdRegex.match(inUserId).hasMatch()){
        userId = inUserId.toStdString();
    }else{
        QMessageBox::warning(ui->lineEditTabAllocatePermissionsUserId, QString("不合法输入"), QString("用户ID只允许数字、小写字母和下划线_，3到6位长度"));
        return;
    }
    // 2、获取类型选择并校验格式
    QString inUserType = ui->comboBoxTabAllocatePermissionsUserType->currentText();
    if(inUserType == "OB-普通读者"){
        newUserType = "OB";
    }else if(inUserType == "FB-教职员读者"){
        newUserType = "FB";
    }else if(inUserType == "LA-图书管理员"){
        newUserType = "LA";
    }else if(inUserType == "UA-用户管理员"){
        newUserType = "UA";
    }else if(inUserType == "SA-系统管理员"){
        newUserType = "SA";
    }else if(inUserType == "DS-冻结状态"){
        newUserType = "DS";
    }else{
        QMessageBox::critical(ui->comboBoxTabAllocatePermissionsUserType, QString("不合法用户类型"),  QString("因程序代码错误或系统字符集编码不一致导致用户输入不合法"));
        return;
    }
    // 3、执行变更操作并弹窗显示修改结果
    std::string msg;
    bool result = UserService::setUserType(msg, userId, newUserType);
    if(result){
        QMessageBox::information(nullptr, QString::fromStdString("用户" + userId + "已变更为" + newUserType + "类型"),  QString::fromStdString(msg));
    }else{
        QMessageBox::warning(nullptr, QString::fromStdString("用户" + userId + "变更" + newUserType + "类型失败"),  QString::fromStdString(msg));
    }
}
void UserPermissionForm::on_pBtnAllocateAllocatePermissionsResetPwd_clicked(){ // 处理用户管理-权限分配-充值用户密码按钮pBtnAllocateAllocatePermissionsResetPwd的点击事件的槽函数
    // 二次认证
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; }
    std::string userId, newPasswd;

    // 1、获取lineEditTabAllocatePermissionsUserId输入并校验格式
    QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 用户ID：3-6位长度，只允许数字、小写字母、下划线_
    QString inUserId = ui->lineEditTabAllocatePermissionsUserId->text();
    if(userIdRegex.match(inUserId).hasMatch()){
        userId = inUserId.toStdString();
    }else{
        QMessageBox::warning(ui->lineEditTabAllocatePermissionsUserId, QString("不合法输入"), QString("用户ID只允许数字、小写字母和下划线_，3到6位长度"));
        return;
    }
    // 2、获取lineEditTabAllocatePermissionsResetPwd1和lineEditTabAllocatePermissionsResetPwd2输入（格式、是否相同）
    QString inPasswd1 = ui->lineEditTabAllocatePermissionsResetPwd1->text();
    QString inPasswd2 = ui->lineEditTabAllocatePermissionsResetPwd2->text(); // 这里可以改用->textEdited()实时比较两个密码是否相同并提醒用户（当然不是写在这个函数中）
    if(inPasswd1 == inPasswd2){
        if(inPasswd1.length() >= 6 && inPasswd1.length() <= 12){
            newPasswd = inPasswd1.toStdString();
        }else{
            QMessageBox::warning(ui->lineEditTabAllocatePermissionsResetPwd1, QString("不合法输入"), QString("密码要求长度为6-12位"));
            return;
        }
    }else{
        QMessageBox::warning(ui->lineEditTabAllocatePermissionsResetPwd2, QString("输入错误"), QString("你两次输入的密码不一致"));
        return;
    }
    // 3、执行修改并弹窗显示修改结果
    std::string msg;
    bool result = UserService::resetPassword(msg, userId, newPasswd);
    if(result){
        QMessageBox::information(nullptr, QString::fromStdString("已重置用户" + userId + "的密码"),  QString::fromStdString(msg));
    }else{
        QMessageBox::warning(nullptr, QString::fromStdString("重置用户" + userId + "的密码失败"),  QString::fromStdString(msg));
    }
}
