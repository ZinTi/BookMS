#ifndef ENTITY_H
#define ENTITY_H
#include <string>

/**
 * 用户类型代码
 */
#define SystemAdmin         "SA"
#define UserAdmin           "UA"
#define LibraryAdmin        "LA"
#define FacultyBorrower     "FB"
#define OrdinaryBorrower    "OB"
#define DisabledStatus      "DS"

typedef struct userInfo {
    std::string id; // 用户账号
    std::string username; // 用户姓名
    std::string password; // 用户密码
    std::string salt; // 随机盐值
    std::string email; // 电子邮箱
    std::string telephone; // 手机号码
    std::string address; // 通讯地址
    bool gender; // 性别
    std::string userType; // 用户类别
} UserInfo;

typedef struct bookInfo {
    std::string id; // 书号
    std::string ISBN; // ISBN
    std::string bookName; // 书名
    std::string author; // 作者
    std::string edition; // 版次
    std::string pubDate; // 出版日期
    int totalQty; // 总数
    int stockRemain; // 库存余量
    double price; // 定价
    std::string publisher; // 出版社
    int bookLevel; // 图书等级
} BookInfo;

typedef struct loanInfo {
    int id; // 与数据库中的 id 对应，存储贷款/借书记录的唯一标识符
    std::string userId; // 存储用户 ID
    std::string bookId; // 存储书籍 ID
    std::string loan_date; // 存储借书日期
    std::string due_date; // 存储应还书日期
    std::string return_date; // 存储实际还书日期，默认为空字符串(DB中为NULL)表示未归还
} LoanInfo;


typedef struct loanInfo2 {
    int id; // 与数据库中的 id 对应，存储贷款/借书记录的唯一标识符
    std::string userId; // 存储用户 ID
    std::string bookId; // 存储书籍 ID
    std::string loan_date; // 存储借书日期
    std::string due_date; // 存储应还书日期
    std::string return_date; // 存储实际还书日期，默认为空字符串(DB中为NULL)表示未归还
    // 以下是扩展信息
    std::string userName; // 用户姓名
    std::string bookName; // 图书名称
    std::string bookAuthor; // 图书作者
    std::string bookISBN; // 图书ISBN

} LoanInfo2;

#endif //ENTITY_H
