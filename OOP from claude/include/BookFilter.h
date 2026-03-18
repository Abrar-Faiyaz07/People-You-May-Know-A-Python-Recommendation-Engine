#ifndef BOOKFILTER_H
#define BOOKFILTER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Book.h"

class BookFilter {
public:
    virtual ~BookFilter() {}
    virtual bool matches(const Book& book) const = 0;
};

class GenreFilter : public BookFilter {
public:
    explicit GenreFilter(const std::string& genre) : genre(genre) {}
    bool matches(const Book& book) const override { return book.getGenre() == genre; }

private:
    std::string genre;
};

class MinYearFilter : public BookFilter {
public:
    explicit MinYearFilter(int year) : year(year) {}
    bool matches(const Book& book) const override { return book.getYear() >= year; }

private:
    int year;
};

class MinRatingFilter : public BookFilter {
public:
    explicit MinRatingFilter(double minRating) : minRating(minRating) {}
    bool matches(const Book& book) const override { return book.getAvgRating() >= minRating; }

private:
    double minRating;
};

class FilterChain {
public:
    template <typename FilterType, typename... Args>
    void addFilter(Args&&... args) {
        filters.push_back(std::unique_ptr<BookFilter>(
            new FilterType(std::forward<Args>(args)...)));
    }

    bool matches(const Book& book) const {
        for (std::size_t i = 0; i < filters.size(); ++i) {
            if (!filters[i]->matches(book)) {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<std::unique_ptr<BookFilter> > filters;
};

#endif
