//
// Created by barjo on 20/04/2026.
//

#include "TextUI.hpp"

TextUI::TextUI()
{
    constexpr auto INPUT_SIZE = 1024;
    _user_input.reserve(INPUT_SIZE);

    //populating the library
    lib.addBook("The_Hobbit", "Tolkein", "0048230693", 1937);
    lib.addBook("1984", "Orwell", "043635019X", 1949);
}

void TextUI::runUI()
{
    std::cout << "welcome to the library! type help to start" << std::endl;
    while (true)
    {
        std::cout << "lib>";
        std::getline(std::cin, _user_input);
        if (_user_input.empty())
        {
            continue;
        }
        if (_user_input == "exit")
            break;
        if (_user_input == "help")
        {
            printHelp();
            continue;
        }
        parseCommand();
    }
}

void TextUI::printHelp()
{
    std::cout << "type: add: [title] [author] [isbn] [published year] to add a book\n"\
            "remove [title/isbn] to remove a book\n"\
            "lookup [title/isbn] to fetch a book\n"\
            "borrow [title/isbn] to borrow a book\n"\
            "status to get the status of current library\n"\
            "exit to exit" << std::endl;
}

std::optional<std::uint32_t> TextUI::parseNum(std::string_view input)
{
    std::uint32_t num = 0;
    auto [ptr,error_code] = std::from_chars(input.data(), input.data() + input.size(), num);;
    if (error_code == std::errc() && ptr == input.end())
    {
        return num;
    }
    if (error_code != std::errc())
    {
        std::cout << "failed to parse num: " << std::make_error_code(error_code).message() << std::endl;
    }
    else { std::cout << "failed to parse num: got mixed string of digits and chars"; }
    return std::nullopt;
}

void TextUI::parseAddCommand(std::vector<std::string> &args)
{
    if (constexpr auto ADD_ARG_COUNT = 5; args.size() != ADD_ARG_COUNT)
    {
        std::cout << "invalid add command" << std::endl;
        return;
    }
    constexpr auto PUB_YEAR_LOC = 4;
    const auto pub_year = parseNum(args.at(PUB_YEAR_LOC));
    if (pub_year == std::nullopt)
    {
        std::cout << "invalid published year" << std::endl;
        return;
    }
    const auto res = lib.addBook(std::move(args.at(1)), std::move(args.at(2)), std::move(args.at(3)),
                                 pub_year.value());
    if (!res)
    {
        std::cout << "failed to add book, already exists" << std::endl;
    }
    else
    {
        std::cout << "successfully added book" << std::endl;
    }
}

void TextUI::handleRemoveBook(const std::vector<std::string> &args)
{
    if (constexpr auto REMOVE_ARG_COUNT = 2; args.size() != REMOVE_ARG_COUNT)
    {
        std::cout << "invalid add command" << std::endl;
        return;
    }
    switch (lib.removeBook(args.at(1)))
    {
        case OperationStatus::SUCCESS:
            std::cout << "removed book: " << args.at(1) << " successfully" << std::endl;
            break;
        case OperationStatus::FAILURE:
            std::cout << "failed to remove: " << args.at(1) << " book is borrowed" << std::endl;
            break;
        case OperationStatus::NOT_FOUND:
            std::cout << "could not remove file. not found" << std::endl;
            break;
    }
}

void TextUI::handleLookupBook(const std::vector<std::string> &args)
{
    if (constexpr auto LOOKUP_ARG_COUNT = 2; args.size() != LOOKUP_ARG_COUNT)
    {
        std::cout << "invalid lookup command" << std::endl;
        return;
    }
    if (const auto book = lib.lookupBook(args.at(1)); book)
    {
        std::cout << "fetched book: " << book.value() << std::endl;
    }
    else
    {
        std::cout << "book: " << args.at(1) << " was not found" << std::endl;
    }
}

void TextUI::handleBorrowBook(const std::vector<std::string> &args)
{
    if (constexpr auto BORROW_ARG_COUNT = 2; args.size() != BORROW_ARG_COUNT)
    {
        std::cout << "invalid borrow command" << std::endl;
        return;
    }
    switch (lib.borrowBook(args.at(1)))
    {
        case OperationStatus::SUCCESS:
            std::cout << "borrowed book: " << args.at(1) << " successfully" << std::endl;
            break;
        case OperationStatus::FAILURE:
            std::cout << args.at(1) << " is already borrowed" << std::endl;
            break;
        case OperationStatus::NOT_FOUND:
            std::cout << "could not borrow book: " << args.at(1) << "not found" << std::endl;
            break;
    }
}

void TextUI::handleReturnBook(const std::vector<std::string> &args)
{
    if (constexpr auto RETURN_ARG_COUNT = 2; args.size() != RETURN_ARG_COUNT)
    {
        std::cout << "invalid return command" << std::endl;
        return;
    }
    switch (lib.returnBook(args.at(1)))
    {
        case OperationStatus::SUCCESS:
            std::cout << "returned book: " << args.at(1) << "successfully" << std::endl;
            break;
        case OperationStatus::FAILURE:
            std::cout << args.at(1) << " wasnt borrowed" << std::endl;
            break;
        case OperationStatus::NOT_FOUND:
            std::cout << "could not return book: " << args.at(1) << " not found" << std::endl;
            break;
    }
}

void TextUI::parseCommand()
{
    std::vector<std::string> args;
    constexpr auto MAX_ARG_COUNT = 4;
    args.reserve(MAX_ARG_COUNT);
    for (auto part: _user_input | std::views::split(' '))
    {
        args.emplace_back(part.begin(), part.end());
    }

    if (const auto cmd = args.at(0); cmd == "add")
    {
        parseAddCommand(args);
    }
    else if (cmd == "remove")
    {
        handleRemoveBook(args);
    }
    else if (cmd == "lookup")
    {
        handleLookupBook(args);
    }
    else if (cmd == "borrow")
    {
        handleBorrowBook(args);
    }
    else if (cmd == "return")
    {
        handleReturnBook(args);
    }
    else if (cmd == "status")
    {
        std::cout << lib.getStatus() << std::endl;
    }
    else
    {
        std::cout << "unknown command! try running help" << std::endl;
    }
}
