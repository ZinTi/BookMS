#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include "entity.h"

namespace MODEL {
    /********************************************** 1/7 USER类 作为Base类 **********************************************************/
    class User {
    public:
        UserInfo info;

        /**
         * @brief 无参构造函数
         */
        User();

        /**
         * @brief 登陆前使用的构造函数
         * @param id 你的账号
         * @param password 你的密码
         */
        User(const std::string& id, const std::string& password);

        virtual ~User();

        User(
            const std::string& id,
            const std::string& username,
            const std::string& password,
            const std::string& salt,
            const std::string& email,
            const std::string& telephone,
            const std::string& address,
            bool gender,
            const std::string& userType
        );
        virtual void displayInfo() const;

    private:
    public:
        /**
         * @brief 查询个人信息
         * @param msg 反馈信息
         * @return 成功与否
         */
        bool fetchPersonalInfo(std::string& msg);

        /**
         * @brief 更改个人用户密码
         * @param msg 反馈信息
         * @param oldPassword 从view层获取的旧密码
         * @param newPassword 从view层获取的新密码
         * @return 成功与否
         */
        bool changePersonalPassword(std::string& msg, const std::string& oldPassword,
                                    const std::string& newPassword) const;

        /**
         * @brief 编辑个人信息
         * @param msg 反馈信息
         * @return 成功与否
         */
        bool editPersonalInfo(std::string& msg);

        /**
         * @brief 注销个人账号
         * @param msg 反馈信息
         * @return 成功与否
         */
        bool invalidatePersonalAccount(std::string& msg) const;

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] virtual std::vector<std::string> getMethodDescriptions() const;
    };

    /********************************* 2/7 系统管理员UserSA类 ******************************************************/
    class UserSA final : public User {
    public:
        /**
         * @brief 结构体引用的含参构造函数
         * @param userInfo 用户信息结构体的引用
         */
        explicit UserSA(const UserInfo& userInfo);

        /**
         * @brief 变更其他用户的信息
         * @param msg 反馈信息
         * @param userId 变更目标的 userId
         * @param userInfo 变更信息
         * @return 成功与否
         */
        bool modifyUserInfoById(std::string& msg, const std::string& userId, const UserInfo& userInfo);

        /**
         * @brief 变更用户类型（用户身份）
         * @param msg 反馈信息
         * @param userId 变更目标的userId
         * @param userType 变更新类型（SA、UA、LA、FB、OB、DS）
         * @return 成功与否
         */
        bool modifyUserType(std::string& msg, const std::string& userId, const std::string& userType);

        /**
         * @brief 停用用户，usertype 设置为 DS (Disabled Status)
         * @param msg 反馈信息
         * @param userId 目标用户的userId
         * @return true-禁用成功，false-禁用失败
         */
        bool deactivateUser(std::string& msg, const std::string& userId);

        /**
         * @brief 系统管理员SA可以无条件重置任意用户的密码、包括其他UA和LA管理员，以及借阅者
         * @param msg 反馈信息
         * @param userId 目标用户的用户ID
         * @param newPassword 重置的默认新密码，默认为666666
         * @return 成功与否
         */
        bool resetUserPassword(std::string& msg, const std::string& userId, const std::string& newPassword = "666666");

        /**
         * @brief 删除用户（删除数据而非逻辑标记）
         * @param msg 反馈信息
         * @param userId 目标用户的用户ID
         * @return 成功与否
         */
        bool deleteUserInfo(std::string& msg, const std::string& userId);

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        std::vector<std::string> getMethodDescriptions() const override;
    };

    /******************************************** 3/7 用户管理员UserUA类 **********************************************************************/
    /**
     * 主要负责用户的管理，拥有管理教职员和普通借阅者账号的权限
     * 不具备管理其他管理员的权限，如需管理管理员用户需要更高的系统管理员
     */
    class UserUA final : public User {
    public:
        explicit UserUA(const UserInfo& userInfo);

        /**
         * @brief 创建借阅者用户（教职员借阅者、普通借阅者）
         * @param msg 反馈信息
         * @param userInfo 新用户信息
         * @return 成功与否
         */
        bool createBorrowerUser(std::string& msg, UserInfo& userInfo);

        /**
         * @brief 变更借阅者用户基本信息
         * @param msg 反馈信息
         * @param userInfo 用户新基本信息（自动忽略其他无权限修改的敏感信息如用户类型）
         * @return 成功与否
         */
        bool updateBorrowerBaseInfo(std::string& msg, const UserInfo& userInfo);

        /**
         * @brief 将借阅者用户类型设置为普通借阅者
         * @param msg 反馈信息
         * @param userId 目标用户ID
         * @return 成功与否
         */
        bool setBorrowerAsOrdinaryBorrower(std::string& msg, const std::string& userId);

        /**
         * @brief 将借阅者用户类型设置为教职员借阅者
         * @param msg 反馈信息
         * @param userId 目标用户ID
         * @return 成功与否
         */
        bool setBorrowerAsFacultyBorrower(std::string& msg, const std::string& userId);

        /**
         * @brief 禁用借阅者账号
         * @param msg 反馈信息
         * @param userId 目标用户的用户ID
         * @return 成功与否
         */
        bool deactivateBorrower(std::string& msg, const std::string& userId);

        /**
         * @brief 重置借阅者密码
         * @param msg 反馈信息
         * @param userId 目标用户的用户ID
         * @param newPassword 默认新密码666666
         * @return 成功与否
         */
        bool resetBorrowerPassword(std::string& msg, const std::string& userId,
                                   const std::string& newPassword = "666666");

        /**
         * @brief 通过用户ID查询借阅者信息
         * @param msg 反馈信息
         * @param userId 目标用户ID
         * @param userInfo 获取目标用户信息的容器
         * @return 成功与否
         */
        bool queryBorrowerInfo(std::string& msg, const std::string& userId, UserInfo& userInfo);

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] std::vector<std::string> getMethodDescriptions() const override;
    };

    /************************************* 4/7 图书管理员UserLA类 **************************************************/
    class UserLA final : public User {
    public:
        explicit UserLA(const UserInfo& userInfo);

        // 1、新增图书信息
        // 2、修改图书信息
        // 3、查询图书信息
        // 4、删除图书信息

        /**
         * @brief 录入图书信息
         * @param msg 反馈消息
         * @param bookInfo 图书信息
         * @return 成功与否
         */
        bool addBookInfo(std::string& msg, const BookInfo& bookInfo);

        /**
         * @brief 修改图书信息
         * @param msg 反馈消息
         * @param bookInfo 图书信息
         * @return 成功与否
         */
        bool modifyBookInfo(std::string& msg, const BookInfo& bookInfo);

        /**
         * @brief 查询图书信息
         * @param msg 反馈消息
         * @param bookInfo 目标图书信息
         * @param booksInfo 查询到的若干个图书信息容器
         * @return 查询到的图书数量
         */
        int queryBookInfo(std::string& msg, const BookInfo& bookInfo, std::vector<BookInfo>& booksInfo);

        /**
         * @brief 删除图书信息
         * @param msg 反馈信息
         * @param bookId 目标图书信息
         * @return 成功与否
         */
        bool deleteBookInfo(std::string& msg, const std::string& bookId);

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] std::vector<std::string> getMethodDescriptions() const override;
    };

    /****************************************** 5/7 UserFB类 *************************************************************/
    // 教师借阅者类，继承自用户类
    class UserFB final : public User {
    public:
        explicit UserFB(const UserInfo& userInfo);

        // 查书

        // 借书

        // 还书

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] std::vector<std::string> getMethodDescriptions() const override;
    };

    /****************************************** 6/7 UserOB类 *************************************************************/
    // 教师借阅者类，继承自用户类
    class UserOB final : public User {
    public:
        explicit UserOB(const UserInfo& userInfo);

        // 查书

        // 借书

        // 还书

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] std::vector<std::string> getMethodDescriptions() const override;
    };

    /************************************************ 7/7 UserDS类 ***********************************************************/
    class UserDS final : public User {
    public:
        explicit UserDS(const UserInfo& userInfo);

        /**
         * @brief 获取所有功能的描述
         * @return 所有功能的描述
         */
        [[nodiscard]] std::vector<std::string> getMethodDescriptions() const override;
    };

    /******************************************************************************************************************/
}

#endif
