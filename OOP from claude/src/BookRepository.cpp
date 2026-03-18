#include "BookRepository.h"

#include <exception>
#include <sstream>

#include "CSVParser.h"

bool BookRepository::load(const std::string& path) {
    books.clear();

    std::vector<std::vector<std::string> > rows;
    try {
        rows = CSVParser::readRows(path, true);
    } catch (const std::exception&) {
        return false;
    }

    for (std::size_t i = 0; i < rows.size(); ++i) {
        const std::vector<std::string>& row = rows[i];
        if (row.size() < 8) {
            continue;
        }

        int id = 0;
        int year = 0;
        int ratingCount = 0;
        int popularity = 0;
        double avgRating = 0.0;

        if (!CSVParser::toInt(row[0], id) || !CSVParser::toInt(row[4], year) ||
            !CSVParser::toDouble(row[5], avgRating) || !CSVParser::toInt(row[6], ratingCount) ||
            !CSVParser::toInt(row[7], popularity)) {
            continue;
        }

        Book b(id, row[1], row[2], row[3], year, avgRating, ratingCount, popularity);
        if (b.isValid()) {
            books.push_back(b);
        }
    }

    rebuildIndex();
    return true;
}

bool BookRepository::save(const std::string& path) const {
    std::vector<std::string> header;
    header.push_back("id");
    header.push_back("title");
    header.push_back("author");
    header.push_back("genre");
    header.push_back("year");
    header.push_back("avg_rating");
    header.push_back("rating_count");
    header.push_back("popularity");

    std::vector<std::vector<std::string> > rows;
    for (std::size_t i = 0; i < books.size(); ++i) {
        const Book& b = books[i];
        std::vector<std::string> row;
        std::ostringstream avg;
        avg << b.getAvgRating();

        row.push_back(std::to_string(b.getId()));
        row.push_back(b.getTitle());
        row.push_back(b.getAuthor());
        row.push_back(b.getGenre());
        row.push_back(std::to_string(b.getYear()));
        row.push_back(avg.str());
        row.push_back(std::to_string(b.getRatingCount()));
        row.push_back(std::to_string(b.getPopularity()));
        rows.push_back(row);
    }

    try {
        return CSVParser::writeRows(path, header, rows);
    } catch (const std::exception&) {
        return false;
    }
}

const std::vector<Book>& BookRepository::getAll() const { return books; }
std::vector<Book>& BookRepository::getAllMutable() { return books; }

const Book* BookRepository::findById(int id) const {
    std::map<int, std::size_t>::const_iterator it = indexById.find(id);
    if (it == indexById.end()) {
        return NULL;
    }
    return &books[it->second];
}

Book* BookRepository::findByIdMutable(int id) {
    std::map<int, std::size_t>::iterator it = indexById.find(id);
    if (it == indexById.end()) {
        return NULL;
    }
    return &books[it->second];
}

void BookRepository::rebuildIndex() {
    indexById.clear();
    for (std::size_t i = 0; i < books.size(); ++i) {
        indexById[books[i].getId()] = i;
    }
}
