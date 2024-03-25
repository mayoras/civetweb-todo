#include "api.hpp"
#include <iostream>

char *getDatabasePath() {
    // check environment variable
    char *env = std::getenv("CIVETWEB_DATABASE_PATH");

    if (env == NULL) {
        // fallback to default database path
        std::cerr << "Falling back to default Database path: "
                  << DEFAULT_DATABASE_PATH << std::endl;
        return (char *)DEFAULT_DATABASE_PATH;
    }

    return env;
}

char *getDocumentRoot() {
    // check environment variable
    char *env = std::getenv("CIVETWEB_DOCUMENT_ROOT");

    if (env == NULL) {
        // fallback to default document root
        std::cerr << "Falling back to default document root: "
                  << DEFAULT_DOCUMENT_ROOT << std::endl;
        return (char *)DEFAULT_DOCUMENT_ROOT;
    }

    return env;
}
char *getPort() {
    // check environment variable
    char *env = std::getenv("CIVETWEB_PORT");

    if (env == NULL) {
        // fallback to default port
        std::cerr << "Falling back to default port: " << DEFAULT_PORT
                  << std::endl;
        return (char *)DEFAULT_PORT;
    }

    return env;
}
