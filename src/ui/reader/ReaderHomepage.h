#ifndef READERHOMEPAGE_H
#define READERHOMEPAGE_H

#include <QMainWindow>
#include <QStackedWidget>
#include "MenuFormReader.h"
#include "BookSelectForm.h"
#include "LoanMgrForm.h"
#include "PerAccEditForm.h"
#include "PerAccCancelForm.h"
#include "BorrowPanelOfReader.h"

namespace Ui {
class ReaderHomepage;
}

class ReaderHomepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReaderHomepage(QWidget *parent = nullptr);
    ~ReaderHomepage();

private slots:
    // 自定义槽函数，用于处理子控件的按钮点击事件
    void handle_MenuFormReader_BtnClick(int index);
    void handle_BorrowPanelOfReader_BtnClick(int index);

    // 界面顶部btn
    void on_menuAbout_triggered(); // 处理QMenuBar中menuAbout的点击事件

private:
    Ui::ReaderHomepage *ui;

    // 0\ 主布局
    MenuFormReader* m_menuFormReader; // 功能菜单（左侧）
    QStackedWidget* m_stackedWidget; // 堆叠部件（右侧）

    // 1\ 自助借还：借阅图书、归还图书
    QTabWidget* m_pageLoanMgr; // 图书管理-选项卡部件
    void initPageLoanMgr();
    QWidget* m_bookBorrowForm; // 借书
    LoanMgrForm* m_bookReturnForm; // 还书

    BookSelectForm* m_bookSelectForm; // 借书-查书控件
    BorrowPanelOfReader* m_borrowPanelOfReader; // 借书-操作面板

    // 2\ 个人管理：编辑资料、注销账号、退出登录
    QTabWidget* m_pagePersonalMgr; // 个人管理-选项卡部件
    void initPagePersonalMgr();
    PerAccEditForm* m_perAccEditForm;
    PerAccCancelForm* m_perAccCancelForm;
};

#endif // READERHOMEPAGE_H
