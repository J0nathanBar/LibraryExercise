//
// Created by barjo on 19/04/2026.
//

#include "BookLibrary.hpp"


BookLibrary::BookLibrary() : _stop_flag(false), _sweep_worker([this] { sweep(); })
{
    _books.reserve(LIBRARY_CAPACITY + 1);
    _book_indices_by_title.reserve(LIBRARY_CAPACITY + 1);
    _book_indices_by_isbn.reserve(LIBRARY_CAPACITY + 1);
}

BookLibrary::~BookLibrary()
{
    _stop_flag.store(true);
    _cv.notify_all();
    if (_sweep_worker.joinable())
    {
        _sweep_worker.join();
    }
    std::cout << "Library closing down\n";
}

bool BookLibrary::addBook(std::string title, std::string author, std::string isbn, std::uint32_t published_year)
{
    std::lock_guard lock(_mtx);
    if (_book_indices_by_title.contains(title) || _book_indices_by_isbn.contains(isbn))
    {
        return false;
    }
    _books.emplace_back(std::move(title), std::move(author), std::move(isbn), published_year);
    const auto &book = _books.back();
    _book_indices_by_title.insert({book.title(), _books.size() - 1});
    _book_indices_by_isbn.insert({book.isbn(), _books.size() - 1});
    if (_books.size() > LIBRARY_CAPACITY)
    {
        _cv.notify_one();
    }

    return true;
}

OperationStatus BookLibrary::removeBook(const std::string &id)
{
    bool result;
    std::unique_lock lock(_mtx);

    switch (identifyBookID(id))
    {
        case IdStatus::TITLE:
            result = innerRemoveBook(_book_indices_by_title.at(id));
            break;
        case IdStatus::ISBN:
            result = innerRemoveBook(_book_indices_by_isbn.at(id));
            break;
        default:
            return OperationStatus::NOT_FOUND;
    }
    return result ? OperationStatus::SUCCESS : OperationStatus::FAILURE;
}

OperationStatus BookLibrary::borrowBook(const std::string &id)
{
    bool result;
    std::shared_lock lock(_mtx);
    switch (identifyBookID(id))
    {
        case IdStatus::TITLE:
            result = _books.at(_book_indices_by_title.at(id)).borrow();
            break;
        case IdStatus::ISBN:
            result = _books.at(_book_indices_by_isbn.at(id)).borrow();
            break;
        default:
            return OperationStatus::NOT_FOUND;
    }
    return result ? OperationStatus::SUCCESS : OperationStatus::FAILURE;
}

OperationStatus BookLibrary::returnBook(const std::string &id)
{
    bool res;
    std::shared_lock lock(_mtx);
    switch (identifyBookID(id))
    {
        case IdStatus::TITLE:
            res = _books.at(_book_indices_by_title.at(id)).returnBorrowed();
            break;
        case IdStatus::ISBN:
            res = _books.at(_book_indices_by_isbn.at(id)).returnBorrowed();
            break;
        default:
            return OperationStatus::NOT_FOUND;
    }
    if (res && _books.size() > LIBRARY_CAPACITY)
    {
        _cv.notify_one();
    }
    return res ? OperationStatus::SUCCESS : OperationStatus::FAILURE;
}

std::optional<std::string> BookLibrary::lookupBook(const std::string &id)
{
    std::shared_lock lock(_mtx);
    switch (identifyBookID(id))
    {
        case IdStatus::TITLE:
            return _books.at(_book_indices_by_title.at(id)).toString();
        case IdStatus::ISBN:
            return _books.at(_book_indices_by_isbn.at(id)).toString();
        default:
            return std::nullopt;
    }
}


bool BookLibrary::innerRemoveBook(const std::size_t book_index)
{
    if (_books.at(book_index).borrowed())
    {
        return false;
    }
    const auto last_idx = _books.size() - 1;
    const auto title_to_del = _books.at(book_index).title();
    const auto isbn_to_del = _books.at(book_index).isbn();
    if (book_index != last_idx)
    {
        const auto title = _books.at(last_idx).title();
        const auto isbn = _books.at(last_idx).isbn();
        _book_indices_by_title[title] = book_index;
        _book_indices_by_isbn[isbn] = book_index;
        _books[book_index] = std::move(_books[last_idx]);
    }
    _books.pop_back();
    _book_indices_by_title.erase(title_to_del);
    _book_indices_by_isbn.erase(isbn_to_del);
    return true;
}

std::optional<std::string> BookLibrary::getLeastPopularTitle() const
{
    auto available_books = _books | std::views::filter([](const Book &book) {
        return !book.borrowed();
    });

    const auto min_element = std::ranges::min_element(available_books, {}, &Book::popularity);
    if (min_element == available_books.end())
    {
        return std::nullopt;
    }
    return min_element->title();
}


std::string BookLibrary::getStatus()
{
    std::shared_lock lock(_mtx);
    const auto borrowed_count = static_cast<std::size_t>(std::ranges::count_if(
        _books, [](const Book &book) {
            return book.borrowed();
        }));
    const auto lib_size = _books.size();
    return std::format("library capacity: {}, library size: {}, borrowed book count: {}, available books: {}",
                       LIBRARY_CAPACITY, lib_size, borrowed_count, lib_size - borrowed_count);
}

void BookLibrary::sweep()
{
    while (!_stop_flag)
    {
        std::unique_lock lock(_mtx);
        _cv.wait(lock, [this]() { return (_books.size() > LIBRARY_CAPACITY) || _stop_flag; });
        if (_stop_flag)
        {
            return;
        }
        if (const auto least_popular_title = getLeastPopularTitle(); least_popular_title.has_value())
        {
            if (innerRemoveBook(_book_indices_by_title.at(least_popular_title.value())))
            {
                std::cout << "Swept: " << least_popular_title.value() << std::endl;
            }
            else
            {
                std::cout << "Failed to sweep book: " << least_popular_title.value() << std::endl;
            }
        }
        else
        {
            std::cout << "No available books to be removed" << std::endl;
        }
    }
}

IdStatus BookLibrary::identifyBookID(const std::string &id) const
{
    if (_book_indices_by_title.contains(id))
    {
        return IdStatus::TITLE;
    }
    if (_book_indices_by_isbn.contains(id))
    {
        return IdStatus::ISBN;
    }
    return IdStatus::NOT_FOUND;
}
