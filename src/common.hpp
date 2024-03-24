#ifndef _COMMON_H
#define _COMMON_H

#include <cJSON.h>
#include <string>

#define HTTP_OK 200
#define HTTP_NO_CONTENT 204
#define HTTP_SERVER_INTERNAL_ERROR 500
#define HTTP_BAD_REQUEST 400

int sendJSON(struct mg_connection *&conn, cJSON *&obj);
std::string getContentType(struct mg_connection *conn);
std::string readBody(struct mg_connection *conn);

#endif /* _COMMON_H */
