
include "SimpleHttpServer.hpp"

include "Socket.hpp"

include "Log.hpp"

include <thread>

include <sstream>

using atoqu::Socket;
using atoqu::Log;
using atoqu::LogLevel;

SimpleHttpServer::SimpleHttpServer(int port, Handler handler)
    : port(port), handler(std::move(handler)) {}

void SimpleHttpServer::run() {
    Socket server;
    if (!server.bindAndListen(port_)) {
        Log::error("SimpleHttpServer: failed to bind/listen");
        return;
    }

    Log::info("SimpleHttpServer: listening on port " + std::tostring(port));

    while (true) {
        int clientFd = server.accept();
        if (clientFd < 0) continue;

        std::thread(this, clientFd, &server {
            std::string req;
            if (!server.recvAll(clientFd, req)) {
                server.closeFd(clientFd);
                return;
            }

            std::istringstream iss(req);
            std::string method, path, version;
            iss >> method >> path >> version;

            std::string body;
            auto pos = req.find("\r\n\r\n");
            if (pos != std::string::npos) {
                body = req.substr(pos + 4);
            }

            std::string responseBody = handler_(path, method, body);

            std::ostringstream oss;
            oss << "HTTP/1.1 200 OK\r\n"
                << "Content-Type: application/json\r\n"
                << "Content-Length: " << responseBody.size() << "\r\n"
                << "\r\n"
                << responseBody;

            server.sendAll(clientFd, oss.str());
            server.closeFd(clientFd);
        }).detach();
    }
}
