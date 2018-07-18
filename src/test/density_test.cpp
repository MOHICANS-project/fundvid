//
// Created by nicola on 04/10/17.
//
#include <boost/geometry.hpp>
#include <iostream>


#include <boost/geometry/index/rtree.hpp>

namespace bg=boost::geometry;
namespace bgi=boost::geometry::index;

typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef point value;

typedef bgi::rtree<value, bgi::quadratic<16>> rtree_t;

int main(int argc, char **argv) {


    rtree_t rtree;

    rtree.insert(point(1.0, 1.0));
    rtree.insert(point(1.5, 2.0));
    rtree.insert(point(1.5, 1.5));
    rtree.insert(point(2.0, 2.0));
    rtree.insert(point(2.0, 1.0));

    box query_box(point(1.2, 0.5), point(5.0, 1.75));
    std::vector<value> result_s;
    rtree.query(bgi::within(query_box), std::back_inserter(result_s));

    //std::cout << result_s.size() << std::endl;
    //std::cout << result_s[0].get<0>() << std::endl;

    for (auto const &result : result_s) {
        std::cout << result.get<0>() << " " << result.get<1>() << std::endl;
    }
    return 0;
}
