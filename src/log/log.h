
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <utility>
namespace log {
enum LogLevel { DEBUG, INFO, WARN, ERROR };

class Log {
private:
  std::string service_name;
  std::string trace_id;
  std::string span_id;
  std::string content;
  std::unordered_map<std::string, std::string> tags;
  bool error;
  long timestamp;
  LogLevel level;

public:
  Log(std::string service_name_, std::string trace_id_, std::string span_id_,
      std::string content_, bool error_, long timestamp_, LogLevel level_,
      std::initializer_list<std::pair<std::string, std::string>> tags_ = {})
      : service_name(service_name_), trace_id(trace_id_), span_id(span_id_),
        content(content_), error(error_), timestamp(timestamp_), level(level_) {

    for (auto &tag : tags_) {
      tags.insert(std::pair<std::string, std::string>(tag.first, tag.second));
    }
  }

  ~Log() = default;
};

} // namespace log
