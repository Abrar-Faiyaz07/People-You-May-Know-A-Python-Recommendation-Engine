#ifndef BOOK_H
#define BOOK_H

#include <iosfwd>
#include <string>

class Book {
public:
    Book();
    Book(int id, const std::string& title, const std::string& author,
         const std::string& genre, int year, double avgRating,
         int ratingCount, int popularity);

    int getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    const std::string& getGenre() const;
    int getYear() const;
    double getAvgRating() const;
    int getRatingCount() const;
    int getPopularity() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setGenre(const std::string& genre);
    void setYear(int year);
    void setPopularity(int popularity);

    bool isValid() const;
    void applyNewRating(int score);
    void applyRatingUpdate(int oldScore, int newScore);
    bool operator==(const Book& other) const;
    bool operator<(const Book& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Book& book);

private:
    int id;
    std::string title;
    std::string author;
    std::string genre;
    int year;
    double avgRating;
    int ratingCount;
    int popularity;
};

#endif
