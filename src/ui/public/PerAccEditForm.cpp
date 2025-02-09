#include "PerAccEditForm.h"
#include "ui_PerAccEditForm.h"
#include "UserService.h"
#include "AuthDlg.h"
#include "User.h"
#include "entity.h"

extern UserInfo* currentUserInfo;

PerAccEditForm::PerAccEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerAccEditForm)
{
    ui->setupUi(this);

    loadPersonalInfo(); // 加载下个人信息管理
}

PerAccEditForm::~PerAccEditForm()
{
    delete ui;
}


void PerAccEditForm::on_refreshToolButton_clicked(){ // 处理refreshToolButton（刷新个人信息）点击事件的槽函数
    std::string msg;
    bool result = UserService::getUserInfoByUserId(msg, currentUserInfo->id, *currentUserInfo);
    loadPersonalInfo();
    if(result){
        QMessageBox::information(nullptr, QString("刷新成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::critical(nullptr, QString("严重错误"), QString::fromStdString(msg));
    }
}
void PerAccEditForm::on_updateToolButton_clicked(){ // 处理updateToolButton（更新个人信息）点击事件的槽函数
    // 验证密码的弹窗，敏感操作都要二次身份验证
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; }

    UserInfo userInfo;
    // 1、用户基本信息（id\name\gender\userType）
    userInfo.id = currentUserInfo->id; // 1.1、id不变
    QString inName = ui->nameLineEdit->text();
    if(inName.length() >= 2 && inName.length() <= 10){ // 其实UI最长可输入长度也有限制，这里最长限制可以删除
        userInfo.username = inName.toStdString(); // 1.2、username
    }else{
        QMessageBox::warning(ui->nameLineEdit, QString("不合法输入"), QString("请将姓名限制在2-10个字"));
        return;
    }
    if(ui->maleRadioButton->isChecked()){
        userInfo.gender = true; // 1.3、性别
    }else{
        userInfo.gender = false;
    }
    // userInfo.userType = currentUserInfo->userType; // userType不变
    // 2、用户联系方式（email\telephone\address）
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"); // 2.1、邮箱格式
    QString inEmail = ui->myEmailLineEdit->text();
    if(emailRegex.match(inEmail).hasMatch()){
        userInfo.email = inEmail.toStdString();
    }else{
        QMessageBox::warning(ui->myEmailLineEdit, QString("不合法输入"), QString("请输入合法的电子邮件地址"));
        return;
    }
    QRegularExpression phoneRegex("^1\\d{10}$"); // 2.2、手机号格式
    QString inPhone = ui->myTelephoneLineEdit->text();
    if (phoneRegex.match(inPhone).hasMatch()) {
        userInfo.telephone = inPhone.toStdString();
    } else {
        QMessageBox::warning(ui->myTelephoneLineEdit, QString("不合法输入"), QString("请输入合法的手机号（以 1 开头的 11 位纯数字）"));
        return;
    }
    QString inAddress = ui->myAddressPlainTextEdit->toPlainText(); // 除省份名称外的地址
    if(inAddress.length() < 30){
        userInfo.address = (ui->myProvinceComboBox->currentText() + inAddress).toStdString();
    }else{
        QMessageBox::warning(ui->myAddressPlainTextEdit, QString("不合法输入"), QString("通讯地址长度最长不得超过30字（不含省份名称）"));
        return;
    }
    //userInfo.password = currentUserInfo->password; // 密码不变
    //userInfo.salt = currentUserInfo->salt; // 随机盐值不变
    std::string msg;
    bool result = UserService::updateUser(msg, userInfo.id, userInfo.username, userInfo.email, userInfo.telephone, userInfo.address, userInfo.gender);
    if(result){
        QMessageBox::information(nullptr, QString("更新成功"), QString::fromStdString(msg));
        currentUserInfo->username = userInfo.username;
        currentUserInfo->email = userInfo.email;
        currentUserInfo->telephone = userInfo.telephone;
        currentUserInfo->address = userInfo.address;
        currentUserInfo->gender = userInfo.gender;
    }else{
        QMessageBox::critical(nullptr, QString("更新失败"), QString::fromStdString(msg));
    }
}

void PerAccEditForm::loadPersonalInfo(){ // 加载个人信息
    ui->idLineEdit->setText(QString::fromStdString(currentUserInfo->id));
    ui->nameLineEdit->setText(QString::fromStdString(currentUserInfo->username));
    if(currentUserInfo->gender == true){
        ui->maleRadioButton->setChecked(true);
    }else{
        ui->femaleRadioButton->setChecked(true);
    }
    ui->userTypeLineEdit->setText(QString::fromStdString(currentUserInfo->userType));
    ui->myEmailLineEdit->setText(QString::fromStdString(currentUserInfo->email));
    ui->myTelephoneLineEdit->setText(QString::fromStdString(currentUserInfo->telephone));
    // 1\遍历 myProvinceComboBox，匹配省份
    QString address = QString::fromStdString(currentUserInfo->address);
    bool found = false;
    for (int i = 0; i < ui->myProvinceComboBox->count(); ++i) {
        QString optionText = ui->myProvinceComboBox->itemText(i);
        if (address.startsWith(optionText)) {
            ui->myProvinceComboBox->setCurrentIndex(i);
            found = true;
            break;
        }
    }
    if (!found) {
        ui->myProvinceComboBox->setCurrentIndex(0);
    }
    // 2\截断匹配到省份，显示 address 后面的内容
    QString addressPart2;
    if (found) {
        int provinceLength = ui->myProvinceComboBox->currentText().length();
        addressPart2 = address.mid(provinceLength).trimmed();
    } else {
        addressPart2 = address;
    }
    ui->myAddressPlainTextEdit->setPlainText(addressPart2);
}
