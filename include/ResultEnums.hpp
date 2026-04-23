//
// Created by barjo on 19/04/2026.
//

#ifndef LIBRARYEXCERCISE_RESULTENUMS_HPP
#define LIBRARYEXCERCISE_RESULTENUMS_HPP

/**
 * @brief used for checking if a book id is a title/isbn
 * since you can only have one copy of each
 */
enum class IdStatus: std::uint8_t
{
    TITLE,
    ISBN,
    NOT_FOUND
};

enum class OperationStatus: std::uint8_t
{
    SUCCESS,
    FAILURE,
    NOT_FOUND
};
#endif //LIBRARYEXCERCISE_RESULTENUMS_HPP
