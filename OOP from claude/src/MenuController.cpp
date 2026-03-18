#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "RecommendationEngine.h"
#include "Repository.h"
#include "SearchService.h"
#include "AppExceptions.h"
#include "HybridStrategy.h"
#include "BookFilter.h"

// =============================================================================
// MenuController  —  Proper class instead of 350 lines of anonymous functions
// =============================================================================
// ORIGINAL PROBLEM: All logic lived in an anonymous namespace as free functions.
// That's procedural C, not OOP. No class, no state, no extensibility.
//
// FIX: MenuController is a class that owns the engine and the search service.
// Each menu action is a named private method — readable, testable, and
// easy to extend without touching other functions.
// =============================================================================

class MenuController {
public:
    MenuController(Repository& repo,
                   const std::string& booksPath,
                   const std::string& usersPath,
                   const std::string& ratingsPath)
        : repo(repo),
          booksPath(booksPath), usersPath(usersPath), ratingsPath(ratingsPath),
          engine(repo.books(), repo.users(), repo.ratings())
    {
        // Default to hybrid — best for users who already have some ratings
        engine.setStrategy(&hybridStrat);
    }

    int run() {
        while (true) {
            printMainMenu();
            int choice = readInt("Choose option: ");
            try {
                switch (choice) {
                    case 1: viewAllBooks();        break;
                    case 2: searchBooks();         break;
                    case 3: rateBook();            break;
                    case 4: getRecommendations();  break;
                    case 5: return exitAndSave();
                    default: std::cout << "Invalid option.\n";
                }
            } catch (const ProjectException& ex) {
                std::cout << "Operation failed: " << ex.what() << "\n";
            }
        }
    }

private:
    Repository&       repo;
    std::string       booksPath, usersPath, ratingsPath;
    SearchService     search;
    HybridStrategy        hybridStrat;
    RecommendationEngine  engine;

    // -------------------------------------------------------------------------
    // Menu display
    // -------------------------------------------------------------------------
    void printMainMenu() const {
        std::cout << "\n========================================\n";
        std::cout << "       Book Recommendation System\n";
        std::cout << "========================================\n";
        std::cout << "1. View all books\n";
        std::cout << "2. Search / filter books\n";
        std::cout << "3. Rate a book\n";
        std::cout << "4. Get recommendations\n";
        std::cout << "5. Exit (save & quit)\n";
        std::cout << "========================================\n";
    }

    // -------------------------------------------------------------------------
    // 1. View all books
    // -------------------------------------------------------------------------
    void viewAllBooks() const {
        const std::vector<Book>& books = repo.books().getAll();
        std::cout << "\nAll books (" << books.size() << " total):\n";
        printBookList(books, false);
    }

    // -------------------------------------------------------------------------
    // 2. Search & filter (uses SearchService — now actually connected to menu!)
    // -------------------------------------------------------------------------
    void searchBooks() const {
        std::cout << "\n--- Search & Filter ---\n";
        std::cout << "1. Keyword search (title/author)\n";
        std::cout << "2. Filter by genre\n";
        std::cout << "3. Filter by min rating + genre (combined)\n";
        std::cout << "4. Top-rated books\n";
        std::cout << "5. Newest books\n";
        int choice = readInt("Choose: ");

        const std::vector<Book>& allBooks = repo.books().getAll();
        std::vector<Book> results;

        if (choice == 1) {
            std::string kw = readLine("Enter keyword: ");
            results = search.searchByKeyword(allBooks, kw);
        } else if (choice == 2) {
            std::string genre = readLine("Enter genre: ");
            results = search.filterByGenre(allBooks, genre);
        } else if (choice == 3) {
            // Demonstrates FilterChain (Composite Pattern)
            std::string genre = readLine("Enter genre: ");
            double minRating  = readDouble("Enter minimum rating (e.g. 4.0): ");
            FilterChain chain;
            chain.addFilter<GenreFilter>(genre);
            chain.addFilter<MinRatingFilter>(minRating);
            results = search.applyChain(allBooks, chain);
            search.sortByRatingDesc(results);
        } else if (choice == 4) {
            results = allBooks;
            search.sortByRatingDesc(results);
            if (results.size() > 20) results.resize(20);
        } else if (choice == 5) {
            results = allBooks;
            search.sortByYearDesc(results);
            if (results.size() > 20) results.resize(20);
        } else {
            std::cout << "Invalid option.\n"; return;
        }

        std::cout << "\nFound " << results.size() << " book(s):\n";
        printBookList(results, false);
    }

    // -------------------------------------------------------------------------
    // 3. Rate a book
    // -------------------------------------------------------------------------
    void rateBook() {
        User* user = selectOrSignUp();
        if (!user) return;

        const std::vector<Book>& books = repo.books().getAll();
        if (books.empty()) { std::cout << "No books available.\n"; return; }

        while (true) {
            std::cout << "1. View books  2. Pick book to rate  3. Cancel\n";
            int action = readInt("Choose: ");
            if (action == 1) { printBookList(books, false); continue; }
            if (action == 3) { std::cout << "Cancelled.\n"; return; }
            if (action != 2) { std::cout << "Invalid.\n"; continue; }

            int num = readInt("Enter book number (1-" + std::to_string(books.size()) + "): ");
            if (num < 1 || (std::size_t)num > books.size()) {
                std::cout << "Out of range.\n"; continue;
            }

            int bookId = books[(std::size_t)(num - 1)].getId();
            Book* book = repo.books().findByIdMutable(bookId);
            if (!book) { std::cout << "Book not found.\n"; continue; }

            int score = readScore("Rate this book (1-5): ");
            bool hadPreviousRating = user->hasRated(bookId);
            int previousScore = user->getRatingForBook(bookId);

            user->addOrUpdateRating(bookId, score);
            repo.ratings().addOrUpdate(user->getId(), bookId, score);

            if (hadPreviousRating) {
                book->applyRatingUpdate(previousScore, score);
            } else {
                book->applyNewRating(score);
            }

            std::cout << "Rating saved! Changes will persist on exit.\n";
            break;
        }
    }

    // -------------------------------------------------------------------------
    // 4. Get recommendations (uses the active strategy via engine)
    // -------------------------------------------------------------------------
    void getRecommendations() {
        User* user = selectOrSignUp();
        if (!user) return;

        collectPreferenceInput(*user);
        std::cout << "\nGenerating recommendations...\n";

        std::vector<Book> recs = engine.recommend(*user, 10);

        // Optional genre filter on top of recommendations
        std::string wantedGenre = readLine("Filter by genre? (Enter to skip): ");
        if (!wantedGenre.empty()) {
            std::string lw = toLower(wantedGenre);
            std::vector<Book> filtered;
            for (const Book& b : recs)
                if (toLower(b.getGenre()) == lw) filtered.push_back(b);
            recs = filtered;
        }

        if (recs.empty()) {
            std::cout << "No recommendations yet. Try adding ratings or preferences first.\n";
            return;
        }

        std::cout << "\nTop " << recs.size() << " recommendations for "
                  << user->getName() << ":\n";
        printBookList(recs, false);
    }

    // -------------------------------------------------------------------------
    // 5. Exit and save
    // -------------------------------------------------------------------------
    int exitAndSave() {
        bool ok = repo.saveAll(booksPath, usersPath, ratingsPath);
        if (!ok) { std::cout << "Warning: failed to save some files.\n"; return 1; }
        std::cout << "All data saved. Goodbye!\n";
        return 0;
    }

    // -------------------------------------------------------------------------
    // Helpers
    // -------------------------------------------------------------------------
    User* selectOrSignUp() {
        int id = readInt("Enter your user ID (0 to register): ");
        if (id == 0) {
            std::string name = readLine("Your name: ");
            std::string pw   = readLine("Set password: ");
            User& u = repo.users().createUser(name, pw);
            std::cout << "Registered! Your user ID is: " << u.getId() << "\n";
            return &u;
        }
        User* user = repo.users().findByIdMutable(id);
        if (!user) { std::cout << "User not found.\n"; return nullptr; }
        if (!user->getPassword().empty()) {
            std::string pw = readLine("Password: ");
            if (!user->checkPassword(pw)) {
                std::cout << "Wrong password.\n"; return nullptr;
            }
        }
        return user;
    }

    void collectPreferenceInput(User& user) {
        std::cout << "\nRecommendation inputs\n";
        std::string genres  = readLine("Enter preferred genres (semicolon-separated, Enter to skip): ");
        std::string authors = readLine("Enter preferred authors (semicolon-separated, Enter to skip): ");

        bool changed = false;
        for (const std::string& g : tokenize(genres, ';')) {
            changed = user.addPreferredGenre(g) || changed;
        }
        for (const std::string& a : tokenize(authors, ';')) {
            changed = user.addPreferredAuthor(a) || changed;
        }

        if (changed) {
            std::cout << "Using these saved preferences for recommendation:\n";
            printPreferenceSummary(user);
        }
    }

    void printBookList(const std::vector<Book>& books, bool showRating) const {
        if (books.empty()) { std::cout << "  (none)\n"; return; }
        std::cout << "  "
                  << std::left << std::setw(4) << "#"
                  << " | " << std::setw(30) << "Title"
                  << " | " << std::setw(20) << "Author"
                  << " | " << std::setw(15) << "Genre"
                  << " | " << std::setw(6) << "Year";
        if (showRating) {
            std::cout << " | " << std::setw(6) << "Rating";
        }
        std::cout << "\n";
        std::cout << "  "
                  << std::setw(4) << "---"
                  << "-+-" << std::setw(30) << "------------------------------"
                  << "-+-" << std::setw(20) << "--------------------"
                  << "-+-" << std::setw(15) << "---------------"
                  << "-+-" << std::setw(6) << "------";
        if (showRating) {
            std::cout << "-+-" << std::setw(6) << "------";
        }
        std::cout << "\n";
        for (std::size_t i = 0; i < books.size(); ++i) {
            const Book& b = books[i];
            std::string title = b.getTitle();
            if (title.size() > 30) title = title.substr(0, 27) + "...";
            std::string author = b.getAuthor();
            if (author.size() > 20) author = author.substr(0, 17) + "...";
            std::string genre = b.getGenre();
            if (genre.size() > 15) genre = genre.substr(0, 12) + "...";
            std::cout << "  "
                      << std::left << std::setw(4) << (i + 1)
                      << " | " << std::setw(30) << title
                      << " | " << std::setw(20) << author
                      << " | " << std::setw(15) << genre
                      << " | " << std::setw(6) << b.getYear();
            if (showRating) {
                std::ostringstream rating;
                rating << std::fixed << std::setprecision(2) << b.getAvgRating();
                std::cout << " | " << std::setw(6) << rating.str();
            }
            std::cout << "\n";
        }
    }

    void printPreferenceSummary(const User& user) const {
        std::cout << "Preferred genres : ";
        printStringSet(user.getPreferredGenres());
        std::cout << "Preferred authors: ";
        printStringSet(user.getPreferredAuthors());
    }

    static void printStringSet(const std::set<std::string>& values) {
        if (values.empty()) {
            std::cout << "(none)\n";
            return;
        }

        bool first = true;
        for (std::set<std::string>::const_iterator it = values.begin(); it != values.end(); ++it) {
            if (!first) {
                std::cout << ", ";
            }
            first = false;
            std::cout << *it;
        }
        std::cout << "\n";
    }

    static int readInt(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            int v;
            if (std::cin >> v) return v;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number.\n";
        }
    }

    static double readDouble(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            double v;
            if (std::cin >> v) return v;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number.\n";
        }
    }

    static std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        // Flush any leftover newline from a previous cin>> call
        if (std::cin.peek() == '\n') std::cin.ignore();
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    static int readScore(const std::string& prompt) {
        while (true) {
            int s = readInt(prompt);
            if (s >= 1 && s <= 5) return s;
            std::cout << "Score must be 1-5.\n";
        }
    }

    static std::vector<std::string> tokenize(const std::string& s, char delim) {
        std::vector<std::string> tokens;
        std::string token;
        for (char c : s) {
            if (c == delim) { if (!token.empty()) tokens.push_back(token); token.clear(); }
            else token.push_back(c);
        }
        if (!token.empty()) tokens.push_back(token);
        return tokens;
    }

    static std::string toLower(const std::string& s) {
        std::string out;
        for (char c : s) out.push_back((char)std::tolower((unsigned char)c));
        return out;
    }
};

// Entry point called from main.cpp
int runMenu(Repository& repo,
            const std::string& booksPath,
            const std::string& usersPath,
            const std::string& ratingsPath) {
    MenuController ctrl(repo, booksPath, usersPath, ratingsPath);
    return ctrl.run();
}

