#ifndef USERDATAFORM_H
#define USERDATAFORM_H

#include <QWidget>

namespace Ui {
class UserDataForm;
}

class UserDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserDataForm(QWidget *parent = nullptr);
    ~UserDataForm();

private:
    Ui::UserDataForm *ui;
};

#endif // USERDATAFORM_H
