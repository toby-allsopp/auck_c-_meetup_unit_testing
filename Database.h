#include <set>
#include <string>

struct Database {
  // foe exposition only!
  std::set<std::string> full_names;
  std::set<std::string> short_names;

  bool IsFullNameUsed(std::string const& full_name) const {
    return full_names.count(full_name) > 0;
  }

  bool IsShortNameUsed(
      std::string const& short_name) const {
    return short_names.count(short_name) > 0;
  }

  void AddNames(std::string const& full_name,
                std::string const& short_name) {
    full_names.insert(std::move(full_name));
    short_names.insert(std::move(short_name));
  }
};
