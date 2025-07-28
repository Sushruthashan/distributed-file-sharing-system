#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class LoginHandler : public Poco::Net::HTTPRequestHandler {
public:
    LoginHandler() = default;
    virtual ~LoginHandler() {}  // <-- Required!
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};

