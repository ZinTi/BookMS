#include "UserDataForm.h"
#include "ui_UserDataForm.h"

UserDataForm::UserDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDataForm)
{
    ui->setupUi(this);
}

UserDataForm::~UserDataForm()
{
    delete ui;
}
