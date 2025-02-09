#ifndef BOOKSERVICE_H
#define BOOKSERVICE_H

#include <string>
#include <vector>
#include "entity.h"

class BookService {
public:
    /**
    * @brief 根据bookId获取图书信息
    * @param msg 反馈信息
    * @param bookId 图书号
    * @param bookInfo 图书信息
    * @return 成功与否
    */
    static bool getBookInfoByBookId(std::string& msg, const std::string& bookId, BookInfo& bookInfo);

    /**
     * @brief 根据ISBN查询图书信息
     * @param msg 反馈信息
     * @param ISBN ISBN号
     * @param bookInfo 图书信息
     * @return 成功与否
     */
    static bool getBookInfoByISBN(std::string& msg, const std::string& ISBN, BookInfo& bookInfo);

    /**
     * @brief 根据多种筛选条件查询图书信息
     * @param msg 反馈信息
     * @param bookName 图书名称
     * @param author 作者
     * @param edition 版次
     * @param pubStartDate 出版日期左区间
     * @param pubEndDate 出版日期右区间
     * @param publisher 出版社
     * @param bookLevel 图书等级（0-代表不筛选，1-代表珍贵图书，2-代表普通图书）
     * @param booksInfo 查询到的图书信息
     * @return 查询到的记录数
     */
    static int getAllBook(std::string& msg,const std::string& bookName,const std::string& author,const std::string& edition,const std::string& pubStartDate,const std::string& pubEndDate, const std::string publisher, const int bookLevel, std::vector<BookInfo>& booksInfo );

    /**
     * @brief 录入图书信息
     * @param msg 反馈信息
     * @param bookInfo 要新增的图书信息
     * @return 成功与否
     */
    static bool addBookInfo(std::string& msg, const BookInfo& bookInfo);

    /**
     * @brief 修改图书信息
     * @param msg 反馈信息
     * @param bookInfo 图书信息（以bookInfo.id作为条件）
     * @return 成功与否
     */
    static bool modifyBookInfo(std::string& msg, const BookInfo& bookInfo);

    /**
     * @brief 删除图书信息
     * @param msg 反馈信息
     * @param bookId 目标图书号
     * @return 成功与否
     */
    static bool deleteBookInfo(std::string& msg, const std::string& bookId);
};



#endif //BOOKSERVICE_H
