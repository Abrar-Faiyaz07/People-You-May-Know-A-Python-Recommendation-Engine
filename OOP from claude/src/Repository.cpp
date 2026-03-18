#include "Repository.h"

Repository::Repository() {}

bool Repository::loadAll(const std::string& booksPath,
                         const std::string& usersPath,
                         const std::string& ratingsPath) {
    bool okBooks = bookRepo.load(booksPath);
    bool okUsers = userRepo.load(usersPath);
    bool okRatings = ratingRepo.load(ratingsPath);
    return okBooks && okUsers && okRatings;
}

bool Repository::saveAll(const std::string& booksPath,
                         const std::string& usersPath,
                         const std::string& ratingsPath) const {
    bool okBooks = bookRepo.save(booksPath);
    bool okUsers = userRepo.save(usersPath);
    bool okRatings = ratingRepo.save(ratingsPath);
    return okBooks && okUsers && okRatings;
}

BookRepository& Repository::books() { return bookRepo; }
UserRepository& Repository::users() { return userRepo; }
RatingRepository& Repository::ratings() { return ratingRepo; }

const BookRepository& Repository::books() const { return bookRepo; }
const UserRepository& Repository::users() const { return userRepo; }
const RatingRepository& Repository::ratings() const { return ratingRepo; }
