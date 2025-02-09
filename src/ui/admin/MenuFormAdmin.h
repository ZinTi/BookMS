#ifndef MENUFORMADMIN_H
#define MENUFORMADMIN_H

#include <QWidget>

namespace Ui {
class MenuFormAdmin;
}

class MenuFormAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit MenuFormAdmin(QWidget *parent = nullptr);
    ~MenuFormAdmin();

signals:
    // 自定义信号，携带按钮索引，用于通知主窗口或查看器显示内容
    void buttonClicked(int index);

private slots:
    // 1\ 用户管理-tPageUser
    void on_pBtnAddUser_clicked();                  // 创建用户 1-pBtnAddUser
    void on_pBtnEditUser_clicked();                 // 信息维护 2-pBtnEditUser
    void on_pBtnAllocatePermissions_clicked();      // 权限分配 3-pBtnAllocatePermissions
    // 2\ 图书管理-tPageBook
    void on_pBtnAddBook_clicked();                  // 新增图书 4-pBtnAddBook
    void on_pBtnEditBook_clicked();                 // 信息维护 5-pBtnEditBook
    // 3\ 借阅事务-tPageLoan
    void on_pBtnHandleBookBorrow_clicked();         // 办理借阅 6-pBtnHandleBookBorrow
    void on_pBtnHandleBookReturn_clicked();         // 处理归还 7-pBtnHandleBookReturn
    // 4\ 数据统计-tPageData
    void on_pBtnShowUserData_clicked();             // 用户数据 8-pBtnShowUserData
    void on_pBtnShowBookData_clicked();             // 馆藏数据 9-pBtnShowBookData
    // 5\ 个人管理-tPagePersonal
    void on_pBtnEditPersonalInfo_clicked();         // 编辑资料 10-pBtnEditPersonalInfo
    void on_pBtnCancelAccount_clicked();            // 注销账号 11-pBtnCancelAccount
    void on_pBtnLogout_clicked();                   // 退出登录 12-pBtnLogout

private:
    Ui::MenuFormAdmin *ui;
};

#endif // MENUFORMADMIN_H
