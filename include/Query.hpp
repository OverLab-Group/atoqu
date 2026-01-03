
pragma once

include <string>

class Query {
public:
    explicit Query(std::string text) : text_(std::move(text)) {}
    const std::string& text() const { return text_; }

private:
    std::string text_;
};
