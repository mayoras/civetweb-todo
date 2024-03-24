#include <CivetServer.h>
#include <cJSON.h>
#include <cstring>

#include "common.hpp"

// writes JSON object into connection response
int sendJSON(struct mg_connection *&conn, cJSON *&obj) {
    // Stringify JSON
    char *json_str = cJSON_PrintUnformatted(obj);
    size_t json_str_len = strlen(json_str);

    /* Send HTTP message header (+1 for \n) */
    mg_send_http_ok(conn, "application/json; charset=utf-8", json_str_len + 1);

    /* Send HTTP message content */
    mg_write(conn, json_str, json_str_len);

    /* Add a newline. This is not required, but the result is more
     * human-readable in a debugger. */
    mg_write(conn, "\n", 1);

    /* Free string allocated by cJSON_Print* */
    cJSON_free(json_str);

    return (int)json_str_len;
}

std::string getContentType(struct mg_connection *conn) {
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    const struct mg_header *headers = req_info->http_headers;

    for (int i = 0; i < req_info->num_headers; ++i) {
        if (std::strcmp(headers[i].name, "Content-Type") == 0) {
            return std::string(headers[i].value);
        }
    }

    return "";
}

std::string readBody(struct mg_connection *conn) {
    const struct mg_request_info *req_info = mg_get_request_info(conn);
    long long rlen, wlen;
    long long nlen = 0;
    long long tlen = req_info->content_length;

    std::string body;
    char buf[1024];

    // read all content
    while (nlen < tlen) {
        // total left
        rlen = tlen - nlen;

        if (rlen > sizeof(buf)) {
            rlen = sizeof(buf);
        }

        rlen = mg_read(conn, buf, (size_t)sizeof(buf));

        // EOF
        if (rlen <= 0) {
            break;
        }

        // write to vector
        long long prev_size = (long long)body.size();
        body.append(buf);

        // check if content written match the content read
        wlen = (long long)body.size() - prev_size;
        if (wlen != rlen) {
            // remove garbage character leftovers
            body = body.substr(0, body.length() - abs((int)(wlen - rlen)));
            break;
        }

        nlen += wlen;
    }

    return body;
}
