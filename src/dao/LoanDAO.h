#ifndef LOANDAO_H
#define LOANDAO_H
#include <string>
#include <vector>

#include "entity.h"
#include "sqlite3.h"

namespace DAO {
    class LoanDAO {
    public:
        // 构造函数，初始化数据库连接
        explicit LoanDAO(const std::string& dbPath);
        // 析构函数，关闭数据库连接
        ~LoanDAO();
        // 获取所有借阅记录
        /**
         * @brief 获取所有记录的方法
         * @return 多个LoanInfo的vector容器
         */
        std::vector<LoanInfo> getAllLoan();

        /**
         * @brief 通过loanId获取借阅记录
         * @param loanId loanId
         * @return 查询结果
         */
        LoanInfo getLoanById(int loanId);

        /**
         * @brief 通过借阅者ID获取借阅记录
         * @param userId 用户ID
         * @param alreadyReturned 是否归还，true-已归还（return_date不为NULL），false-未归还（return_date为NULL）
         * @return 查询结果
         */
        std::vector<LoanInfo> getLoanByUserId(const std::string& userId, bool alreadyReturned);

        /**
         * @brief 通过图书ID获取借阅记录
         * @param bookId 图书ID
         * @param alreadyReturned 筛选条件，是否归还
         * @return 查询结果
         */
        std::vector<LoanInfo> getLoanByBookId(const std::string& bookId, bool alreadyReturned);

        /**
         * @brief 通过用户ID和图书ID查询图书记录
         * @param userId 用户ID
         * @param bookId 图书ID
         * @param alreadyReturned 是否归还，默认查找未归还记录
         * @return 查询结果
         */
        LoanInfo getLoanByUserIdAndBookId(const std::string& userId, const std::string& bookId, bool alreadyReturned = false);

        /**
         * @brief 新增借阅记录
         * @param loanInfo 新增的loanInfo信息
         * @return 成功与否
         */
        [[nodiscard]] bool addLoan(const LoanInfo& loanInfo) const;

        /**
         * @brief 更新借阅信息
         * @param loanInfo  新的借阅记录
         * @return 成功与否
         */
        [[nodiscard]] bool updateLoan(const LoanInfo& loanInfo) const;

        /**
         * @brief 删除借阅记录
         * @param loanId 删除目标记录的id
         * @return 成功与否
         */
        [[nodiscard]] bool deleteLoanById(int loanId) const;

    private:
        sqlite3* db;

        /**
         * @brief 用于将查询结果集的一行数据转换为LoanInfo结构体
         * @param stmt SQL查询结果集
         * @return LoanInfo结构体
         */
        LoanInfo convertRowToLoanInfo(sqlite3_stmt* stmt);

        /**
         * @brief 初始化数据库连接的私有方法
         * @param dbPath 数据库路径
         * @return 是否初始化数据库连接成功
         */
        bool initDatabaseConnection(const std::string& dbPath);
    };
}

#endif
