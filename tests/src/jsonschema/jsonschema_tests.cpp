// Copyright 2019 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <catch/catch.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <ctime>
#include <new>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonschema/jsonschema.hpp>

using namespace jsoncons;

namespace jsoncons { namespace jsonschema {

void validate(jsonschema::validator& validator, const std::string& jtext)    
{
    json j = json::parse(jtext);
    j.dump(validator);
}

}}

TEST_CASE("jsonschema tests")
{
    std::string s = "{}";
    json schema = json::parse(s);

    jsonschema::validator validator(std::move(schema)); 
    jsonschema::validate(validator, "42");

}

