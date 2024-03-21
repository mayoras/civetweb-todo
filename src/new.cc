#include "new.hpp"
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

bool NewHandler::handlePost(CivetServer *, struct mg_connection *conn) {

    return true;
}
