#ifndef BOOKUPDATEDLG_H
#define BOOKUPDATEDLG_H

#include <QDialog>
#include "entity.h"

namespace Ui {
class BookUpdateDlg;
}

class BookUpdateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BookUpdateDlg(const QString& bookId, QWidget *parent = nullptr);
    ~BookUpdateDlg();
    int exec() override;
    void show(); // 同名函数覆盖

private slots:
    void on_pBtnReset_clicked();
    void on_pBtnSubmit_clicked();

private:
    bool m_validFlag = false; // 如果是true可以执行，否则不能执行
    Ui::BookUpdateDlg *ui;
    QString m_bookId;
    BookInfo m_bookInfo;

    void setValueToEditer();
};

#endif // BOOKUPDATEDLG_H
