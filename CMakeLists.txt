cmake_minimum_required(VERSION 3.16)

project(BookMS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置 OpenSSL 路径
set(OPENSSL_ROOT_DIR "A:/app/OpenSSL-Win64-MinGW")

# 查找 OpenSSL
find_package(OpenSSL REQUIRED)

# 如果找不到 OpenSSL，手动指定
if (NOT OpenSSL_FOUND)
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libcrypto.a")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libssl.a")
    set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
endif()

# 包含头文件目录
include_directories(
        ${OPENSSL_INCLUDE_DIR} # 添加 OpenSSL 头文件
        ${CMAKE_SOURCE_DIR}/lib
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/dao
        ${CMAKE_SOURCE_DIR}/src/service
        ${CMAKE_SOURCE_DIR}/src/ui
        ${CMAKE_SOURCE_DIR}/src/ui/public
        ${CMAKE_SOURCE_DIR}/src/ui/admin
        ${CMAKE_SOURCE_DIR}/src/ui/reader
)

set(PROJECT_SOURCES
        # (1)主函数
        src/main.cpp

        # (2)MODEL层
        src/model/User.cpp
        src/model/Book.cpp
        # (3)DAO层
        src/dao/sqlite3.o
        src/dao/UserDAO.cpp
        src/dao/BookDAO.cpp
        src/dao/LoanDAO.cpp
        # (4)Service层
        src/service/datetime.cpp
        src/service/Environ.cpp
        src/service/PwdUtils.cpp
        src/service/UserService.cpp
        src/service/BookService.cpp
        src/service/LoanService.cpp

        # (5)UI层
        # (5.0) 公用函数
        src/ui/WidgetPublicMethod.cpp
        src/ui/WidgetPublicMethod.h

        # （5.1）public部分：登录界面、自助注册界面
        src/ui/public/LoginDlg.cpp
        src/ui/public/LoginDlg.ui
        src/ui/public/SignupDlg.cpp
        src/ui/public/SignupDlg.ui
        # 个人管理模块
        src/ui/public/PerAccEditForm.h
        src/ui/public/PerAccEditForm.cpp
        src/ui/public/PerAccEditForm.ui
        src/ui/public/PerAccCancelForm.h
        src/ui/public/PerAccCancelForm.cpp
        src/ui/public/PerAccCancelForm.ui
        # 图书通用查询选择模块
        src/ui/public/BookSelectForm.h
        src/ui/public/BookSelectForm.cpp
        src/ui/public/BookSelectForm.ui

        # （5.2）管理员端（管理员工作台）
        src/ui/admin/AdminDashboard.h # 管理员主页
        src/ui/admin/AdminDashboard.cpp
        src/ui/admin/AdminDashboard.ui
        src/ui/admin/MenuFormAdmin.h # 管理员菜单
        src/ui/admin/MenuFormAdmin.cpp
        src/ui/admin/MenuFormAdmin.ui
        # 用户管理模块：新增用户页面\管理用户页面\修改用户权限页面
        src/ui/admin/UserAddForm.h
        src/ui/admin/UserAddForm.cpp
        src/ui/admin/UserAddForm.ui
        src/ui/admin/UserMgrForm.h
        src/ui/admin/UserMgrForm.cpp
        src/ui/admin/UserMgrForm.ui
        src/ui/admin/UserPermissionForm.h
        src/ui/admin/UserPermissionForm.cpp
        src/ui/admin/UserPermissionForm.ui
        # 图书管理模块：新增图书页面\管理图书页面
        src/ui/admin/BookAddForm.h
        src/ui/admin/BookAddForm.cpp
        src/ui/admin/BookAddForm.ui
        src/ui/admin/BookMgrForm.h
        src/ui/admin/BookMgrForm.cpp
        src/ui/admin/BookEditPanelOfAdmin.h # 图书编辑面板
        src/ui/admin/BookEditPanelOfAdmin.cpp
        # 借阅事务模块：
        src/ui/admin/LoanAddForm.h
        src/ui/admin/LoanAddForm.cpp
        src/ui/admin/LoanAddForm.ui
        src/ui/admin/LoanMgrForm.h
        src/ui/admin/LoanMgrForm.cpp
        src/ui/admin/LoanMgrForm.ui
        # 数据分析模块：
        src/ui/admin/UserDataForm.h
        src/ui/admin/UserDataForm.cpp
        src/ui/admin/UserDataForm.ui
        src/ui/admin/BookDataForm.h
        src/ui/admin/BookDataForm.cpp
        src/ui/admin/BookDataForm.ui

        # （5.3）读者端（读者自助服务台）
        src/ui/reader/ReaderHomepage.h # 读者端主页
        src/ui/reader/ReaderHomepage.cpp
        src/ui/reader/ReaderHomepage.ui
        src/ui/reader/MenuFormReader.h # 读者菜单
        src/ui/reader/MenuFormReader.cpp
        src/ui/reader/MenuFormReader.ui
        src/ui/reader/BorrowPanelOfReader.h # 借书工具面板
        src/ui/reader/BorrowPanelOfReader.cpp

        # (7)资源文件
        res/icon_logo.rc
        res/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BookMS
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/ui/admin/BookUpdateDlg.h src/ui/admin/BookUpdateDlg.cpp src/ui/admin/BookUpdateDlg.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BookMS APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BookMS SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BookMS
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接 Qt6 和 OpenSSL、Windows 系统库
target_link_libraries(BookMS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OPENSSL_LIBRARIES} ws2_32 user32 advapi32 crypt32)

# 设置可执行文件的输出路径
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) # 如果目录不存在则创建
set_target_properties(BookMS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BookMS)
endif()
set_target_properties(BookMS PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BookMS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BookMS)
endif()
