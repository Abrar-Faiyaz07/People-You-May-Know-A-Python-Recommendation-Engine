#include "Rating.h"

Rating::Rating() : userId(0), bookId(0), score(0) {}

Rating::Rating(int userId, int bookId, int score)
    : userId(userId), bookId(bookId), score(score) {}

int Rating::getUserId() const { return userId; }
int Rating::getBookId() const { return bookId; }
int Rating::getScore() const { return score; }

void Rating::setScore(int value) {
    if (value >= 1 && value <= 5) {
        score = value;
    }
}

bool Rating::isValid() const {
    return userId > 0 && bookId > 0 && score >= 1 && score <= 5;
}
