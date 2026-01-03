/
 *  Atoqu JSON Engine - Minimal JSON Implementation
 *  Compatible with:
 *    - CUnit, Catch2, Unity (no global state)
 *    - ASan, TSan, UBSan (no UB, no raw pointer tricks)
 *    - CPPCheck, Clang-tidy (clean API)
 *    - Doxygen (fully documented)
 *
 *  Author: Atoqu Engine Team
 /

pragma once

include <string>

include <map>

include <vector>

include <variant>

include <stdexcept>

include <cctype>

namespace atoqu {

/
 * @brief Minimal JSON value class for Atoqu Engine.
 */
class Json {
public:
    using object_t = std::map<std::string, Json>;
    using array_t  = std::vector<Json>;
    using valuet  = std::variant<std::nullptrt, bool, double, std::string, objectt, arrayt>;

    Json() : value_(nullptr) {}
    Json(std::nullptrt) : value(nullptr) {}
    Json(bool b) : value_(b) {}
    Json(double d) : value_(d) {}
    Json(const std::string& s) : value_(s) {}
    Json(const char* s) : value_(std::string(s)) {}
    Json(const objectt& o) : value(o) {}
    Json(const arrayt& a) : value(a) {}

    bool isObject() const { return std::holdsalternative<objectt>(value_); }
    bool isArray()  const { return std::holdsalternative<arrayt>(value_); }
    bool isString() const { return std::holdsalternative<std::string>(value); }
    bool isNumber() const { return std::holdsalternative<double>(value); }
    bool isBool()   const { return std::holdsalternative<bool>(value); }
    bool isNull()   const { return std::holdsalternative<std::nullptrt>(value_); }

    objectt&       asObject()       { return std::get<objectt>(value_); }
    const objectt& asObject() const { return std::get<objectt>(value_); }

    arrayt&        asArray()        { return std::get<arrayt>(value_); }
    const arrayt&  asArray()  const { return std::get<arrayt>(value_); }

    const std::string& asString() const { return std::get<std::string>(value_); }
    double              asNumber() const { return std::get<double>(value_); }
    bool                asBool()   const { return std::get<bool>(value_); }

    Json& operator {
        if (!isObject()) value = objectt{};
        return std::get<objectt>(value)[key];
    }

    const Json& operator const {
        return std::get<objectt>(value).at(key);
    }

    std::string dump(int indent = 0) const {
        return dumpImpl(indent, 0);
    }

    static Json parse(const std::string& text) {
        std::size_t pos = 0;
        return parseValue(text, pos);
    }

private:
    valuet value;

    static void skipWS(const std::string& s, std::size_t& pos) {
        while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) pos++;
    }

    static Json parseValue(const std::string& s, std::size_t& pos) {
        skipWS(s, pos);
        if (pos >= s.size()) throw std::runtime_error("Unexpected end of JSON");

        char c = s[pos];
        if (c == '{') return parseObject(s, pos);
        if (c == '[') return parseArray(s, pos);
        if (c == '"') return parseString(s, pos);
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') return parseNumber(s, pos);
        if (s.compare(pos, 4, "true") == 0)  { pos += 4; return Json(true); }
        if (s.compare(pos, 5, "false") == 0) { pos += 5; return Json(false); }
        if (s.compare(pos, 4, "null") == 0)  { pos += 4; return Json(nullptr); }

        throw std::runtime_error("Invalid JSON value");
    }

    static Json parseObject(const std::string& s, std::size_t& pos) {
        object_t obj;
        pos++; // skip '{'
        skipWS(s, pos);

        if (pos < s.size() && s[pos] == '}') { pos++; return obj; }

        while (true) {
            skipWS(s, pos);
            Json keyJson = parseString(s, pos);
            std::string key = keyJson.asString();
            skipWS(s, pos);

            if (pos >= s.size() || s[pos] != ':') throw std::runtime_error("Expected ':'");
            pos++;

            Json val = parseValue(s, pos);
            obj[key] = val;

            skipWS(s, pos);
            if (pos < s.size() && s[pos] == '}') { pos++; break; }
            if (pos >= s.size() || s[pos] != ',') throw std::runtime_error("Expected ','");
            pos++;
        }

        return obj;
    }

    static Json parseArray(const std::string& s, std::size_t& pos) {
        array_t arr;
        pos++; // skip '['
        skipWS(s, pos);

        if (pos < s.size() && s[pos] == ']') { pos++; return arr; }

        while (true) {
            Json val = parseValue(s, pos);
            arr.push_back(val);

            skipWS(s, pos);
            if (pos < s.size() && s[pos] == ']') { pos++; break; }
            if (pos >= s.size() || s[pos] != ',') throw std::runtime_error("Expected ','");
            pos++;
        }

        return arr;
    }

    static Json parseString(const std::string& s, std::size_t& pos) {
        if (s[pos] != '"') throw std::runtime_error("Expected '\"'");
        pos++; // skip "
        std::string out;
        while (pos < s.size() && s[pos] != '"') {
            // NOTE: minimal, no escape handling for v0.7
            out.push_back(s[pos++]);
        }
        if (pos >= s.size()) throw std::runtime_error("Unterminated string");
        pos++; // skip closing "
        return out;
    }

    static Json parseNumber(const std::string& s, std::size_t& pos) {
        std::size_t start = pos;
        while (pos < s.size() &&
               (std::isdigit(static_cast<unsigned char>(s[pos])) ||
                s[pos] == '.' || s[pos] == '-' || s[pos] == '+')) {
            pos++;
        }
        return std::stod(s.substr(start, pos - start));
    }

    std::string dumpImpl(int indent, int level) const {
        if (isNull()) return "null";
        if (isBool()) return asBool() ? "true" : "false";
        if (isNumber()) return std::to_string(asNumber());
        if (isString()) return "\"" + asString() + "\"";

        if (isArray()) {
            const auto& arr = asArray();
            if (arr.empty()) return "[]";
            std::string out = "[";
            for (std::size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) out += ", ";
                out += arr[i].dumpImpl(indent, level + 1);
            }
            out += "]";
            return out;
        }

        if (isObject()) {
            const auto& obj = asObject();
            if (obj.empty()) return "{}";
            std::string out = "{";
            bool first = true;
            for (const auto& [k, v] : obj) {
                if (!first) out += ", ";
                first = false;
                out += "\"" + k + "\": " + v.dumpImpl(indent, level + 1);
            }
            out += "}";
            return out;
        }

        return "null";
    }
};

} // namespace atoqu
