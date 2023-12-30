#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
struct JSONObject;

using JSONDict = std::unordered_map<std::string, JSONObject>;
using JSONList = std::vector<JSONObject>;
using JSONDictPtr = std::shared_ptr<JSONDict>;
using JSONListPtr = std::shared_ptr<JSONList>;

struct JSONObject {
  std::variant<std::monostate, std::string, int, bool, double, JSONListPtr,
               JSONDictPtr>
      inner;

  JSONObject() = default;

  // template<class T>
  // JSONObject(const T&o) : inner{o} {
  // }

  template <class T> JSONObject(T &&o) {
    // std::cout << "move\n";
    this->inner = std::forward<T>(o);
  }

  friend std::ostream &operator<<(std::ostream &out, JSONObject &o);

  std::string toString();
  std::string toString() const;
};

#endif