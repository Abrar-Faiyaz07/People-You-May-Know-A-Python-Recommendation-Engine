#include <fstream>
#include <iostream>
#include <string>
#include "Repository.h"

int runMenu(Repository& repo,
            const std::string& booksPath,
            const std::string& usersPath,
            const std::string& ratingsPath);

namespace {
std::string exeDir(const char* argv0) {
    if (!argv0) return "";
    std::string full(argv0);
    std::size_t pos = full.find_last_of("/\\");
    return pos == std::string::npos ? "" : full.substr(0, pos + 1);
}
bool canRead(const std::string& path) {
    std::ifstream f(path.c_str()); return f.is_open();
}
std::string resolve(const std::string& base, const std::string& rel) {
    std::string p = base + rel;
    return canRead(p) ? p : (canRead(rel) ? rel : p);
}
}

int main(int argc, char* argv[]) {
    (void)argc;
    std::string base = exeDir(argv ? argv[0] : nullptr);

    const std::string booksPath   = resolve(base, "data/books.csv");
    const std::string usersPath   = resolve(base, "data/users.csv");
    const std::string ratingsPath = resolve(base, "data/ratings.csv");

    Repository repo;
    if (!repo.loadAll(booksPath, usersPath, ratingsPath)) {
        std::cerr << "Error: Could not load data files.\n"
                  << "Expected: data/books.csv, data/users.csv, data/ratings.csv\n";
        return 1;
    }

    std::cout << "Loaded " << repo.books().getAll().size()   << " books, "
                           << repo.users().getAll().size()   << " users, "
                           << repo.ratings().getAll().size() << " ratings.\n";

    return runMenu(repo, booksPath, usersPath, ratingsPath);
}
