#include "BookAddForm.h"
#include "ui_BookAddForm.h"
#include <QMessageBox>
#include "BookService.h"
#include "entity.h"

BookAddForm::BookAddForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookAddForm)
{
    ui->setupUi(this);
    ui->editBookPubDate->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->editBookPubDate->setCalendarPopup(true);
}

BookAddForm::~BookAddForm()
{
    delete ui;
}

void BookAddForm::on_pBtnAddBookClear_clicked(){ // 清空TabAddUser页面的输入框
    ui->editBookId->clear(); // lineEdit
    ui->editBookISBN->clear(); // lineEdit
    ui->editBookName->clear(); // lineEdit
    ui->editBookAuthor->clear(); // lineEdit
    ui->editBookEdition->clear(); // lineEdit
    ui->editBookPubDate->setDate(QDate::currentDate()); // DateEdit
    ui->editBookTotal->clear(); // spinBox
    ui->editBookRemain->clear(); // spinBox
    ui->editBookPrice->clear(); // doubleSpinBox
    ui->editBookPublisher->clear(); // lineEdit
    ui->editBookLevel->setCurrentIndex(0); // comboBox
}
void BookAddForm::on_pBtnAddBookSubmit_clicked() {
    // 获取数据
    QString inBookId = ui->editBookId->text();
    QString inISBN = ui->editBookISBN->text();
    QString inBookName = ui->editBookName->text();
    QString inAuthor = ui->editBookAuthor->text();
    QString inEdition = ui->editBookEdition->text();
    QString inPubDate = ui->editBookPubDate->date().toString("yyyy-MM-dd");
    int inTotal = ui->editBookTotal->value();
    int inRemain = ui->editBookRemain->value();
    double inPrice = ui->editBookPrice->value();
    QString inPublisher = ui->editBookPublisher->text();
    int inLevel = ui->editBookLevel->currentIndex();

    // 检查数据合法性
    // 定义正则表达式
    QRegularExpression idRegex("^[a-z0-9]{8}$"); // 长度为8位、仅小写字母和数字
    QRegularExpression isbnRegex("^[0-9]{13}$"); // 长度为13位，纯数字字符
    QRegularExpression nameRegex("^.{1,50}$"); // 不为空，50字以内
    QRegularExpression authorRegex("^.{1,50}$"); // 不为空，50字以内
    QRegularExpression editionRegex("^.{1,20}$"); // 不为空，20字以内
    QRegularExpression publisherRegex("^.{1,50}$"); // 不为空，50字以内

    // 检查 BookId 合法性
    if (!idRegex.match(inBookId).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书 ID 长度应为 8 位，仅包含小写字母和数字。"));
        return;
    }
    // 检查 ISBN 合法性
    if (!isbnRegex.match(inISBN).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书 ISBN 长度应为 13 位，仅包含数字。"));
        return;
    }
    // 检查 BookName 合法性
    if (!nameRegex.match(inBookName).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书名称不能为空，且应在 50 字以内。"));
        return;
    }
    // 检查 Author 合法性
    if (!authorRegex.match(inAuthor).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("作者名称不能为空，且应在 50 字以内。"));
        return;
    }
    // 检查 Edition 合法性
    if (!editionRegex.match(inEdition).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书版本不能为空，且应在 20 字以内。"));
        return;
    }
    // 检查 Publisher 合法性
    if (!publisherRegex.match(inPublisher).hasMatch()) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("出版社名称不能为空，且应在 50 字以内。"));
        return;
    }
    // 检查 TotalQty 合法性
    if (inTotal < 0) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书总数量不能小于 0。"));
        return;
    }
    // 检查 StockRemain 合法性
    if (inRemain > inTotal) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书剩余数量不能大于总数量。"));
        return;
    }
    // 检查 Price 合法性
    if (inPrice < 0) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("图书价格不能小于 0。"));
        return;
    }
    // 检查 PubDate 合法性
    QDateTime currentDate = QDateTime::currentDateTime();
    QDateTime inputDate = QDateTime::fromString(inPubDate, "yyyy-MM-dd");
    if (inputDate > currentDate) {
        QMessageBox::warning(nullptr, QString("数据不合法"), QString("出版日期不能在当前日期之后。"));
        return;
    }

    BookInfo bookInfo;
    bookInfo.id = inBookId.toStdString();
    bookInfo.ISBN = inISBN.toStdString();
    bookInfo.bookName = inBookName.toStdString();
    bookInfo.author = inAuthor.toStdString();
    bookInfo.edition = inEdition.toStdString();
    bookInfo.pubDate = inPubDate.toStdString();
    bookInfo.totalQty = inTotal;
    bookInfo.stockRemain = inRemain;
    bookInfo.price = inPrice;
    bookInfo.publisher = inPublisher.toStdString();
    if (inLevel == 0) {
        bookInfo.bookLevel = 2; // 代表普通图书
    } else if (inLevel == 1) {
        bookInfo.bookLevel = 1; // 代表珍贵图书
    } else {
        QMessageBox::critical(ui->editBookLevel, QString("程序错误导致的不合法输入"), QString("图书等级只允许 2 种可选项，因程序逻辑错误或系统故障导致目前的输入数据不合法"));
        return;
    }
    std::string msg;
    bool result = BookService::addBookInfo(msg, bookInfo);
    if (result) {
        QMessageBox::information(nullptr, QString("成功录入图书信息"), QString::fromStdString(msg));
    } else {
        QMessageBox::warning(nullptr, QString("录入图书信息失败"), QString::fromStdString(msg));
        return;
    }
}
