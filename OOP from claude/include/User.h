#ifndef USER_H
#define USER_H

#include <map>
#include <set>
#include <string>

class User {
public:
    User();
    User(int id, const std::string& name);
    User(int id, const std::string& name, const std::string& password);

    int getId() const;
    const std::string& getName() const;
    const std::string& getPassword() const;
    const std::set<std::string>& getPreferredGenres() const;
    const std::set<std::string>& getPreferredAuthors() const;
    const std::set<int>& getReadBooks() const;
    const std::map<int, int>& getRatedBooks() const;

    void setName(const std::string& name);
    void setPassword(const std::string& password);
    bool addPreferredGenre(const std::string& genre);
    bool addPreferredAuthor(const std::string& author);
    bool markBookRead(int bookId);
    bool addOrUpdateRating(int bookId, int score);
    bool checkPassword(const std::string& candidate) const;

    bool hasRead(int bookId) const;
    bool hasRated(int bookId) const;
    int getRatingForBook(int bookId) const;
    bool isValid() const;

private:
    int id;
    std::string name;
    std::string password;
    std::set<std::string> preferredGenres;
    std::set<std::string> preferredAuthors;
    std::set<int> readBooks;
    std::map<int, int> ratedBooks;
};

#endif
