#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "database.hpp"
#include <CivetServer.h>

class Server : public CivetServer {
  private:
    Database *db;

  public:
    Server(const char *options[], struct CivetCallbacks *callbacks,
           const std::string &dbFilePath)
        : CivetServer(options, callbacks) {
        this->db = new Database(dbFilePath);
    }

    ~Server() { delete this->db; }

    inline Database *getDatabase() const { return this->db; }
};

#endif /* _SERVER_HPP */
