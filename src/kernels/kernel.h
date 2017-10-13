//
// Created by nicola on 05/10/17.
//

#ifndef MULTICAMNET_KERNEL_H
#define MULTICAMNET_KERNEL_H


#include <utility>

class Kernel {

public:
    virtual double K(std::pair<double, double> s, const std::vector<std::pair<double, double>> &data)=0;

    virtual void declare()=0;
};

#endif //MULTICAMNET_KERNEL_H
