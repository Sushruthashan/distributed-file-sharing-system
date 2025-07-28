#include "utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <iostream>
#include <pqxx/pqxx>


std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

bool verifyPassword(const std::string& password, const std::string& hash) {
    return hashPassword(password) == hash;
}
