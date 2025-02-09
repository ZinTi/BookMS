#ifndef BOOKDAO_H
#define BOOKDAO_H

#include <string>
#include <vector>
#include <map>
#include "entity.h"
#include <sqlite3.h>

namespace DAO {
    class BookDAO {
    public:
        /**
         * @brief 构造函数，初始化数据库连接
         * @param dbPath 数据库路径
         */
        explicit BookDAO(const std::string& dbPath);

        /**
         * @brief 析构函数，关闭数据库连接
         */
        ~BookDAO();


        /**
         * @brief 图书Id是否存在
         * @param id 查询目标
         * @return 是否存在，true存在，false不存在
         */
        [[nodiscard]] bool bookIdExists(const std::string& id) const;

        /**
         * @brief 图书ISBN是否存在（图书馆的ISBN有唯一性约束，图书商店的ISBN则没有，因为同一图书有成色区分——二手或全新等）
         * @param ISBN 查询目标
         * @return 是否存在，true存在，false不存在
         */
        [[nodiscard]] bool bookISBNExists(const std::string& ISBN) const;


        /**
         * @brief 获取所有书籍信息，支持筛选和排序
         * @param conditions 筛选条件
         * @param orderBy 排序条件
         * @return 图书信息
         */
        std::vector<BookInfo> getAllBooks(const std::map<std::string, std::string>& conditions = {}, const std::vector<std::string>& orderBy = {});
        /**
         * @brief 根据书籍ID获取单个书籍信息
         * @param id bookId
         * @return 图书信息
         */
        BookInfo getBookById(const std::string& id);

        /**
         * @brief 根据书籍ISBN获取单个书籍信息
         * @param ISBN ISBN
         * @return 图书信息
         */
        BookInfo getBookByISBN(const std::string& ISBN);

        /**
         * @brief 增加图书
         * @param bookInfo 图书信息
         * @return 成功与否
         */
        [[nodiscard]] bool addBook(const BookInfo& bookInfo) const;

        /**
         * @brief 更新书籍信息
         * @param bookInfo 图书信息
         * @return 成功与否
         */
        [[nodiscard]] bool updateBook(const BookInfo& bookInfo) const;

        /**
         * @brief 删除图书信息
         * @param id 目标图书Id
         * @return 成功与否
         */
        [[nodiscard]] bool deleteBook(const std::string& id) const;

    private:
        sqlite3* db; // 数据库连接对象

        // 初始化数据库连接
        bool initDatabaseConnection(const std::string& dbPath);

        // 构建模糊查找的SQL语句片段
        std::string buildLikeClause(const std::string& field, const std::string& value);

        // 构建筛选条件的SQL语句片段
        std::string buildWhereClause(const std::map<std::string, std::string>& conditions);

        // 构建排序条件的SQL语句片段
        std::string buildOrderByClause(const std::vector<std::string>& orderBy);

        // 将查询结果的一行数据转换为BookInfo结构体
        BookInfo convertRowToBook(sqlite3_stmt* stmt);
    };
}

#endif // BOOKDAO_H
