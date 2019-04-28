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

namespace ns {

struct book
{
    std::string author;
    std::string title;
    double price;

    friend std::ostream& operator<<(std::ostream& os, const book& b)
    {
        std::cout << "author: " << b.author << ", title: " << b.title << ", price: " << b.price << "\n";
        return os;
    }
};

JSONCONS_MEMBER_TRAITS_DECL(ns::book,author,title,price);

} // namespace ns

TEST_CASE("JSONCONS_MEMBER_TRAITS_DECL tests")
{
    ns::book book_list{"Haruki Murakami", "Kafka on the Shore", 25.17};

    std::string s;
    encode_json(book_list, s, indenting::indent);

    std::cout << "s: " << s << "\n";
}

