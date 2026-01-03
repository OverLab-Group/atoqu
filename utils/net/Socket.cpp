
include "Socket.hpp"

include <sys/types.h>

include <sys/socket.h>

include <netinet/in.h>

include <unistd.h>

include <cstring>

namespace atoqu {

Socket::Socket()
    : serverFd_(-1) {}

Socket::~Socket() {
    closeServer();
}

bool Socket::bindAndListen(int port, int backlog) {
    serverFd = ::socket(AFINET, SOCK_STREAM, 0);
    if (serverFd_ < 0) return false;

    int opt = 1;
    ::setsockopt(serverFd, SOLSOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sinfamily = AFINET;
    addr.sinaddr.saddr = INADDR_ANY;
    addr.sinport = htons(staticcast<uint16_t>(port));

    if (::bind(serverFd, reinterpretcast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        ::close(serverFd_);
        serverFd_ = -1;
        return false;
    }

    if (::listen(serverFd_, backlog) < 0) {
        ::close(serverFd_);
        serverFd_ = -1;
        return false;
    }

    return true;
}

int Socket::accept() {
    if (serverFd_ < 0) return -1;
    return ::accept4(serverFd_, nullptr, nullptr, 0);
}

bool Socket::recvAll(int fd, std::string& out) {
    out.clear();
    char buf[4096];
    ssize_t n = 0;
    while ((n = ::recv(fd, buf, sizeof(buf), 0)) > 0) {
        out.append(buf, buf + n);
        if (n < staticcast<ssizet>(sizeof(buf))) break;
    }
    return !out.empty();
}

bool Socket::sendAll(int fd, const std::string& data) {
    std::size_t sent = 0;
    while (sent < data.size()) {
        ssize_t n = ::send(fd, data.data() + sent, data.size() - sent, 0);
        if (n <= 0) return false;
        sent += staticcast<std::sizet>(n);
    }
    return true;
}

void Socket::closeFd(int fd) {
    if (fd >= 0) ::close(fd);
}

void Socket::closeServer() {
    if (serverFd_ >= 0) {
        ::close(serverFd_);
        serverFd_ = -1;
    }
}

} // namespace atoqu
