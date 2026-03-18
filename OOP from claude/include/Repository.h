#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>

#include "BookRepository.h"
#include "RatingRepository.h"
#include "UserRepository.h"

class Repository {
public:
    Repository();

    bool loadAll(const std::string& booksPath,
                 const std::string& usersPath,
                 const std::string& ratingsPath);

    bool saveAll(const std::string& booksPath,
                 const std::string& usersPath,
                 const std::string& ratingsPath) const;

    BookRepository& books();
    UserRepository& users();
    RatingRepository& ratings();

    const BookRepository& books() const;
    const UserRepository& users() const;
    const RatingRepository& ratings() const;

private:
    BookRepository bookRepo;
    UserRepository userRepo;
    RatingRepository ratingRepo;
};

#endif
