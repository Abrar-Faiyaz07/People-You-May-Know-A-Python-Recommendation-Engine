#include "ContentBasedStrategy.h"
#include <algorithm>
#include <vector>

namespace {
struct Candidate { Book book; double score; };
}

std::vector<Book> ContentBasedStrategy::recommend(
    const User&                user,
    const std::vector<Book>&   books,
    const std::vector<User>&   /*users*/,
    const std::vector<Rating>& /*ratings*/,
    std::size_t                topN) const
{
    std::vector<Candidate> candidates;
    candidates.reserve(books.size());

    for (const Book& b : books) {
        if (user.hasRead(b.getId()) || user.hasRated(b.getId())) continue;
        Candidate c;
        c.book  = b;
        c.score = scoreBook(user, b);
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

double ContentBasedStrategy::scoreBook(const User& user, const Book& book) const {
    // Genre match: 60%
    double genreScore = 0.0;
    if (!user.getPreferredGenres().empty())
        genreScore = user.getPreferredGenres().count(book.getGenre()) ? 1.0 : 0.0;

    // Author match: 25%
    double authorScore = 0.0;
    if (!user.getPreferredAuthors().empty())
        authorScore = user.getPreferredAuthors().count(book.getAuthor()) ? 1.0 : 0.0;

    // Normalized avg rating: 15%
    double ratingScore = book.getAvgRating() / 5.0;

    return 0.60 * genreScore + 0.25 * authorScore + 0.15 * ratingScore;
}
