#ifndef _COMPLETE_HPP
#define _COMPLETE_HPP

#include <CivetServer.h>
#include <cJSON.h>

class CompleteHandler : public CivetHandler {
  public:
    // should be static, but suppose it can only exist one handler for each
    // endpoint
    unsigned int nextId = 0;

    bool handlePut(CivetServer *server, struct mg_connection *conn);

    inline unsigned int getNextId() { return this->nextId++; }
};

#endif /* _COMPLETE_HPP */
