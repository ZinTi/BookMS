#ifndef BORROWPANELOFREADER_H
#define BORROWPANELOFREADER_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class BorrowPanelOfReader : public QWidget
{
    Q_OBJECT

public:
    explicit BorrowPanelOfReader(QWidget *parent = nullptr);
    ~BorrowPanelOfReader();

    void setMsgDisplayInformation(const QString& information);

signals:
    // 自定义信号，用于通知主窗口或查看器显示内容
    void pBtnBorrowClicked(int index);

private slots:
    void on_pBtnBorrow_clicked(); // 借书
    void on_pBtnClear_clicked(); // 清空显示器

private:
    QTextEdit* m_msgDisplay;
    QHBoxLayout* m_hLayoutMain;
    QVBoxLayout* m_vLayoutPushBtn;
    QPushButton* m_pBtnBorrow;
    QPushButton* m_pBtnClear;
};

#endif // BORROWPANELOFREADER_H
