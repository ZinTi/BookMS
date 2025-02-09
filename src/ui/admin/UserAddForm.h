#ifndef USERADDFORM_H
#define USERADDFORM_H

#include <QWidget>

namespace Ui {
class UserAddForm;
}

class UserAddForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserAddForm(QWidget *parent = nullptr);
    ~UserAddForm();

private slots:
    void on_pBtnTabAddUserSubmitRegistration_clicked(); // 处理用户管理-创建用户-确定注册按钮pBtnTabAddUserSubmitRegistration的点击事件

private:
    Ui::UserAddForm *ui;
};

#endif // USERADDFORM_H
