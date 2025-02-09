#include "UserAddForm.h"
#include "ui_UserAddForm.h"
#include "User.h"
#include <QMessageBox>
#include "UserService.h"

UserAddForm::UserAddForm(QWidget *parent): QWidget(parent), ui(new Ui::UserAddForm){
    ui->setupUi(this);
}

UserAddForm::~UserAddForm(){
    delete ui;
}

void UserAddForm::on_pBtnTabAddUserSubmitRegistration_clicked(){
    UserInfo userInfo;
    // 1、获取用户基本信息（姓名、性别）
    QString inName = ui->lineEditTabAddUserUsername->text();
    if(inName.length() >= 2 && inName.length() <= 10){ // 其实UI最长可输入长度也有限制，这里最长限制可以删除
        userInfo.username = inName.toStdString();
    }else{
        QMessageBox::warning(ui->lineEditTabAddUserUsername,  QString("不合法输入"),  QString("请将姓名限制在2-10个字"));
        return;
    }
    if(ui->radioButtonTabAddUserMale->isChecked()){ // UI界面就限制了性别只能输入男和女
        userInfo.gender = true;
    }else{
        userInfo.gender = false;
    }
    // 2、获取用户联系方式等信息（邮箱、手机号、通讯住址），初步合法性校验（邮箱格式、手机号格式、通讯地址长度）
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"); // 邮箱格式
    QString inEmail = ui->lineEditTabAddUserEmail->text();
    if(emailRegex.match(inEmail).hasMatch()){
        userInfo.email = inEmail.toStdString();
    }else{
        QMessageBox::warning(ui->lineEditTabAddUserEmail,  QString("不合法输入"),  QString("请输入合法格式的电子邮箱地址"));
        return;
    }
    QRegularExpression phoneRegex("^1\\d{10}$"); // 手机号格式：以1开头、11位长度的纯数字字符串
    QString inPhone = ui->lineEditTabAddUserTelephone->text();
    if (phoneRegex.match(inPhone).hasMatch()) {
        userInfo.telephone = inPhone.toStdString();
    } else {
        QMessageBox::warning(ui->lineEditTabAddUserTelephone,  QString("不合法输入"),  QString("请输入合法的手机号（以 1 开头的 11 位纯数字）"));
        return;
    }
    QString inAddress = ui->plainTextEditTabAddUserAddress->toPlainText(); // 除省份名称外的地址
    if(inAddress.length() < 30){
        userInfo.address = (ui->comboBoxTabAddUserProvince->currentText() + inAddress).toStdString();
    }else{
        QMessageBox::warning(ui->plainTextEditTabAddUserAddress,  QString("不合法输入"),  QString("通讯地址长度最长不得超过30字（不含省份名称）"));
        return;
    }

    // 3、获取用户账号等信息（用户ID、密码、确定密码、用户类型），简单合法性校验（用户ID格式、两次输入密码是否一致以及长度和复杂程度）
    QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 用户ID：3-6位长度，只允许数字、小写字母、下划线_
    QString inId = ui->lineEditTabAddUserUserId->text();
    if(userIdRegex.match(inId).hasMatch()){
        userInfo.id = inId.toStdString();
    }else{
        QMessageBox::warning(ui->lineEditTabAddUserUserId,  QString("不合法输入"),  QString("用户ID只允许数字、小写字母和下划线_，3到6位长度"));
        return;
    }

    QString inPasswd1 = ui->lineEditTabAddUserPasswd->text();
    QString inPasswd2 = ui->lineEditTabAddUserPasswdComfirm->text(); // 这里可以改用->textEdited()实时比较两个密码是否相同并提醒用户（当然不是写在这个函数中）
    if(inPasswd1 == inPasswd2){
        if(inPasswd1.length() >= 6){
            userInfo.password = inPasswd1.toStdString();
        }else{
            QMessageBox::warning(ui->lineEditTabAddUserPasswd,  QString("不合法输入"),  QString("你输入的密码不够复杂（至少6位）"));
            return;
        }
    }else{
        QMessageBox::warning(ui->lineEditTabAddUserPasswdComfirm,  QString("输入错误"),  QString("你两次输入的密码不一致"));
        return;
    }

    QString inUserType = ui->comboBoxTabAddUserUserType->currentText();
    if(inUserType == "OB-普通读者"){
        userInfo.userType = "OB";
    }else if(inUserType == "FB-教职员读者"){
        userInfo.userType = "FB";
    }else if(inUserType == "LA-图书管理员"){
        userInfo.userType = "LA";
    }else if(inUserType == "UA-用户管理员"){
        userInfo.userType = "UA";
    }else if(inUserType == "SA-系统管理员"){
        userInfo.userType = "SA";
    }else if(inUserType == "DS-冻结状态"){
        userInfo.userType = "DS";
    }else{
        QMessageBox::critical(ui->comboBoxTabAddUserUserType, QString("不合法用户类型"),  QString("因程序代码错误或系统字符集编码不一致导致用户输入不合法"));
        return;
    }

    // 5、使用MainService服务执行注册操作并使用弹窗显示注册结果（用户ID是否存在）
    std::string msg; // 注册反馈信息
    bool result = UserService::registerUser(msg, userInfo, userInfo.userType);
    if(result){
        QMessageBox::information(nullptr, QString("创建用户成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::critical(nullptr, QString("创建用户失败"), QString::fromStdString(msg));
    }
}