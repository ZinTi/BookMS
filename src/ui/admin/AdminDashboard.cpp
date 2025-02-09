#include "AdminDashboard.h"
#include "ui_AdminDashboard.h"
#include <QHBoxLayout>
#include <QMessageBox>

AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);

    m_menuFormAdmin = new MenuFormAdmin(this);
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(m_menuFormAdmin);

    m_stackedWidget = new QStackedWidget(this);  // 堆叠部件
    hLayout->addWidget(m_stackedWidget);

    m_pageUserMgr = new QTabWidget(m_stackedWidget); // 用户管理-选项卡部件
    m_pageBookMgr = new QTabWidget(m_stackedWidget); // 图书管理-选项卡部件
    m_pageLoanMgr = new QTabWidget(m_stackedWidget); // 借阅事务-选项卡部件
    m_pageDataMgr = new QTabWidget(m_stackedWidget); // 数据统计-选项卡部件
    m_pagePersonalMgr = new QTabWidget(m_stackedWidget); // 个人管理-选项卡部件

    // 添加几个page
    m_stackedWidget->addWidget(m_pageUserMgr);
    m_stackedWidget->addWidget(m_pageBookMgr);
    m_stackedWidget->addWidget(m_pageLoanMgr);
    m_stackedWidget->addWidget(m_pageDataMgr);
    m_stackedWidget->addWidget(m_pagePersonalMgr);

    initPageUserMgr();
    initPageBookMgr();
    initPageLoanMgr();
    initPageDataMgr();
    initPagePersonalMgr();

    // 设置拉伸因子，第一个参数是要设置的索引，第二个参数是拉伸因子
    hLayout->setStretch(0, 1); // 给 m_menuFormAdmin 设置拉伸因子为 1
    hLayout->setStretch(1, 5); // 给 m_stackedWidget 设置拉伸因子为 5
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(m_menuFormAdmin, &MenuFormAdmin::buttonClicked, this, &AdminDashboard::handle_MenuFormAdmin_BtnClick);

    // 因为 Qt Designer 自动连接的机制，所以不需要写下面这些connect了。UI会自动查询on_按钮名称_clicked()的槽函数，注意槽函数按约定命名就行

}

AdminDashboard::~AdminDashboard()
{
    delete ui;
    delete m_menuFormAdmin; // 功能菜单（左侧）
    delete m_stackedWidget; // 堆叠部件（右侧）
}

void AdminDashboard::handle_MenuFormAdmin_BtnClick(int index){
    // 设置当前显示的widget页面
    if(index>=1 && index<=3){
        m_stackedWidget->setCurrentIndex(0);
    }else if(index>=4 && index<=5){
        m_stackedWidget->setCurrentIndex(1);
    }else if(index>=6 && index<=7){
        m_stackedWidget->setCurrentIndex(2);
    }else if(index>=8 && index<=9){
        m_stackedWidget->setCurrentIndex(3);
    }else if(index>=10 && index<=12){
        m_stackedWidget->setCurrentIndex(4);
    }else{
        QMessageBox::information(this, QString("无效的按键信号"), QString::fromStdString(std::to_string(index)));
    }
    // 设置当前tab
    switch(index){
    case 1:{
        m_pageUserMgr->setCurrentIndex(0);
        break;
    }
    case 2:{
        m_pageUserMgr->setCurrentIndex(1);
        break;
    }
    case 3:{
        m_pageUserMgr->setCurrentIndex(2);
        break;
    }
    case 4:{
        m_pageBookMgr->setCurrentIndex(0);
        break;
    }
    case 5:{
        m_pageBookMgr->setCurrentIndex(1);
        break;
    }
    case 6:{
        m_pageLoanMgr->setCurrentIndex(0);
        break;
    }
    case 7:{
        m_pageLoanMgr->setCurrentIndex(1);
        break;
    }
    case 8:{
        m_pageDataMgr->setCurrentIndex(0);
        break;
    }
    case 9:{
        m_pageDataMgr->setCurrentIndex(1);
        break;
    }
    case 10:{
        m_pagePersonalMgr->setCurrentIndex(0);
        break;
    }
    case 11:{
        m_pagePersonalMgr->setCurrentIndex(1);
        break;
    }
    case 12:{
        // 弹出确认退出的消息框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString("退出程序"), QString("确定退出吗？"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) { // 用户点击了“确定”，退出应用程序
            QApplication::quit();
        }
        // break;
    }
    default:
        ;
    }
}

void AdminDashboard::initPageUserMgr(){
    m_userAddForm = new UserAddForm(m_pageUserMgr); // 用户管理
    m_pageUserMgr->addTab(m_userAddForm, QString("创建用户"));
    m_userMgrForm = new UserMgrForm(m_pageUserMgr);
    m_pageUserMgr->addTab(m_userMgrForm, QString("用户信息维护"));
    m_userPermissionForm = new UserPermissionForm(m_pageUserMgr);
    m_pageUserMgr->addTab(m_userPermissionForm, QString("权限分配"));
}
void AdminDashboard::initPageBookMgr(){
    m_bookAddForm = new BookAddForm(m_pageBookMgr); // 图书管理
    m_pageBookMgr->addTab(m_bookAddForm, QString("新增图书"));
    m_bookMgrForm = new BookMgrForm(m_pageBookMgr);
    m_pageBookMgr->addTab(m_bookMgrForm, QString("图书信息维护"));
}
void AdminDashboard::initPageLoanMgr(){
    m_loanAddForm = new LoanAddForm(m_pageLoanMgr); // 借阅事务
    m_pageLoanMgr->addTab(m_loanAddForm, QString("办理借阅"));
    m_loanMgrForm = new LoanMgrForm(m_pageLoanMgr);
    m_pageLoanMgr->addTab(m_loanMgrForm, QString("借阅信息维护"));
}
void AdminDashboard::initPageDataMgr(){
    m_userDataForm = new UserDataForm(m_pageDataMgr); // 数据分析
    m_pageDataMgr->addTab(m_userDataForm, QString("用户数据"));
    m_bookDataForm = new BookDataForm(m_pageDataMgr);
    m_pageDataMgr->addTab(m_bookDataForm, QString("馆藏数据"));
}
void AdminDashboard::initPagePersonalMgr(){
    m_perAccEditForm = new PerAccEditForm(m_pagePersonalMgr); // 个人管理
    m_pagePersonalMgr->addTab(m_perAccEditForm, QString("编辑资料"));
    m_perAccCancelForm = new PerAccCancelForm(m_pagePersonalMgr);
    m_pagePersonalMgr->addTab(m_perAccCancelForm, QString("注销账号"));
}

// 0、界面顶部通用btn
void AdminDashboard::on_menuAbout_triggered(){ // QMenuBar中menuExit的点击事件的槽函数
    QMessageBox::information(this, QString("关于(ABOUT)"), QString("图书借阅管理系统（C++实训-2024年12月）"));
}
