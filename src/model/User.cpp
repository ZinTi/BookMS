#include "User.h"
#include <iostream>
#include "UserService.h"

namespace MODEL {
    /***********************************************(1/7 User类的实现)**********************************************************/
    User::User(): info() {}
    User::User(const std::string& id, const std::string& password) {
        info.id = id;
        info.password = password;
    }
    User::User( const std::string& id,
                const std::string& username,
                const std::string& password,
                const std::string& salt,
                const std::string& email,
                const std::string& telephone,
                const std::string& address,
                const bool gender,
                const std::string& userType ) {
        info.id = id;
        info.username = username;
        info.password = password;
        info.salt = salt;
        info.email = email;
        info.telephone = telephone;
        info.address = address;
        info.gender = gender;
        info.userType = userType;
    }
    User::~User() = default;

    void User::displayInfo() const {
        std::cout << "+-----------| User |-----------+" << std::endl;
        std::cout << "| 账号: " << info.id << std::endl;
        std::cout << "| 姓名: " << info.username << std::endl;
        std::cout << "| 密码: " << info.password << std::endl;
        std::cout << "| 盐值: " << info.salt << std::endl;
        std::cout << "| 邮箱: " << info.email << std::endl;
        std::cout << "| 电话: " << info.telephone << std::endl;
        std::cout << "| 地址: " << info.address << std::endl;
        std::cout << "| 性别: " << (info.gender ? "男" : "女") << std::endl;
        std::cout << "| 身份: " << info.userType << std::endl;
        std::cout << "+-------------------------------+" << std::endl;
    }

    bool User::fetchPersonalInfo(std::string& msg) {
        if (UserService userService; userService.getUserInfoByUserId(msg, info.id, info)) {
            return true;
        }
        return false;
    }

    bool User::changePersonalPassword(std::string& msg, const std::string& oldPassword,
                                      const std::string& newPassword) const {
        if (UserService userService; !userService.changePassword(msg, info.id, oldPassword, newPassword)) {
            return true;
        }
        return false;
    }

    bool User::editPersonalInfo(std::string& msg) {
        if (UserService userService; userService.updateUser(msg, info.id, info.username, info.email, info.telephone,
                                                            info.address, info.gender)) {
            std::string tmpMsg;
            userService.getUserInfoByUserId(tmpMsg, info.id, info); // 刷新信息
            return true;
        }
        return false;
    }

    bool User::invalidatePersonalAccount(std::string& msg) const {
        UserService userService;
        return userService.setUserType(msg, this->info.id);
    }

    std::vector<std::string> User::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号");
        return result;
    }

    /***********************************************(2/7 UserSA类的实现)**********************************************************/
    UserSA::UserSA(const UserInfo& userInfo) {
        info = userInfo;
    }

    bool UserSA::modifyUserInfoById(std::string& msg, const std::string& userId, const UserInfo& userInfo) {
        const UserService userService;
        const bool result = userService.updateUser(
            msg,
            userId,
            userInfo.username,
            userInfo.email,
            userInfo.telephone,
            userInfo.address,
            userInfo.gender
        );

        return result;
    }

    bool UserSA::resetUserPassword(std::string& msg, const std::string& userId, const std::string& newPassword) {
        UserService userService;
        return userService.resetPassword(msg, userId, newPassword);
    }

    bool UserSA::modifyUserType(std::string& msg, const std::string& userId, const std::string& userType) {
        UserService userService;
        return userService.setUserType(msg, userId, userType);
    }

    bool UserSA::deactivateUser(std::string& msg, const std::string& userId) {
        UserService userService;
        return userService.setUserType(msg, userId, "DS");
    }

    bool UserSA::deleteUserInfo(std::string& msg, const std::string& userId) {
        UserService userService;
        return userService.removeUser(msg, userId);
    }

    std::vector<std::string> UserSA::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("修改用户信息");
        result.emplace_back("重置用户密码");
        result.emplace_back("变更用户类型");
        result.emplace_back("冻结用户账号");
        result.emplace_back("删除用户记录"); // 扩展功能
        return result;
    }

    /***********************************************(3/7 UserUA类的实现)**********************************************************/
    UserUA::UserUA(const UserInfo& userInfo) {
        info = userInfo;
    }

    bool UserUA::createBorrowerUser(std::string& msg, UserInfo& userInfo) {
        UserService userService;
        return userService.registerUser(msg, userInfo);
    }

    bool UserUA::updateBorrowerBaseInfo(std::string& msg, const UserInfo& userInfo) {
        UserService userService;
        return userService.updateUser(
            msg,
            userInfo.id,
            userInfo.username,
            userInfo.email,
            userInfo.telephone,
            userInfo.address,
            userInfo.gender
        ); // UA管理员可以更新的信息条目（id不是，仅用于定位目标用户）
    }

    bool UserUA::setBorrowerAsOrdinaryBorrower(std::string& msg, const std::string& userId) {
        UserService userService;
        return userService.setUserType(msg, userId, "OB");
    }

    bool UserUA::setBorrowerAsFacultyBorrower(std::string& msg, const std::string& userId) {
        UserService userService;
        return userService.setUserType(msg, userId, "FB");
    }

    bool UserUA::deactivateBorrower(std::string& msg, const std::string& userId) {
        UserService userService;
        return userService.setUserType(msg, userId, "DS");
    }

    bool UserUA::resetBorrowerPassword(std::string& msg, const std::string& userId, const std::string& newPassword) {
        UserService userService;
        return userService.resetPassword(msg, userId, newPassword);
    }

    bool UserUA::queryBorrowerInfo(std::string& msg, const std::string& userId, UserInfo& userInfo) {
        UserService userService;
        return userService.getUserInfoByUserId(msg, userId, userInfo);
    }

    std::vector<std::string> UserUA::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("创建借阅者用户账号");
        result.emplace_back("更新借阅者基本信息");
        result.emplace_back("变更借阅者用户类型为普通借阅者");
        result.emplace_back("变更借阅者用户类型为教职员借阅者");
        result.emplace_back("冻结借阅者用户的账号");
        result.emplace_back("重置借阅者用户的密码");
        result.emplace_back("查询借阅者用户的信息");
        return result;
    }

    /***********************************************(4/7 UserLA类的实现)**********************************************************/
    UserLA::UserLA(const UserInfo& userInfo) {
        info = userInfo;
    }
    std::vector<std::string> UserLA::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("查询图书信息");
        result.emplace_back("录入图书信息");
        result.emplace_back("修改图书信息");
        result.emplace_back("删除图书信息");
        return result;
    }
    /***********************************************(5/7 UserFB类的实现)**********************************************************/
    UserFB::UserFB(const UserInfo& userInfo) {
        info = userInfo;
    }
    std::vector<std::string> UserFB::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("查询图书信息");
        result.emplace_back("借书");
        result.emplace_back("还书");
        return result;
    }
    /***********************************************(6/7 UserOB类的实现)**********************************************************/
    UserOB::UserOB(const UserInfo& userInfo) {
        info = userInfo;
    }
    std::vector<std::string> UserOB::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("查询图书信息");
        result.emplace_back("借书");
        result.emplace_back("还书");
        return result;
    }
    /***********************************************(7/7 UserDS类的实现)**********************************************************/
    UserDS::UserDS(const UserInfo& userInfo) {
        info = userInfo;
    }
    std::vector<std::string> UserDS::getMethodDescriptions() const {
        std::vector<std::string> result;
        result.emplace_back("查看个人信息");
        result.emplace_back("修改个人密码");
        result.emplace_back("编辑个人信息");
        result.emplace_back("注销个人账号"); // 从父类继承而来的功能
        result.emplace_back("查询图书信息");
        result.emplace_back("借书");
        result.emplace_back("还书");
        return result;
    }
    /***************************************************************************************************************************/
}
