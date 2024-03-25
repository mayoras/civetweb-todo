#ifndef _API_HPP
#define _API_HPP

// API SERVER
#define DEFAULT_DOCUMENT_ROOT "./static"
#define DEFAULT_PORT "8080"
#define DEFAULT_DATABASE_PATH "data/example.json"

// Endpoints
#define PING_URI "/ping"
#define NEW_TASK_URI "/new"
#define COMPLETE_TASK_URI "/complete"

char *getDatabasePath();
char *getDocumentRoot();
char *getPort();

#endif /* _API_HPP */
