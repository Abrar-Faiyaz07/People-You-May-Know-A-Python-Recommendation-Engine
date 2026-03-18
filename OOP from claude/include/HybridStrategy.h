#ifndef HYBRIDSTRATEGY_H
#define HYBRIDSTRATEGY_H

#include "IRecommendationStrategy.h"

// =============================================================================
// HybridStrategy  —  Weighted Hybrid (Content + Collaborative + Popularity)
// =============================================================================
// Real recommendation systems (Amazon, Goodreads) use hybrid approaches:
//   - For new users with no ratings: fall back to content-based
//   - For experienced users: blend collaborative signal with content signal
//   - Popularity is a tie-breaker, not a primary signal
//
// WEIGHTS (tunable):
//   - Content-based score:       40%
//   - Collaborative score:       45%
//   - Popularity (normalized):   15%
//
// Cold-start detection: if user has <3 ratings, automatically shifts
// weight toward content-based (like Goodreads' "new user" mode).
// =============================================================================

class HybridStrategy : public IRecommendationStrategy {
public:
    std::vector<Book> recommend(
        const User&                user,
        const std::vector<Book>&   books,
        const std::vector<User>&   users,
        const std::vector<Rating>& ratings,
        std::size_t                topN) const override;

    const char* name() const override { return "Hybrid (Content + Collaborative)"; }

private:
    double contentScore(const User& user, const Book& book) const;
    double collaborativeScore(const User& user, const Book& book,
                              const std::vector<User>& users) const;
    double popularityScore(const Book& book, int maxPopularity) const;
    double cosineSimilarity(const User& a, const User& b) const;
};

#endif
