#ifndef LOANSERVICE_H
#define LOANSERVICE_H

#include <string>
#include <vector>
#include "entity.h"

class LoanService {
public:
    /**
   * @brief 借书
   * @param msg 反馈信息
   * @param userId 借阅者
   * @param bookId 目标图书
   * @return 成功与否
   */
    static bool borrowBook(std::string& msg, const std::string& userId, const std::string& bookId);

    /**
     * @brief 还书
     * @param msg 反馈信息
     * @param userId 还书人
     * @param bookId 目标图书
     * @return 成功与否
     */
    static bool returnBook(std::string& msg, const std::string& userId, const std::string& bookId);

    /**
     * @brief 通过借阅id查询借阅记录
     * @param msg 反馈信息
     * @param userId 用户ID
     * @param bookId 图书ID
     * @param loanInfo 查询到的记录
     * @return 成功与否
     */
    static bool findLoanById(std::string& msg, const std::string& userId, const std::string& bookId, LoanInfo2& loanInfo);

    /**
     * @brief 查询借阅记录
     * @param msg 反馈信息
     * @param userId 用户ID
     * @param bookId 图书ID
     * @param loansInfo 查询到的记录
     * @return 成功与否
     */
    static bool findLoan(std::string& msg, const std::string& userId, const std::string& bookId, std::vector<LoanInfo2>& loansInfo);

};

#endif
