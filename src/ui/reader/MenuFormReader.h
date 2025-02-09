#ifndef MENUFORMREADER_H
#define MENUFORMREADER_H

#include <QWidget>

namespace Ui {
class MenuFormReader;
}

class MenuFormReader : public QWidget
{
    Q_OBJECT

public:
    explicit MenuFormReader(QWidget *parent = nullptr);
    ~MenuFormReader();

signals:
    // 自定义信号，携带按钮索引，用于通知主窗口或查看器显示内容
    void buttonClicked(int index);

private slots:
    // 1\ 自助借还
    void on_pBtnBookBorrow_clicked();    // 借阅图书 1-pBtnBookBorrow
    void on_pBtnBookReturn_clicked();    // 归还图书 2-pBtnBookReturn
    // 2\ 个人资料
    void on_pBtnEditPersonalInfo_clicked();    // 编辑资料 3-pBtnEditPersonalInfo
    void on_pBtnCancelAccount_clicked();    // 注销账号 4-pBtnCancelAccount
    void on_pBtnLogout_clicked();    // 退出登录 5-pBtnLogout

private:
    Ui::MenuFormReader *ui;
};

#endif // MENUFORMREADER_H
