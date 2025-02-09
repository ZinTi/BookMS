#include "BorrowPanelOfReader.h"

BorrowPanelOfReader::BorrowPanelOfReader(QWidget *parent)
    : QWidget(parent){

    m_hLayoutMain = new QHBoxLayout(this);
    m_msgDisplay = new QTextEdit(this);
    m_msgDisplay->setReadOnly(true);
    m_hLayoutMain->addWidget(m_msgDisplay);
    m_vLayoutPushBtn = new QVBoxLayout(this);
    m_pBtnBorrow = new QPushButton(QString("借书"),this);
    m_pBtnClear = new QPushButton(QString("清空"),this);
    m_vLayoutPushBtn->addWidget(m_pBtnBorrow);
    m_vLayoutPushBtn->addWidget(m_pBtnClear);
    m_hLayoutMain->addLayout(m_vLayoutPushBtn);

    connect(m_pBtnBorrow, &QPushButton::clicked, this, &BorrowPanelOfReader::on_pBtnBorrow_clicked);
    connect(m_pBtnClear, &QPushButton::clicked, this, &BorrowPanelOfReader::on_pBtnClear_clicked);
}

BorrowPanelOfReader::~BorrowPanelOfReader(){
    if(m_hLayoutMain){
        delete m_hLayoutMain;
    }
}


void BorrowPanelOfReader::setMsgDisplayInformation(const QString& information){
    m_msgDisplay->append(information);
}

void BorrowPanelOfReader::on_pBtnBorrow_clicked(){
    emit pBtnBorrowClicked(1);
}

void BorrowPanelOfReader::on_pBtnClear_clicked(){
    m_msgDisplay->clear();
}
