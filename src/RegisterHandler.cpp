#include "RegisterHandler.h"

// Only implement the constructor and methods
RegisterHandler::RegisterHandler() {
    // constructor logic
}

void RegisterHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                    Poco::Net::HTTPServerResponse& response) {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/plain");
    std::ostream& out = response.send();
    out << "Register endpoint hit.";
}



