#include "LoginHandler.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPResponse.h>

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <iostream>
#include <pqxx/pqxx>

using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

extern bool verifyPassword(const std::string& input, const std::string& storedHash);

void LoginHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    std::stringstream ss;
    Poco::StreamCopier::copyStream(request.stream(), ss);
    std::string body = ss.str();

    try {
        Parser parser;
        Var result = parser.parse(body);
        Object::Ptr obj = result.extract<Object::Ptr>();

        if (!obj->has("username") || !obj->has("password")) {
            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
            response.setContentType("application/json");
            response.send() << R"({"status": "fail", "message": "Missing username or password field"})";
            return;
        }

        std::string username = obj->getValue<std::string>("username");
        std::string password = obj->getValue<std::string>("password");

        pqxx::connection conn("host=localhost port=5433 dbname=yugabyte user=yugabyte");
        pqxx::work txn(conn);

        pqxx::result r = txn.exec(
            pqxx::zview("SELECT password_hash FROM users WHERE username = $1"),
            pqxx::params{username}
        );
        txn.commit();

        if (!r.empty()) {
            std::string hash = r[0]["password_hash"].c_str();

            if (verifyPassword(password, hash)) {
                response.setStatus(HTTPResponse::HTTP_OK);
                response.setContentType("application/json");
                response.send() << R"({"status": "success", "message": "Login successful"})";
                return;
            }
        }

        response.setStatus(HTTPResponse::HTTP_UNAUTHORIZED);
        response.setContentType("application/json");
        response.send() << R"({"status": "fail", "message": "Invalid credentials"})";

    } catch (const std::exception& e) {
        response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.setContentType("application/json");
        response.send() << R"({"status": "error", "message": ")" << e.what() << R"("})";
    }
}

