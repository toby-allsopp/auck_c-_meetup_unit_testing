#include "catch.hpp"
#include "fakeit.hpp"

#include "Database.h"

#include <iostream>
#include <utility>

using namespace std;

namespace NameEntity_virtual_mocks2 {
  // As before...

  string ValidateFullName(string const& full_name) {
    if (full_name.empty()) {
      return "Full name cannot be empty";
    }
    return {};
  }

  TEST_CASE("ValidateFullName with empty name") {
    CHECK(!ValidateFullName("").empty());
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

  // One option is to use an interface (abstract base class)

  class IDatabase {
   public:
    virtual bool IsFullNameUsed(string full_name) = 0;
    virtual bool IsShortNameUsed(string short_name) = 0;
    virtual void AddNames(string full_name,
                          string short_name) = 0;
  };

  class IUserIO {
   public:
    virtual void Prompt(string prompt) = 0;
    virtual string GetLine() = 0;
  };

  string GetFullNameFromUser(IDatabase& db, IUserIO& io) {
    for (;;) {
      io.Prompt("Full name: ");
      string full_name = io.GetLine();
      if (auto msg = ValidateFullName(full_name);
          !msg.empty()) {
        io.Prompt(msg + "; try again.\n");
      } else if (db.IsFullNameUsed(full_name)) {
        io.Prompt("Name already used; try again.\n");
      } else {
        return full_name;
      }
    }
  }

  using namespace fakeit;
  TEST_CASE("GetFullNameFromUser") {
    Mock<IDatabase> db;
    Mock<IUserIO> io;

    SECTION("user enters empty name then acceptable name") {
      Fake(Method(io, Prompt));
      When(Method(io, GetLine)).Return("", "acceptable");
      When(Method(db, IsFullNameUsed)).Return(false);

      const auto result =
          GetFullNameFromUser(db.get(), io.get());

      Verify(Method(io, Prompt).Using("Full name: ") +
             Method(io, GetLine) +
             Method(io, Prompt)
                 .Using("Full name cannot be empty; try "
                        "again.\n") +
             Method(io, Prompt).Using("Full name: ") +
             Method(io, GetLine) +
             Method(db, IsFullNameUsed).Using("acceptable"))
          .Exactly(Once);
      VerifyNoOtherInvocations(io);
      VerifyNoOtherInvocations(db);
      CHECK(result == "acceptable");
    }

    SECTION(
        "user enters in-use name then acceptable name") {
      Fake(Method(io, Prompt));
      When(Method(io, GetLine))
          .Return("in-use", "acceptable");
      When(Method(db, IsFullNameUsed)).Return(true, false);

      const auto result =
          GetFullNameFromUser(db.get(), io.get());

      Verify(Method(io, Prompt).Using("Full name: ") +
             Method(io, GetLine) +
             Method(db, IsFullNameUsed).Using("in-use") +
             Method(io, Prompt)
                 .Using("Name already used; try again.\n") +
             Method(io, Prompt).Using("Full name: ") +
             Method(io, GetLine) +
             Method(db, IsFullNameUsed).Using("acceptable"))
          .Exactly(Once);
      VerifyNoOtherInvocations(io);
      VerifyNoOtherInvocations(db);
      CHECK(result == "acceptable");
    }
  }

  string GetShortNameFromUser(IDatabase& db, IUserIO& io) {
    for (;;) {
      io.Prompt("Short name: ");
      string short_name = io.GetLine();
      if (auto msg = ValidateShortName(short_name);
          !msg.empty()) {
        io.Prompt(msg + "; try again.\n");
      } else if (db.IsShortNameUsed(short_name)) {
        io.Prompt("Short name already used; try again.\n");
      } else {
        return short_name;
      }
    }
  }

  // insert pretty much the same tests here

  class INameEntityDependencies {
  public:
    virtual string GetFullNameFromUser() = 0;
    virtual bool IsShortNameUsed(string short_name) = 0;
    virtual string GetShortNameFromUser() = 0;
    virtual void AddNames(string full_name, string short_name) = 0;
  };

  void NameEntity(INameEntityDependencies& deps) {
    auto const full_name = deps.GetFullNameFromUser();

    string short_name = DefaultShortName(full_name);
    if (deps.IsShortNameUsed(short_name)) {
      short_name = deps.GetShortNameFromUser();
    }

    deps.AddNames(full_name, short_name);
  }

}  // namespace NameEntity_virtual_mocks2
