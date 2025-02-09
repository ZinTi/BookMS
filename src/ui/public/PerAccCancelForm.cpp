#include "PerAccCancelForm.h"
#include "ui_PerAccCancelForm.h"
#include "UserService.h"
#include "AuthDlg.h"
#include "entity.h"

extern UserInfo* currentUserInfo;

PerAccCancelForm::PerAccCancelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PerAccCancelForm)
{
    ui->setupUi(this);
}

PerAccCancelForm::~PerAccCancelForm()
{
    delete ui;
}

void PerAccCancelForm::on_pBtnCancelAccountDeletePerAcc_clicked(){
    AuthDlg authDlg;
    if(authDlg.exec() == QDialog::Accepted){
        std::string msg;
        UserService::setUserType(msg, currentUserInfo->id, "DS");
    }
}
