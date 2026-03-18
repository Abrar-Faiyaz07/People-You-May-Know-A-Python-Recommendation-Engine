#include "RecommendationEngine.h"
#include <stdexcept>

RecommendationEngine::RecommendationEngine(
    const BookRepository&   books,
    const UserRepository&   users,
    const RatingRepository& ratings)
    : bookRepo(books), userRepo(users), ratingRepo(ratings), strategy(nullptr) {}

void RecommendationEngine::setStrategy(IRecommendationStrategy* s) {
    strategy = s;
}

const char* RecommendationEngine::activeStrategyName() const {
    return strategy ? strategy->name() : "None";
}

std::vector<Book> RecommendationEngine::recommend(const User& user, std::size_t topN) const {
    if (!strategy)
        throw std::runtime_error("No recommendation strategy set.");
    return strategy->recommend(user,
                               bookRepo.getAll(),
                               userRepo.getAll(),
                               ratingRepo.getAll(),
                               topN);
}
