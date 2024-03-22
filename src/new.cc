#include "new.hpp"
#include "common.hpp"
#include "database.hpp"
#include "server.hpp"
#include "task.hpp"

#include <cJSON.h>
#include <cstring>
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

    Database *db = dynamic_cast<Server *>(server)->getDatabase();
    std::string body;
    cJSON *json = nullptr;

    std::string title;
    std::string desc;

    Task task;

    // check req body is JSON
    const std::string contentType = getContentType(conn);
    if (contentType.empty() || contentType != "application/json") {
        *status_code = HTTP_BAD_REQUEST;
        goto end;
    }

    // parse the JSON and gather task info
    body = readBody(conn);
    json = cJSON_Parse(body.c_str());

    if (json == NULL) {
        *status_code = HTTP_SERVER_INTERNAL_ERROR;
        goto end;
    }

    // instantiate a new Task
    task = Task(json);

    // Save task
    db->insertTask(task);
end:
    cJSON_Delete(json);

    return true;
}
