#ifndef PRINT_H
#define PRINT_H
#include <variant>
#include <string>

template<class T>
concept T_String = requires(T a) {
    std::to_string(a);
};

// template<class...Ts>
// struct overloaded : Ts... {
//     using Ts::operator()...;
// };

// template<class...Ts>
// overloaded(Ts...) -> overloaded<Ts...>;

// bool特化(字面量字符串会优先匹配到bool)
std::string variantToString(const bool b){
    return b?"ture":"false";
}

// string特化
std::string variantToString(const std::string&s){
    return '\"' + s + '\"';
}

std::string variantToString(const std::monostate&s){
    return "null";
}

// std::string variantToString(JSONDictPtr& ptr);
// std::string variantToString(JSONListPtr& ptr);

// to_string()特化
template<T_String T>
std::string variantToString(const T &t){
    return std::to_string(t);
}


// variant打印
template<class...Ts>
std::string variantToString(const std::variant<Ts...>&v){
    return std::visit(
        [](auto arg)  { 
            return variantToString(arg); 
        }, v);
}


#endif