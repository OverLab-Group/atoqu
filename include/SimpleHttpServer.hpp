
pragma once

include <functional>

include <string>

class SimpleHttpServer {
public:
    using Handler = std::function<std::string(const std::string& path,
                                              const std::string& method,
                                              const std::string& body)>;

    SimpleHttpServer(int port, Handler handler);
    void run();

private:
    int port_;
    Handler handler_;
};
