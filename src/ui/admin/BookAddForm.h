#ifndef BOOKADDFORM_H
#define BOOKADDFORM_H

#include <QWidget>

namespace Ui {
class BookAddForm;
}

class BookAddForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookAddForm(QWidget *parent = nullptr);
    ~BookAddForm();

private slots:
    void on_pBtnAddBookClear_clicked(); // 清空TabAddUser页面的输入框
    void on_pBtnAddBookSubmit_clicked(); // 提交TabAddUser页面的图书输入数据

private:
    Ui::BookAddForm *ui;
};

#endif // BOOKADDFORM_H
