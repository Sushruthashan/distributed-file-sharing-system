#pragma once
#include <Poco/Net/HTTPRequestHandler.h>

class FileDownloadHandler : public Poco::Net::HTTPRequestHandler {
public:
    FileDownloadHandler();
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override;
};
