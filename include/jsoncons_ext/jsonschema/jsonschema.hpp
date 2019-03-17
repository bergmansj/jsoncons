// Copyright 2019 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSONSCHEMA_JSONSCHEMA_HPP
#define JSONCONS_JSONSCHEMA_JSONSCHEMA_HPP

#include <jsoncons/json.hpp>

namespace jsoncons { namespace jsonschema {

template <class Json>
class basic_validator : public basic_json_content_handler<typename Json::char_type>
{
    Json schema_;
public:
    typedef typename Json::char_type char_type;
    typedef typename basic_json_content_handler<char_type>::string_view_type string_view_type;

    basic_validator(const Json& schema)
        : schema_(schema)
    {
    }
    basic_validator(Json&& schema)
        : schema_(std::move(schema))
    {
    }
    basic_validator(const basic_validator&) = default;
    basic_validator(basic_validator&&) = default;

    basic_validator& operator=(const basic_validator&) = default;
    basic_validator& operator=(basic_validator&&) = default;

    bool is_valid() const
    {
        return true;
    }
private:
    void do_flush() override
    {
    }

    bool do_begin_object(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_end_object(const serializing_context&) override
    {
        return true;
    }

    bool do_begin_array(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_end_array(const serializing_context&) override
    {
        return true;
    }

    bool do_name(const string_view_type&, const serializing_context&) override
    {
        return true;
    }

    bool do_null_value(semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_string_value(const string_view_type&, semantic_tag_type, const serializing_context&) override
    {
        return true;
    }

    bool do_byte_string_value(const byte_string_view&,
                              semantic_tag_type, 
                              const serializing_context&) override
    {
        return true;
    }

    bool do_int64_value(int64_t, 
                        semantic_tag_type, 
                        const serializing_context&) override
    {
        return true;
    }

    bool do_uint64_value(uint64_t, 
                         semantic_tag_type, 
                         const serializing_context&) override
    {
        return true;
    }

    bool do_double_value(double, 
                         semantic_tag_type,
                         const serializing_context&) override
    {
        return true;
    }

    bool do_bool_value(bool, semantic_tag_type, const serializing_context&) override
    {
        return true;
    }
};

typedef basic_validator<json> validator;

}}

#endif
