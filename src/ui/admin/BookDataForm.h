#ifndef BOOKDATAFORM_H
#define BOOKDATAFORM_H

#include <QWidget>

namespace Ui {
class BookDataForm;
}

class BookDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookDataForm(QWidget *parent = nullptr);
    ~BookDataForm();

private:
    Ui::BookDataForm *ui;
};

#endif // BOOKDATAFORM_H
