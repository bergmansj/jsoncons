// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://sourceforge.net/projects/jsoncons/files/ for latest version
// See https://sourceforge.net/p/jsoncons/wiki/Home/ for documentation.

#ifndef JSONCONS_JSON_DESERIALIZER_HPP
#define JSONCONS_JSON_DESERIALIZER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <istream>
#include <cstdlib>
#include <memory>
#include "jsoncons/json_char_traits.hpp"
#include "jsoncons/json1.hpp"
#include "jsoncons/json_input_handler.hpp"

namespace jsoncons {

template <class Char>
class basic_json_deserializer : public basic_json_input_handler<Char>
{
    enum structure_type {object_t, array_t};
    struct stack_item
    {
        stack_item(structure_type type, size_t minimum_structure_capacity)
            : type_(type)
        {
            minimum_structure_capacity_ = minimum_structure_capacity;
            switch (type)
            {
            case object_t:
                structure_.object_ = new json_object<Char>();
                structure_.object_->reserve(minimum_structure_capacity);
                break;
            case array_t:
                structure_.array_ =  new json_array<Char>();
                structure_.array_->reserve(minimum_structure_capacity);
                break;
            }
        }
        bool is_object() const
        {
            return type_ == object_t;
        }
        bool is_array() const
        {
            return type_ == array_t;
        }

        std::pair<std::basic_string<Char>,basic_json<Char>> pair_;
        structure_type type_;
        union {
            json_object<Char>* object_;
            json_array<Char>* array_;
        } structure_;
        size_t minimum_structure_capacity_;
    };

public:
    ~basic_json_deserializer()
    {
        if (stack_.size() > 0)
        {
            // Abnormal termination, clean up stack
            for (size_t i = 0; i < stack_.size(); ++i)
            {
                try
                {
                    switch (stack_[i].type_)
                    {
                    case object_t:
                        delete stack_[i].structure_.object_;
                        break;
                    case array_t:
                        delete stack_[i].structure_.array_;
                        break;
                    }
                }
                catch (...)
                {
                }
            }
        }
    }

    virtual void begin_json()
    {
    }

    virtual void end_json()
    {
    }

    virtual void begin_object(const basic_parsing_context<Char>& context)
    {
        stack_.push_back(stack_item(object_t,context.minimum_structure_capacity()));
    }

    virtual void end_object(const basic_parsing_context<Char>& context)
    {
        //std::cout << "OBJECT minimum=" << stack_.back().minimum_structure_capacity_ << ", actual=" << stack_.back().structure_.object_->capacity() << ", size=" << stack_.back().structure_.object_->size() << std::endl;
		std::unique_ptr<json_object<Char>> var = std::unique_ptr<json_object<Char>>(stack_.back().structure_.object_);
        var->sort_members();
		stack_.pop_back();
        if (stack_.size() > 0)
        {
            if (stack_.back().is_object())
            {
                stack_.back().pair_.second = basic_json<Char>(var.release());	    
                stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
            }
            else
            {
                basic_json<Char> val(var.release());	    
                stack_.back().structure_.array_->push_back(std::move(val));
            }
        }
        else
        {
            basic_json<Char> val(var.release());	    
            root_ = std::move(val);
        }
    }

    virtual void begin_array(const basic_parsing_context<Char>& context)
    {
        stack_.push_back(stack_item(array_t,context.minimum_structure_capacity()));
    }

    virtual void end_array(const basic_parsing_context<Char>& context)
    {
        //std::cout << "ARRAY minimum=" << stack_.back().minimum_structure_capacity_ << ", actual=" << stack_.back().structure_.array_->capacity() << ", size=" << stack_.back().structure_.array_->size() << std::endl;
        json_array<Char>* var = stack_.back().structure_.array_;
        stack_.pop_back();
        if (stack_.size() > 0)
        {
            if (stack_.back().is_object())
            {
                stack_.back().pair_.second = basic_json<Char>(var);	    
                stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
            }
            else
            {
                basic_json<Char> val(var);	    
                stack_.back().structure_.array_->push_back(std::move(val));
            }
        }
        else
        {
            basic_json<Char> val(var);	    
            root_ = std::move(val);
        }
    }

    virtual void name(const std::basic_string<Char>& name, const basic_parsing_context<Char>& context)
    {
        stack_.back().pair_.first = name;
    }

    virtual void value(const std::basic_string<Char>& value, const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>(value);	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else 
        {
            basic_json<Char> val(value);
            stack_.back().structure_.array_->push_back(std::move(val));
        }
    }

    virtual void value(double value, const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>(value);	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else
        {
            basic_json<Char> val(value);
            stack_.back().structure_.array_->push_back(val);
        }
    }

    virtual void value(long long value, const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>(value);	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else
        {
            basic_json<Char> val(value);
            stack_.back().structure_.array_->push_back(val);
        }
    }

    virtual void value(unsigned long long value, const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>(value);	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else
        {
            basic_json<Char> val(value);
            stack_.back().structure_.array_->push_back(val);
        }
    }

    virtual void value(bool value, const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>(value);	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else
        {
            basic_json<Char> val(value);
            stack_.back().structure_.array_->push_back(val);
        }
    }

    virtual void null_value(const basic_parsing_context<Char>& context)
    {
        if (stack_.back().is_object())
        {
            stack_.back().pair_.second = basic_json<Char>();	    
            stack_.back().structure_.object_->push_back(std::pair<std::basic_string<Char>,basic_json<Char>>(std::move(stack_.back().pair_)));
        } 
        else
        {
            basic_json<Char> val;
            stack_.back().structure_.array_->push_back(val);
        }
    }

    basic_json<Char>& root()
    {
        return root_;
    }

private:
	basic_json<Char> root_;
    std::vector<stack_item> stack_;
};

typedef basic_json_deserializer<char> json_deserializer;

}

#endif
