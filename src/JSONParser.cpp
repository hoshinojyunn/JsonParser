#include "JSONParser.h"
#include "JSONObject.h"
#include <charconv>
#include <optional>
#include <regex>
#include <string_view>
#include <variant>
// #include <iostream>

template <class T> std::optional<T> parse_num(std::string_view s) {
    T value;
    auto start = s[0] == '+' ? s.data() + 1 : s.data();
    auto end = s.data() + s.size();
    auto res = std::from_chars(start, end, value);
    // std::cout << s.data();
    /*
        加上res.ptr == end判断 如果不加这个判断
        则如果s为"3.14"时 parse_num<int>会返回3 就轮不到parse_num<double>解析了
    */
    if (res.ec == std::errc() && res.ptr == end) {
        return value;
    }
    return std::nullopt;
}
// 转义
char unescaped_char(char c) {
    switch (c) {
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case '0':
        return '\0';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    case 'f':
        return '\f';
    case 'b':
        return '\b';
    case 'a':
        return '\a';
    default:
        return c;
    }
}

std::pair<JSONObject, size_t> JSONParser::parse(std::string_view json) {
    if (json.empty())
        return {std::monostate{}, 1};
    // 检查有没有空白字符 ' '、'\r'、'\n'等
    else if (size_t i = json.find_first_not_of(" \n\r\t\v\f\0");
             i != 0 && i != json.npos) {
        auto [ret, len] = parse(json.substr(i));
        /*
            要加上i 因为空白字符也算
            比如：解析"   abc"(三个空格)时，i会为3 len为3(吃掉abc三个字符)
            解析下一个JSONObject时 自然要从6开始 即3+3
        */
        return {std::move(ret), i + len};
    }
    // 解析数字类型 int double
    else if ((json[0] >= '0' && json[0] <= '9') || json[0] == '+' ||
             json[0] == '-') {
        std::regex reg{"[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?"};
        std::cmatch match;
        // 正则匹配能够自动剔除垃圾字符 如3.516asdxxxa 则只会匹配3.516
        if (std::regex_search(json.data(), json.data() + json.size(), match,
                              reg)) {
            // 匹配字符串
            std::string s = match.str();
            if (auto res = parse_num<int>(s); res.has_value()) {
                return {res.value(), s.size()};
            }
            if (auto res = parse_num<double>(s); res.has_value()) {
                return {res.value(), s.size()};
            }
        }
    }
    // 解析字符串
    else if (json[0] == '"') {
        std::string str{};
        enum {
            Raw,     // 原生字符
            Escaped, // 转义字符
        } phase = Raw;
        size_t i{};
        for (i = 1; i < json.size(); ++i) {
            if (phase == Raw) {
                // 如果是 '\' 后面跟的是转义
                if (json[i] == '\\') {
                    phase = Escaped;
                }
                /*
                    要把'"'判断放到'\\'判断的后面
                    否则如果遇到\" 就会提前退出 而不会把'"'转义
                    不能直接break 要把i++
                */
                else if (json[i] == '"') {
                    i++;
                    break;
                } else {
                    str += json[i];
                }
            } else {
                str += unescaped_char(json[i]);
                phase = Raw;
            }
        }
        return {std::move(str), i};
    }
    // 解析列表
    else if (json[0] == '[') {
        JSONList list{};
        JSONListPtr res = std::make_shared<JSONList>(list);
        size_t i{};
        for (i = 1; i < json.size();) {
            if (json[i] == ']') {
                i++;
                break;
            }
            auto [ret, len] = parse(json.substr(i));
            // 解析出错
            if (len == 0) {
                i = 0;
                break;
            }
            res->push_back(std::move(ret));
            i += len;
            if (json[i] == ',')
                i++;
        }
        return {std::move(res), i};
    }
    // 解析map
    else if (json[0] == '{') {
        JSONDict dict{};
        JSONDictPtr res = std::make_shared<JSONDict>(dict);
        size_t i{};
        for (i = 1; i < json.size();) {
            if (json[i] == '}') {
                i++;
                break;
            }
            // size_t pos = json.find_first_of(':', i);
            // // 获得key值 substr: 从i开始 往后截取pos-i个字符
            // auto key = std::string{json.substr(i, pos-i)};
            // i = pos + 1;
            // if(json[i] == ' ')i++;

            // 解析key 改进版
            auto [_key, key_len] = parse(json.substr(i));
            // 解析key错误
            if (key_len == 0) {
                i = 0;
                break;
            }
            i += key_len;
            // 限制_key.inner类型
            if (!std::holds_alternative<std::string>(_key.inner)) {
                i = 0;
                break;
            }
            if (json[i] == ':')
                i++;
            auto key = std::move(std::get<std::string>(_key.inner));
            // 解析value
            auto [_value, len] = parse(json.substr(i));
            // 解析value错误
            if (len == 0) {
                i = 0;
                break;
            }
            res->insert_or_assign(std::move(key), std::move(_value));
            i += len;
            if (json[i] == ',')
                i++;
        }
        return {std::move(res), i};
    }

    return {std::monostate{}, 1};
}

std::string JSONParser::beautify(std::string_view json) {
    auto [obj, _] = parse(json);
    // json解析出来必为dict 不是dict则解析错误
    if (!std::holds_alternative<JSONDictPtr>(obj.inner)) {
        return {};
    }
    auto dictPtr = std::get<JSONDictPtr>(obj.inner);
    std::string res{};
    res.append("{\n\t");
    for (auto [key, value] : *dictPtr) {
        res.append('"' + key + '"' + ": ");
        res.append(value.toString() + ",\n\t");
    }
    if (!(*dictPtr).empty()) {
        res.pop_back(); // 除去最后的\t
        res.pop_back(); // 除去最后的\n
        res.pop_back(); // 除去最后的 ‘,’
    }
    res.push_back('\n');
    res.push_back('}');
    return std::move(res);
}

JSONDict JSONParser::toJSONDict(std::string_view json) {
    auto [obj, _] = parse(json);
    // 解析出JSONDict失败
    if (!std::holds_alternative<JSONDictPtr>(obj.inner)) {
        return {};
    }
    auto dictPtr = std::get<JSONDictPtr>(obj.inner);
    return std::move(*dictPtr);
}
