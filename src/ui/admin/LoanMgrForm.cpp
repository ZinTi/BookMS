#include "LoanMgrForm.h"
#include "ui_LoanMgrForm.h"

LoanMgrForm::LoanMgrForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoanMgrForm)
{
    ui->setupUi(this);
}

LoanMgrForm::~LoanMgrForm()
{
    delete ui;
}
