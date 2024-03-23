#include <CivetServer.h>
#include <cJSON.h>

#include <csignal>
#include <iostream>
#include <unistd.h>

#include "api.hpp"
#include "new.hpp"
#include "ping.hpp"
#include "server.hpp"

#define DATABASE_PATH "data/example.json"

static unsigned long request = 0;

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

    const char *options[] = {"document_root", DOCUMENT_ROOT, "listening_ports",
                             PORT, 0};

    callbacks.log_message = log_message;

    Server server(options, &callbacks, DATABASE_PATH);

    // add handler for PING URI
    PingHandler h_ping;
    server.addHandler(PING_URI, h_ping);

    // add handler for NEW TASK URI
    NewHandler h_new;
    server.addHandler(NEW_TASK_URI, h_new);

    std::cout << "Server fired up at http://localhost:" << PORT << std::endl;

    // setup exit signal
    std::signal(SIGINT, h_exit);

    while (!exitNow) {
        sleep(1);
    }

    std::cout << "Bye." << std::endl;
    mg_exit_library();

    return 0;
}
