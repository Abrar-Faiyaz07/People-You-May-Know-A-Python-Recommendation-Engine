#ifndef RECOMMENDATIONENGINE_H
#define RECOMMENDATIONENGINE_H

#include "IRecommendationStrategy.h"
#include "BookRepository.h"
#include "UserRepository.h"
#include "RatingRepository.h"

// =============================================================================
// RecommendationEngine  —  Strategy Pattern host
// =============================================================================
// ORIGINAL PROBLEM: The engine did everything itself — mixed content-based,
// collaborative, and popularity logic with hardcoded weights in one class.
//
// FIX: The engine is now a thin wrapper that:
//   1. Holds a pointer to any IRecommendationStrategy
//   2. Delegates all scoring logic to the strategy
//   3. Can hot-swap algorithms at runtime (setStrategy)
//
// The engine also holds references to the data repos so strategies don't need
// to receive giant vectors as parameters on every call.
// =============================================================================

class RecommendationEngine {
public:
    // Inject repositories so strategies can access data directly
    RecommendationEngine(const BookRepository&   books,
                         const UserRepository&   users,
                         const RatingRepository& ratings);

    // Set or swap the active strategy at runtime
    void setStrategy(IRecommendationStrategy* strategy);

    const char* activeStrategyName() const;

    // Get top-N recommendations for a user using the active strategy
    std::vector<Book> recommend(const User& user, std::size_t topN) const;

private:
    const BookRepository&   bookRepo;
    const UserRepository&   userRepo;
    const RatingRepository& ratingRepo;
    IRecommendationStrategy* strategy;  // owned externally (no delete)
};

#endif
