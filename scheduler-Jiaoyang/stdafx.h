// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <vector>
 #include  <utility>

#include <google/dense_hash_map>

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/graph/adjacency_list.hpp>
// boost graph helpers
// http://stackoverflow.com/questions/13453350/replace-bgl-iterate-over-vertexes-with-pure-c11-alternative
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::bidirectionalS > searchGraphTraits_t;
typedef searchGraphTraits_t::vertex_descriptor vertex_t;
typedef searchGraphTraits_t::edge_descriptor edge_t;
typedef std::pair<float, float> position_t;


// TODO: reference additional headers your program requires here


using std::vector;