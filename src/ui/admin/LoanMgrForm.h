#ifndef LOANMGRFORM_H
#define LOANMGRFORM_H

#include <QWidget>

namespace Ui {
class LoanMgrForm;
}

class LoanMgrForm : public QWidget
{
    Q_OBJECT

private slots:
    // -- 显示面板
    // 变更表格坐标

    // -- 查询面板
    // 清空筛选条件
    // 执行查询

    // -- 数据操作面板
    // 删除借阅记录
    // 管理员手动还书

public:
    explicit LoanMgrForm(QWidget *parent = nullptr);
    ~LoanMgrForm();

private:
    Ui::LoanMgrForm *ui;
};

#endif // LOANMGRFORM_H
