#include "FileUploadHandler.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/PartHandler.h>
#include <Poco/Net/MessageHeader.h>
#include <Poco/UUIDGenerator.h>

#include <sstream>
#include <iostream>
#include <pqxx/pqxx>

FileUploadHandler::FileUploadHandler() {}

class UploadPartHandler : public Poco::Net::PartHandler {
public:
    std::string fileContent;
    std::string fileName;

    void handlePart(const Poco::Net::MessageHeader& header, std::istream& stream) override {
    std::ostringstream oss;
    Poco::StreamCopier::copyStream(stream, oss);
    fileContent = oss.str();

    std::string disposition = header.get("Content-Disposition", "");
    std::size_t pos = disposition.find("filename=");
    if (pos != std::string::npos) {
        std::string rawName = disposition.substr(pos + 9);
        if (!rawName.empty() && rawName.front() == '"') rawName.erase(0, 1);
        if (!rawName.empty() && rawName.back() == '"') rawName.pop_back();
        fileName = rawName;
    }

    std::cout << "[INFO] Received file (raw): " << fileName << ", size: " << fileContent.size() << " bytes\n";
}


};

void FileUploadHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                      Poco::Net::HTTPServerResponse& response) {
    try {
        UploadPartHandler partHandler;
        Poco::Net::HTMLForm form(request, request.stream(), partHandler);

        std::cout << "Received file: " << partHandler.fileName
                  << " (" << partHandler.fileContent.size() << " bytes)\n";

        pqxx::connection conn("host=localhost port=5433 dbname=yugabyte user=yugabyte");
        pqxx::work txn(conn);
        
txn.exec(R"(
    CREATE TABLE IF NOT EXISTS files (
        id SERIAL PRIMARY KEY,
        filename TEXT NOT NULL,
        data TEXT,
        uploaded_at TIMESTAMP DEFAULT NOW()
    )
)");

txn.exec("ALTER TABLE files ADD COLUMN IF NOT EXISTS share_token TEXT UNIQUE");


       std::string shareToken = Poco::UUIDGenerator().createRandom().toString();

       txn.exec_params(
    "INSERT INTO files (filename, data, share_token) VALUES ($1, $2, $3)",
    partHandler.fileName,
    partHandler.fileContent,
    shareToken
);



txn.exec("ALTER TABLE files ADD COLUMN IF NOT EXISTS data TEXT");

        txn.exec_params(
    "INSERT INTO files (filename, data) VALUES ($1, $2)",
    partHandler.fileName, partHandler.fileContent
);


        txn.commit();

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.send() << "{\"status\": \"success\", \"link\": \"http://<your-ip>:8080/download?token=" << shareToken << "\"}";


    } catch (const std::exception& e) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.setContentType("application/json");
        response.send() << "{\"error\": \"" << e.what() << "\"}";
    }
}
