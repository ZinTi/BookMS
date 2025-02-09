#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <string>
#include "entity.h"

namespace MODEL {
    // 图书类，作为基类
    class Book {
    public:
        BookInfo info;

        Book();
        Book(
            const std::string& newId,
            const std::string& newISBN,
            const std::string& newBookName,
            const std::string& newAuthor,
            const std::string& newEdition,
            const std::string& newPubDate,
            int newTotalQty,
            int newStockRemain,
            double newPrice,
            const std::string& newPublisher,
            int newBookLevel
        );
        virtual void displayBookInfo() const;
        virtual ~Book();
    };

    // 普通图书类，继承自图书类
    class CommonBook final : public Book {
    public:
        explicit CommonBook(const BookInfo& newBookInfo);
    };

    // 珍贵图书类，继承自图书类
    class RareBook final : public Book {
    public:
        explicit RareBook(const BookInfo& newBookInfo);
    };
}

#endif
