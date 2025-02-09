#include "MenuFormAdmin.h"
#include "ui_MenuFormAdmin.h"

MenuFormAdmin::MenuFormAdmin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuFormAdmin)
{
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0); // 菜单默认展开第1页
}

MenuFormAdmin::~MenuFormAdmin()
{
    delete ui;
}

// 1\ 用户管理-tPageUser
void MenuFormAdmin::on_pBtnAddUser_clicked(){                  // 创建用户 1-pBtnAddUser
    // 发出自定义信号，携带按钮索引 1
    emit buttonClicked(1);
}
void MenuFormAdmin::on_pBtnEditUser_clicked(){                 // 信息维护 2-pBtnEditUser
    // 发出自定义信号，携带按钮索引 2
    emit buttonClicked(2);
}
void MenuFormAdmin::on_pBtnAllocatePermissions_clicked(){      // 权限分配 3-pBtnAllocatePermissions
    emit buttonClicked(3);
}
// 2\ 图书管理-tPageBook
void MenuFormAdmin::on_pBtnAddBook_clicked(){                  // 新增图书 4-pBtnAddBook
    emit buttonClicked(4);
}
void MenuFormAdmin::on_pBtnEditBook_clicked(){                 // 信息维护 5-pBtnEditBook
    emit buttonClicked(5);
}
// 3\ 借阅事务-tPageLoan
void MenuFormAdmin::on_pBtnHandleBookBorrow_clicked(){         // 办理借阅 6-pBtnHandleBookBorrow
    emit buttonClicked(6);
}
void MenuFormAdmin::on_pBtnHandleBookReturn_clicked(){         // 处理归还 7-pBtnHandleBookReturn
    emit buttonClicked(7);
}
// 4\ 数据统计-tPageData
void MenuFormAdmin::on_pBtnShowUserData_clicked(){             // 用户数据 8-pBtnShowUserData
    emit buttonClicked(8);
}
void MenuFormAdmin::on_pBtnShowBookData_clicked(){             // 馆藏数据 9-pBtnShowBookData
    emit buttonClicked(9);
}
// 5\ 个人管理-tPagePersonal
void MenuFormAdmin::on_pBtnEditPersonalInfo_clicked(){         // 编辑资料 10-pBtnEditPersonalInfo
    emit buttonClicked(10);
}
void MenuFormAdmin::on_pBtnCancelAccount_clicked(){            // 注销账号 11-pBtnCancelAccount
    emit buttonClicked(11);
}
void MenuFormAdmin::on_pBtnLogout_clicked(){                   // 退出登录 12-pBtnLogout
    emit buttonClicked(12);
}
