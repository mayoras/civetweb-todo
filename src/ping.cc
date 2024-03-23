#include "ping.hpp"

#include "common.hpp"
#include <cJSON.h>

bool PingHandler::handleGet(CivetServer *, struct mg_connection *conn) {
    // build JSON hello obj
    cJSON *obj = cJSON_CreateObject();

    if (!obj) {
        /* insufficient memory */
        mg_send_http_error(conn, HTTP_SERVER_INTERNAL_ERROR,
                           "Could not create JSON response object");
        return HTTP_SERVER_INTERNAL_ERROR;
    }

    // log request
    const struct mg_request_info *info = mg_get_request_info(conn);

    fprintf(stdout, "PING Handler:\t\t%s %s\n", info->request_method,
            info->request_uri);

    cJSON_AddStringToObject(obj, "message", "PONG");
    cJSON_AddStringToObject(obj, "version", CIVETWEB_VERSION);
    cJSON_AddNumberToObject(obj, "request", static_cast<double>(++request));
    sendJSON(conn, obj);

    return true;
}
