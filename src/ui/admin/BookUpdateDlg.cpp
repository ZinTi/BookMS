#include "BookUpdateDlg.h"
#include "ui_BookUpdateDlg.h"
#include "BookService.h"
#include <QMessageBox>

BookUpdateDlg::BookUpdateDlg(const QString& bookId, QWidget *parent)
    : m_bookId(bookId)
    , QDialog(parent)
    , ui(new Ui::BookUpdateDlg)
{
    ui->setupUi(this);

    std::string msgFindBook;
    if(false == BookService::getBookInfoByBookId(msgFindBook, this->m_bookId.toStdString(), m_bookInfo)){
        QMessageBox::warning(this, QString("无效BookId"), QString("根据当前所选的bookId找不到对应图书信息：") + this->m_bookId);
        return;
    }
    this->m_validFlag = true; // 标记为可执行

    setWindowTitle(QString("变更图书信息"));

    ui->editBookId->setReadOnly(true);
    ui->editBookPubDate->setDisplayFormat(QString("yyyy-MM-dd"));
    ui->editBookPubDate->setCalendarPopup(true);

    ui->editBookId->setText(this->m_bookId);

    setValueToEditer();
}

BookUpdateDlg::~BookUpdateDlg(){
    delete ui;
}

int BookUpdateDlg::exec(){
    if(m_validFlag){
        return QDialog::exec(); // 调用系统 exec()
    }else{
        reject(); // 或 close()，根据需求
        return QDialog::Rejected; // 返回结果（结束）
    }
}

void BookUpdateDlg::show(){
    if(m_validFlag){
        QDialog::show(); // 调用系统 show()
    }else{
        close();
    }
}

void BookUpdateDlg::setValueToEditer(){
    ui->editBookISBN->setText(QString::fromStdString(m_bookInfo.ISBN));
    ui->editBookName->setText(QString::fromStdString(m_bookInfo.bookName));
    ui->editBookAuthor->setText(QString::fromStdString(m_bookInfo.author));
    ui->editBookEdition->setText(QString::fromStdString(m_bookInfo.edition));

    QDate pubDate = QDate::fromString(QString::fromStdString(m_bookInfo.pubDate), QString("yyyy-MM-dd"));
    if(pubDate.isValid()){
        ui->editBookPubDate->setDate(pubDate);
    }else{
        // 转换失败，先清除之前可能存储的有效值
        ui->editBookPubDate->clear();
        ui->editBookPubDate->setDateTime(QDateTime());
        // 设置特殊值文本
        ui->editBookPubDate->setSpecialValueText("NULL");
        // 让控件失去焦点，确保特殊值文本能正常显示
        ui->editBookPubDate->clearFocus();
    }

    ui->editBookTotal->setValue(m_bookInfo.totalQty);
    ui->editBookRemain->setValue(m_bookInfo.stockRemain);
    ui->editBookPrice->setValue(m_bookInfo.price);
    ui->editBookPublisher->setText(QString::fromStdString(m_bookInfo.publisher));
    ui->editBookLevel->setCurrentText(QString::fromStdString(m_bookInfo.bookLevel == 1 ? "珍贵图书" : "普通图书"));
}

void BookUpdateDlg::on_pBtnReset_clicked(){
    setValueToEditer();
}

void BookUpdateDlg::on_pBtnSubmit_clicked(){// 执行更新
    // 1、获取目标行的每一列数据
    BookInfo newInfo;

    // {"图书ID","ISBN","图书名称","作者","版次","图书等级","出版日期","总册数","库存余量","定价","出版社"};
    QRegularExpression bookIdRegex("^[a-z0-9]{8}$"); // 长度为8位、仅小写字母和数字
    QRegularExpression isbnRegex("^[0-9]{13}$"); // 长度为13位，纯数字字符
    QRegularExpression bookNameRegex("^.{1,50}$"); // 不为空，50字以内
    QRegularExpression authorRegex("^.{1,50}$"); // 不为空，50字以内
    QRegularExpression editionRegex("^.{1,20}$"); // 不为空，20字以内
    QRegularExpression publisherRegex("^.{1,50}$"); // 不为空，50字以内
    QRegularExpression pubDateRegex("^(\\d{4})-(\\d{2})-(\\d{2})$"); // "yyyy-MM-dd" 格式的正则表达式

    QString inBookId = ui->editBookId->text();
    QString inISBN = ui->editBookISBN->text(); // 长度为13的纯数字
    QString inBookName = ui->editBookName->text(); // 50字以内
    QString inAuthor = ui->editBookAuthor->text(); // 50字以内
    QString inEdition = ui->editBookEdition->text(); // 20字以内
    QString inPubDate = ui->editBookPubDate->text(); // "yyyy-MM-dd"格式
    int inTotal = ui->editBookTotal->text().toInt();
    int inRemain = ui->editBookRemain->text().toInt();
    double inPrice = ui->editBookPrice->text().toDouble();
    QString inPublisher = ui->editBookPublisher->text(); // 不为空，50字以内
    QString inBookLevel = ui->editBookLevel->currentText();

    if (bookIdRegex.match(inBookId).hasMatch()) {
        newInfo.id = inBookId.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("图书ID只允许8位长度，由数字和小写字母组成"));
        return;
    }
    if (isbnRegex.match(inISBN).hasMatch()) {
        newInfo.ISBN = inISBN.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("图书ISBN只允许13位长度，由纯数字组成"));
        return;
    }
    if (bookNameRegex.match(inBookName).hasMatch()) {
        newInfo.bookName = inBookName.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("图书名称长度应在50字以内且不为空"));
        return;
    }
    if (authorRegex.match(inAuthor).hasMatch()) {
        newInfo.author = inAuthor.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("作者名称长度应在50字以内且不为空"));
        return;
    }
    if (editionRegex.match(inEdition).hasMatch()) {
        newInfo.edition = inEdition.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("版次长度应在20字以内且不为空"));
        return;
    }
    if (inBookLevel == "珍贵图书" || inBookLevel == "珍贵" || inBookLevel == "1") {
        newInfo.bookLevel = 1;
    }else if(inBookLevel == "普通图书" || inBookLevel == "普通" || inBookLevel == "2"){
        newInfo.bookLevel = 2;
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("图书等级只能是\"珍贵图书\"/\"普通图书\" 、\"珍贵\"/\"普通\"或者 \"1\"/\"2\""));
        return;
    }
    if (pubDateRegex.match(inPubDate).hasMatch()) {
        newInfo.pubDate = inPubDate.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("出版日期格式应为 yyyy-MM-dd"));
        return;
    }
    if (publisherRegex.match(inPublisher).hasMatch()) {
        newInfo.publisher = inPublisher.toStdString();
    } else {
        QMessageBox::warning(nullptr, QString("不合法修改"), QString("出版社名称长度应在50字以内且不为空"));
        return;
    }
    newInfo.totalQty = inTotal;
    newInfo.stockRemain = inRemain;
    newInfo.price = inPrice;

    // 2、执行修改并弹窗显示修改结果
    std::string msg;
    bool result = BookService::modifyBookInfo(msg, newInfo);
    if(result){
        QMessageBox::information(nullptr, QString::fromStdString("更新图书" + newInfo.id + "信息成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::warning(nullptr, QString::fromStdString("更新图书" + newInfo.id + "信息失败"), QString::fromStdString(msg));
    }
}

