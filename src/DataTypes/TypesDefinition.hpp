#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace Alg
{
    using String = std::string;

    template <typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    template <typename Value>
    using Set = std::set<Value>;

    template <typename Value>
    using Vector = std::vector<Value>;

    template <typename Element1, typename Element2>
    using Pair = std::pair<Element1, Element2>;

}

namespace Data
{
    using String = std::string;

    template <typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;

    template <typename Value>
    using Set = std::set<Value>;

    template <typename Value>
    using Vector = std::vector<Value>;

    template <typename Element1, typename Element2>
    using Pair = std::pair<Element1, Element2>;

}