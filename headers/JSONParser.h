#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JSONObject.h"
#include <string_view>

class JSONParser {

public:
  std::pair<JSONObject, size_t> parse(std::string_view json);
  std::string beautify(std::string_view json);
};

#endif