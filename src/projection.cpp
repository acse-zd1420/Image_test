/** 
* @file projection.cpp
* @brief this file contains the implementation of all the projections methods that can be applied to a volume.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#include "projection.h"
#include "filter.h"

/**
 * @details This function computes the Maximum Intensity Projection (MIP) from the given Volume.
 * Volume is a class that contains a vector of Image objects. The MIP is computed by taking the maximum pixel value of each Image in the Volume.
 * @author Shengzhi Tian
 */
Image Projection::MIP(Volume &vol, const int& filter_method, int kernelSize, double sigma) {
    if (filter_method == 1) {
        Filter::gaussian_blur_3d(vol, kernelSize, sigma);
    } else if (filter_method == 2) {
        Filter::median_blur_3d(vol, kernelSize);
    } else if (filter_method == 3) {
        // Do nothing
    } else {
        throw std::invalid_argument("Unsupported filter method");
    }

    std::vector<Image> imgs = vol.getImages();
    int num_imgs = imgs.size();
    int w = imgs[0].width();
    int h = imgs[0].height();
    int c = imgs[0].channels();

    auto* data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++) {
        unsigned char max_val = 0;
        for (int z = 0; z < num_imgs; z++) {
            unsigned char* img_data = imgs[z].get_data();
            max_val = std::max(max_val, img_data[i]);
        }
        data[i] = max_val;
    }

    return {data, w, h, c};
}

/**
 * @details This function computes the Minimum Intensity Projection (MinIP) from the given Volume.
 * Volume is a class that contains a vector of Image objects. The MinIP is computed by taking the minimum pixel value of each Image in the Volume.
 * @author Shengzhi Tian
 */
Image Projection::MinIP(Volume &vol, const int& filter_method, int kernelSize, double sigma) {
    if (filter_method == 1) {
        Filter::gaussian_blur_3d(vol, kernelSize, sigma);
    } else if (filter_method == 2) {
        Filter::median_blur_3d(vol, kernelSize);
    } else if (filter_method == 3) {
        // Do nothing
    } else {
        throw std::invalid_argument("Unsupported filter method");
    }

    std::vector<Image> imgs = vol.getImages();
    int num_imgs = imgs.size();
    int w = imgs[0].width();
    int h = imgs[0].height();
    int c = imgs[0].channels();

    auto* data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++) {
        unsigned char min_val = 255;
        for (int z = 0; z < num_imgs; z++) {
            unsigned char* img_data = imgs[z].get_data();
            min_val = std::min(min_val, img_data[i]);
        }
        data[i] = min_val;
    }

    return {data, w, h, c};
}


/**
 * @details This function computes the Average Intensity Projection (AIP) from the given Volume.
 * Volume is a class that contains a vector of Image objects. The AIP is computed by averaging the pixel values of each Image in the Volume.
 * @author Shengzhi Tian
 */
Image Projection::AIP(Volume &vol, const int& filter_method, int kernelSize, double sigma) {
    if (filter_method == 1) {
        Filter::gaussian_blur_3d(vol, kernelSize, sigma);
    } else if (filter_method == 2) {
        Filter::median_blur_3d(vol, kernelSize);
    } else if (filter_method == 3) {
        // Do nothing
    } else {
        throw std::invalid_argument("Unsupported filter method");
    }

    std::vector<Image> imgs = vol.getImages();
    int num_imgs = imgs.size();
    int w = imgs[0].width();
    int h = imgs[0].height();
    int c = imgs[0].channels();

    auto* data = new unsigned char[w * h * c];
    for (int i = 0; i < w * h * c; i++) {
        double sum = 0;
        for (int z = 0; z < num_imgs; z++) {
            unsigned char* img_data = imgs[z].get_data();
            sum += img_data[i];
        }
        auto avg = static_cast<unsigned char>(std::round(sum / num_imgs));
        data[i] = avg;
    }

    return {data, w, h, c};
}
