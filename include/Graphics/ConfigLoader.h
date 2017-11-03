#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/log/trivial.hpp>
#include <exception>
#include <type_traits>

namespace gpx {
namespace Tags {

#define SPIKY_CONFIG_DEF_TYPES(ID, TYPE) \
struct TYPE : std::integral_constant<int, ID> { \
};

SPIKY_CONFIG_DEF_TYPES(1, DisplayConfig)
SPIKY_CONFIG_DEF_TYPES(2, GLSpecConfig)
#undef SPIKY_CONFIG_DEF_TYPES
} //namespace Tags

template <typename T>
class ConfigLoader {};
} //namespace Spiky
