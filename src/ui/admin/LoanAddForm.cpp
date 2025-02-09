#include "LoanAddForm.h"
#include "ui_LoanAddForm.h"
#include <QMessageBox>
#include "BookService.h"
#include "UserService.h"
#include "LoanService.h"

LoanAddForm::LoanAddForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoanAddForm)
{
    ui->setupUi(this);
    ui->msgDisplayBook->setReadOnly(true);
    ui->msgDisplayUser->setReadOnly(true);
    ui->msgDisplayLoan->setReadOnly(true);
}

LoanAddForm::~LoanAddForm()
{
    delete ui;
}

void LoanAddForm::on_pBtnClearBook_clicked(){
    ui->editBookId->clear();
    ui->msgDisplayBook->clear();
}
void LoanAddForm::on_pBtnQueryBook_clicked(){
    BookInfo bookInfo;
    QString inBookId = ui->editBookId->text();
    if(inBookId.isEmpty()){
        QMessageBox::warning(ui->editBookId, QString("搜索框为空"), QString("请输入图书ID"));
        return;
    }
    std::string msg;
    if(BookService::getBookInfoByBookId(msg, inBookId.toStdString(), bookInfo)){
        // 清空 QTextEdit
        ui->msgDisplayBook->clear();

        // 使用 insertHtml() 插入带颜色的文本
        ui->msgDisplayBook->insertHtml(
            QString("<span style='color:rgb(0,100,255);'>图书名称: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span><br>").arg(QString::fromStdString(bookInfo.bookName)) +
            QString("<span style='color:rgb(0,100,255);'>图书作者: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span><br>").arg(QString::fromStdString(bookInfo.author)) +
            QString("<span style='color:rgb(0,100,255);'>ISBN: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span><br>").arg(QString::fromStdString(bookInfo.ISBN)) +
            QString("<span style='color:rgb(0,100,255);'>图书类型: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span>").arg(QString::fromStdString(bookInfo.bookLevel == 1 ? "珍贵图书" : "普通图书"))
            );
    }else{
        ui->msgDisplayBook->clear();
        ui->msgDisplayBook->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->msgDisplayBook->insertPlainText(QString("查询失败："));
        ui->msgDisplayBook->insertPlainText(QString::fromStdString(msg));
        return;
    }
}
void LoanAddForm::on_pBtnClearUser_clicked(){
    ui->editUserId->clear();
    ui->msgDisplayUser->clear();
}
void LoanAddForm::on_pBtnQueryUser_clicked(){
    UserInfo userInfo;
    QString inUserId = ui->editUserId->text();
    if(inUserId.isEmpty()){
        QMessageBox::warning(ui->editUserId, QString("搜索框为空"), QString("请输入用户ID"));
        return;
    }
    std::string msg;
    if(UserService::getUserInfoByUserId(msg, inUserId.toStdString(), userInfo)){
        // 清空 QTextEdit
        ui->msgDisplayUser->clear();

        // 使用 insertHtml() 插入带颜色的文本
        ui->msgDisplayUser->insertHtml(
            QString("<span style='color:rgb(0,100,255);'>用户姓名: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span><br>").arg(QString::fromStdString(userInfo.username)) +
            QString("<span style='color:rgb(0,100,255);'>用户性别: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span><br>").arg(userInfo.gender ? "男" : "女") +
            QString("<span style='color:rgb(0,100,255);'>用户类别: </span>") +
            QString("<span style='color:rgb(255,100,0);'>%1</span>").arg(QString::fromStdString(userInfo.userType))
            );
    }else{
        ui->msgDisplayUser->clear();
        ui->msgDisplayUser->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->msgDisplayUser->insertPlainText(QString("查询失败："));
        ui->msgDisplayUser->insertPlainText(QString::fromStdString(msg));
        return;
    }
}
void LoanAddForm::on_pBtnClearLoan_clicked(){
    ui->msgDisplayLoan->clear();
}
void LoanAddForm::on_pBtnHandleLoan_clicked(){
    QString inBookId = ui->editBookId->text();
    QString inUserId = ui->editUserId->text();
    if(inBookId.isEmpty()){
        QMessageBox::warning(ui->editBookId, QString("搜索框为空"), QString("请输入图书ID"));
        return;
    }
    if(inUserId.isEmpty()){
        QMessageBox::warning(ui->editUserId, QString("搜索框为空"), QString("请输入用户ID"));
        return;
    }
    std::string msgLoan;
    if(LoanService::borrowBook(msgLoan, inUserId.toStdString(), inBookId.toStdString())){
        ui->msgDisplayLoan->setTextColor(QColor::fromRgb(0,255,0));
    }else{
        ui->msgDisplayLoan->setTextColor(QColor::fromRgb(255,0,0));
    }
    ui->msgDisplayLoan->append(QString::fromStdString(msgLoan));
}
