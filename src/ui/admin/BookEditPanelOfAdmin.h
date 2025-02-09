#ifndef BOOKEDITPANELOFADMIN_H
#define BOOKEDITPANELOFADMIN_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>


class BookEditPanelOfAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit BookEditPanelOfAdmin(QWidget *parent = nullptr);
    ~BookEditPanelOfAdmin();

    void setMsgDisplayInformation(const QString& information);

signals:
    // 自定义信号，用于通知主窗口或查看器显示内容
    void pBtnBorrowClicked(int index);

private slots:
    void on_pBtnEditBookClear_clicked();
    void on_pBtnEditBookDelete_clicked();
    void on_pBtnEditBookUpdate_clicked();

private:
    QTextEdit* m_msgDisplay;
    QHBoxLayout* m_hLayoutMain;
    QVBoxLayout* m_vLayoutPushBtn;
    QPushButton* m_pBtnEditBookClear;
    QPushButton* m_pBtnEditBookDelete;
    QPushButton* m_pBtnEditBookUpdate;
};

#endif

