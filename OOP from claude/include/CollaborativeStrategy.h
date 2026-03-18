#ifndef COLLABORATIVESTRATEGY_H
#define COLLABORATIVESTRATEGY_H

#include <map>
#include "IRecommendationStrategy.h"

// =============================================================================
// CollaborativeStrategy  —  Cosine Similarity Collaborative Filtering
// =============================================================================
// "Users who rated similar books to you also loved X."
// This is the core algorithm of Netflix, Spotify, and Amazon.
//
// ALGORITHM:
//   1. Build a rating vector for each user  (bookId -> score)
//   2. For each other user, compute cosine similarity with target user:
//        cosine(A, B) = (A · B) / (|A| * |B|)
//      where A · B = sum of (ratingA[book] * ratingB[book]) for shared books
//   3. For each unread candidate book, compute a weighted predicted score:
//        predicted = sum(similarity[u] * rating[u][book]) / sum(similarity[u])
//   4. Rank candidates by predicted score descending
//
// WHY COSINE OVER SIMPLE OVERLAP:
//   Simple overlap (your original approach) doesn't normalize for user rating
//   style — a user who always gives 5s looks "similar" to everyone.
//   Cosine similarity is direction-aware and magnitude-normalized.
// =============================================================================

class CollaborativeStrategy : public IRecommendationStrategy {
public:
    std::vector<Book> recommend(
        const User&                user,
        const std::vector<Book>&   books,
        const std::vector<User>&   users,
        const std::vector<Rating>& ratings,
        std::size_t                topN) const override;

    const char* name() const override { return "Collaborative Filtering (Cosine)"; }

private:
    // Cosine similarity between two users' rating vectors
    double cosineSimilarity(const User& a, const User& b) const;

    // Predicted score for a book based on similar users' ratings
    double predictedScore(const User&              target,
                          int                      bookId,
                          const std::vector<User>& users,
                          const std::map<int,double>& similarityCache) const;
};

#endif
