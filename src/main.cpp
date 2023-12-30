#include "JSONParser.h"
#include <iostream>
#include <ostream>
// #include <type_traits>

using namespace std::string_literals;

// template <class T> void judge(T &t) {
//   if (std::is_same<std::decay_t<decltype(t)>, int>::value) {
//     std::printf("int\n");
//   } else if (std::is_same<std::decay_t<decltype(t)>, std::string>::value) {
//     std::printf("string\n");
//   }
// }

int main() {

  JSONParser parser{};
  std::string s =
      R"({"a": 165.5,"b":{"asx":999,"abx":"213465"},"c": [123.9,156,"asxasx"]})";
  std::string s1 = R"([["awdawd",1561],165])";
  auto [res, _] = parser.parse(s1);
  std::cout << res << std::endl;
  std::cout << parser.beautify(s) << std::endl;
  auto json = parser.toJSONDict(s);
  std::cout << json["a"].toString() << std::endl;
  std::cout << json["b"].toString() << std::endl;
  std::cout << json["c"].toString() << std::endl;

}