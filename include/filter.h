/** 
* @file filter.h 
* @brief this header file will contain all the filters (2D / 3D) that can be applied to the image and volume data
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#ifndef FILTER_H
#define FILTER_H

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <memory>
#include <random>
#include <cstring>
#include <unordered_set>
#include "Image.h"
#include "volume.h"


/**
 * @brief The Filter class contains various image processing filters and transformations.
 */
class Filter {
public:
    /**
     * @brief Adjusts the brightness of the image.
     * @param img The image to adjust.
     * @param brightness The amount of brightness adjustment.
     */
    static void adjust_brightness(Image& img, int brightness); //adjusts the brightness of the image
    
    /**
     * @brief Automatically adjusts the brightness of the image.
     * @param img The image to adjust.
     */
    static void auto_adjust_brightness(Image& img); //automatically adjusts the brightness of the image

    /**
     * @brief Applies median blur to the image.
     * @param img The image to blur.
     * @param kernelSize The size of the kernel for median blur.
     */
    static void median_blur(Image& img, int kernelSize); //applies median blur to the image

    /**
     * @brief Applies box blur to the image.
     * @param img The image to blur.
     * @param kernelSize The size of the kernel for box blur.
     */
    static void box_blur(Image& img, int kernelSize); //applies box blur to the image

    /**
     * @brief Applies Gaussian blur to the image.
     * @param img The image to blur.
     * @param kernelSize The size of the kernel for Gaussian blur.
     */
    static void gaussian_blur_2d(Image &img, int kernelSize, double sigma=2.0); //applies gaussian blur to the image

    /**
     * @brief Converts the image to grayscale.
     * @param img The image to convert.
     */
    static void RGB2Gray(Image& img); //converts the image to grayscale

    /**
     * @brief Converts the image to HSV color space.
     * @param img The image to convert.
     */
    static void RGB2HSV(Image& img); //converts the image to HSV

    /**
     * @brief Converts the image to RGB color space from HSV.
     * @param img The image to convert.
     */
    static void HSV2RGB(Image& img); //converts the image to RGB from HSV

    /**
     * @brief Converts the image to HSL color space.
     * @param img The image to convert.
     */ 
    static void RGB2HSL(Image& img); //converts the image to HSL

    /**
     * @brief Converts the image to RGB color space from HSL.
     * @param img The image to convert.
     */
    static void HSL2RGB(Image& img); //converts the image to RGB from HSL

    /**
     * @brief Applies histogram equalization to the image.
     * @param img The image to equalize.
     * @param transform The type of transformation to apply.
     */
    static void HistogramEqualization(Image& img, int transform); //applies histogram equalization to the image

    /**
     * @brief Applies thresholding to the image.
     * @param img The image to threshold.
     * @param threshold The threshold value.
     * @param transform The type of transformation to apply.
     */
    static void Tresholding(Image& img, int threshold, int transform); //applies thresholding to the image

    /**
     * @brief Adds salt and pepper noise to the image.
     * @param img The image to add noise to.
     * @param density The density (ratio) of the noise to add.
     */
    static void add_salt_and_pepper(Image& img, float density); //adds salt and pepper noise to the image

    /**
     * @brief Applies Sobel edge detection to the image.
     * @param img The image to apply edge detection to.
     */
    static void apply_sobel_edge_detection(Image& img); //applies sobel edge detection to the image

    /**
     * @brief Applies Prewitt edge detection to the image.
     * @param img The image to apply edge detection to.
     */
    static void apply_prewitt_edge_detection(Image& img); //applies prewitt edge detection to the image

    /**
     * @brief Applies Scharr edge detection to the image.
     * @param img The image to apply edge detection to.
     */
    static void apply_scharr_edge_detection(Image& img); //applies scharr edge detection to the image

    /**
     * @brief Applies Roberts edge detection to the image.
     * @param img The image to apply edge detection to.
     */
    static void apply_roberts_edge_detection(Image& img); //applies roberts edge detection to the image

    /**
     * @brief Applies 3D median blur to the volume.
     * @param vol The volume to blur.
     * @param kernelSize The size of the kernel for 3D median blur.
     */
    static void median_blur_3d(Volume &vol, int kernelSize);

    /**
     * @brief Applies 3D Gaussian blur to the volume.
     * @param vol The volume to blur.
     * @param kernelSize The size of the kernel for 3D Gaussian blur.
     */
    static void gaussian_blur_3d(Volume &vol, int kernelSize, double sigma=2.0);

private:
    /**
     * @brief Returns a 1D array of the Gaussian kernel.
     * @param kernel_size The size of the Gaussian kernel.
     * @param sigma The standard deviation of the Gaussian kernel.
     * @return Pointer to the Gaussian kernel.
     */
    static double *getGaussianKernel(int kernel_size, double sigma);

    /**
     * @brief Applies Gaussian blur to the image in the x direction.
     * @param src The source image data.
     * @param dst The destination image data.
     * @param w The width of the image.
     * @param h The height of the image.
     * @param kernelSize The size of the kernel for Gaussian blur.
     * @param gaussianArray The Gaussian kernel array.
     * @param nc The number of channels of the image.
     */
    static void GaussBlur_x(unsigned char *src, unsigned char *dst, int w, int h, int kernelSize, double *gaussianArray, int nc);

    /**
     * @brief Applies Gaussian blur to the image in the y direction.
     * @param src The source image data.
     * @param dst The destination image data.
     * @param w The width of the image.
     * @param h The height of the image.
     * @param kernelSize The size of the kernel for Gaussian blur.
     * @param gaussianArray The Gaussian kernel array.
     * @param nc The number of channels of the image.
     */
    static void GaussBlur_y(unsigned char *src, unsigned char *dst, int w, int h, int kernelSize, double *gaussianArray, int nc);

    /**
     * @brief Applies edge detection to the image.
     * @param img The image to apply edge detection to.
     * @param horizontal_kernel The horizontal 3x3 kernel for edge detection.
     * @param vertical_kernel The vertical 3x3 kernel for edge detection.
     */
    static void apply_edge_detection(Image& img, const int horizontal_kernel[3][3], const int vertical_kernel[3][3]); //applies edge detection to the image

    /**
     * @brief Performs quick selection algorithm to find the k-th smallest element in the given array.
     * @param arr The array of unsigned chars.
     * @param low The lower index of the array.
     * @param high The higher index of the array.
     * @param k The order statistic to find (1-based index).
     * @return The k-th smallest element in the array.
     */
    static unsigned char quickSelect(std::vector<unsigned char>& arr, int low, int high, int k);

    /**
     * @brief Finds the median value from the given neighborhood.
     * @param neighborhood The neighborhood vector.
     * @return The median value.
     */
    static unsigned char findMedian(std::vector<unsigned char>& neighborhood);
};

#endif // FILTER_H
