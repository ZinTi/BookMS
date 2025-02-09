#include "MenuFormReader.h"
#include "ui_MenuFormReader.h"

MenuFormReader::MenuFormReader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuFormReader)
{
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0); // 菜单默认展开第1页
}

MenuFormReader::~MenuFormReader()
{
    delete ui;
}

// 1\ 自助借还
void MenuFormReader::on_pBtnBookBorrow_clicked(){    // 借阅图书 1-pBtnBookBorrow
    // 发出自定义信号，携带按钮索引 1
    emit buttonClicked(1);
}

void MenuFormReader::on_pBtnBookReturn_clicked(){    // 归还图书 2-pBtnBookReturn
    emit buttonClicked(2);
}

// 2\ 个人资料
void MenuFormReader::on_pBtnEditPersonalInfo_clicked(){    // 编辑资料 3-pBtnEditPersonalInfo
    emit buttonClicked(3);
}

void MenuFormReader::on_pBtnCancelAccount_clicked(){    // 注销账号 4-pBtnCancelAccount
    emit buttonClicked(4);
}

void MenuFormReader::on_pBtnLogout_clicked(){    // 退出登录 5-pBtnLogout
    emit buttonClicked(5);
}
