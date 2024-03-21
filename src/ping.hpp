#ifndef _PING_HPP
#define _PING_HPP

#include "CivetServer.h"

class PingHandler : public CivetHandler {
  public:
    unsigned int request = 0;

    bool handleGet(CivetServer *, struct mg_connection *conn);
};

#endif /* _PING_HPP */
