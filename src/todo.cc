#include <CivetServer.h>
#include <cJSON.h>

#include <csignal>
#include <iostream>
#include <unistd.h>

#include "api.hpp"
#include "complete.hpp"
#include "new.hpp"
#include "ping.hpp"
#include "server.hpp"

volatile bool exitNow = false;
void h_exit(int) {
    std::cout << "Preparing exit..." << '\n';
    exitNow = true;
}

static int log_message(const struct mg_connection *, const char *message) {
    std::cout << message << "\n";
    return 0;
}

int main(void) {
    mg_init_library(0);

    struct CivetCallbacks callbacks;

    const char *documentRoot = getDocumentRoot();
    const char *port = getPort();

    const char *options[] = {"document_root", documentRoot, "listening_ports",
                             port, 0};

    callbacks.log_message = log_message;

    // get database JSON path
    const char *databasePath = getDatabasePath();

    Server server(options, &callbacks, databasePath);

    // add handler for PING URI
    PingHandler h_ping;
    server.addHandler(PING_URI, h_ping);

    // add handler for NEW TASK URI
    NewHandler h_new;
    server.addHandler(NEW_TASK_URI, h_new);

    // add handler for NEW TASK URI
    CompleteHandler h_complete;
    server.addHandler(COMPLETE_TASK_URI, h_complete);

    std::cout << "Server fired up at http://localhost:" << port << std::endl;

    // setup exit signal
    std::signal(SIGINT, h_exit);

    while (!exitNow) {
        sleep(1);
    }

    std::cout << "Bye." << std::endl;
    mg_exit_library();

    return 0;
}
