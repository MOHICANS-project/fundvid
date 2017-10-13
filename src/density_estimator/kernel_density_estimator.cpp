//
// Created by nicola on 04/10/17.
//

#include "kernel_density_estimator.h"


KernelDensityEstimator::KernelDensityEstimator(std::unique_ptr<Kernel> kernel, double radius) : kernel(
        std::move(kernel)), radius(radius) {}

double KernelDensityEstimator::estimateDensity(std::pair<double, double> s) {
    //look for points around x
    box query_box(point(s.first - radius, s.second - radius), point(s.first + radius, s.second + radius));
    std::vector<value> result_query;
    rtree.query(bgi::within(query_box), std::back_inserter(result_query));

    if (result_query.empty())return 0;

    //evaluate kernel function w.r.t to neighbor points
    std::vector<std::pair<double, double>> input;
    for (auto &&item : result_query) {
        input.emplace_back(item.get<0>(), item.get<1>());
    }

    return kernel->K(s, input);
}

void KernelDensityEstimator::initEstimator(std::vector<std::pair<double, double>> &pointCloud) {
    rtree.clear();
    for (auto const &pt : pointCloud) {
        rtree.insert(point(pt.first, pt.second));
    }
}

