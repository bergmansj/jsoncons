// Copyright 2013 Daniel Parker
// Distributed under Boost license

#if defined(_MSC_VER)
#include "windows.h" // test no inadvertant macro expansions
#endif
#include <jsoncons/json.hpp>
#include <jsoncons/json_encoder.hpp>
#include <catch/catch.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <cstdint>

using namespace jsoncons;

namespace jsoncons_member_traits_decl_tests {

    struct book
    {
        std::string author;
        std::string title;
        double price;
    };

    class book2
    {
        std::string author_;
        std::string title_;
    public:
        book2() = default;

        book2(const std::string& author, const std::string& title)
            : author_(author), title_(title)
        {
        }

        book2(const book2&) = default;

        book2& operator=(const book2&) = default;

        const std::string& author() const
        {
            return author_;
        }

        const std::string& title() const
        {
            return title_;
        }

    };
} // namespace jsoncons_member_traits_decl_tests
 
namespace ns = jsoncons_member_traits_decl_tests;

JSONCONS_MEMBER_TRAITS_DECL(ns::book,author,title,price);

JSONCONS_ACONS_TRAITS_DECL(ns::book2,author,title);

TEST_CASE("JSONCONS_MEMBER_TRAITS_DECL tests")
{
    std::string author = "Haruki Murakami"; 
    std::string title = "Kafka on the Shore";
    double price = 25.17;

    ns::book book{author, title, price};
    ns::book2 book2{author, title};

    SECTION("book")
    {
        std::string s;

        encode_json(book, s);

        json j = decode_json<json>(s);

        REQUIRE(j.is<ns::book>() == true);
        REQUIRE(j.is<ns::book2>() == true);

        CHECK(j["author"].as<std::string>() == author);
        CHECK(j["title"].as<std::string>() == title);
        CHECK(j["price"].as<double>() == Approx(price).epsilon(0.001));

        json j2(book);

        CHECK(j == j2);

        ns::book val = j.as<ns::book>();
    }

    SECTION("book2")
    {
        std::string s;

        encode_json(book2, s);

        json j = decode_json<json>(s);

        REQUIRE(j.is<ns::book2>() == true);
        REQUIRE(j.is<ns::book>() == false);

        CHECK(j["author"].as<std::string>() == author);
        CHECK(j["title"].as<std::string>() == title);

        json j2(book2);

        CHECK(j == j2);

        ns::book2 val = j.as<ns::book2>();
    }
}

