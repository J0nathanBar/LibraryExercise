//
// Created by barjo on 20/04/2026.
//

#ifndef LIBRARYEXCERCISE_TEXTUI_HPP
#define LIBRARYEXCERCISE_TEXTUI_HPP

#include "BookLibrary.hpp"

class TextUI
{
public:
    TextUI();

    ~TextUI() = default;

    TextUI(const TextUI &other) = delete;

    TextUI(TextUI &&other) noexcept = delete;

    TextUI &operator=(const TextUI &other) = delete;

    TextUI &operator=(TextUI &&other) noexcept = delete;

    void runUI();

private:
    static void printHelp();

    static std::optional<std::uint32_t> parseNum(std::string_view input);

    void parseAddCommand(std::vector<std::string> &args);

    void handleRemoveBook(const std::vector<std::string> &args);

    void handleLookupBook(const std::vector<std::string> &args);

    void handleBorrowBook(const std::vector<std::string> &args);

    void handleReturnBook(const std::vector<std::string> &args);

    void parseCommand();

    BookLibrary lib;
    std::string _user_input;
};


#endif //LIBRARYEXCERCISE_TEXTUI_HPP
