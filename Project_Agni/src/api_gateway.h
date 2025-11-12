#ifndef AGNI_API_GATEWAY_H
#define AGNI_API_GATEWAY_H

#include <string>
#include <map>
#include "scheduler.h"

struct APIRequest {
    std::string method;
    std::string endpoint;
    std::string body;
    std::map<std::string, std::string> headers;
};

struct APIResponse {
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
};

class APIGateway {
public:
    APIGateway(Scheduler* scheduler);
    ~APIGateway();

    APIResponse handle_request(const APIRequest& req);

    void start_server(int port);
    void stop_server();

private:
    Scheduler* scheduler;

    APIResponse handle_axiom(const APIRequest& req);
    APIResponse handle_upload(const APIRequest& req);
    APIResponse handle_status(const APIRequest& req);
};

#endif // AGNI_API_GATEWAY_H
