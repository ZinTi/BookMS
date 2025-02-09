#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include <vector>
#include "entity.h"

class UserService {
public:
    /**
     * @brief 根据UserId获取用户信息
     * @param msg 反馈信息
     * @param userId 用户Id
     * @param userInfo 用户信息容器
     * @return 成功与否
     */
    static bool getUserInfoByUserId(std::string& msg, const std::string& userId, UserInfo& userInfo);

    /**
     * @brief 获取所有用户信息（除了userID以外的6个筛选条件）
     * @param msg 反馈信息
     * @param username 用户姓名
     * @param gender 性别，输入 0 代表不筛选性别，1 代表筛选男性，2 代表筛选女性
     * @param email 电子邮箱
     * @param telephone 电话号码
     * @param address 通讯地址
     * @param userType 用户类型，可选（OB/FB/LA/UA/SA/DS），如果置空代表不筛选性别
     * @param usersInfo 输出数据，查询到的用户信息将存于此vector中
     * @return 查询到的用户数量
     */
    static int getAllUser(std::string& msg,
                          const std::string& username,
                          const int gender,
                          const std::string& email,
                          const std::string& telephone,
                          const std::string& address,
                          const std::string& userType,
                          std::vector<UserInfo>& usersInfo
                          );

    /**
     * @brief 验证用户合法性
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param password 用户密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    static bool validateUser(std::string& msg, const std::string& userId, const std::string& password);

    /**
     * @brief 验证当前登录用户的合法性
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param password 用户密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    static bool validateCurrentUser(std::string& msg, const std::string& password);

    /**
     * @brief 用户登录
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param password 用户密码
     * @param userInfo 验证成功的用户信息
     * @return 成功与否，true-代表成功
     */
    static bool login(std::string& msg, const std::string& userId, const std::string& password, UserInfo& userInfo);

    /**
     * @brief 修改账号密码
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param oldPassword 旧密码
     * @param newPassword 新密码
     * @return 成功与否
     */
    static bool changePassword(std::string& msg, const std::string& userId, const std::string& oldPassword, const std::string& newPassword);

    /**
     * @brief 重置密码功能
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param newPassword 设置为新密码
     * @return 成功与否
     */
    static bool resetPassword(std::string& msg, const std::string& userId, const std::string& newPassword);

    /**
     * @brief 注册新用户
     * @param msg 反馈信息
     * @param user 注册新用户的信息
     * @param userType 注册用户的类型，默认为冻结状态（SA-系统管理员、UA-用户管理员、LA-图书管理员、FB-教职员借阅者、OB-普通借阅者、DS-冻结状态）
     * @return 返回 true 代表注册成功， false 代表注册失败
     */
    static bool registerUser(std::string& msg, UserInfo& user, const std::string& userType = "DS");

    /**
     * @brief 删除用户（删除数据而非逻辑标记）
     * @param msg 反馈信息
     * @param userId 目标用户的userId
     * @return 删除用户成功与否
     */
    static bool removeUser(std::string& msg, const std::string& userId);


    /**
     * @brief 设置用户类型
     * @param userId 目标的userId
     * @param msg 反馈信息
     * @param userType 设置的新userType
     * @return 成功与否
     */
    static bool setUserType(std::string& msg, const std::string& userId, const std::string& userType = std::string("DS")); // 默认与deactivateUser一致

    /**
     * @brief 更新用户信息
     * @param msg 反馈信息
     * @param userId 更新目标的用户Id
     * @param username 用户姓名
     * @param email 用户邮箱
     * @param telephone 手机号码
     * @param address 电子邮件
     * @param gender 性别
     * @return 成功与否
     */
    static bool updateUser(std::string& msg, const std::string& userId, const std::string&username, const std::string& email, const std::string& telephone, const std::string& address, bool gender) ;

    /**
     * @brief 更新用户信息（借阅者自己不能修改姓名和真实性别，只能联系管理员修改）
     * @param msg 反馈信息
     * @param userId 更新目标的用户Id
     * @param email 用户邮箱
     * @param telephone 手机号码
     * @param address 电子邮件
     * @return 成功与否
     */
    static bool updateUser(std::string& msg, const std::string& userId, const std::string& email, const std::string& telephone, const std::string& address) ;

};



#endif //USERSERVICE_H
