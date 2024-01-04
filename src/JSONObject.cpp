#include "JSONObject.h"
#include "print.h"

// vector<variant>打印
std::string variantToString(const JSONListPtr &ptr) {
    std::string result = "[";
    // elem: JSONObject
    for (auto &elem : (*ptr)) {
        result += variantToString(elem.inner) + ", ";
    }
    // 若当前JSONListPtr不为空 即以上for循环至少执行一次
    if (!(*ptr).empty()) {
        result.pop_back(); // 移除末尾多余的空格与逗号
        result.pop_back();
    }
    result += "]";
    return result;
}

// unordered_map<string, variant>打印
std::string variantToString(const JSONDictPtr &ptr) {
    std::string res = "{";
    for (auto [key, value] : (*ptr)) {
        res += "\"" + key + "\": " + variantToString(value.inner) + ", ";
    }
    if (!(*ptr).empty()) {
        res.pop_back();
        res.pop_back();
    }
    res += '}';
    return res;
}

std::string JSONObject::toString() { return variantToString(this->inner); }

std::string JSONObject::toString() const {
    return variantToString(this->inner);
}

std::ostream &operator<<(std::ostream &out, JSONObject &o) {
    return out << o.toString();
}
