// NotFoundHandler.h
#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <ostream>

class NotFoundHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override {
        (void)request;  // Suppress unused parameter warning

        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.setContentType("application/json");
        std::ostream &ostr = response.send();
        ostr << R"({"error": "Endpoint not found."})";
    }
};
