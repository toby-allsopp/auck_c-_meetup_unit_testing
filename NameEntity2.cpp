#include "catch.hpp"

#include "Database.h"

#include <iostream>
#include <utility>

using namespace std;

namespace NameEntity2 {
  // If we factor out the pure logic then we can test it.

  string ValidateFullName(string const& full_name) {
    if (full_name.empty()) {
      return "Full name cannot be empty";
    }
    return {};
  }

  TEST_CASE("ValidateFullName with empty name") {
    CHECK(ValidateFullName("") != "");
  }
  TEST_CASE("ValidateFullName with non-empty name") {
    CHECK(ValidateFullName("not empty").empty());
  }

  string ValidateShortName(string const& short_name) {
    if (short_name.empty()) {
      return "Short name cannot be empty";
    }
    if (short_name.size() > 5) {
      return "Short name cannot be longer than 5 "
             "characters";
    }
    return {};
  }

  TEST_CASE("ValidateShortName with empty name") {
    CHECK(!ValidateShortName("").empty());
  }
  TEST_CASE(
      "ValidateShortName with short, non-empty name") {
    CHECK(ValidateShortName("full").empty());
  }
  TEST_CASE("ValidateShortName with long, non-empty name") {
    CHECK(!ValidateShortName("not empty").empty());
  }

  string DefaultShortName(string const& full_name) {
    return full_name.substr(0, 5);
  }

  TEST_CASE("DefaultShortName with short name") {
    CHECK(DefaultShortName("123") == "123");
  }
  TEST_CASE("DefaultShortName with long name") {
    CHECK(DefaultShortName("1234567") == "12345");
  }

  // But the remainder of the code still does I/O.

  string GetFullNameFromUser(Database& db) {
    string full_name;
    for (;;) {
      cout << "Full name: ";
      getline(cin, full_name);
      if (auto msg = ValidateFullName(full_name);
          !msg.empty()) {
        cout << msg << "; try again." << endl;
      } else if (db.IsFullNameUsed(full_name)) {
        cout << "Name already used; try again." << endl;
      } else {
        break;
      }
    }

    return full_name;
  }

  string GetShortNameFromUser(Database& db) {
    string short_name;
    for (;;) {
      cout << "Short name: ";
      getline(cin, short_name);
      if (auto msg = ValidateShortName(short_name);
          !msg.empty()) {
        cout << msg << "; try again." << endl;
      } else if (db.IsShortNameUsed(short_name)) {
        cout << "Short name already used; try again."
             << endl;
      } else {
        break;
      }
    }
    return short_name;
  }

  void NameEntity(Database& db) {
    auto const full_name = GetFullNameFromUser(db);

    string short_name = DefaultShortName(full_name);
    if (db.IsShortNameUsed(short_name)) {
      short_name = GetShortNameFromUser(db);
    }

    db.AddNames(full_name, short_name);
  }
}  // namespace NameEntity2
