#ifndef USERPERMISSIONFORM_H
#define USERPERMISSIONFORM_H

#include <QWidget>

namespace Ui {
class UserPermissionForm;
}

class UserPermissionForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserPermissionForm(QWidget *parent = nullptr);
    ~UserPermissionForm();

private slots:
    void on_pBtnAllocateAllocatePermissionsResetPwd_clicked(); // 处理用户管理-权限分配-充值用户密码按钮的点击事件
    void on_pBtnAllocateAllocatePermissionsUserType_clicked(); // 处理用户管理-权限分配-修改用户类型按钮的点击事件

private:
    Ui::UserPermissionForm *ui;
};

#endif // USERPERMISSIONFORM_H
