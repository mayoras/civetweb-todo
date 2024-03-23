#ifndef _COMMON_H
#define _COMMON_H

#include <cJSON.h>

#define HTTP_OK 200
#define HTTP_NO_CONTENT 204
#define HTTP_SERVER_INTERNAL_ERROR 500
#define HTTP_BAD_REQUEST 400

int sendJSON(struct mg_connection *&conn, cJSON *&obj);

#endif /* _COMMON_H */
