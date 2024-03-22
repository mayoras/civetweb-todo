#include "task.hpp"
#include "cJSON.h"

Task::Task(const std::string &title, const std::string &description)
    : completed{false} {
    this->m_title = title;
    this->m_description = description;
}

Task::Task(const cJSON *json) : Task("", "") {
    this->m_title = cJSON_GetObjectItem(json, "title")->valuestring;
    this->m_description = cJSON_GetObjectItem(json, "description")->valuestring;
}

cJSON *Task::getJSON() const {
    cJSON *taskObj = cJSON_CreateObject();

    cJSON_AddStringToObject(taskObj, "title", this->m_title.c_str());
    cJSON_AddStringToObject(taskObj, "description",
                            this->m_description.c_str());
    cJSON_AddBoolToObject(taskObj, "isCompleted", isCompleted());

    return taskObj;
}
