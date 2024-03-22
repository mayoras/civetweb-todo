#ifndef _NEW_HPP
#define _NEW_HPP

#include <CivetServer.h>

class NewHandler : public CivetHandler {
  public:
    // should be static, but suppose it can only exist one handler for each
    // endpoint
    unsigned int request = 0;

    bool handlePost(CivetServer *server, struct mg_connection *conn,
                    int *status_code);
};

#endif /* _NEW_HPP */
