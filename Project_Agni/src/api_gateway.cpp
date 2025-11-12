#include "api_gateway.h"
#include "common.h"
#include <sstream>

APIGateway::APIGateway(Scheduler* sched) : scheduler(sched) {
    LOG_INFO("API Gateway initialized");
}

APIGateway::~APIGateway() {
    LOG_INFO("API Gateway destroyed");
}

APIResponse APIGateway::handle_request(const APIRequest& req) {
    APIResponse resp;
    resp.headers["Content-Type"] = "application/json";

    if (req.method == "POST" && req.endpoint == "/v59/axiom") {
        return handle_axiom(req);
    } else if (req.method == "POST" && req.endpoint == "/v59/upload") {
        return handle_upload(req);
    } else if (req.method == "GET" && req.endpoint.find("/v59/status/") == 0) {
        return handle_status(req);
    }

    resp.status_code = 404;
    resp.body = R"({"error": "Not found"})";
    return resp;
}

APIResponse APIGateway::handle_axiom(const APIRequest&) {
    APIResponse resp;
    resp.status_code = 200;

    // Parse JSON request (simplified)
    std::string job_id_str = "WEAPON_1";
    std::string prompt = "test";

    uint32_t job_id = scheduler->submit_job(job_id_str, prompt);

    std::stringstream ss;
    ss << R"({"job_uuid": ")" << job_id << R"(", "status": "QUEUED"})";
    resp.body = ss.str();

    return resp;
}

APIResponse APIGateway::handle_upload(const APIRequest&) {
    APIResponse resp;
    resp.status_code = 200;
    resp.body = R"({"file_upload_id": "file_123", "size_mb": 5.2})";
    return resp;
}

APIResponse APIGateway::handle_status(const APIRequest&) {
    APIResponse resp;
    resp.status_code = 200;
    resp.body = R"({"status": "PROCESSING", "progress": 50})";
    return resp;
}

void APIGateway::start_server(int port) {
    LOG_INFO("API Gateway listening on port %d", port);
}

void APIGateway::stop_server() {
    LOG_INFO("API Gateway stopped");
}
