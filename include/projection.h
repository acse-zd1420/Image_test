/** 
* @file projection.h 
* @brief this header file contains all the declarations of projections methods that can be applied to a volume.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#ifndef ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_PROJECTION_H
#define ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_PROJECTION_H

#include "Image.h"
#include "volume.h"

/**
 * @brief The Projection class provides functions for generating projections from a Volume.
 */
class Projection {
public:
    /**
     * @brief Computes the Maximum Intensity Projection (MIP) from the given Volume.
     * @param vol The Volume object from which to generate the MIP.
     * @param filter_method The method used for filtering (default is 3).
     * @param kernelSize The size of the kernel used for filtering (default is 7).
     * @return The MIP image.
     */
    static Image MIP(Volume &vol, const int& filter_method=3, int kernelSize=7, double sigma = 2.0);

    /**
     * @brief Computes the Minimum Intensity Projection (MinIP) from the given Volume.
     * @param vol The Volume object from which to generate the MinIP.
     * @param filter_method The method used for filtering (default is 3).
     * @param kernelSize The size of the kernel used for filtering (default is 7).
     * @return The MinIP image.
     */
    static Image MinIP(Volume &vol, const int& filter_method=3, int kernelSize=7, double sigma = 2.0);

    /**
     * @brief Computes the Average Intensity Projection (AIP) from the given Volume.
     * @param vol The Volume object from which to generate the AIP.
     * @param filter_method The method used for filtering (default is 3).
     * @param kernelSize The size of the kernel used for filtering (default is 7).
     * @return The AIP image.
     */
    static Image AIP(Volume &vol, const int& filter_method=3, int kernelSize=7, double sigma = 2.0);
};

#endif //ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_PROJECTION_H
