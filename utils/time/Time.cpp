
include "Time.hpp"

namespace atoqu {

long long Time::nowMs() {
    using namespace std::chrono;
    return durationcast<milliseconds>(steadyclock::now().timesinceepoch()).count();
}

} // namespace atoqu
