//
// Created by barjo on 19/04/2026.
//

#include "Book.hpp"

Book::Book(std::string title, std::string author_name, std::string isbn,
           const std::uint32_t published_year) : _title(std::move(title)),
                                                 _author_name(std::move(author_name)),
                                                 _isbn(std::move(isbn)),
                                                 _published_year(published_year),
                                                 _popularity(0),
                                                 _borrowed(false)
{
}

Book::Book(Book &&other) noexcept : _title(std::move(other._title)),
                                    _author_name(std::move(other._author_name)),
                                    _isbn(std::move(other._isbn)),
                                    _published_year(other._published_year),
                                    _popularity(other._popularity.load()),
                                    _borrowed(other._borrowed.load())
{
}

Book &Book::operator=(Book &&other) noexcept
{
    if (this == &other)
        return *this;
    _title = std::move(other._title);
    _author_name = std::move(other._author_name);
    _isbn = std::move(other._isbn);
    _published_year = other._published_year;
    _popularity = other._popularity.load();
    _borrowed = other._borrowed.load();
    return *this;
}

std::string Book::title() const
{
    return _title;
}

std::string Book::authorName() const
{
    return _author_name;
}

std::string Book::isbn() const
{
    return _isbn;
}

std::uint32_t Book::publishedYear() const
{
    return _published_year;
}

bool Book::borrowed() const
{
    return _borrowed;
}

bool Book::borrow()
{
    const auto result = _borrowed.exchange(true);
    if (!result)
    {
        const auto now = static_cast<std::uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().
            count());
        _popularity.store(now);
    }
    return !result;
}

std::uint64_t Book::popularity() const
{
    return _popularity.load();
}

bool Book::returnBorrowed()
{
    return _borrowed.exchange(false);
}

std::string Book::toString() const
{
    return std::format("title: {}, author name: {}, isbn: {}, published year: {}, popularity: {}, borrowed: {}", _title,
                       _author_name, _isbn, _published_year, _popularity.load(), _borrowed.load());
}
