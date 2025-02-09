#include "LoanDAO.h"
#include <iostream>

/* SQL:
CREATE VIEW view_loan AS
    SELECT l.id, l.user_id, u.username, l.book_id, b.book_name, b.author, b.ISBN, l.loan_date, l.due_date, l.return_date FROM loan AS l
    LEFT JOIN user AS u ON l.user_id = u.id
    LEFT JOIN book AS b ON l.book_id = b.id;

SELECT * FROM view_loan;
 */

namespace DAO {
    // 构造函数，初始化数据库连接
    LoanDAO::LoanDAO(const std::string& dbPath) : db(nullptr) {
        if (!initDatabaseConnection(dbPath)) {
            std::cerr << "Failed to initialize database connection." << std::endl;
        }
    }

    LoanDAO::~LoanDAO() {
        sqlite3_close(db);
    }

    std::vector<LoanInfo> LoanDAO::getAllLoan() {
        std::vector<LoanInfo> loanInfos;
        sqlite3_stmt* stmt;
        const auto sql = "SELECT * FROM loan";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return loanInfos;
        }
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            loanInfos.push_back(convertRowToLoanInfo(stmt));
        }
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to step through results: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return loanInfos;
    }

    LoanInfo LoanDAO::getLoanById(const int loanId) {
        LoanInfo loanInfo = {};
        sqlite3_stmt* stmt;
        const auto sql = "SELECT * FROM loan WHERE id =? ";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return loanInfo;
        }
        sqlite3_bind_int(stmt, 1, loanId);
        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            loanInfo = convertRowToLoanInfo(stmt);
        }
        else {
            std::cerr << "No record found for loanId: " << loanId << std::endl;
        }
        sqlite3_finalize(stmt);
        return loanInfo;
    }

    std::vector<LoanInfo> LoanDAO::getLoanByUserId(const std::string& userId, bool alreadyReturned) {
        std::vector<LoanInfo> loanInfos;
        sqlite3_stmt* stmt;
        const char* sql;
        if (alreadyReturned) {
            sql = "SELECT * FROM loan WHERE user_id =? AND return_date IS NOT NULL";
        }
        else {
            sql = "SELECT * FROM loan WHERE user_id =? AND return_date IS NULL";
        }
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return loanInfos;
        }
        sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            loanInfos.push_back(convertRowToLoanInfo(stmt));
        }
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to step through results: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return loanInfos;
    }

    std::vector<LoanInfo> LoanDAO::getLoanByBookId(const std::string& bookId, const bool alreadyReturned) {
        std::vector<LoanInfo> loanInfos;
        sqlite3_stmt* stmt;
        const char* sql;
        if (alreadyReturned) {
            sql = "SELECT * FROM loan WHERE book_id =? AND return_date IS NOT NULL";
        }
        else {
            sql = "SELECT * FROM loan WHERE book_id =? AND return_date IS NULL";
        }
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return loanInfos;
        }
        sqlite3_bind_text(stmt, 1, bookId.c_str(), -1, SQLITE_STATIC);
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            loanInfos.push_back(convertRowToLoanInfo(stmt));
        }
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to step through results: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        return loanInfos;
    }

    LoanInfo LoanDAO::getLoanByUserIdAndBookId(const std::string& userId, const std::string& bookId, const bool alreadyReturned) {
        LoanInfo loanInfo = {};
        sqlite3_stmt* stmt;
        const char* sql;
        if (alreadyReturned) {
            sql = "SELECT * FROM loan WHERE user_id =? AND book_id =? AND return_date IS NOT NULL";
        }
        else {
            sql = "SELECT * FROM loan WHERE user_id =? AND book_id =? AND return_date IS NULL";
        }
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return loanInfo;
        }
        sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, bookId.c_str(), -1, SQLITE_STATIC);
        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            loanInfo = convertRowToLoanInfo(stmt);
        }
        else {
            std::cerr << "No record found!" << std::endl;
        }
        sqlite3_finalize(stmt);
        return loanInfo;
    }

    bool LoanDAO::addLoan(const LoanInfo& loanInfo) const {
        sqlite3_stmt* stmt;
        const auto sql = "INSERT INTO loan (user_id, book_id, loan_date, due_date, return_date) VALUES (?,?,?,?,?)";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_bind_text(stmt, 1, loanInfo.userId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, loanInfo.bookId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, loanInfo.loan_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, loanInfo.due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, loanInfo.return_date.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute insert statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    bool LoanDAO::updateLoan(const LoanInfo& loanInfo) const {
        sqlite3_stmt* stmt;
        const auto sql = "UPDATE loan SET user_id =?, book_id =?, loan_date =?, due_date =?, return_date =? WHERE id =?";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_bind_text(stmt, 1, loanInfo.userId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, loanInfo.bookId.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, loanInfo.loan_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, loanInfo.due_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, loanInfo.return_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, loanInfo.id);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute update statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    bool LoanDAO::deleteLoanById(const int loanId) const {
        sqlite3_stmt* stmt;
        const auto sql = "DELETE FROM loan WHERE id =?";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_bind_int(stmt, 1, loanId);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute delete statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
        return true;
    }

    LoanInfo LoanDAO::convertRowToLoanInfo(sqlite3_stmt* stmt) {
        LoanInfo loanInfo;
        loanInfo.id = sqlite3_column_int(stmt, 0);
        loanInfo.userId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        loanInfo.bookId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        loanInfo.loan_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        loanInfo.due_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        loanInfo.return_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        return loanInfo;
    }

    bool LoanDAO::initDatabaseConnection(const std::string& dbPath) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }
}
