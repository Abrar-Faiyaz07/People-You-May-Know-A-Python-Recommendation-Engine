#include "User.h"

User::User() : id(0) {}

User::User(int id, const std::string& name)
    : id(id), name(name) {}

User::User(int id, const std::string& name, const std::string& password)
    : id(id), name(name), password(password) {}

int User::getId() const { return id; }
const std::string& User::getName() const { return name; }
const std::string& User::getPassword() const { return password; }
const std::set<std::string>& User::getPreferredGenres() const { return preferredGenres; }
const std::set<std::string>& User::getPreferredAuthors() const { return preferredAuthors; }
const std::set<int>& User::getReadBooks() const { return readBooks; }
const std::map<int, int>& User::getRatedBooks() const { return ratedBooks; }

void User::setName(const std::string& value) { name = value; }
void User::setPassword(const std::string& value) { password = value; }

bool User::addPreferredGenre(const std::string& genre) {
    if (genre.empty()) {
        return false;
    }
    return preferredGenres.insert(genre).second;
}

bool User::addPreferredAuthor(const std::string& author) {
    if (author.empty()) {
        return false;
    }
    return preferredAuthors.insert(author).second;
}

bool User::markBookRead(int bookId) {
    if (bookId <= 0) {
        return false;
    }
    return readBooks.insert(bookId).second;
}

bool User::addOrUpdateRating(int bookId, int score) {
    if (bookId <= 0 || score < 1 || score > 5) {
        return false;
    }

    ratedBooks[bookId] = score;
    readBooks.insert(bookId);
    return true;
}

bool User::checkPassword(const std::string& candidate) const {
    return password == candidate;
}

bool User::hasRead(int bookId) const {
    return readBooks.find(bookId) != readBooks.end();
}

bool User::hasRated(int bookId) const {
    return ratedBooks.find(bookId) != ratedBooks.end();
}

int User::getRatingForBook(int bookId) const {
    std::map<int, int>::const_iterator it = ratedBooks.find(bookId);
    if (it == ratedBooks.end()) {
        return 0;
    }
    return it->second;
}

bool User::isValid() const {
    return id > 0 && !name.empty();
}
