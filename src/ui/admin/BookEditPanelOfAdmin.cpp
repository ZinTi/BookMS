#include "BookEditPanelOfAdmin.h"

BookEditPanelOfAdmin::BookEditPanelOfAdmin(QWidget *parent) : QWidget(parent){
    m_hLayoutMain = new QHBoxLayout(this);
    m_msgDisplay = new QTextEdit(this);
    m_msgDisplay->setReadOnly(true);
    m_hLayoutMain->addWidget(m_msgDisplay);
    m_vLayoutPushBtn = new QVBoxLayout();
    m_pBtnEditBookClear = new QPushButton(QString("清空"), this);
    m_pBtnEditBookDelete = new QPushButton(QString("删除"), this);
    m_pBtnEditBookUpdate = new QPushButton(QString("更新"), this);
    m_vLayoutPushBtn->addWidget(m_pBtnEditBookClear);
    m_vLayoutPushBtn->addWidget(m_pBtnEditBookDelete);
    m_vLayoutPushBtn->addWidget(m_pBtnEditBookUpdate);
    m_hLayoutMain->addLayout(m_vLayoutPushBtn);

    connect(m_pBtnEditBookClear, &QPushButton::clicked, this, &BookEditPanelOfAdmin::on_pBtnEditBookClear_clicked);
    connect(m_pBtnEditBookDelete, &QPushButton::clicked, this, &BookEditPanelOfAdmin::on_pBtnEditBookDelete_clicked);
    connect(m_pBtnEditBookUpdate, &QPushButton::clicked, this, &BookEditPanelOfAdmin::on_pBtnEditBookUpdate_clicked);
}

BookEditPanelOfAdmin::~BookEditPanelOfAdmin(){
    if(m_hLayoutMain){
        delete m_hLayoutMain;
    }
}

void BookEditPanelOfAdmin::setMsgDisplayInformation(const QString& information){
    m_msgDisplay->append(information);
}

void BookEditPanelOfAdmin::on_pBtnEditBookClear_clicked(){
    m_msgDisplay->clear();
}

void BookEditPanelOfAdmin::on_pBtnEditBookDelete_clicked(){
    emit pBtnBorrowClicked(1);
}
void BookEditPanelOfAdmin::on_pBtnEditBookUpdate_clicked(){
    emit pBtnBorrowClicked(2);
}
