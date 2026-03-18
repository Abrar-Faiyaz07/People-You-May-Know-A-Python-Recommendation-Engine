#include "CollaborativeStrategy.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

namespace {
struct Candidate { Book book; double score; };
}

// =============================================================================
// Cosine Similarity:  cosine(A, B) = (A · B) / (|A| * |B|)
//
// A and B are rating vectors over shared books.
// Each component is the user's normalized rating (score / 5.0).
// This is magnitude-normalized — a user who gives all 5s doesn't appear
// artificially similar to everyone, which was the flaw in the original
// simple-overlap approach.
// =============================================================================

double CollaborativeStrategy::cosineSimilarity(const User& a, const User& b) const {
    const std::map<int,int>& ra = a.getRatedBooks();
    const std::map<int,int>& rb = b.getRatedBooks();

    double dot    = 0.0;
    double magA   = 0.0;
    double magB   = 0.0;

    // Dot product over shared books only
    for (const auto& kv : ra) {
        magA += (double)kv.second * kv.second;
        auto it = rb.find(kv.first);
        if (it != rb.end())
            dot += (double)kv.second * it->second;
    }
    for (const auto& kv : rb)
        magB += (double)kv.second * kv.second;

    if (magA <= 0.0 || magB <= 0.0) return 0.0;
    return dot / (std::sqrt(magA) * std::sqrt(magB));
}

double CollaborativeStrategy::predictedScore(
    const User&              target,
    int                      bookId,
    const std::vector<User>& users,
    const std::map<int,double>& simCache) const
{
    double weighted = 0.0;
    double totalSim = 0.0;

    for (const User& other : users) {
        if (other.getId() == target.getId()) continue;
        if (!other.hasRated(bookId)) continue;

        auto it = simCache.find(other.getId());
        double sim = (it != simCache.end()) ? it->second : 0.0;
        if (sim <= 0.0) continue;

        weighted += sim * (double)other.getRatingForBook(bookId) / 5.0;
        totalSim += sim;
    }

    return totalSim > 0.0 ? weighted / totalSim : 0.0;
}

std::vector<Book> CollaborativeStrategy::recommend(
    const User&                user,
    const std::vector<Book>&   books,
    const std::vector<User>&   users,
    const std::vector<Rating>& /*ratings*/,
    std::size_t                topN) const
{
    // Step 1: Pre-compute cosine similarity between target user and all others
    // This avoids O(books * users) — we compute it once and cache it
    std::map<int,double> simCache;
    for (const User& other : users) {
        if (other.getId() == user.getId()) continue;
        double sim = cosineSimilarity(user, other);
        if (sim > 0.0) simCache[other.getId()] = sim;
    }

    // Step 2: Collect candidate books not yet read/rated by the target user
    // and compute predicted scores
    std::vector<Candidate> candidates;
    candidates.reserve(books.size());

    for (const Book& b : books) {
        if (user.hasRead(b.getId()) || user.hasRated(b.getId())) continue;
        double score = predictedScore(user, b.getId(), users, simCache);
        // Only include books that at least one similar user rated
        if (score > 0.0) {
            Candidate c; c.book = b; c.score = score;
            candidates.push_back(c);
        }
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            return a.score != b.score ? a.score > b.score
                                      : a.book.getAvgRating() > b.book.getAvgRating();
        });

    std::vector<Book> result;
    result.reserve(topN);
    for (std::size_t i = 0; i < candidates.size() && result.size() < topN; ++i)
        result.push_back(candidates[i].book);
    return result;
}
