# 1.JSONObject类
* JSONObject为存储JSON数据对象的类，使用c++17新特性std::variant实现存储不同类型的对象。
* JSONObject中成员变量inner为std::variant类型，存储JSON数据可能的类型对象
* 该类重载了<<运算符，可直接打印输出JSONObject对象
# 2.JSONParser类
* JSONParser类提供了三个有用的成员函数：1.parse(string_view json)，用于解析json字符串，从json字符串中解析出数据对象，并将其转换为c++对应的类型。2.beautify(string_view json)，用于格式化json字符串。3.toJSONDict(std::string_view json),用于将json字符串转换为c++对象。(详细查看代码)
# 3.print.h
  JSONObject的打印实现，使用std::visit实现variant对象的对不同数据类型的处理。
# 示例
### 1.测试
```c++
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
```
### 2.运行结果
```txt
[["awdawd", 1561], 165]
{
        "c": [123.900000, 156, "asxasx"],
        "b": {"abx": "213465", "asx": 999},
        "a": 165.500000
}
165.500000
{"abx": "213465", "asx": 999}
[123.900000, 156, "asxasx"]
```