#ifndef LOANADDFORM_H
#define LOANADDFORM_H

#include <QWidget>

namespace Ui {
class LoanAddForm;
}

class LoanAddForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoanAddForm(QWidget *parent = nullptr);
    ~LoanAddForm();

private slots:
    void on_pBtnClearBook_clicked();
    void on_pBtnQueryBook_clicked();
    void on_pBtnClearUser_clicked();
    void on_pBtnQueryUser_clicked();
    void on_pBtnClearLoan_clicked();
    void on_pBtnHandleLoan_clicked();

private:
    Ui::LoanAddForm *ui;
};

#endif // LOANADDFORM_H
