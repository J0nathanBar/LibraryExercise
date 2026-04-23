//
// Created by barjo on 19/04/2026.
//

#ifndef LIBRARYEXCERCISE_BOOK_HPP
#define LIBRARYEXCERCISE_BOOK_HPP
#include <string>
#include <cstdint>
#include <atomic>
#include <ostream>
#include <chrono>


class Book
{
public:
    Book(std::string title, std::string author_name, std::string isbn, std::uint32_t published_year);

    ~Book() = default;

    Book(const Book &other) = delete;

    Book(Book &&other) noexcept;

    Book &operator=(const Book &other) = delete;

    Book &operator=(Book &&other) noexcept;

    [[nodiscard]] std::string title() const;

    [[nodiscard]] std::string authorName() const;

    [[nodiscard]] std::string isbn() const;

    [[nodiscard]] std::uint32_t publishedYear() const;

    [[nodiscard]] bool borrowed() const;

    /**
     * @brief atomically borrows a book
     * @return success/fail
     */
    bool borrow();

    [[nodiscard]] std::uint64_t popularity() const;

    bool returnBorrowed();

    [[nodiscard]] std::string toString() const;

private:
    std::string _title;
    std::string _author_name;
    std::string _isbn;
    std::uint32_t _published_year;
    std::atomic_uint64_t _popularity;
    std::atomic_bool _borrowed;
};


#endif //LIBRARYEXCERCISE_BOOK_HPP
