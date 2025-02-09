#include "ReaderHomepage.h"
#include "ui_ReaderHomepage.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include "LoanService.h"

extern UserInfo* currentUserInfo;

ReaderHomepage::ReaderHomepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReaderHomepage)
{
    ui->setupUi(this);


    m_menuFormReader = new MenuFormReader(this);
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(m_menuFormReader);

    m_stackedWidget = new QStackedWidget(this);  // 堆叠部件
    hLayout->addWidget(m_stackedWidget);

    m_pageLoanMgr = new QTabWidget(m_stackedWidget); // 借阅图书-选项卡部件
    m_pagePersonalMgr = new QTabWidget(m_stackedWidget); // 个人管理-选项卡部件

    // 添加几个page
    m_stackedWidget->addWidget(m_pageLoanMgr);
    m_stackedWidget->addWidget(m_pagePersonalMgr);

    initPageLoanMgr();
    initPagePersonalMgr();

    // 设置拉伸因子，第一个参数是要设置的索引，第二个参数是拉伸因子
    hLayout->setStretch(0, 1); // 给 m_menuFormReader 设置拉伸因子为 1
    hLayout->setStretch(1, 5); // 给 m_stackedWidget 设置拉伸因子为 5
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(m_menuFormReader, &MenuFormReader::buttonClicked, this, &ReaderHomepage::handle_MenuFormReader_BtnClick);
    connect(m_borrowPanelOfReader, &BorrowPanelOfReader::pBtnBorrowClicked, this, &ReaderHomepage::handle_BorrowPanelOfReader_BtnClick);

    // 因为 Qt Designer 自动连接的机制，所以不需要写下面这些connect了。UI会自动查询on_按钮名称_clicked()的槽函数，注意槽函数按约定命名就行

}

ReaderHomepage::~ReaderHomepage()
{
    delete ui;
    delete m_menuFormReader; // 功能菜单（左侧）
    delete m_stackedWidget; // 堆叠部件（右侧）
}

void ReaderHomepage::handle_MenuFormReader_BtnClick(int index){
    // 设置当前显示的widget页面
    if(index>=1 && index<=2){
        m_stackedWidget->setCurrentIndex(0);
    }else if(index>=3 && index<=5){
        m_stackedWidget->setCurrentIndex(1);
    }else{
        QMessageBox::information(this, QString("无效的按键信号"), QString::fromStdString(std::to_string(index)));
    }
    // 设置当前tab
    switch(index){
    case 1:{
        m_pageLoanMgr->setCurrentIndex(0);
        break;
    }
    case 2:{
        m_pageLoanMgr->setCurrentIndex(1);
        break;
    }
    case 3:{
        m_pagePersonalMgr->setCurrentIndex(0);
        break;
    }
    case 4:{
        m_pagePersonalMgr->setCurrentIndex(1);
        break;
    }
    case 5:{
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

void ReaderHomepage::handle_BorrowPanelOfReader_BtnClick(int index){
    if(index == 1){
        // 处理
        QString info = QString("你想要借的书：");
        QString currentBookId;
        if(m_bookSelectForm->getCurrentSelectedBookId(currentBookId)){
            info += currentBookId;
            std::string msg;
            LoanService::borrowBook(msg, currentUserInfo->id, currentBookId.toStdString());
            info += msg;
        }else{
            info = QString("没有有效数据");
        }
        // 返回结果
        m_borrowPanelOfReader->setMsgDisplayInformation(info);
    }
}


void ReaderHomepage::initPageLoanMgr(){
    m_bookSelectForm = new BookSelectForm(m_pageLoanMgr); // 查书选书控件
    m_borrowPanelOfReader = new BorrowPanelOfReader(m_pageLoanMgr); // 借书控件

    QVBoxLayout* vLayoutBorrow = new QVBoxLayout(m_pageLoanMgr);
    vLayoutBorrow->addWidget(m_bookSelectForm);
    vLayoutBorrow->addWidget(m_borrowPanelOfReader);
    m_bookBorrowForm = new QWidget(m_pageLoanMgr);
    m_bookBorrowForm->setLayout(vLayoutBorrow);
    vLayoutBorrow->setStretch(0, 10); // 设置索引和拉伸因子
    vLayoutBorrow->setStretch(1, 1); // 设置索引和拉伸因子
    m_pageLoanMgr->addTab(m_bookBorrowForm, QString("借阅图书"));

    m_bookReturnForm = new LoanMgrForm(m_pageLoanMgr);
    m_pageLoanMgr->addTab(m_bookReturnForm, QString("归还图书"));
}

void ReaderHomepage::initPagePersonalMgr(){
    m_perAccEditForm = new PerAccEditForm(m_pagePersonalMgr); // 个人管理
    m_pagePersonalMgr->addTab(m_perAccEditForm, QString("编辑资料"));
    m_perAccCancelForm = new PerAccCancelForm(m_pagePersonalMgr);
    m_pagePersonalMgr->addTab(m_perAccCancelForm, QString("注销账号"));
}

// 界面顶部通用btn
void ReaderHomepage::on_menuAbout_triggered(){ // QMenuBar中menuExit的点击事件的槽函数
    QMessageBox::information(this, QString("关于(ABOUT)"), QString("图书借阅管理系统（C++实训-2024年12月）"));
}
