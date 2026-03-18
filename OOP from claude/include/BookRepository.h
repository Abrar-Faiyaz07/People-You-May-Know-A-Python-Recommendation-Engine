#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H

#include <map>
#include <string>
#include <vector>

#include "Book.h"

class BookRepository {
public:
    bool load(const std::string& path);
    bool save(const std::string& path) const;

    const std::vector<Book>& getAll() const;
    std::vector<Book>& getAllMutable();

    const Book* findById(int id) const;
    Book* findByIdMutable(int id);

private:
    std::vector<Book> books;
    std::map<int, std::size_t> indexById;
    void rebuildIndex();
};

#endif
