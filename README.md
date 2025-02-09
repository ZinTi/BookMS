# BookMS 图书借阅管理系统

## 一、项目简介

此项目是我 `2024-2025学年第一学期` 期末（2024年12月）的C++程序设计实训作业，一个简单的CRUD管理系统。仅供参考，无实用价值。

如果有人看到了这个程序，想要拿来应付下作业，需要提醒下，这个程序不仅没写完，还有很多Bug，很多设计相当不合理！你最好自己完整看一下，我懒得整理了。

C++实训指导书给了11个选题，自由选择其中之一（也可以自由发挥），我选择了下面这个题目：
```
题目三：图书借还管理系统
功能要求：
    图书分类两类：普通图书和珍贵图书，不同类型的图书有不同的借书和还书标准，借阅人分类教师和学生，类型不同在图书的借阅权限上不同。设计相应的类，完成相应功能。
    1）从屏幕输入一组借书信息
    2）将借书信息存入文件
    3）从文件读入借书信息
    4）增加一次借书信息
    5）还一本书
    6）根据输入条件查询借书信息
    7）根据条件对借书信息排序
    8）修改指定条件的借书信息。
技术要求：
    1）要有角色登录界面认证。
    2）要有主界面，每个主题模块数据尽量有统计分析。
    3）知识运用要科学合理，知识运用不限于教材所学。
```

原来是使用CSV格式的文本存储的，后来改成了`SQLite3`。并且我自己封装了一套小的彩色TUI控件库，写着写着发现调整设计TUI界面进度有点慢，然后就改用Win32那套API，写起来不悦，又改用`Qt`重写了（也就是现在能看到的这些文件），于是没写完。

因为没有提前画流程图、数据图之类的，数据库结构设计也没整好，想到什么写什么，导致写到一半了才发现设计相当不合理，但是又懒得改，只能将就这样了。

而且本来还要写相应的“C++程序设计实训报告”的，因为答应了还要帮同学写他们的程序，导致我自己的实训报告论文一个字没动，只能拿着我以前学MS SQL server数据库时写的项目实践论文交上去。

## 二、使用与编译

### 1. 使用提示
```
登录系统管理员root用户
账号：root
密码：666666
```

### 2. 编译环境

- 第三方库： `Qt6`、`SQLite3`、 `OpenSSL`
- 编译器：我用的 `MinGW-w64 g++`，你也可以改成 `MSVC` 或者 `Clang`
- 构建工具：`CMake`

### 3. 项目结构
编译后打包二进制文件，目录结构如下：
```
release/
│  CMakeLists.txt
│  README.md
│
├─bin
│      BookMS.exe   # 程序可执行文件
│
└─data
        bookms.db   # 数据库文件
```
源代码目录结构：
```
sources/
│  CMakeLists.txt
│  README.md
│
├─res
│      book.ico
│      book.png
│      icon_logo.rc
│      logo.ico
│      logo.png
│      resources.qrc
│
└─src
    │  main.cpp
    │
    ├─dao
    │      BookDAO.cpp
    │      BookDAO.h
    │      LoanDAO.cpp
    │      LoanDAO.h
    │      UserDAO.cpp
    │      UserDAO.h
    │
    ├─model
    │      Book.cpp
    │      Book.h
    │      entity.h
    │      User.cpp
    │      User.h
    │
    ├─service
    │      BookService.cpp
    │      BookService.h
    │      datetime.cpp
    │      datetime.h
    │      Environ.cpp
    │      Environ.h
    │      LoanService.cpp
    │      LoanService.h
    │      PwdUtils.cpp
    │      PwdUtils.h
    │      UserService.cpp
    │      UserService.h
    │
    └─ui
        │  AuthDlg.h
        │  WidgetPublicMethod.cpp
        │  WidgetPublicMethod.h
        │
        ├─admin
        │      AdminDashboard.cpp
        │      AdminDashboard.h
        │      AdminDashboard.ui
        │      BookAddForm.cpp
        │      BookAddForm.h
        │      BookAddForm.ui
        │      BookDataForm.cpp
        │      BookDataForm.h
        │      BookDataForm.ui
        │      BookEditPanelOfAdmin.cpp
        │      BookEditPanelOfAdmin.h
        │      BookMgrForm.cpp
        │      BookMgrForm.h
        │      BookUpdateDlg.cpp
        │      BookUpdateDlg.h
        │      BookUpdateDlg.ui
        │      LoanAddForm.cpp
        │      LoanAddForm.h
        │      LoanAddForm.ui
        │      LoanMgrForm.cpp
        │      LoanMgrForm.h
        │      LoanMgrForm.ui
        │      MenuFormAdmin.cpp
        │      MenuFormAdmin.h
        │      MenuFormAdmin.ui
        │      UserAddForm.cpp
        │      UserAddForm.h
        │      UserAddForm.ui
        │      UserDataForm.cpp
        │      UserDataForm.h
        │      UserDataForm.ui
        │      UserMgrForm.cpp
        │      UserMgrForm.h
        │      UserMgrForm.ui
        │      UserPermissionForm.cpp
        │      UserPermissionForm.h
        │      UserPermissionForm.ui
        │
        ├─public
        │      BookSelectForm.cpp
        │      BookSelectForm.h
        │      BookSelectForm.ui
        │      LoginDlg.cpp
        │      LoginDlg.h
        │      LoginDlg.ui
        │      PerAccCancelForm.cpp
        │      PerAccCancelForm.h
        │      PerAccCancelForm.ui
        │      PerAccEditForm.cpp
        │      PerAccEditForm.h
        │      PerAccEditForm.ui
        │      SignupDlg.cpp
        │      SignupDlg.h
        │      SignupDlg.ui
        │
        └─reader
                BorrowPanelOfReader.cpp
                BorrowPanelOfReader.h
                MenuFormReader.cpp
                MenuFormReader.h
                MenuFormReader.ui
                ReaderHomepage.cpp
                ReaderHomepage.h
                ReaderHomepage.ui
```

## 三、开源协议
使用GNU GPL 3.0协议。

