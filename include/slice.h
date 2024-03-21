/** 
* @file slice.h 
* @brief this header file contains all the declarations of slicing methods that can be applied to a volume.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

# pragma once

#include "volume.h"
#include "Image.h"

/**
 * @brief Enumerates slice types.
 */
enum class SliceType {
    XZ,
    YZ
};

/**
 * @brief The Slice class provides methods for slicing a volume.
 */
class Slice {
public:
    /**
     * @brief Generates a slice from the given volume for any plane.
     * @param volume The Volume object from which to generate the slice.
     * @param n The index of the slice.
     * @param type The type of slice (XZ or YZ).
     * @return The generated slice image.
     */
    static Image slice(const Volume& volume, int n, SliceType type);
};