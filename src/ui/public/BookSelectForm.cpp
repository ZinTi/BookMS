#include "BookSelectForm.h"
#include "ui_BookSelectForm.h"
#include <QMessageBox>
#include "BookService.h"
#include "WidgetPublicMethod.h"

BookSelectForm::BookSelectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookSelectForm){
    ui->setupUi(this);

    ui->queryBookStartDate->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->queryBookStartDate->setCalendarPopup(true);
    ui->queryBookEndDate->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->queryBookEndDate->setCalendarPopup(true);
    ui->queryBookEndDate->setDate(QDate::currentDate());

    // 连接信号和槽函数，用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
    connect(ui->tableWidTabEditBook, &QTableWidget::itemClicked, this, &BookSelectForm::on_tableEditBookTableItemClicked); // BookTable表

}

BookSelectForm::~BookSelectForm(){
    delete ui;
}

void BookSelectForm::getPosition(int* x, int* y){
    *x = this->column_tableW;
    *y = this->row_tableW;
}

bool BookSelectForm::getCurrentSelectedBookId(QString& bookId){
    const auto item = ui->tableWidTabEditBook->item(row_tableW, 0);
    if(item == nullptr){
        return false;
    }
    bookId = item->text();
    return true;
}
bool BookSelectForm::getCurrentSelectedBookName(QString& bookName){
    const auto item = ui->tableWidTabEditBook->item(row_tableW, 2);
    if(item == nullptr){
        return false;
    }
    bookName = item->text();
    return true;
}

void BookSelectForm::deleteFromTableWidget(QString bookId){
    // 更新表格控件ui->tableWidget
    if(ui->tableWidTabEditBook->item(row_tableW, 0)->text() == bookId){ // 若还是这行（未因其他操作篡改）则直接删除
        ui->tableWidTabEditBook->removeRow(row_tableW);
    }else{
        // 否则找一下再删
        bool found = false;
        int rowCount = ui->tableWidTabEditBook->rowCount();
        for (int i = 0; i < rowCount; ++i) {
            if (ui->tableWidTabEditBook->item(i, 0)->text() == bookId) {
                ui->tableWidTabEditBook->removeRow(i);
                found = true;
                break;
            }
        }
    }
}

void BookSelectForm::on_pBtnEditBookClear_clicked(){// 清空筛选器按钮
    ui->queryBookByISBN->clear();
    ui->queryBookByName->clear();
    ui->queryBookByAuthor->clear();
    ui->queryBookByLevel->setCurrentIndex(0);
    ui->queryBookByPublisher->clear();
    ui->queryBookStartDate->setDate(QDate(2000,1,1));
    ui->queryBookEndDate->setDate(QDate::currentDate());
}
void BookSelectForm::on_pBtnEditBookQuery_clicked(){// 查询按钮
    std::vector<BookInfo> booksInfo;
    BookInfo bookInfo;
    int numRows;
    //
    QString isbnFilter;
    QString bookNameFilter;
    QString authorFilter;
    int bookLevelFilter;
    QString publisherFilter;
    QString pubStartDateFilter;
    QString pubEndDateFilter;
    //
    isbnFilter = ui->queryBookByISBN->text();
    if(isbnFilter.isEmpty()){ // 通过其他多个筛选条件模糊查询
        std::string isbn;
        std::string bookName;
        std::string author;
        int bookLevel = 0;
        std::string publisher;
        std::string pubStartDate;
        std::string pubEndDate;

        bookNameFilter = ui->queryBookByName->text();
        bookName = bookNameFilter.toStdString();
        authorFilter = ui->queryBookByAuthor->text();
        author = authorFilter.toStdString();
        bookLevelFilter = ui->queryBookByLevel->currentIndex();
        if(bookLevelFilter == 0){ // 不筛选
            bookLevel = 0;
        }else if(bookLevelFilter == 1){ // 普通图书
            bookLevel = 2;
        }else if(bookLevelFilter == 2){ // 珍贵图书
            bookLevel = 1;
        }else{
            QMessageBox::critical(ui->queryBookByLevel, QString("不合法图书类型"),  QString("因程序代码错误或系统字符集编码不一致导致用户输入不合法"));
            return;
        }
        publisherFilter = ui->queryBookByPublisher->text();
        publisher = publisherFilter.toStdString();
        pubStartDateFilter = ui->queryBookStartDate->date().toString("yyyy-MM-dd");
        pubStartDate = pubStartDateFilter.toStdString();
        pubEndDateFilter = ui->queryBookEndDate->date().toString("yyyy-MM-dd");
        pubEndDate = pubEndDateFilter.toStdString();
        // 查询
        std::string msg;
        numRows = BookService::getAllBook(msg, bookName, author, "", pubStartDate, pubEndDate, publisher, bookLevel, booksInfo);
        if(numRows == 0){
            QMessageBox::warning(nullptr, QString("查询到0条记录"), QString::fromStdString(msg));
        }
    }else{ // 通过ISBN单个条件精确查询
        QRegularExpression isbnRegex("^[0-9]{13}$"); // ISBN，13位纯数字
        if(isbnRegex.match(isbnFilter).hasMatch()){ // 合法ID
            std::string msg;
            if(BookService::getBookInfoByISBN(msg, isbnFilter.toStdString(), bookInfo)){
                if(bookInfo.id != "" && !bookInfo.id.empty()){
                    booksInfo.push_back(bookInfo);
                    numRows = 1; // 不为空才push，避免push空结构体
                }else{
                    numRows = 0;
                }
            }else{
                QMessageBox::warning(nullptr, QString("查询失败"), QString::fromStdString(msg));
                return;
            }
        }else{
            QMessageBox::warning(ui->queryBookByISBN, QString("不合法输入"), QString("ISBN只允许13位长度纯数字，你可以选择置空ISBN筛选器"));
            return;
        }
    }
    // 将查询到的数据填充到表格中显示
    ui->tableWidTabEditBook->setRowCount(numRows);
    ui->tableWidTabEditBook->setColumnCount(11); // 11 个图书信息列
    QStringList headers = {"图书ID","ISBN","图书名称","作者","版次","图书等级","出版日期","总册数","库存余量","定价","出版社"};
    ui->tableWidTabEditBook->setHorizontalHeaderLabels(headers);

    for (size_t i = 0; i < numRows; ++i) {
        const BookInfo& info = booksInfo[i];
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(info.id));
        ui->tableWidTabEditBook->setItem(i, 0, idItem); // 设置 "bookID" 列
        QTableWidgetItem* isbnItem = new QTableWidgetItem(QString::fromStdString(info.ISBN));
        ui->tableWidTabEditBook->setItem(i, 1, isbnItem);  // 设置 "ISBN" 列
        QTableWidgetItem* bookNameItem = new QTableWidgetItem(QString::fromStdString(info.bookName));
        ui->tableWidTabEditBook->setItem(i, 2, bookNameItem); // 设置 "bookName" 列
        QTableWidgetItem* authorItem = new QTableWidgetItem(QString::fromStdString(info.author));
        ui->tableWidTabEditBook->setItem(i, 3, authorItem); // 设置 "author" 列
        QTableWidgetItem* editionItem = new QTableWidgetItem(QString::fromStdString(info.edition));
        ui->tableWidTabEditBook->setItem(i, 4, editionItem); // 设置 "edition" 列
        QTableWidgetItem* levelItem = new QTableWidgetItem(QString::fromStdString(info.bookLevel == 1 ? "珍贵图书" : "普通图书"));
        ui->tableWidTabEditBook->setItem(i, 5, levelItem); // 设置 "level" 列
        QTableWidgetItem* pubDateItem = new QTableWidgetItem(QString::fromStdString(info.pubDate));
        ui->tableWidTabEditBook->setItem(i, 6, pubDateItem); // 设置 "pubDate" 列
        QTableWidgetItem* totalQtyItem = new QTableWidgetItem(QString::number(info.totalQty));
        ui->tableWidTabEditBook->setItem(i, 7, totalQtyItem); // 设置 "total" 列
        QTableWidgetItem* remainItem = new QTableWidgetItem(QString::number(info.stockRemain));
        ui->tableWidTabEditBook->setItem(i, 8, remainItem); // 设置 "remain" 列
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(info.price, 'f', 2));
        ui->tableWidTabEditBook->setItem(i, 9, priceItem); // 设置 "price" 列
        QTableWidgetItem* publisherItem = new QTableWidgetItem(QString::fromStdString(info.publisher));
        ui->tableWidTabEditBook->setItem(i, 10, publisherItem); // 设置 "publisher" 列
    }

    // 4、将列0、列1、列7、列8设置为不可编辑
    setColumnNonEditable(ui->tableWidTabEditBook, 0); // bookId列
    setColumnNonEditable(ui->tableWidTabEditBook, 1); // ISBN列
    setColumnNonEditable(ui->tableWidTabEditBook, 7); // 总数列
    setColumnNonEditable(ui->tableWidTabEditBook, 8); // 可借余量列
}
void BookSelectForm::on_tableEditBookTableItemClicked(QTableWidgetItem *item){ // 当用户点击Book表格中的项时会触发该信号，此槽函数用于处理信号
    if (item) {
        row_tableW = item->row();
        column_tableW = item->column();
    }
}
