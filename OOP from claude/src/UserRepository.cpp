#include "UserRepository.h"

#include <exception>
#include <vector>

#include "CSVParser.h"

namespace {
std::vector<std::string> toVector(const std::set<std::string>& values) {
    std::vector<std::string> out;
    for (std::set<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
        out.push_back(*it);
    }
    return out;
}

std::string serializeReadBooks(const std::set<int>& books) {
    std::string out;
    bool first = true;
    for (std::set<int>::const_iterator it = books.begin(); it != books.end(); ++it) {
        if (!first) {
            out += ';';
        }
        first = false;
        out += std::to_string(*it);
    }
    return out;
}

std::string serializeRatedBooks(const std::map<int, int>& ratings) {
    std::string out;
    bool first = true;
    for (std::map<int, int>::const_iterator it = ratings.begin(); it != ratings.end(); ++it) {
        if (!first) {
            out += ';';
        }
        first = false;
        out += std::to_string(it->first) + ":" + std::to_string(it->second);
    }
    return out;
}
}

bool UserRepository::load(const std::string& path) {
    users.clear();

    std::vector<std::vector<std::string> > rows;
    try {
        rows = CSVParser::readRows(path, true);
    } catch (const std::exception&) {
        return false;
    }

    for (std::size_t i = 0; i < rows.size(); ++i) {
        const std::vector<std::string>& row = rows[i];
        if (row.size() < 6) {
            continue;
        }

        int id = 0;
        if (!CSVParser::toInt(row[0], id)) {
            continue;
        }

        std::string password;
        if (row.size() >= 7) {
            password = row[6];
        }
        User user(id, row[1], password);

        std::vector<std::string> genres = CSVParser::splitTokenList(row[2], ';');
        for (std::size_t g = 0; g < genres.size(); ++g) {
            user.addPreferredGenre(genres[g]);
        }

        std::vector<std::string> authors = CSVParser::splitTokenList(row[3], ';');
        for (std::size_t a = 0; a < authors.size(); ++a) {
            user.addPreferredAuthor(authors[a]);
        }

        std::vector<std::string> readBooks = CSVParser::splitTokenList(row[4], ';');
        for (std::size_t r = 0; r < readBooks.size(); ++r) {
            int bookId = 0;
            if (CSVParser::toInt(readBooks[r], bookId)) {
                user.markBookRead(bookId);
            }
        }

        std::vector<std::string> ratedBooks = CSVParser::splitTokenList(row[5], ';');
        for (std::size_t rb = 0; rb < ratedBooks.size(); ++rb) {
            std::vector<std::string> pair = CSVParser::splitTokenList(ratedBooks[rb], ':');
            if (pair.size() != 2) {
                continue;
            }
            int bookId = 0;
            int score = 0;
            if (CSVParser::toInt(pair[0], bookId) && CSVParser::toInt(pair[1], score)) {
                user.addOrUpdateRating(bookId, score);
            }
        }

        if (user.isValid()) {
            users.push_back(user);
        }
    }

    rebuildIndex();
    recomputeNextId();
    return true;
}

bool UserRepository::save(const std::string& path) const {
    std::vector<std::string> header;
    header.push_back("id");
    header.push_back("name");
    header.push_back("preferred_genres");
    header.push_back("preferred_authors");
    header.push_back("read_books");
    header.push_back("rated_books");
    header.push_back("password");

    std::vector<std::vector<std::string> > rows;
    for (std::size_t i = 0; i < users.size(); ++i) {
        const User& u = users[i];
        std::vector<std::string> row;

        row.push_back(std::to_string(u.getId()));
        row.push_back(u.getName());
        row.push_back(CSVParser::joinSet(toVector(u.getPreferredGenres()), ';'));
        row.push_back(CSVParser::joinSet(toVector(u.getPreferredAuthors()), ';'));
        row.push_back(serializeReadBooks(u.getReadBooks()));
        row.push_back(serializeRatedBooks(u.getRatedBooks()));
        row.push_back(u.getPassword());

        rows.push_back(row);
    }

    try {
        return CSVParser::writeRows(path, header, rows);
    } catch (const std::exception&) {
        return false;
    }
}

const std::vector<User>& UserRepository::getAll() const { return users; }
std::vector<User>& UserRepository::getAllMutable() { return users; }

const User* UserRepository::findById(int id) const {
    std::map<int, std::size_t>::const_iterator it = indexById.find(id);
    if (it == indexById.end()) {
        return NULL;
    }
    return &users[it->second];
}

User* UserRepository::findByIdMutable(int id) {
    std::map<int, std::size_t>::iterator it = indexById.find(id);
    if (it == indexById.end()) {
        return NULL;
    }
    return &users[it->second];
}

User& UserRepository::createUser(const std::string& name, const std::string& password) {
    users.push_back(User(nextId, name, password));
    ++nextId;
    rebuildIndex();
    return users.back();
}

void UserRepository::rebuildIndex() {
    indexById.clear();
    for (std::size_t i = 0; i < users.size(); ++i) {
        indexById[users[i].getId()] = i;
    }
}

void UserRepository::recomputeNextId() {
    nextId = 1;
    for (std::size_t i = 0; i < users.size(); ++i) {
        if (users[i].getId() >= nextId) {
            nextId = users[i].getId() + 1;
        }
    }
}
