#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include <QStackedWidget>
#include "MenuFormAdmin.h"
#include "UserAddForm.h"
#include "UserMgrForm.h"
#include "UserPermissionForm.h"
#include "BookAddForm.h"
#include "BookMgrForm.h"
#include "LoanAddForm.h"
#include "LoanMgrForm.h"
#include "UserDataForm.h"
#include "BookDataForm.h"
#include "PerAccEditForm.h"
#include "PerAccCancelForm.h"

namespace Ui {
class AdminDashboard;
}

class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:
    // 自定义槽函数，用于处理子控件的按钮点击事件
    void handle_MenuFormAdmin_BtnClick(int index);

    // 界面顶部btn
    void on_menuAbout_triggered(); // 处理QMenuBar中menuAbout的点击事件

private:
    Ui::AdminDashboard *ui;

    // 0\ 主布局
    MenuFormAdmin* m_menuFormAdmin; // 功能菜单（左侧）
    QStackedWidget* m_stackedWidget; // 堆叠部件（右侧）

    // 1\ 用户管理：创建用户、信息维护、权限分配
    QTabWidget* m_pageUserMgr; // 用户管理-选项卡部件
    void initPageUserMgr();
    UserAddForm* m_userAddForm;
    UserMgrForm* m_userMgrForm;
    UserPermissionForm* m_userPermissionForm;

    // 2\ 图书管理：新增图书、信息维护
    QTabWidget* m_pageBookMgr; // 图书管理-选项卡部件
    void initPageBookMgr();
    BookAddForm* m_bookAddForm;
    BookMgrForm* m_bookMgrForm;

    // 3\ 借阅事务：办理借阅、信息维护
    QTabWidget* m_pageLoanMgr; // 借阅事务-选项卡部件
    void initPageLoanMgr();
    LoanAddForm* m_loanAddForm;
    LoanMgrForm* m_loanMgrForm;

    // 4\ 数据统计：用户数据、馆藏数据
    QTabWidget* m_pageDataMgr; // 数据统计-选项卡部件
    void initPageDataMgr();
    UserDataForm* m_userDataForm;
    BookDataForm* m_bookDataForm;

    // 5\ 个人管理：编辑资料、注销账号、退出登录
    QTabWidget* m_pagePersonalMgr; // 个人管理-选项卡部件
    void initPagePersonalMgr();
    PerAccEditForm* m_perAccEditForm;
    PerAccCancelForm* m_perAccCancelForm;

};

#endif // ADMINDASHBOARD_H
