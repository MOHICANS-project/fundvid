//
// Created by nicola on 06/10/17.
//

#include <opencv2/opencv.hpp>


#include <src/kernels/kernel.h>
#include <src/density_estimator/kernel_density_estimator.h>
#include <fstream>
#include <src/sigma_functions/sigma_sigmoid_function.h>
#include <src/kernels/epanechnikov_kernel.h>


int main(int argc, char **argv) {
    const char *name = "/mnt/dfb2ead7-9641-4658-8517-aee3c9acb560/Crowd_PETS09/video_eval_rp_normal_badinit_eps100/debug/inliers936-78.xml";
    //const char *name="/mnt/dfb2ead7-9641-4658-8517-aee3c9acb560/Crowd_PETS09/video_eval_rp_normal_badinit_eps100/debug/inliers480-78.xml";

    cv::FileStorage fs(name, cv::FileStorage::READ);
    if (!fs.isOpened())return 1;
    std::vector<cv::Point2f> inls0;
    fs["Points0"] >> inls0;
    fs.release();

    std::vector<std::pair<double, double>> mmm;
    for (auto &&inl : inls0) {
        mmm.emplace_back(inl.x, inl.y);
    }



    //std::unique_ptr<Kernel> kernel(new HistogramKernel(100));
    std::unique_ptr<Kernel> kernel(new EpanechnikovKernel(100));
    KernelDensityEstimator estim(std::move(kernel), 100);

    std::vector<std::pair<double, double>> fakePoints;
    for (int i = 0; i < 5; i++)fakePoints.emplace_back(0.0, 100 / 2);
    estim.initEstimator(fakePoints);
    double crosspoint = estim.estimateDensity(std::make_pair(0.0, 0.0));

    //SigmaStepFunction sigmaFunc(0.5,5,crosspoint);

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
    std::ofstream ptt("points936sk.txt");
    for (auto &&m : mmm) {
        ptt << m.first << " " << m.second << std::endl;
    }
    ptt.close();
    std::ofstream dent("density936sk.txt");
    std::ofstream sigt("sigma936sk.txt");
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

    return 0;
}