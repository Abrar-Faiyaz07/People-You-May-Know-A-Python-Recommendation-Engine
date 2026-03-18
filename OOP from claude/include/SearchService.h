#ifndef SEARCHSERVICE_H
#define SEARCHSERVICE_H

#include <algorithm>
#include <string>
#include <vector>

#include "Book.h"
#include "BookFilter.h"

class SearchService {
public:
    std::vector<Book> searchByKeyword(const std::vector<Book>& books, const std::string& keyword) const;
    std::vector<Book> filterByGenre(const std::vector<Book>& books, const std::string& genre) const;
    std::vector<Book> filterByMinYear(const std::vector<Book>& books, int year) const;
    std::vector<Book> filterByMinRating(const std::vector<Book>& books, double minRating) const;
    std::vector<Book> applyChain(const std::vector<Book>& books, const FilterChain& chain) const;
    void sortByRatingDesc(std::vector<Book>& books) const;
    void sortByYearDesc(std::vector<Book>& books) const;

    template <typename Comparator>
    void sortBooks(std::vector<Book>& books, Comparator comparator) const {
        std::sort(books.begin(), books.end(), comparator);
    }

private:
    std::vector<Book> applyFilter(const std::vector<Book>& books, const BookFilter& filter) const;
    static std::string toLower(const std::string& text);
};

#endif
