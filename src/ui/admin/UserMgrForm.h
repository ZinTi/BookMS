#ifndef USERMGRFORM_H
#define USERMGRFORM_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class UserMgrForm;
}

class UserMgrForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserMgrForm(QWidget *parent = nullptr);
    ~UserMgrForm();

private slots:
    void on_pBtnTabEditUserQuery_clicked(); // 处理用户管理-(用户)信息维护-查询按钮pBtnTabEditUserQuery的点击事件
    void on_pBtnTabEditUserClear_clicked(); // 处理用户管理-(用户)信息维护-清空按钮pBtnTabEditUserClear的点击事件
    //void on__clicked(); // 处理用户管理-(用户)信息维护-刷新按钮的点击事件
    void on_pBtnTabEditUserUpdateUser_clicked(); // 处理用户管理-(用户)信息维护-执行更新按钮pBtnTabEditUserUpdateUser的点击事件
    void on_pBtnTabEditUserDeleteUser_clicked(); // 处理用户管理-(用户)信息维护-删除用户按钮pBtnTabEditUserDeleteUser的点击事件
    void on_tableEditUserTableItemClicked(QTableWidgetItem *item); // 当用户点击UserTable表格中的项时会触发该信号，此槽函数用于处理信号

private:
    Ui::UserMgrForm *ui;

    int row_tableW; // 当前行
    int column_tableW; // 当前列
};

#endif // USERMGRFORM_H
