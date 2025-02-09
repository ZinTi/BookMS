#include "BookService.h"
#include "BookDAO.h"
#include "Environ.h"
#include <map>
#include <algorithm>

extern Environ* BookMSEnv;
extern UserInfo* currentUserInfo;

bool BookService::getBookInfoByBookId(std::string& msg, const std::string& bookId, BookInfo& bookInfo) {
    bool result;
    DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
    if(bookDAO.bookIdExists(bookId) == false){
        msg = "失败！目标图书不存在: " + bookId;
        result = false;
        return result;
    }

    bookInfo = bookDAO.getBookById(bookId);
    if (bookInfo.id == bookId) {
        msg = "查询到图书信息";
        result = true;
    }else {
        msg = "未查询到图书信息";
        result = false;
    }
    return result;
}

bool BookService::getBookInfoByISBN(std::string& msg, const std::string& ISBN, BookInfo& bookInfo) {
    bool result;
    DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
    if(bookDAO.bookISBNExists(ISBN) == false){
        msg = "失败！目标图书不存在: " + ISBN;
        result = false;
        return result;
    }

    bookInfo = bookDAO.getBookByISBN(ISBN);
    if (bookInfo.ISBN == ISBN) {
        msg = "查询到图书信息";
        result = true;
    }else {
        msg = "未查询到图书信息";
        result = false;
    }
    return result;
}

int BookService::getAllBook(std::string& msg, const std::string& bookName, const std::string& author, const std::string& edition, const std::string& pubStartDate, const std::string& pubEndDate, const std::string publisher, const int bookLevel, std::vector<BookInfo>& booksInfo) {
    int result = 0; // 查询到的图书数量
    DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
    std::map<std::string, std::string> conditions;
    std::vector<std::string> orderBy = {"id ASC"};
    if (bookName!= "" &&!bookName.empty()) {
        conditions["book_name"] = bookName;
    }
    if (author!= "" &&!author.empty()) {
        conditions["author"] = author;
    }
    if (edition!= "" &&!edition.empty()) {
        conditions["edition"] = edition;
        orderBy.emplace_back("edition DESC");
    }
    if (publisher!= "" &&!publisher.empty()) {
        conditions["publisher"] = publisher;
    }
    if (bookLevel != 0) { // 0 代表不筛选
        conditions["book_level"] = std::to_string(bookLevel);
    }

    if (currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == UserAdmin || currentUserInfo->userType == LibraryAdmin || currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower) {
        booksInfo = bookDAO.getAllBooks(conditions, orderBy); // 查询所有符合条件的图书信息
        if (!booksInfo.empty()) {
            // 过滤掉已获取记录中出版日期不在区间内的图书信息（其实可以在 SQL 语句中实现，因为是 C++ 实训，所以还是在 C++ 代码层面实现逻辑好了）
            if (pubStartDate!= "" &&!pubStartDate.empty()) {
                // 删除 booksInfo 记录中所有 BookInfo.pubDate(出版日期) 在 pubStartDate(出版日期左筛选区间) 之前的记录
                booksInfo.erase(std::remove_if(booksInfo.begin(), booksInfo.end(), [pubStartDate](const BookInfo& book) {
                                    return book.pubDate < pubStartDate;
                                }), booksInfo.end());
            }
            if (pubEndDate!= "" &&!pubEndDate.empty()) {
                // 删除 booksInfo 记录中所有 BookInfo.pubDate(出版日期) 在 pubEndDate(出版日期右筛选区间) 之后的记录
                booksInfo.erase(std::remove_if(booksInfo.begin(), booksInfo.end(), [pubEndDate](const BookInfo& book) {
                                    return book.pubDate > pubEndDate;
                                }), booksInfo.end());
            }
            result = static_cast<int>(booksInfo.size());
        }
        else {
            result = 0;
        }
        msg = "完成！执行查询操作完成，查询到" + std::to_string(result) + "条图书信息";
    }
    else if (currentUserInfo->userType == DisabledStatus) {
        msg = "失败！已冻结用户无权查询图书信息！";
        result = 0;
    }
    else {
        msg = "失败！当前用户未登录或不合法";
        result = 0;
    }
    return result;
}

bool BookService::addBookInfo(std::string& msg, const BookInfo& bookInfo) {
    bool result;
    const DAO::BookDAO bookDAO(BookMSEnv->getDbPath());
    // 先检查bookId和ISBN是否已经存在了
    if(bookDAO.bookIdExists(bookInfo.id)){
        msg = "失败！图书ID已存在: " + bookInfo.id;
        result = false;
        return result;
    }
    if(bookDAO.bookISBNExists(bookInfo.ISBN)){
        msg = "失败！图书ISBN已存在: " + bookInfo.ISBN + ". \n注：商城类图书ISBN可以重复，因为存在ISBN相同但图书成色(全新/二手)不同的情况；图书馆则可以为ISBN设置唯一约束";
        result = false;
        return result;
    }

    result = bookDAO.addBook(bookInfo);
    if (result) {
        msg = "完成！新增图书信息完成";
    }else {
        msg = "失败！错误发生在操作数据库时";
    }
    return result;
}

bool BookService::modifyBookInfo(std::string& msg, const BookInfo& bookInfo) {
    bool result;
    const DAO::BookDAO bookDAO(BookMSEnv->getDbPath());

    if(bookDAO.bookIdExists(bookInfo.id) == false){
        msg = "失败！目标图书不存在: " + bookInfo.id;
        result = false;
        return result;
    }

    if (currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == LibraryAdmin  ) {
        result = bookDAO.updateBook(bookInfo);
        if (result) {
            msg = "完成！更新图书信息成功";
        }else {
            msg = "失败！错误发生在操作数据库时";
        }
    }
    else if (currentUserInfo->userType == UserAdmin || currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower || currentUserInfo->userType == DisabledStatus) {
        msg = "失败！" + currentUserInfo->userType + "类型无更新图书记录的权限";
        result = false;
    }else {
        msg = "失败！当前用户未登录或不合法";
        result = false;
    }

    return result;
}

bool BookService::deleteBookInfo(std::string& msg, const std::string& bookId) {
    bool result;
    const DAO::BookDAO bookDAO(BookMSEnv->getDbPath());

    if(bookDAO.bookIdExists(bookId) == false){
        msg = "失败！目标图书不存在: " + bookId;
        result = false;
        return result;
    }

    if (currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == LibraryAdmin  ) {
        result = bookDAO.deleteBook(bookId);
        if (result) {
            msg = "完成！删除图书信息成功";
        }else {
            msg = "错误！操作数据库时删除图书记录" + bookId + "失败";
        }
    }
    else if (currentUserInfo->userType == UserAdmin || currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower || currentUserInfo->userType == DisabledStatus) {
        msg = "失败！" + currentUserInfo->userType + "类型无删除图书记录的权限";
        result = false;
    }else {
        msg = "失败！当前用户未登录或不合法";
        result = false;
    }

    return result;
}
