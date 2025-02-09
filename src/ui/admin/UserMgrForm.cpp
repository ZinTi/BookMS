#include "UserMgrForm.h"
#include "ui_UserMgrForm.h"
#include "User.h"
#include "UserService.h"
#include <QMessageBox>
#include "AuthDlg.h"
#include "WidgetPublicMethod.h"

UserMgrForm::UserMgrForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMgrForm)
{
    ui->setupUi(this);

    // 连接信号和槽函数，用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
    connect(ui->tableWidTabEditUser, &QTableWidget::itemClicked, this, &UserMgrForm::on_tableEditUserTableItemClicked); // UserTable表
}

UserMgrForm::~UserMgrForm()
{
    delete ui;
}


void UserMgrForm::on_pBtnTabEditUserQuery_clicked(){ // 处理用户管理-(用户)信息维护-查询按钮pBtnTabEditUserQuery的点击事件的槽函数
    // 0、定义几个变量获取用户输入的筛选条件
    std::vector<UserInfo> usersInfo; // 保存查询到的用户信息的容器
    UserInfo userInfo; // ID单条件查询时
    int numRows = 0; // 查询到用户的行数

    QString idFilter;
    QString nameFilter;
    int genderFilter;
    QString emailFilter;
    QString teleFilter;
    QString usertypeFilter;
    // QString addressFilter; // 地址省略

    // 1、首先获取 1 个（ID）或 5 个(其他几个)筛选条件目前的值，包括简单的合法性检查
    idFilter = ui->lineEditTabEditUserUserId->text();
    if(idFilter.isEmpty()){ // -- 不通过ID查询
        // 获取其他几个筛选条件
        nameFilter = ui->lineEditTabEditUserUsername->text(); // (1)获取姓名
        genderFilter = ui->comboBoxTabEditUserGender->currentIndex(); // (2)获取性别：0-所有性别（不筛选）, 1-男性 , 2-女性
        if(genderFilter == 0 || genderFilter == 1 || genderFilter == 2){
            ; // 0-所有性别, 1-男性 , 2-女性 （和DAO层不一样）
        }else{
            QMessageBox::critical(ui->comboBoxTabEditUserGender, QString("不合法性别筛选条件"), QString("因程序代码错误或系统故障导致输入的筛选条件不合法"));
            return;
        }
        usertypeFilter = ui->comboBoxTabEditUserUserType->currentText(); // (3)获取用户类别
        if( usertypeFilter == "OB-普通读者"){
            usertypeFilter = "OB";
        }else if( usertypeFilter == "FB-教职员读者"){
            usertypeFilter = "FB";
        }else if( usertypeFilter == "LA-图书管理员"){
            usertypeFilter = "LA";
        }else if( usertypeFilter == "UA-用户管理员"){
            usertypeFilter = "UA";
        }else if( usertypeFilter == "SA-系统管理员"){
            usertypeFilter = "SA";
        }else if( usertypeFilter == "DS-冻结状态"){
            usertypeFilter = "DS";
        }else{
            usertypeFilter = ""; // 不筛选用户类别（所有用户）
        }

        emailFilter = ui->lineEditTabEditUserEmail->text(); // （4）获取电子邮箱
        teleFilter = ui->lineEditTabEditUserTelephone->text(); // (5) 获取手机号码

        // 2、使用MainService执行多条件查询操作
        std::string msg;
        numRows = UserService::getAllUser(msg, nameFilter.toStdString(), genderFilter, emailFilter.toStdString(), teleFilter.toStdString(), "", usertypeFilter.toStdString(), usersInfo); // 为了简化就不筛选地址了
        if(numRows == 0){
            QMessageBox::information(nullptr, QString("查询到0条记录"), QString::fromStdString(msg));
        }
    }else{ // -- 通过ID查询
        QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 用户ID：(1)要么为空；（2）要么3-6位长度，只允许数字、小写字母、下划线_；
        if(userIdRegex.match(idFilter).hasMatch()){ // 合法ID
            std::string msg;
            if(UserService::getUserInfoByUserId(msg, idFilter.toStdString(), userInfo)){
                if(userInfo.id != "" && !userInfo.id.empty()){
                    usersInfo.push_back(userInfo);
                    numRows = 1; // 不为空才push，避免push空结构体
                }else{
                    numRows = 0;
                }
            }else{
                QMessageBox::warning(nullptr, QString("查询失败"), QString::fromStdString(msg));
                return;
            }
        }else{
            QMessageBox::warning(ui->lineEditTabEditUserUserId, QString("不合法输入"), QString("用户ID只允许数字、小写字母和下划线_，3到6位长度，你可以选择置空ID筛选器"));
            return;
        }
    }

    // 3、将查询到的数据填充到表格中显示
    ui->tableWidTabEditUser->setRowCount(numRows);
    ui->tableWidTabEditUser->setColumnCount(7); // 7 个用户信息列
    QStringList headers = {"ID", "姓名", "性别", "用户类型", "电子邮箱", "手机号码", "通讯地址"};
    ui->tableWidTabEditUser->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < numRows; ++i) {
        const UserInfo& info = usersInfo[i];
        // 设置 "ID" 列
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(info.id));
        ui->tableWidTabEditUser->setItem(i, 0, idItem); // 行，列，值
        // 设置 "姓名" 列
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(info.username));
        ui->tableWidTabEditUser->setItem(i, 1, nameItem);
        // 设置 "性别" 列
        QTableWidgetItem* genderItem = new QTableWidgetItem(QString::fromStdString( info.gender == true ? "男" : "女"  ));
        ui->tableWidTabEditUser->setItem(i, 2, genderItem);
        // 设置 "用户类型" 列
        QTableWidgetItem* typeItem = new QTableWidgetItem(QString::fromStdString(info.userType));
        ui->tableWidTabEditUser->setItem(i, 3, typeItem);
        // 设置 "电子邮箱" 列
        QTableWidgetItem* emailItem = new QTableWidgetItem(QString::fromStdString(info.email));
        ui->tableWidTabEditUser->setItem(i, 4, emailItem);
        // 设置 "手机号码" 列
        QTableWidgetItem* phoneItem = new QTableWidgetItem(QString::fromStdString(info.telephone));
        ui->tableWidTabEditUser->setItem(i, 5, phoneItem);
        // 设置 "通讯地址" 列
        QTableWidgetItem* addressItem = new QTableWidgetItem(QString::fromStdString(info.address));
        ui->tableWidTabEditUser->setItem(i, 6, addressItem);
    }

    // 4、将这个列0和列3设置为不可编辑
    setColumnNonEditable(ui->tableWidTabEditUser, 0); // ID列
    setColumnNonEditable(ui->tableWidTabEditUser, 3); // userType列

}
void UserMgrForm::on_pBtnTabEditUserClear_clicked(){ // 处理用户管理-(用户)信息维护-清空按钮pBtnTabEditUserClear的点击事件的槽函数
    // 清空几个筛选输入控件的文本和重置选项
    ui->lineEditTabEditUserUserId->clear(); // 清空账号筛选输入控件
    ui->lineEditTabEditUserUsername->clear(); // 清空姓名筛选输入控件
    ui->comboBoxTabEditUserGender->setCurrentIndex(0); // 设置为第一个选项（所有性别，即不筛选）
    ui->comboBoxTabEditUserUserType->setCurrentIndex(0); // 设置第一个选项（所有类型，即不筛选）
    ui->lineEditTabEditUserEmail->clear(); // 清空邮箱筛选输入条件控件
    ui->lineEditTabEditUserTelephone->clear(); // 清空手机号码筛选输入条件控件
}
//void on__clicked(); // 处理用户管理-(用户)信息维护-刷新按钮的点击事件的槽函数
void UserMgrForm::on_pBtnTabEditUserUpdateUser_clicked(){ // 处理用户管理-(用户)信息维护-执行更新按钮pBtnTabEditUserUpdateUser的点击事件的槽函数
    // 二次认证
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; }
    // 1、获取目标行的每一列数据
    UserInfo newInfo;
    int targetRow = row_tableW; // 要获取数据的目标行

    QRegularExpression userIdRegex("^[a-z0-9_]{3,6}$"); // 这里其实不用验证，因为这个字段的表格item已经设置为不可编辑
    QString inUserId = ui->tableWidTabEditUser->item(targetRow, 0)->text();
    if(userIdRegex.match(inUserId).hasMatch()){
        newInfo.id = inUserId.toStdString();
    }else{
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("用户ID只允许数字、小写字母和下划线_，3到6位长度"));
        return;
    }

    QString inUsername = ui->tableWidTabEditUser->item(targetRow, 1)->text();
    if(inUsername.length() >= 2 && inUsername.length() <= 10){ // 其实UI最长可输入长度也有限制，这里最长限制可以删除
        newInfo.username = inUsername.toStdString();
    }else{
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("请将姓名限制在2-10个字"));
        return;
    }


    QString inGender = ui->tableWidTabEditUser->item(targetRow, 2)->text();
    if(inGender == QString("男") || inGender == QString("男性") || inGender == QString("male")){
        newInfo.gender = true;
    }else if(inGender == QString("女") || inGender == QString("女性") || inGender == QString("female")){
        newInfo.gender = false;
    }else{
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("请输入以下合法词：男、女、男性、女性、male、female"));
        return;
    }

    // std::string inUserType = ui->tableWidTabEditUser->item(targetRow, 3)->text().toStdString(); // UserType不能在这里改

    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"); // 邮箱格式
    QString inEmail = ui->tableWidTabEditUser->item(targetRow, 4)->text();
    if(emailRegex.match(inEmail).hasMatch()){
        newInfo.email = inEmail.toStdString();
    }else{
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("请输入合法格式的电子邮箱地址"));
        return;
    }

    QRegularExpression phoneRegex("^1\\d{10}$"); // 手机号格式：以1开头、11位长度的纯数字字符串
    QString inTelephone = ui->tableWidTabEditUser->item(targetRow, 5)->text();
    if (phoneRegex.match(inTelephone).hasMatch()) {
        newInfo.telephone = inTelephone.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("请输入合法的手机号（以 1 开头的 11 位纯数字）"));
        return;
    }

    QString inAddress = ui->tableWidTabEditUser->item(targetRow, 6)->text();
    if(inAddress.length() <= 36){
        newInfo.address = inAddress.toStdString();
    }else{
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("通讯地址长度最长不得超过36字"));
        return;
    }

    // 2、执行修改并弹窗显示修改结果
    std::string msg;
    bool result = UserService::updateUser(msg, newInfo.id, newInfo.username, newInfo.email, newInfo.telephone, newInfo.address, newInfo.gender);
    if(result){
        QMessageBox::information(nullptr, QString::fromStdString("更新用户" + newInfo.id + "基本信息成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::warning(nullptr, QString::fromStdString("更新用户" + newInfo.id + "基本信息失败"), QString::fromStdString(msg));
    }
}
void UserMgrForm::on_pBtnTabEditUserDeleteUser_clicked(){ // 处理用户管理-(用户)信息维护-删除用户按钮pBtnTabEditUserDeleteUser的点击事件的槽函数
    // 二次认证
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; }
    // 获取目标用户ID
    std::string userId = ui->tableWidTabEditUser->item(row_tableW, 0)->text().toStdString();
    // 执行删除操作并显示执行结果
    std::string msg;
    bool result = UserService::removeUser(msg, userId);
    if(result){
        QMessageBox::information(nullptr, QString::fromStdString("删除用户" + userId +"成功"), QString::fromStdString(msg));

        // 删除用户后在ui->tableWidget中也删除那一行（相当于刷新操作，且无需再次从数据库中查询）
        if(ui->tableWidTabEditUser->item(row_tableW, 0)->text() == QString::fromStdString(userId)){
            // 若还是这行（未因其他操作篡改）则直接删除
            ui->tableWidTabEditUser->removeRow(row_tableW);
        }else{
            // 否则找一下再删
            bool found = false;
            int rowCount = ui->tableWidTabEditUser->rowCount();
            for (int i = 0; i < rowCount; ++i) {
                if (ui->tableWidTabEditUser->item(i, 0)->text() == QString::fromStdString(userId)) {
                    ui->tableWidTabEditUser->removeRow(i);
                    found = true;
                    break;
                }
            }
        }
    }else{
        QMessageBox::warning(nullptr, QString::fromStdString("删除用户" + userId + "失败"), QString::fromStdString(msg));
    }
}

void UserMgrForm::on_tableEditUserTableItemClicked(QTableWidgetItem *item){ // 当用户点击User表格中的项时会触发该信号，此槽函数用于处理信号
    if (item) {
        // 保存到私有变量中
        row_tableW = item->row();
        column_tableW = item->column();

        QTableWidget *tableWidget = qobject_cast<QTableWidget*>(sender());
        if (tableWidget) {
            // ui->lineEditSelectUserId->setText(QString("r%1, c%2").arg(row_tableW).arg(column_tableW)); // 测试：显示坐标
            ui->lineEditSelectUserId->setText("选中用户: " + tableWidget->item(row_tableW, 0)->text());  // 用户信息维护表的显示控件：实时显示该行数据的userId
        }
    }
}
