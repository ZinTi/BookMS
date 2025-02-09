#include "LoginDlg.h"
#include "AdminDashboard.h"
#include "ReaderHomepage.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include "Environ.h"
#include "entity.h"

Environ* BookMSEnv;
UserInfo* currentUserInfo;

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    BookMSEnv = new Environ(); // 初始化运行环境
    currentUserInfo = new UserInfo();

    LoginDlg loginDlg(nullptr); // 实例化登录窗口

    if (loginDlg.exec() == QDialog::Accepted) {
        // 登录成功执行其他处理
        if (currentUserInfo->userType == "SA" || currentUserInfo->userType == "UA" || currentUserInfo->userType ==
            "LA") {
            // 管理员工作台
            AdminDashboard adminDashboard;
            adminDashboard.show();
            return a.exec();
        }
        else {
            // 读者自助服务台
            ReaderHomepage readerHomepage;
            readerHomepage.show();
            return a.exec();
        }
    }
    else {
        return 0;
    }
}
