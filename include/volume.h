/** 
* @file volume.h 
* @brief this header file contains the declarations of the Volume class that handles a collection of images as a volume.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#ifndef ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_VOLUME_H
#define ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_VOLUME_H

#include "stb_image.h"
#include "stb_image_write.h"
#include "Image.h"

#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


/**
 * @brief The Volume class handles a collection of images as a volume.
 */
class Volume {
public:
    /**
     * @brief Constructs a Volume object from the images in the specified directory.
     * @param directoryPath The path to the directory containing the images.
     * @param desiredChannels The desired number of channels in the loaded images (default is 0).
     */
    Volume(const std::string& directoryPath, int desiredChannels = 0);

    /**
     * @brief Constructs a Volume object from the images within the specified range in the directory.
     * @param directoryPath The path to the directory containing the images.
     * @param z1 The starting index of images to include in the volume.
     * @param z2 The ending index of images to include in the volume.
     * @param desiredChannels The desired number of channels in the loaded images (default is 0).
     */
    Volume(const std::string& directoryPath, int z1, int z2, int desiredChannels = 0);

    ~Volume();

    // ~Volume();

    /**
     * @brief Saves the volume to the specified directory.
     * @param directoryPath The path to the directory to save the volume.
     */
    void save(const std::string& directoryPath);

    /**
     * @brief Retrieves the images in the volume.
     * @return A vector containing the images in the volume.
     */
    std::vector<Image> getImages() const;

    /**
     * @brief Retrieves the filenames in a directory.
     * @return A vector containing the filenames in the directory.
     */
    static std::vector<fs::directory_entry> getFileEntries(const std::string& directoryPath);

private:
    std::vector<Image> images; /**< Vector to hold loaded images. */

    /**
     * @brief Partitions the directory entries for quicksort.
     * @param entries The vector of directory entries to partition.
     * @param low The lower index of the partition.
     * @param high The higher index of the partition.
     * @return The index of the pivot element.
     */
    size_t partition(std::vector<fs::directory_entry>& entries, size_t low, size_t high);

    /**
     * @brief Sorts the filenames of directory entries.
     * @param entries The vector of directory entries to sort.
     */
    void sortFilenames(std::vector<fs::directory_entry>& entries);

    /**
     * @brief Sorts the directory entries using quicksort algorithm.
     * @param entries The vector of directory entries to sort.
     * @param low The lower index of the partition.
     * @param high The higher index of the partition.
     */
    void quicksort(std::vector<fs::directory_entry>& entries, size_t low, size_t high);
};

#endif //ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_VOLUME_H
