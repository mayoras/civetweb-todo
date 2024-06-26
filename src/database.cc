#include "database.hpp"
#include "cJSON.h"
#include "task.hpp"
#include <cstdlib>
#include <iostream>

static long getFileSize(std::fstream &s) {
    long len = 0;
    if (s) {
        s.seekg(0, s.end);
        len = s.tellg();
        s.seekg(0, s.beg);
    }

    return len;
}

bool Database::openFile(const std::string &filepath, std::fstream &fs,
                        std::fstream::openmode mode) const {

    fs.open(filepath, mode);

    if (!fs.is_open()) {
        std::cerr << "ERROR: File " << filepath << " couldn't be opened"
                  << std::endl;
        return false;
    }

    return true;
}

bool Database::syncRawData() {
    std::fstream fs;
    if (!openFile(this->m_filepath, fs, std::fstream::in)) {
        return false;
    }

    long fileSize = getFileSize(fs);
    char *buffer = new char[fileSize + 1];

    fs.read(buffer, fileSize);
    if (fs) {
        std::cout << "Synching with Database..." << std::endl;
    } else {
        std::cerr << "ERROR: only " << fs.gcount() << " could be read"
                  << std::endl;
        return false;
    }
    fs.close();

    buffer[fileSize] = '\0';

    this->m_raw = buffer;

    return true;
}

Database::Database(const std::string &filepath) : m_filepath(filepath) {

    std::fstream fs;
    if (!openFile(filepath, fs, std::fstream::in)) {
        return;
    }

    long fileSize = getFileSize(fs);
    char *buffer = new char[fileSize + 1];

    fs.read(buffer, fileSize);
    if (fs) {
        std::cout << "Database data read successfully" << std::endl;
    } else {
        std::cerr << "ERROR: only " << fs.gcount() << " could be read"
                  << std::endl;
    }
    fs.close();

    buffer[fileSize] = '\0';

    this->m_raw = buffer;
}

bool Database::insertTask(const Task &task) {
    cJSON *taskObj;
    cJSON *tasks;
    char *updatedTasks;

    // sync up raw data
    syncRawData();

    // parse raw JSON
    cJSON *json = parseJSON();

    if (json == NULL) {
        std::cerr << "ERROR: Raw JSON could not be parsed" << std::endl;
        return false;
    }

    // insert new task object into tasks array
    taskObj = task.getJSON();

    tasks = cJSON_GetObjectItem(json, "tasks");
    if (tasks == NULL) {
        tasks = cJSON_AddArrayToObject(json, "tasks");
    }
    cJSON_AddItemToArray(tasks, taskObj);

    // Stringify JSON
    updatedTasks = cJSON_PrintUnformatted(json);

    // write raw JSON to file
    this->m_raw = updatedTasks;

    // commit change
    commitChanges();

    // free resources
    cJSON_Delete(json);
    free(updatedTasks);
    return true;
}

int Database::getTaskIndex(cJSON *tasks, unsigned int id) const {
    int numTasks = cJSON_GetArraySize(tasks);

    for (int i = 0; i < numTasks; ++i) {
        cJSON *t = cJSON_GetArrayItem(tasks, i);
        unsigned int tId = (unsigned int)cJSON_GetObjectItem(t, "id")->valueint;

        if (id == tId) {
            return i;
        }
    }
    return -1;
}

Task *Database::findTaskById(unsigned int id) {
    // sync up raw data
    syncRawData();

    cJSON *tasks = getTasks();

    if (tasks == NULL || !cJSON_IsArray(tasks)) {
        return NULL;
    }

    int idx = getTaskIndex(tasks, id);

    if (idx < 0)
        return NULL;

    cJSON *t = cJSON_GetArrayItem(tasks, idx);
    return new Task(id, t);
}

bool Database::updateTask(unsigned int id, const Task &task) {
    // sync up raw data
    syncRawData();

    cJSON *data = parseJSON();
    cJSON *tasks = cJSON_GetObjectItem(data, "tasks");
    cJSON_DeleteItemFromArray(tasks, getTaskIndex(tasks, id));

    // sync up the data
    this->m_raw = cJSON_PrintUnformatted(data);

    cJSON_Delete(data);
    return insertTask(task);
}

bool Database::commitChanges() const {
    std::fstream fs;
    if (!openFile(this->m_filepath, fs, std::fstream::out)) {
        std::cerr << "ERROR: Could not commit tasks to file "
                  << this->m_filepath << std::endl;
        return false;
    }

    fs << this->m_raw;

    fs.close();

    std::cout << "Changes committed successfully." << std::endl;
    return true;
}
