#ifndef _TASK_HPP
#define _TASK_HPP

#include <cJSON.h>
#include <string>

class Task {
  private:
    unsigned int id;
    std::string m_title;
    std::string m_description;
    bool completed;

  public:
    Task() = default;
    Task(unsigned int id, const std::string &title,
         const std::string &description);
    Task(unsigned int id, const cJSON *json);
    ~Task() = default;

    inline const std::string &getTitle() const { return this->m_title; }
    inline const std::string &getDescription() const {
        return this->m_description;
    }
    inline bool isCompleted() const { return this->completed; };

    inline void setCompleted(bool val) { this->completed = val; };

    cJSON *getJSON() const;
};

#endif /* _TASK_HPP */
