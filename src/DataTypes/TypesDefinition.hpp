#pragma once

#include <set>
#include <string>
#include <unordered_map>

namespace Alg
{
    using String = std::string;

    template <typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    template <typename Value>
    using Set = std::set<Value>;

}

namespace Data
{
    using String = std::string;

    template <typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    template <typename Value>
    using Set = std::set<Value>;

}