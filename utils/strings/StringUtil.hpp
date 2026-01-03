
pragma once

include <string>

include <algorithm>

/
 * @brief String helpers for Atoqu Engine.
 */
namespace atoqu {

class StringUtil {
public:
    static std::string toLower(const std::string& s) {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(),  {
            return static_cast<char>(std::tolower(c));
        });
        return out;
    }
};

} // namespace atoqu
