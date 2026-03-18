#ifndef IRECOMMENDATIONSTRATEGY_H
#define IRECOMMENDATIONSTRATEGY_H

#include <vector>
#include "Book.h"
#include "User.h"
#include "Rating.h"

// =============================================================================
// IRecommendationStrategy  —  Strategy Pattern
// =============================================================================
// ORIGINAL PROBLEM: All recommendation logic (content-based, collaborative,
// popularity) was hardcoded into ONE class with magic weights (0.5, 0.2, 0.1).
// Impossible to swap or extend algorithms without touching the core class.
//
// FIX: Abstract strategy interface. Each algorithm is its own class.
// RecommendationEngine holds a pointer to a strategy and delegates to it.
//
// Real systems (Goodreads, Netflix, Amazon) use exactly this pattern to A/B
// test different algorithms without changing the recommendation pipeline.
// =============================================================================

class IRecommendationStrategy {
public:
    virtual ~IRecommendationStrategy() {}

    virtual std::vector<Book> recommend(
        const User&              user,
        const std::vector<Book>& books,
        const std::vector<User>& users,
        const std::vector<Rating>& ratings,
        std::size_t              topN) const = 0;

    virtual const char* name() const = 0;
};

#endif
