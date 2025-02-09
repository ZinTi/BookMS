#include "UserDAO.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <sqlite3.h>

namespace DAO {
    // 构造函数，初始化数据库连接
    UserDAO::UserDAO(const std::string& dbPath) : db(nullptr) {
        if (!initDatabaseConnection(dbPath)) {
            // 如果数据库连接初始化失败，可以在这里进行错误处理，比如抛出异常等
            // 此处简单输出错误信息示例
            std::cerr << "Failed to initialize database connection." << std::endl;
        }
    }

    // 析构函数，关闭数据库连接
    UserDAO::~UserDAO() {
        if (db) {
            sqlite3_close(db);
        }
    }

    // 初始化数据库连接的具体实现
    bool UserDAO::initDatabaseConnection(const std::string& dbPath) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }

    // 构建模糊查找的SQL语句片段（针对字符串类型字段）
    std::string buildLikeClause(const std::string& field, const std::string& value) {
        return field + " LIKE '%" + value + "%'";
    }

    // 构建筛选条件的SQL语句片段
    std::string buildWhereClause(const std::map<std::string, std::string>& conditions) {
        std::stringstream ss;
        if (!conditions.empty()) {
            ss << " WHERE ";
            auto it = conditions.begin();
            // 处理第一个条件
            if (it->first == "id") {
                ss << it->first << " = '" << it->second << "'";
            }
            else {
                ss << buildLikeClause(it->first, it->second);
            }
            ++it;
            // 处理后续条件（用 AND 连接）
            for (; it != conditions.end(); ++it) {
                if (it->first == "id") {
                    ss << " AND " << it->first << " = '" << it->second << "'";
                }
                else {
                    ss << " AND " << buildLikeClause(it->first, it->second);
                }
            }
        }
        return ss.str();
    }

    // 构建排序条件的SQL语句片段
    std::string buildOrderByClause(const std::vector<std::string>& orderBy) {
        std::stringstream ss;
        if (!orderBy.empty()) {
            ss << " ORDER BY ";
            for (size_t i = 0; i < orderBy.size(); ++i) {
                ss << orderBy[i];
                if (i < orderBy.size() - 1) {
                    ss << ", ";
                }
            }
        }
        return ss.str();
    }

    // 检查用户是否存在（userId是否存在）
    bool UserDAO::userExists(const std::string& id) const {
        std::string sql = "SELECT 1 FROM user WHERE id =?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        else {
            std::cerr << "SQL error in userExists: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 更灵活的 获取所有用户信息的方法实现，支持筛选、排序、模糊查找
    std::vector<UserInfo> UserDAO::getAllUsers(const std::map<std::string, std::string>& conditions, const std::vector<std::string>& orderBy) {
        std::vector<UserInfo> usersInfo;
        std::string baseSql = "SELECT * FROM user ";
        std::string whereClause = buildWhereClause(conditions);
        std::string orderByClause = buildOrderByClause(orderBy);
        std::string sql = baseSql + whereClause + orderByClause;
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                UserInfo userInfo = convertRowToUser(stmt);
                usersInfo.push_back(userInfo);
            }
        }
        else {
            std::cerr << "SQL error in getAllUsers: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return usersInfo;
    }

    // 根据用户ID获取单个用户信息的方法实现
    UserInfo UserDAO::getUserById(const std::string& id) {
        UserInfo userInfo;
        std::string sql = "SELECT * FROM user WHERE id =?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                userInfo = convertRowToUser(stmt);
            }
        }
        else {
            std::cerr << "SQL error in getUserById: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return userInfo;
    }

    // 添加用户的方法实现
    bool UserDAO::addUser(const UserInfo& userInfo) const {
        std::string sql =
            "INSERT INTO user (id, username, password, salt, email, telephone, address, gender, usertype) "
            "VALUES (?,?,?,?,?,?,?,?,?)";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, userInfo.id.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, userInfo.username.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, userInfo.password.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, userInfo.salt.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, userInfo.email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 6, userInfo.telephone.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 7, userInfo.address.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 8, userInfo.gender);
            sqlite3_bind_text(stmt, 9, userInfo.userType.c_str(), -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in addUser: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in addUser: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 更新用户信息的方法实现
    bool UserDAO::updateUser(const UserInfo& userInfo) const {
        std::string sql = "UPDATE user SET username =?, password =?, salt =?, email =?, telephone =?, address =?, "
            "gender =?, usertype =? WHERE id =?";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, userInfo.username.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, userInfo.password.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, userInfo.salt.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, userInfo.email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, userInfo.telephone.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 6, userInfo.address.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 7, userInfo.gender);
            sqlite3_bind_text(stmt, 8, userInfo.userType.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 9, userInfo.id.c_str(), -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in updateUser: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in updateUser: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 删除用户的方法实现
    bool UserDAO::deleteUser(const std::string& id) const {
        std::string sql = "DELETE FROM user WHERE id =?";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in deleteUser: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in deleteUser: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 将查询结果集的一行数据转换为UserInfo结构体的方法实现
    UserInfo UserDAO::convertRowToUser(sqlite3_stmt* stmt) {
        UserInfo userInfo;
        userInfo.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        userInfo.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        userInfo.password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        userInfo.salt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        userInfo.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        userInfo.telephone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        userInfo.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        userInfo.gender = sqlite3_column_int(stmt, 7) == 1;
        userInfo.userType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        return userInfo;
    }
}
