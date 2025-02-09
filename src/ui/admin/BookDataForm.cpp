#include "BookDataForm.h"
#include "ui_BookDataForm.h"

BookDataForm::BookDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookDataForm)
{
    ui->setupUi(this);
}

BookDataForm::~BookDataForm()
{
    delete ui;
}
