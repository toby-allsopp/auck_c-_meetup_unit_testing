#include "Database.h"

#include <iostream>
#include <utility>

using namespace std;

namespace NameEntity {
  void NameEntity(Database& db) {
    string full_name;
    for (;;) {
      cout << "Full name: ";
      getline(cin, full_name);
      if (full_name.empty()) {
        cout << "Name cannot be empty, try again." << endl;
      } else if (db.IsFullNameUsed(full_name)) {
        cout << "Name already used, try again." << endl;
      } else {
        break;
      }
    }

    string short_name = full_name.substr(0, 5);
    if (db.IsShortNameUsed(short_name)) {
      for (;;) {
        cout << "Short name: ";
        getline(cin, short_name);
        if (short_name.empty()) {
          cout << "Short name cannot be empty, try again."
               << endl;
        } else if (db.IsShortNameUsed(short_name)) {
          cout << "Short name already used, try again."
               << endl;
        } else {
          break;
        }
      }
    }

    db.AddNames(full_name, short_name);
  }

  // How to unit test this?

}  // namespace NameEntity
