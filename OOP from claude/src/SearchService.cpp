#include "SearchService.h"

#include <cctype>

std::string SearchService::toLower(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(text[i]))));
    }
    return out;
}

std::vector<Book> SearchService::searchByKeyword(const std::vector<Book>& books, const std::string& keyword) const {
    std::vector<Book> out;
    const std::string needle = toLower(keyword);
    for (std::size_t i = 0; i < books.size(); ++i) {
        const std::string title = toLower(books[i].getTitle());
        const std::string author = toLower(books[i].getAuthor());
        if (title.find(needle) != std::string::npos || author.find(needle) != std::string::npos) {
            out.push_back(books[i]);
        }
    }
    return out;
}

std::vector<Book> SearchService::filterByGenre(const std::vector<Book>& books, const std::string& genre) const {
    GenreFilter filter(genre);
    return applyFilter(books, filter);
}

std::vector<Book> SearchService::filterByMinYear(const std::vector<Book>& books, int year) const {
    MinYearFilter filter(year);
    return applyFilter(books, filter);
}

std::vector<Book> SearchService::filterByMinRating(const std::vector<Book>& books, double minRating) const {
    MinRatingFilter filter(minRating);
    return applyFilter(books, filter);
}

std::vector<Book> SearchService::applyChain(const std::vector<Book>& books, const FilterChain& chain) const {
    std::vector<Book> out;
    for (std::size_t i = 0; i < books.size(); ++i) {
        if (chain.matches(books[i])) {
            out.push_back(books[i]);
        }
    }
    return out;
}

std::vector<Book> SearchService::applyFilter(const std::vector<Book>& books, const BookFilter& filter) const {
    std::vector<Book> out;
    for (std::size_t i = 0; i < books.size(); ++i) {
        if (filter.matches(books[i])) {
            out.push_back(books[i]);
        }
    }
    return out;
}

void SearchService::sortByRatingDesc(std::vector<Book>& books) const {
    sortBooks(books, [](const Book& a, const Book& b) {
        if (a.getAvgRating() == b.getAvgRating()) {
            return a.getPopularity() > b.getPopularity();
        }
        return a.getAvgRating() > b.getAvgRating();
    });
}

void SearchService::sortByYearDesc(std::vector<Book>& books) const {
    sortBooks(books, [](const Book& a, const Book& b) {
        if (a.getYear() == b.getYear()) {
            return a.getAvgRating() > b.getAvgRating();
        }
        return a.getYear() > b.getYear();
    });
}
