#ifndef _DATABASE_HPP
#define _DATABASE_HPP

#include "cJSON.h"
#include "task.hpp"
#include <fstream>
#include <string>

class Database {
  private:
    std::string m_filepath;
    std::string m_raw;

  public:
    Database(const std::string &filepath);
    ~Database() = default;

    bool insertTask(const Task &task);
    bool updateTask(unsigned int id, const Task &task);
    Task *findTaskById(unsigned int id);

    inline cJSON *parseJSON() const { return cJSON_Parse(this->m_raw.c_str()); }
    inline cJSON *getTasks() const {
        return cJSON_GetObjectItem(parseJSON(), "tasks");
    }

  private:
    bool syncRawData();
    int getTaskIndex(cJSON *tasks, unsigned int id) const;
    bool openFile(const std::string &filepath, std::fstream &fs,
                  std::fstream::openmode mode) const;
    bool commitChanges() const;
};

#endif /* _DATABASE_HPP */
