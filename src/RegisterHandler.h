// RegisterHandler.h
#pragma

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class RegisterHandler : public Poco::Net::HTTPRequestHandler {
public:
    RegisterHandler();

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;
};

