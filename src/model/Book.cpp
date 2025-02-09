#include "Book.h"

namespace MODEL {
    Book::Book() {
    }

    Book::Book(
        const std::string& newId,
        const std::string& newISBN,
        const std::string& newBookName,
        const std::string& newAuthor,
        const std::string& newEdition,
        const std::string& newPubDate,
        const int newTotalQty,
        const int newStockRemain,
        const double newPrice,
        const std::string& newPublisher,
        const int newBookLevel
    ) {
        info.id = newId;
        info.ISBN = newISBN;
        info.bookName = newBookName;
        info.author = newAuthor;
        info.edition = newEdition;
        info.pubDate = newPubDate;
        info.totalQty = newTotalQty;
        info.stockRemain = newStockRemain;
        info.price = newPrice;
        info.bookLevel = newBookLevel;
        info.bookLevel = newBookLevel;
    }

    // Book类displayBookInfo函数实现
    void Book::displayBookInfo() const {
        std::cout << "+-----------| Book |-----------+" << std::endl;
        std::cout << "| 书号: " << info.id << std::endl;
        std::cout << "| ISBN: " << info.ISBN << std::endl;
        std::cout << "| 书名: " << info.bookName << std::endl;
        std::cout << "| 作者: " << info.author << std::endl;
        std::cout << "| 版本: " << info.edition << std::endl;
        std::cout << "| 出版日期: " << info.pubDate << std::endl;
        std::cout << "| 馆藏册数: " << info.totalQty << std::endl;
        std::cout << "| 可借余量: " << info.stockRemain << std::endl;
        std::cout << "| 定价: " << info.price << std::endl;
        std::cout << "| 出版社: " << info.publisher << std::endl;
        std::cout << "+-------------------------------+" << std::endl;
    }

    Book::~Book() = default;

    CommonBook::CommonBook(const BookInfo& newBookInfo) {
        info = newBookInfo;
    }


    // RareBook类构造函数实现
    RareBook::RareBook(const BookInfo& newBookInfo) {
        info = newBookInfo;
    }
}
