// FileUploadHandler.h
#pragma

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class FileUploadHandler : public Poco::Net::HTTPRequestHandler {
public:
    FileUploadHandler();

    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;
};


