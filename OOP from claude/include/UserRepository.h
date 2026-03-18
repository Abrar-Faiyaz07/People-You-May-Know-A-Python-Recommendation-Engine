#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <map>
#include <string>
#include <vector>

#include "User.h"

class UserRepository {
public:
    bool load(const std::string& path);
    bool save(const std::string& path) const;

    const std::vector<User>& getAll() const;
    std::vector<User>& getAllMutable();

    const User* findById(int id) const;
    User* findByIdMutable(int id);

    User& createUser(const std::string& name, const std::string& password);

private:
    std::vector<User> users;
    std::map<int, std::size_t> indexById;
    int nextId = 1;

    void rebuildIndex();
    void recomputeNextId();
};

#endif
