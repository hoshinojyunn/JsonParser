#include "JSONParser.h"
#include <iostream>
#include <ostream>
#include <type_traits>

using namespace std::string_literals;

template <class T> void judge(T &t) {
  if (std::is_same<std::decay_t<decltype(t)>, int>::value) {
    std::printf("int\n");
  } else if (std::is_same<std::decay_t<decltype(t)>, std::string>::value) {
    std::printf("string\n");
  }
}

int main() {

  // JSONList l{false, true, 1,32.645,"awxxxazzz"s};
  // JSONListPtr list_ptr = std::make_shared<JSONList>(l);

  // JSONList list = {1,2612.3, 16516, list_ptr};
  // JSONListPtr ptr1 = std::make_shared<decltype(list)>(list);
  // ptr1->push_back("awdawdxxxx"s);

  // JSONDict o4 = {{"awdda", 2131}, {"xx", 23.654}, {"str", "axxax"}};

  // JSONDictPtr ptr2 = std::make_shared<decltype(o4)>(o4);
  // ptr1->push_back(ptr2);
  // // [1,2612.3, 16516, [1,32.645,"awxxxazzz"], "awdawdxxxx", {"awdda": 2131},
  // {"xx": 23.654}, {"str": "axxax"}] JSONObject temp = ptr1;

  // std::cout << temp.toString() << std::endl;

  // const JSONObject &o = 165;

  // std::cout << o.toString() << std::endl;

  // // 默认拷贝构造
  // JSONObject o1 = o;
  // std::cout << o1 << std::endl;
  // // std::cout << o1 << std::endl;

  // std::cout << "---------------\n";

  JSONParser parser{};
  std::string s =
      R"({"adas": 165.5,"asdas":{"asx":999,"abx":"213465"},"xxx": [123.9,156,"asxasx"]})";
  std::string s1 = R"([["awdawd",1561],165])";
  auto res = parser.parse(s);
  std::cout << res.first << std::endl;
  std::cout << parser.beautify(s) << std::endl;
  
}