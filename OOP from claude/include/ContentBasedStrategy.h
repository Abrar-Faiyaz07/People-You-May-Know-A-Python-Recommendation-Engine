#ifndef CONTENTBASEDSTRATEGY_H
#define CONTENTBASEDSTRATEGY_H

#include "IRecommendationStrategy.h"

// =============================================================================
// ContentBasedStrategy
// =============================================================================
// Recommends books based on the user's own declared preferences:
//   - Preferred genres  (weight: 60%)
//   - Preferred authors (weight: 25%)
//   - Normalized avg rating of the book (weight: 15%)
//
// This is what Goodreads does when a new user first sets up their shelves —
// it doesn't know your history yet, so it uses your stated preferences.
// =============================================================================

class ContentBasedStrategy : public IRecommendationStrategy {
public:
    std::vector<Book> recommend(
        const User&                user,
        const std::vector<Book>&   books,
        const std::vector<User>&   users,
        const std::vector<Rating>& ratings,
        std::size_t                topN) const override;

    const char* name() const override { return "Content-Based"; }

private:
    double scoreBook(const User& user, const Book& book) const;
};

#endif
