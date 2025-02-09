#ifndef BOOKSELECTFORM_H
#define BOOKSELECTFORM_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class BookSelectForm;
}

class BookSelectForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookSelectForm(QWidget *parent = nullptr);
    ~BookSelectForm();

    void getPosition(int* x, int* y);
    bool getCurrentSelectedBookId(QString& bookId);
    bool getCurrentSelectedBookName(QString& bookName);

    void deleteFromTableWidget(QString bookId);

private slots:
    void on_pBtnEditBookClear_clicked();// 清空按钮
    void on_pBtnEditBookQuery_clicked();// 查询按钮
    void on_tableEditBookTableItemClicked(QTableWidgetItem *item); // 当用户点击BookTable表格中的项时会触发该信号，此槽函数用于处理信号

private:
    Ui::BookSelectForm *ui;

    int row_tableW = 0; // 当前行
    int column_tableW = 0; // 当前列
};

#endif // BOOKSELECTFORM_H
