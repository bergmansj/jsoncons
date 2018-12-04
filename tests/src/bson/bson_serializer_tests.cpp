// Copyright 2016 Daniel Parker
// Distributed under Boost license

#include <catch/catch.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/bson/bson.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <limits>

using namespace jsoncons;
using namespace jsoncons::bson;

TEST_CASE("serialize object to bson")
{
    std::vector<uint8_t> v;
    bson_bytes_serializer serializer(v);

    serializer.begin_object();
    serializer.name("null");
    serializer.null_value();
    serializer.end_object();
    serializer.flush();

    try
    {
        json result = decode_bson<json>(v);
        std::cout << result << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
} 

namespace jsoncons { namespace bson {

        void are_equal(const std::vector<uint8_t>& v, const std::vector<uint8_t>& expected)
        {
            REQUIRE(v.size() == expected.size());

            for (size_t i = 0; i < v.size(); ++i)
            {
                if (v[i] != expected[i])
                {
                    std::cout << "i: " << i << "\n";
                }
                CHECK(v[i] == expected[i]);
            }
        }
}}

TEST_CASE("serialize array to bson")
{
    SECTION("array")
    {
        std::vector<uint8_t> v;
        bson_bytes_serializer serializer(v);

        serializer.begin_array();
        serializer.int64_value((std::numeric_limits<int64_t>::max)());
        serializer.uint64_value((uint64_t)(std::numeric_limits<int64_t>::max)());
        serializer.double_value((std::numeric_limits<double>::max)());
        serializer.bool_value(true);
        serializer.bool_value(false);
        serializer.null_value();
        serializer.string_value("Pussy cat");
        serializer.byte_string_value(byte_string({'h','i','s','s'}));
        serializer.end_array();
        serializer.flush();

        std::vector<uint8_t> bson = {0x4d,0x00,0x00,0x00,
                                     0x12, // int64
                                     0x30, // '0'
                                     0x00, // terminator
                                     0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,
                                     0x12, // int64
                                     0x31, // '1'
                                     0x00, // terminator
                                     0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,
                                     0x01, // double
                                     0x32, // '2'
                                     0x00, // terminator
                                     0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7f,
                                     0x08, // bool
                                     0x33, // '3'
                                     0x00, // terminator
                                     0x01,
                                     0x08, // bool
                                     0x34, // '4'
                                     0x00, // terminator
                                     0x00,
                                     0x0a, // null
                                     0x35, // '5'
                                     0x00, // terminator
                                     0x02, // string
                                     0x36, // '6'
                                     0x00, // terminator
                                     0x0a,0x00,0x00,0x00, // string length
                                     'P','u','s','s','y',' ','c','a','t',
                                     0x00, // terminator
                                    0x05, // binary
                                    0x37, // '7'
                                    0x00, // terminator
                                    0x04,0x00,0x00,0x00, // byte string length
                                    'h','i','s','s',
                                     0x00 // terminator
                                     };
        jsoncons::bson::are_equal(v,bson);
    }
}
