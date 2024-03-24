#include "complete.hpp"
#include "cJSON.h"
#include "common.hpp"
#include "database.hpp"
#include "server.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>

bool CompleteHandler::handlePut(CivetServer *server,
                                struct mg_connection *conn) {
    const struct mg_request_info *info = mg_get_request_info(conn);
    fprintf(stdout, "COMPLETE Handler:\t\t%s %s\n", info->request_method,
            info->request_uri);

    Database *db = dynamic_cast<Server *>(server)->getDatabase();
    std::string body;
    unsigned int taskId;

    cJSON *json = NULL;
    cJSON *resp_json = NULL;
    cJSON *item = NULL;

    Task *t = nullptr;

    // check req body is JSON
    const std::string contentType = getContentType(conn);
    if (contentType.empty() || contentType != "application/json") {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "Content Type is not JSON");
        goto end;
    }

    // parse the JSON and gather task info
    body = readBody(conn);

    printf(">> Body read\n");

    if (body.empty() || body == "" || body.length() == 0) {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "Body is empty.");
        goto end;
    }

    json = cJSON_Parse(body.c_str());

    if (json == NULL) {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "JSON object is invalid");
        goto end;
    }

    printf(">> JSON parsed\n");

    // verify task id
    item = cJSON_GetObjectItem(json, "task_id");
    if (item == NULL) {
        mg_send_http_error(conn, HTTP_BAD_REQUEST, "JSON object is invalid");
        goto end;
    }

    taskId = (unsigned int)cJSON_GetNumberValue(item);

    std::cout << "Task ID requested is " << taskId << std::endl;

    // find task by ID
    t = db->findTaskById(taskId);

    if (t == NULL) {
        mg_send_http_error(conn, HTTP_SERVER_INTERNAL_ERROR,
                           "Couldn't fetch task");
        goto end;
    }

    // mark as complete
    t->setCompleted(true);

    // write to DB
    if (!db->updateTask(taskId, *t)) {
        mg_send_http_error(conn, HTTP_SERVER_INTERNAL_ERROR,
                           "Couldn't update task");
        goto end;
    }

    // return success message
    resp_json = cJSON_CreateObject();
    if (resp_json == NULL) {
        mg_send_http_error(conn, HTTP_SERVER_INTERNAL_ERROR,
                           "Couldn't generate response body.");
        goto end;
    }

    cJSON_AddNumberToObject(resp_json, "rows_changed", 1.0);
    sendJSON(conn, resp_json);
end:
    delete t;
    cJSON_Delete(json);
    return true;
}
