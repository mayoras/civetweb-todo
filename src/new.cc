#include "new.hpp"
#include "civetweb.h"
#include "common.hpp"
#include "database.hpp"
#include "server.hpp"
#include "task.hpp"

#include <cJSON.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

static std::string readBody(struct mg_connection *conn) {
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

static std::string getContentType(struct mg_connection *conn) {
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    const struct mg_header *headers = req_info->http_headers;

    for (int i = 0; i < req_info->num_headers; ++i) {
        if (std::strcmp(headers[i].name, "Content-Type") == 0) {
            return std::string(headers[i].value);
        }
    }

    return "";
}

bool NewHandler::handlePost(CivetServer *server, struct mg_connection *conn,
                            int *status_code) {

    const struct mg_request_info *info = mg_get_request_info(conn);
    fprintf(stdout, "NEW Handler:\t\t%s %s\n", info->request_method,
            info->request_uri);

    Database *db = dynamic_cast<Server *>(server)->getDatabase();
    std::string body;

    cJSON *json = nullptr;

    std::string title;
    std::string desc;

    Task task;

    // check req body is JSON
    const std::string contentType = getContentType(conn);
    if (contentType.empty() || contentType != "application/json") {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "Content Type is not JSON");
        goto end;
    }

    printf(">>> Content Type checked.\n");

    // parse the JSON and gather task info
    body = readBody(conn);

    if (body.empty() || body == "" || body.length() == 0) {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "Body is empty.");
        goto end;
    }

    printf(">>> body checked.\n");

    std::cout << body << std::endl;

    json = cJSON_Parse(body.c_str());

    if (json == NULL) {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "JSON object is invalid");
        goto end;
    }

    printf(">>> body parsed.\n");

    // instantiate a new Task
    task = Task(json);

    // Save task
    db->insertTask(task);

    printf(">>> task inserted.\n");
end:
    cJSON_Delete(json);
    return true;
}
