#include "BookDAO.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace DAO {
    BookDAO::BookDAO(const std::string& dbPath) : db(nullptr) {
        if (!initDatabaseConnection(dbPath)) {
            std::cerr << "Failed to initialize database connection." << std::endl;
        }
    }

    BookDAO::~BookDAO() {
        if (db) {
            sqlite3_close(db);
        }
    }

    bool BookDAO::bookIdExists(const std::string& id) const{
        std::string sql = "SELECT 1 FROM book WHERE id =?";
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
            std::cerr << "SQL error in bookIdExists: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return false;
    }

    bool BookDAO::bookISBNExists(const std::string& ISBN) const{
        std::string sql = "SELECT 1 FROM book WHERE ISBN =?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        else {
            std::cerr << "SQL error in bookISBNExists: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return false;
    }

    bool BookDAO::initDatabaseConnection(const std::string& dbPath) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }

    // 获取所有图书信息的方法实现，支持筛选、排序
    std::vector<BookInfo> BookDAO::getAllBooks(const std::map<std::string, std::string>& conditions, const std::vector<std::string>& orderBy) {
        std::vector<BookInfo> booksInfo;
        const std::string baseSql = "SELECT * FROM book ";
        const std::string whereClause = buildWhereClause(conditions);
        const std::string orderByClause = buildOrderByClause(orderBy);
        const std::string sql = baseSql + whereClause + orderByClause;
        sqlite3_stmt* stmt;
        const int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                BookInfo bookInfo = convertRowToBook(stmt);
                booksInfo.push_back(bookInfo);
            }
        }
        else {
            std::cerr << "SQL error in getAllBooks: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return booksInfo;
    }

    // 根据图书ID获取单个图书信息的方法实现
    BookInfo BookDAO::getBookById(const std::string& id) {
        BookInfo bookInfo;
        const std::string sql = "SELECT * FROM book WHERE id =?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                bookInfo = convertRowToBook(stmt);
            }
        }
        else {
            std::cerr << "SQL error in getBookById: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return bookInfo;
    }

    BookInfo BookDAO::getBookByISBN(const std::string& ISBN) {
        BookInfo bookInfo;
        const std::string sql = "SELECT * FROM book WHERE ISBN =?";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                bookInfo = convertRowToBook(stmt);
            }
        }
        else {
            std::cerr << "SQL error in getBookById: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return bookInfo;
    }

    // 添加图书的方法实现
    bool BookDAO::addBook(const BookInfo& bookInfo) const {
        std::string sql =
            "INSERT INTO book (id, ISBN, book_name, author, edition, pub_date, total_qty, stock_remain, price, publisher, book_level) "
            "VALUES (?,?,?,?,?,?,?,?,?,?,?)";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, bookInfo.id.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, bookInfo.ISBN.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, bookInfo.bookName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, bookInfo.author.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, bookInfo.edition.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 6, bookInfo.pubDate.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 7, bookInfo.totalQty);
            sqlite3_bind_int(stmt, 8, bookInfo.stockRemain);
            sqlite3_bind_double(stmt, 9, bookInfo.price);
            sqlite3_bind_text(stmt, 10, bookInfo.publisher.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 11, bookInfo.bookLevel);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in addBook: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in addBook: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 更新图书信息的方法实现
    bool BookDAO::updateBook(const BookInfo& bookInfo) const {
        std::string sql =
            "UPDATE book SET ISBN =?, book_name =?, author =?, edition =?, pub_date =?, total_qty =?, "
            "stock_remain =?, price =?, publisher =?, book_level =? WHERE id =?";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, bookInfo.ISBN.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, bookInfo.bookName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, bookInfo.author.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, bookInfo.edition.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, bookInfo.pubDate.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 6, bookInfo.totalQty);
            sqlite3_bind_int(stmt, 7, bookInfo.stockRemain);
            sqlite3_bind_double(stmt, 8, bookInfo.price);
            sqlite3_bind_text(stmt, 9, bookInfo.publisher.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 10, bookInfo.bookLevel);
            sqlite3_bind_text(stmt, 11, bookInfo.id.c_str(), -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in updateBook: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in updateBook: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 删除图书的方法实现
    bool BookDAO::deleteBook(const std::string& id) const {
        std::string sql = "DELETE FROM book WHERE id =?";
        sqlite3_stmt* stmt;
        if (int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "SQL error in deleteBook: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        else {
            std::cerr << "SQL error in deleteBook: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    // 将查询结果集的一行数据转换为BookInfo结构体的方法实现
    BookInfo BookDAO::convertRowToBook(sqlite3_stmt* stmt) {
        BookInfo bookInfo;
        bookInfo.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        bookInfo.ISBN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        bookInfo.bookName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        bookInfo.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        bookInfo.edition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        bookInfo.pubDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        bookInfo.totalQty = sqlite3_column_int(stmt, 6);
        bookInfo.stockRemain = sqlite3_column_int(stmt, 7);
        bookInfo.price = sqlite3_column_double(stmt, 8);
        bookInfo.publisher = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        bookInfo.bookLevel = sqlite3_column_int(stmt, 10);
        return bookInfo;
    }

    // 构建模糊查找的SQL语句片段
    std::string BookDAO::buildLikeClause(const std::string& field, const std::string& value) {
        if (field.empty() || value.empty()) {
            return "";
        }
        std::ostringstream oss;
        oss << field << " LIKE '%" << value << "%'";
        return oss.str();
    }

    // 构建筛选条件的 SQL 语句片段
    std::string BookDAO::buildWhereClause(const std::map<std::string, std::string>& conditions) {
        if (conditions.empty()) {
            return "";
        }
        std::ostringstream oss;
        oss << "WHERE ";
        bool first = true;
        for (const auto& [field, value] : conditions) {
            if (!first) {
                oss << " AND ";
            }
            // 调用 buildLikeClause 进行模糊匹配
            oss << buildLikeClause(field, value);
            first = false;
        }
        return oss.str();
    }

    // 构建排序条件的SQL语句片段
    std::string BookDAO::buildOrderByClause(const std::vector<std::string>& orderBy) {
        if (orderBy.empty()) {
            return "";
        }
        std::ostringstream oss;
        oss << "ORDER BY ";
        for (size_t i = 0; i < orderBy.size(); ++i) {
            if (i > 0) {
                oss << ", ";
            }
            oss << orderBy[i];
        }
        return oss.str();
    }
}
