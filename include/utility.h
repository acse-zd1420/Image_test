/** 
* @file utility.h
* @brief this header file contains the declarations of the Utility class that is used to help build the UI interface in main.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @note all functions in this class are created by Georgia Ray (edsml-ger23)
* @date 19/03/2024
*/

#include <iostream>
#include "filter.h"
#include "projection.h"
#include "slice.h"

/**
 * @brief The Utility class provides methods for building the UI interface in main.
 */
class Utility {
public:
    /**
     * @brief Let user Perform 2D image processing on the specified image file.
     * @param imagePath The path to the image file to be processed.
     * @return The processed 2D image.
     */
    static Image twoDImageProcessing(std::string imagePath = "None");

    /**
     * @brief Let user Perform 3D image processing on the specified volume file.
     * @return The processed 3D image.
     */
    static Image threeDImageProcessing();
private:
    /**
     * @brief Adjusts the kernel size to be an odd number if it is not already.
     * @param kernelSize The original kernel size.
     * @return The adjusted kernel size.
     */
    static int adjust_kernel_size(int kernelSize);

    /**
     * @brief Prompts the user to choose the type of projection.
     * @return The chosen projection type.
     */
    static int get_projection_type(double& sigma);

    /**
     * @brief Prompts the user to input the kernel size.
     * @return The kernel size chosen by the user.
     */
    static int get_kernel_size();

    /**
     * @brief Checks if the given range of z values is valid.
     * @param z1 The starting index.
     * @param z2 The ending index.
     * @param maxZ The maximum allowed z value.
     * @param minZ The minimum allowed z value.
     * @return True if the range is valid, false otherwise.
     */
    static bool checkZValidity(int z1, int z2, int maxZ, int minZ);

    // Methods for image enhancement and operations

    /**
     * @brief Allows the user to choose between auto brightness and manual brightness adjustment.
     * @param img The image to apply brightness adjustment.
     */
    static void apply_brightness(Image& img);

    /**
     * @brief Allows the user to choose between median, guassian, and box blur.
     * @param img The image to apply blur.
     */
    static void apply_blur(Image& img);

    /**
     * @brief Allows the user to add salt and pepper noise to the image.
     * @param img The image to apply salt and pepper noise.
     */
    static void apply_salt_and_pepper(Image& img);

    /**
     * @brief Allows the user to choose between manual and auto thresholding.
     * @param img The image to apply thresholding.
     */
    static void apply_thresholding(Image& img);

    /**
     * @brief Allows the user to choose between sobel, prewitt, scharr, and roberts edge detection.
     * @param img The image to apply edge detection.
     */
    static void apply_edge_detection(Image& img);

    /**
     * @brief Allows the user to choose between dilation and erosion.
     * @param img The image to apply dilation or erosion.
     */
    static void apply_histogram(Image& img);

    /**
     * @brief Allows the user to choose between different projection method to use.
     * @param vol The volume to apply projection.
     */
    static Image apply_projection(Volume& vol);

    /**
     * @brief Allows the user to choose between using a slab or the whole volume of images.
     * @param volPath The file path to the volume.
     */
    static Volume slab_or_whole(std::string volPath);

    /**
     * @brief Allows the user to choose between different slice method to use.
     * @param vol The volume to apply slice.
     */
    static Image take_a_slice(Volume vol);

    /**
     * @brief Displays a message and prompts the user to try again.
     * @param message The message to display.
     */
    static void try_again(std::string message);

    /**
     * @brief Applies the sigma filter to the image.
     */
    static double apply_sigma();
};