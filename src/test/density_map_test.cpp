//
// Created by nicola on 06/10/17.
//

#include <opencv2/opencv.hpp>


#include <src/kernels/kernel.h>
#include <src/density_estimator/kernel_density_estimator.h>
#include <fstream>
#include <src/sigma_functions/sigma_sigmoid_function.h>
#include <src/kernels/epanechnikov_kernel.h>
#include <src/kernels/histogram_kernel.h>


int main(int argc, char **argv) {
    const char *base = "/mnt/dfb2ead7-9641-4658-8517-aee3c9acb560/Crowd_PETS09/rp_normal_hist_density/60/debug/inliers";
    const char *based = "/mnt/dfb2ead7-9641-4658-8517-aee3c9acb560/Crowd_PETS09/rp_normal_hist_density/60/denseepa/";

    int init = 312;

    float bandwidth = 60;
    for (int j = init; j < 1587; j += 24) {
        std::string name = base + std::to_string(j) + "-0.xml";
        cv::FileStorage fs(name, cv::FileStorage::READ);
        while (!fs.isOpened()) {
            j++;
            name = base + std::to_string(j) + "-0.xml";
            fs = cv::FileStorage(name, cv::FileStorage::READ);
        }
        std::vector<cv::Point2f> inls0;
        fs["Points0"] >> inls0;
        fs.release();
        std::vector<std::pair<double, double>> mmm;
        for (auto &&inl : inls0) {
            mmm.emplace_back(inl.x, inl.y);
        }
        std::unique_ptr<Kernel> kernel(new EpanechnikovKernel(bandwidth));
        KernelDensityEstimator estim(std::move(kernel), bandwidth);
        std::vector<std::pair<double, double>> fakePoints;
        for (int i = 0; i < 5; i++)fakePoints.emplace_back(0.0, bandwidth / 2);
        estim.initEstimator(fakePoints);
        double crosspoint = estim.estimateDensity(std::make_pair(0.0, 0.0));
        double steepness = (2.0 / crosspoint) * log(0.99 / 0.01);
        SigmaSigmoidFunction sigmaFunc(0.5, 5, crosspoint, steepness);

        estim.initEstimator(mmm);

        std::vector<std::vector<double>> density(1234, std::vector<double>(1624));
        for (int y = 0; y < 1234; y++) {
            for (int x = 0; x < 1624; x++) {
                density[y][x] = estim.estimateDensity(std::make_pair(x, y));
            }
        }


        std::vector<std::vector<double>> sigma(1234, std::vector<double>(1624));
        for (int y = 0; y < 1234; y++) {
            for (int x = 0; x < 1624; x++) {
                sigma[y][x] = sigmaFunc.evaluateSigma(density[y][x]);
            }
        }

        std::ofstream ptt(based + std::string("points") + std::to_string(j) + ".txt");
        for (auto &&m : mmm) {
            ptt << m.first << " " << m.second << std::endl;
        }
        ptt.close();

        std::ofstream dent(based + std::string("density") + std::to_string(j) + ".txt");
        std::ofstream sigt(based + std::string("sigma") + std::to_string(j) + ".txt");
        for (int y = 0; y < 1234; y++) {
            for (int x = 0; x < 1624; x++) {
                dent << density[y][x] << " ";
                sigt << sigma[y][x] << " ";
            }
            dent << std::endl;
            sigt << std::endl;
        }
        dent.close();
        sigt.close();
    }




    return 0;
}