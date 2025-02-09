#ifndef PERACCEDITFORM_H
#define PERACCEDITFORM_H

#include <QWidget>

namespace Ui {
class PerAccEditForm;
}

class PerAccEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit PerAccEditForm(QWidget *parent = nullptr);
    ~PerAccEditForm();

private slots:
    void on_refreshToolButton_clicked(); // 处理refreshToolButton（刷新个人信息）点击事件的槽函数
    void on_updateToolButton_clicked(); // 处理updateToolButton（更新个人信息）点击事件的槽函数

private:
    Ui::PerAccEditForm *ui;

    void loadPersonalInfo(); // 加载个人信息
};

#endif // PERACCEDITFORM_H
