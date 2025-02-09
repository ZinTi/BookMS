#ifndef PERACCCANCELFORM_H
#define PERACCCANCELFORM_H

#include <QWidget>

namespace Ui {
class PerAccCancelForm;
}

class PerAccCancelForm : public QWidget
{
    Q_OBJECT

public:
    explicit PerAccCancelForm(QWidget *parent = nullptr);
    ~PerAccCancelForm();

private slots:
    void on_pBtnCancelAccountDeletePerAcc_clicked(); // 注销账号

private:
    Ui::PerAccCancelForm *ui;
};

#endif // PERACCCANCELFORM_H
