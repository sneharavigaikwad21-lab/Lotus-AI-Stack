#ifndef AGNI_FIREBASE_AUTH_H
#define AGNI_FIREBASE_AUTH_H

#include <string>
#include <map>

struct FirebaseUser {
    std::string uid;
    std::string email;
    std::string tier;
};

class FirebaseAuth {
public:
    FirebaseAuth();
    ~FirebaseAuth();

    bool verify_token(const std::string& token, FirebaseUser& user);
    bool validate_quota(const std::string& uid, const std::string& tier);

private:
    std::map<std::string, FirebaseUser> user_cache;
};

#endif // AGNI_FIREBASE_AUTH_H
