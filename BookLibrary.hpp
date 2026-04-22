//
// Created by barjo on 19/04/2026.
//

#ifndef LIBRARYEXCERCISE_BOOKLIBRARY_HPP
#define LIBRARYEXCERCISE_BOOKLIBRARY_HPP
#include <algorithm>
#include <condition_variable>
#include <memory>
#include <unordered_map>
#include <thread>
#include <functional>
#include <iostream>
#include <ranges>
#include <optional>
#include <shared_mutex>

#include "Book.hpp"
#include "ResultEnums.hpp"


class BookLibrary
{
public:
    BookLibrary();

    ~BookLibrary();

    bool addBook(std::string title, std::string author, std::string isbn, std::uint32_t published_year);

    /**
     * @brief removes a book from the library (fails if the book is borrowed)
     * @param id
     * @return status of said operation
     */
    OperationStatus removeBook(const std::string &id);


    std::optional<std::string> lookupBook(const std::string &id);

    OperationStatus borrowBook(const std::string &id);

    OperationStatus returnBook(const std::string &id);


    std::string getStatus();

private:
    void sweep();

    /**
     * @brief utility function to check if a given id is a title/isbn/nonexistent
     * @param id
     * @return
     */
    IdStatus identifyBookID(const std::string &id) const;

    /**
     *
     * @param book_index
     * @return true if operation was successful, false if not
     * @note I prefer using cpp23's std::expected for result checking
     * @warning this function assumes the mutex is already locked
     * it was made this way for reusability across both removeBook and the sweeping process
     */
    bool innerRemoveBook(std::size_t book_index);

    /**
     *
     * @return the title of the least popular available book
     * @warning requires a lock from the outside
     */
    std::optional<std::string> getLeastPopularTitle() const;


    static constexpr std::size_t LIBRARY_CAPACITY = 5;

    std::vector<Book> _books;
    std::unordered_map<std::string, std::size_t> _book_indices_by_title;
    std::unordered_map<std::string, std::size_t> _book_indices_by_isbn;

    //the mutex only protects the vector adn the maps!
    //it doesn't cover the mutation of the books themselves.
    //the books were supplied with atomics for their own protection
    std::shared_mutex _mtx{};

    std::condition_variable_any _cv{};
    std::atomic_bool _stop_flag;
    std::thread _sweep_worker;
};


#endif //LIBRARYEXCERCISE_BOOKLIBRARY_HPP
