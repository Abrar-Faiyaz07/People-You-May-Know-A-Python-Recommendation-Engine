#ifndef RATING_H
#define RATING_H

class Rating {
public:
    Rating();
    Rating(int userId, int bookId, int score);

    int getUserId() const;
    int getBookId() const;
    int getScore() const;

    void setScore(int score);
    bool isValid() const;

private:
    int userId;
    int bookId;
    int score;
};

#endif
