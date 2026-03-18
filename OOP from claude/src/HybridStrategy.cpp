#include "HybridStrategy.h"
#include <algorithm>
#include <cmath>
#include <map>

namespace {
struct Candidate { Book book; double score; };
}

// Cold-start threshold: users with fewer than this many ratings
// get a heavier content-based weight (like Goodreads' new-user mode)
static const std::size_t COLD_START_THRESHOLD = 3;

std::vector<Book> HybridStrategy::recommend(
    const User&                user,
    const std::vector<Book>&   books,
    const std::vector<User>&   users,
    const std::vector<Rating>& /*ratings*/,
    std::size_t                topN) const
{
    bool coldStart = user.getRatedBooks().size() < COLD_START_THRESHOLD;

    // Compute max popularity for normalization
    int maxPop = 1;
    for (const Book& b : books)
        if (b.getPopularity() > maxPop) maxPop = b.getPopularity();

    // Pre-compute cosine similarities once (not per book)
    std::map<int,double> simCache;
    if (!coldStart) {
        for (const User& other : users) {
            if (other.getId() == user.getId()) continue;
            double sim = cosineSimilarity(user, other);
            if (sim > 0.0) simCache[other.getId()] = sim;
        }
    }

    std::vector<Candidate> candidates;
    candidates.reserve(books.size());

    for (const Book& b : books) {
        if (user.hasRead(b.getId()) || user.hasRated(b.getId())) continue;

        double cContent = contentScore(user, b);
        double cCollab  = coldStart ? 0.0 : collaborativeScore(user, b, users);
        double cPop     = popularityScore(b, maxPop);

        double finalScore;
        if (coldStart) {
            // New user: content 80%, popularity 20%
            finalScore = 0.80 * cContent + 0.20 * cPop;
        } else {
            // Experienced user: collaborative 45%, content 40%, popularity 15%
            finalScore = 0.45 * cCollab + 0.40 * cContent + 0.15 * cPop;
        }

        Candidate c; c.book = b; c.score = finalScore;
        candidates.push_back(c);
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

double HybridStrategy::cosineSimilarity(const User& a, const User& b) const {
    const std::map<int,int>& ra = a.getRatedBooks();
    const std::map<int,int>& rb = b.getRatedBooks();
    double dot = 0.0, magA = 0.0, magB = 0.0;
    for (const auto& kv : ra) {
        magA += (double)kv.second * kv.second;
        auto it = rb.find(kv.first);
        if (it != rb.end()) dot += (double)kv.second * it->second;
    }
    for (const auto& kv : rb) magB += (double)kv.second * kv.second;
    if (magA <= 0.0 || magB <= 0.0) return 0.0;
    return dot / (std::sqrt(magA) * std::sqrt(magB));
}

double HybridStrategy::contentScore(const User& user, const Book& book) const {
    double genreScore = 0.0;
    if (!user.getPreferredGenres().empty())
        genreScore = user.getPreferredGenres().count(book.getGenre()) ? 1.0 : 0.0;
    double authorScore = 0.0;
    if (!user.getPreferredAuthors().empty())
        authorScore = user.getPreferredAuthors().count(book.getAuthor()) ? 1.0 : 0.0;
    return 0.65 * genreScore + 0.35 * authorScore;
}

double HybridStrategy::collaborativeScore(const User& user, const Book& book,
                                          const std::vector<User>& users) const {
    double weighted = 0.0, totalSim = 0.0;
    for (const User& other : users) {
        if (other.getId() == user.getId() || !other.hasRated(book.getId())) continue;
        double sim = cosineSimilarity(user, other);
        if (sim <= 0.0) continue;
        weighted += sim * (double)other.getRatingForBook(book.getId()) / 5.0;
        totalSim += sim;
    }
    return totalSim > 0.0 ? weighted / totalSim : 0.0;
}

double HybridStrategy::popularityScore(const Book& book, int maxPopularity) const {
    if (maxPopularity <= 0) return 0.0;
    return (double)std::max(0, book.getPopularity()) / maxPopularity;
}
