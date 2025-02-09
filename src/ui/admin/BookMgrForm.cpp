#include "BookMgrForm.h"
#include <QMessageBox>
#include "BookService.h"
#include "AuthDlg.h"
#include "BookUpdateDlg.h"

BookMgrForm::BookMgrForm(QWidget *parent) :
    QWidget(parent)
{

    m_vLayoutMain = new QVBoxLayout(this);
    m_title = new QLabel(QString("图书管理-信息维护"), this);
    QFont fontTitle;
    fontTitle.setPointSize(12);
    m_title->setFont(fontTitle);
    m_title->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignTop);
    m_bookSelectForm = new BookSelectForm(this); // 查书选书控件
    m_bookEditPanelOfAdmin = new BookEditPanelOfAdmin(this); // 编辑按钮操作面板
    m_vLayoutMain->addWidget(m_title);
    m_vLayoutMain->addWidget(m_bookSelectForm);
    m_vLayoutMain->addWidget(m_bookEditPanelOfAdmin);
    m_vLayoutMain->setAlignment(Qt::AlignmentFlag::AlignCenter);
    m_vLayoutMain->setStretch(0, 1); // 设置索引和拉伸因子
    m_vLayoutMain->setStretch(1, 10); // 设置索引和拉伸因子
    m_vLayoutMain->setStretch(2, 1); // 设置索引和拉伸因子

    //connect(m_bookSelectForm, &MenuFormReader::tableItemClicked, this, &BookMgrForm::handle_BookSelectForm_tableItemClick);
    connect(m_bookEditPanelOfAdmin, &BookEditPanelOfAdmin::pBtnBorrowClicked, this, &BookMgrForm::handle_BookEditPanelOfReader_BtnClick);

}

BookMgrForm::~BookMgrForm(){
    if(m_vLayoutMain){
        delete m_vLayoutMain;
    }
}

void BookMgrForm::handle_BookEditPanelOfReader_BtnClick(int index){
    if(index == 1){ // 删除
        // 处理
        QString info = QString("删除图书：");
        QString currentBookId;
        if(m_bookSelectForm->getCurrentSelectedBookId(currentBookId)){
            info += currentBookId;
            // 二次认证
            AuthDlg authDlg;
            if (authDlg.exec() == QDialog::Accepted) {} else { return; }
            deleteBookRecord(currentBookId);
        }else{
            info = QString("没有选中有效数据，请先从表格中选中一条目标图书记录");
        }
        // 返回结果
        m_bookEditPanelOfAdmin->setMsgDisplayInformation(info);
    }else if(index == 2){ // 更新
        // 处理
        QString info = QString("修改图书：");
        QString currentBookId;
        if(m_bookSelectForm->getCurrentSelectedBookId(currentBookId)){
            info += currentBookId;
            updateBookInfo(currentBookId);
        }else{
            info = QString("没有选中有效数据，请先从表格中选中一条目标图书记录");
        }
        // 返回结果
        m_bookEditPanelOfAdmin->setMsgDisplayInformation(info);
    }

}

void BookMgrForm::deleteBookRecord(QString bookId){// 执行删除
    // 执行删除操作并显示执行结果
    std::string msg;
    bool result = BookService::deleteBookInfo(msg, bookId.toStdString());
    if(result){
        QMessageBox::information(nullptr, QString("删除图书成功: ") + bookId, QString::fromStdString(msg));
        this->m_bookSelectForm->deleteFromTableWidget(bookId);

    }else{
        QMessageBox::warning(nullptr, QString("删除图书失败: ") + bookId, QString::fromStdString(msg));
    }
}
void BookMgrForm::updateBookInfo(QString bookId){ // 执行更新
    BookUpdateDlg bookUpdateDlg(bookId, this);
    bookUpdateDlg.exec();
}
