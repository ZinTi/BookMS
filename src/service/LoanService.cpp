#include "LoanService.h"

#include "UserDAO.h"
#include "BookDAO.h"
#include "LoanDAO.h"
#include "Environ.h"
#include "datetime.h"

extern Environ* BookMSEnv;
extern UserInfo* currentUserInfo;

bool LoanService::borrowBook(std::string& msg, const std::string& userId, const std::string& bookId) {
    bool result;
    // 1、检查图书id是否存在、库存余量是否为0、图书类型以及操作者和被登记读者类型是否有权限借阅
    // 2、如果上述条件都符合则新增loan记录并更新库存余量，否则提示失败
    if (currentUserInfo->id != userId) { // 不是为当前自己的账号借书
        if (currentUserInfo->userType != LibraryAdmin && currentUserInfo->userType != SystemAdmin) {
            msg = "无权限！您不是图书管理员LA或系统管理员SA，无权为目标用户" + userId + "操作借书";
            return false;
        }
    }
    DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
    BookInfo bookInfo = bookDAO.getBookById(bookId); // 查询目标图书信息
    if (bookInfo.id.empty()) {
        msg = "失败！图书信息不存在";
        result = false;
    }else {
        if (bookInfo.stockRemain < 1) {
            msg = "失败！图书可借库存量不足";
            result = false;
        }else {
            DAO::UserDAO userDAO(BookMSEnv->getDbPath());
            UserInfo userInfo = userDAO.getUserById(userId);

            if (userInfo.userType == FacultyBorrower || (userInfo.userType == OrdinaryBorrower && bookInfo.bookLevel == 2)) {
                // 借阅者为FB 或 借阅者为OB且图书等级为2（普通图书）
                LoanInfo loanInfo;
                loanInfo.userId = userId;
                loanInfo.bookId = bookId;
                getCurrentDateTimeAndGenerateDueDateTime(loanInfo.loan_date, loanInfo.due_date);
                DAO::LoanDAO loanDAO(BookMSEnv->getDbPath());
                result = loanDAO.addLoan(loanInfo);
                if (result) {
                    bookInfo.stockRemain -= 1; // 更新库存余量
                    result = bookDAO.updateBook(bookInfo);
                    if (result) {
                        msg = "成功！借书成功，请在30天内完成还书";
                    }else {
                        msg = "警告！借书成功，但更新图书可借库存余量时出错，请管理员处理";
                        result = true;
                    }
                }else {
                    msg = "错误！新增借书记录时出错";
                }
            }else if (userInfo.userType == OrdinaryBorrower && bookInfo.bookLevel == 1) {
                msg = "失败！用户" + userId + "无权借阅珍贵图书";
                result = false;
            }else if (userInfo.userType == SystemAdmin || userInfo.userType == UserAdmin || userInfo.userType == LibraryAdmin) {
                msg = "失败！" + userId + "为管理员身份，暂不支持为该工作账号登记借阅";
                result = false;
            }else if (userInfo.userType == DisabledStatus) {
                msg = "失败！用户" + userId + "的账号处于冻结状态";
                result = false;
            }else {
                msg = "失败！其他导致不符合借书条件的情况";
                result = false;
            }
        }
    }
    return result;
}

bool LoanService::returnBook(std::string& msg, const std::string& userId, const std::string& bookId) {
    bool result;
    DAO::LoanDAO loanDAO(BookMSEnv->getDbPath());
    LoanInfo loanInfo = loanDAO.getLoanByUserIdAndBookId(userId, bookId);
    if (loanInfo.userId.empty()) {
        msg = "失败！未查询到符合条件的有效数据";
        result = false;
    }else {
        loanInfo.return_date = getCurrentDateTime();
        result = loanDAO.updateLoan(loanInfo);
        if (result) {
            DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
            BookInfo bookInfo = bookDAO.getBookById(bookId);
            bookInfo.stockRemain += 1;
            result = bookDAO.updateBook(bookInfo);
            if (result) {
                msg = "成功！已归还图书";
            }else {
                msg = "警告！图书已归还，但图书可借库存余量恢复失败，请管理员处理";
                result = true;
            }
        }else {
            msg = "失败！操作数据库更新借阅记录时失败";
            result = false;
        }
    }
    return result;
}


/**
 * @brief 通过借阅id查询借阅记录
 */
bool LoanService::findLoanById(std::string& msg, const std::string& userId, const std::string& bookId, LoanInfo2& loanInfo) {
    return true;
}

/**
 * @brief 查询借阅记录
 */
bool LoanService::findLoan(std::string& msg, const std::string& userId, const std::string& bookId, std::vector<LoanInfo2>& loansInfo) {
    return true;
}