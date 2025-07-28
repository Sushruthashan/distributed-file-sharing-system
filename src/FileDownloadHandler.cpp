#include "FileDownloadHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>
#include <pqxx/pqxx>
#include <sstream>
#include <iostream>

FileDownloadHandler::FileDownloadHandler() {}

void FileDownloadHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                        Poco::Net::HTTPServerResponse& response) {
    try {
        Poco::URI uri(request.getURI());
        std::vector<std::pair<std::string, std::string>> query;
        uri.getQueryParameters(query);

        std::string filename;
        for (auto& pair : query) {
            if (pair.first == "filename") {
                filename = pair.second;
                break;
            }
        }

        if (filename.empty()) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            response.send() << R"({"error": "Missing filename parameter"})";
            return;
        }

        pqxx::connection conn("host=localhost port=5433 dbname=yugabyte user=yugabyte");
        pqxx::work txn(conn);
        pqxx::result r = txn.exec_params("SELECT data FROM files WHERE filename=$1", filename);

        if (r.empty()) {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            response.send() << R"({"error": "File not found"})";
            return;
        }

        std::string fileContent = r[0]["data"].as<std::string>();
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/octet-stream");
        response.setContentLength(fileContent.size());
        response.set("Content-Disposition", "attachment; filename=\"" + filename + "\"");

        std::ostream& out = response.send();
        out << fileContent;

    } catch (const std::exception& e) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send() << "{\"error\": \"" << e.what() << "\"}";
    }
}
