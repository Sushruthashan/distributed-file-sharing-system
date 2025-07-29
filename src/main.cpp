// C++ TASK 2

#include <iostream>
#include <iomanip>
#include <sstream>

#include <pqxx/pqxx>
#include <openssl/sha.h>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>

#include "RegisterHandler.h"
#include "LoginHandler.h"
#include "FileUploadHandler.h"
#include "NotFoundHandler.h"  
#include "utils.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace std;

pqxx::connection conn("host=localhost port=5433 dbname=yugabyte user=yugabyte password=yugabyte");

// ------------------ HTTP Request Handler ------------------
class RequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override {
        response.setContentType("application/json");
        std::ostream &ostr = response.send();

        // Read request body
        std::stringstream ss;
        Poco::StreamCopier::copyStream(request.stream(), ss);
        std::string body = ss.str();

        try {
            
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(body);
            Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();

            std::string username = obj->getValue<std::string>("username");
            std::string password = obj->getValue<std::string>("password");

            if (request.getURI() == "/register") {
                std::string hashedPassword = hashPassword(password);

                pqxx::work txn(conn);
                txn.exec("INSERT INTO users (username, password_hash) VALUES (" +
                         txn.quote(username) + ", " + txn.quote(hashedPassword) + ")");
                txn.commit();

                ostr << R"({"status": "success", "message": "User registered successfully."})";
            } else if (request.getURI() == "/login") {
                pqxx::work txn(conn);
                pqxx::result r = txn.exec("SELECT password_hash FROM users WHERE username = " + txn.quote(username));

                if (!r.empty() && verifyPassword(password, r[0]["password_hash"].c_str())) {
                    ostr << R"({"status": "success", "message": "Login successful."})";
                } else {
                    ostr << R"({"status": "fail", "message": "Invalid credentials."})";
                }
            } else {
                response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
                ostr << R"({"error": "Unknown endpoint."})";
            }
        } catch (const std::exception &e) {
            response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            ostr << R"({"error": ")" << e.what() << R"("})";
        }
    }
};

// ------------------ Handler Factory ------------------
class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override {
        const std::string& uri = request.getURI();

        if (uri == "/register") {
            return new RegisterHandler();
        } else if (uri == "/login") {
            return new LoginHandler();
        } else if (uri == "/upload") {
            return new FileUploadHandler();
        } else {
            return new NotFoundHandler();  
        }
    }
};

// ------------------ Main Web Application ------------------
class WebApp : public ServerApplication {
protected:
    int main(const std::vector<std::string> &) override {
        HTTPServer server(new RequestHandlerFactory, ServerSocket(9090), new HTTPServerParams);
        server.start();

        std::cout << "Server started at http://localhost:9090" << std::endl;
        waitForTerminationRequest();

        server.stop();
        std::cout << "Server stopped." << std::endl;
        return Application::EXIT_OK;
    }
};

int main(int argc, char **argv) {
    WebApp app;
    return app.run(argc, argv);
}
