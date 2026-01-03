
pragma once

include <string>

/
 * @brief Minimal TCP socket wrapper for Atoqu HTTP server.
 *
 * No dynamic allocation, no global state, friendly to sanitizers and static analyzers.
 */
namespace atoqu {

class Socket {
public:
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    bool bindAndListen(int port, int backlog = 16);
    int  accept();
    bool recvAll(int fd, std::string& out);
    bool sendAll(int fd, const std::string& data);
    void closeFd(int fd);
    void closeServer();

private:
    int serverFd_;
};

} // namespace atoqu
