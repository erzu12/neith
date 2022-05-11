#include <stdexcept>

namespace neith {
class WindowCreationException : virtual public std::runtime_error {
public:
    explicit WindowCreationException(const std::string& msg) : std::runtime_error(msg) {}
};
}  // namespace neith
