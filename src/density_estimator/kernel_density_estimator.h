//
// Created by nicola on 04/10/17.
//

#ifndef MULTICAMNET_UNBIASED_DENSITY_ESTIMATOR_H
#define MULTICAMNET_UNBIASED_DENSITY_ESTIMATOR_H

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <src/kernels/kernel.h>


#include "density_estimator.h"

namespace bg=boost::geometry;
namespace bgi=boost::geometry::index;


class KernelDensityEstimator : public DensityEstimator {

    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef point value;
    typedef bgi::rtree<value, bgi::quadratic<16>> rtree_t;


    rtree_t rtree;
    std::unique_ptr<Kernel> kernel;
    double radius;

public:
    KernelDensityEstimator(std::unique_ptr<Kernel> kernel, double radius);

    void initEstimator(std::vector<std::pair<double, double>> &pointCloud) override;

    double estimateDensity(std::pair<double, double> s) override;
};


#endif //MULTICAMNET_UNBIASED_DENSITY_ESTIMATOR_H
