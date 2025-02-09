#ifndef USERDAO_H
#define USERDAO_H

#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"
#include "entity.h"

namespace DAO {
    class UserDAO {
    public:
        explicit UserDAO(const std::string& dbPath);
        ~UserDAO();

        /**
         * @brief 用户是否存在
         * @param id 查询目标
         * @return 是否存在，true存在，false不存在
         */
        [[nodiscard]] bool userExists(const std::string& id) const;

        /**
         * @brief 获取所有用户信息的方法，支持传入筛选条件和排序条件，对字符串类型字段进行模糊查找
         * @param conditions 筛选条件，以字段名为键，对应匹配值为值的map，例如{"username": "abc", "email": "example"}，id字段为精确匹配，其他字符串字段模糊匹配
         * @param orderBy 排序条件，存放字段名的vector，按顺序表示排序优先级，例如{"username", "email"}
         * @return 多个用户的vector容器
         */
        std::vector<UserInfo> getAllUsers(const std::map<std::string, std::string>& conditions, const std::vector<std::string>& orderBy);

        /**
         * @brief 根据用户ID获取单个用户信息的方法
         * @param id 用户账号
         * @return 查询到的用户信息
         */
        UserInfo getUserById(const std::string& id);

        /**
         * @brief 添加用户的方法
         * @param userInfo 新增用户的用户信息
         * @return 是否添加成功
         */
        [[nodiscard]] bool addUser(const UserInfo& userInfo) const;

        /**
         * @brief 更新用户信息的方法
         * @param userInfo 更新目标新用户的信息
         * @return 是否更新成功
         */
        [[nodiscard]] bool updateUser(const UserInfo& userInfo) const;

        /**
         * @brief 删除用户的方法
         * @param id 用户账号
         * @return 是否删除成功
         */
        [[nodiscard]] bool deleteUser(const std::string& id) const;

    private:
        sqlite3* db;
        /**
         * @brief 用于将查询结果集的一行数据转换为UserInfo结构体
         * @param stmt SQL查询结果集
         * @return UserInfo结构体
         */
        UserInfo convertRowToUser(sqlite3_stmt* stmt);

        /**
         * @brief 初始化数据库连接的私有方法
         * @param dbPath 数据库路径
         * @return 是否初始化数据库连接成功
         */
        bool initDatabaseConnection(const std::string& dbPath);
    };
}
#endif
