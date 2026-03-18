#ifndef RATINGREPOSITORY_H
#define RATINGREPOSITORY_H

#include <string>
#include <vector>

#include "Rating.h"

class RatingRepository {
public:
    bool load(const std::string& path);
    bool save(const std::string& path) const;

    const std::vector<Rating>& getAll() const;
    std::vector<Rating>& getAllMutable();

    void addOrUpdate(int userId, int bookId, int score);

private:
    std::vector<Rating> ratings;
};

#endif
