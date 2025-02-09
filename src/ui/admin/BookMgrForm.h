#ifndef BOOKMGRFORM_H
#define BOOKMGRFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include "BookSelectForm.h"
#include "BookEditPanelOfAdmin.h"

class BookMgrForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookMgrForm(QWidget *parent = nullptr);
    ~BookMgrForm();

private slots:
    void handle_BookEditPanelOfReader_BtnClick(int index);

    // 旧版本
    void deleteBookRecord(QString bookId);// 删除图书
    void updateBookInfo(QString bookId);// 更新图书

private:
    QVBoxLayout* m_vLayoutMain; // 主要布局
    QLabel* m_title; // 页面标题
    BookSelectForm* m_bookSelectForm; // 查书控件
    BookEditPanelOfAdmin* m_bookEditPanelOfAdmin; // 图书操作面板

    int row_tableW; // 当前行
    int column_tableW; // 当前列
};

#endif // BOOKMGRFORM_H
