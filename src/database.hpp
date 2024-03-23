#ifndef _DATABASE_HPP
#define _DATABASE_HPP

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

    inline cJSON *parseJSON() const { return cJSON_Parse(this->m_raw.c_str()); }

  private:
    bool openFile(const std::string &filepath, std::fstream &fs,
                  std::fstream::openmode mode) const;
    bool commitChanges() const;
};

#endif /* _DATABASE_HPP */
