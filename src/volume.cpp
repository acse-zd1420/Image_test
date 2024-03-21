/** 
* @file volume.cpp 
* @brief this header file contains the implementation of the Volume class that handles a collection of images as a volume.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#include <algorithm>
#include "volume.h"

/**
 * @details Constructs a Volume object from the images in the specified directory.
 * The images are loaded in sorted order based on filenames.
 * @author Shengzhi Tian
 */
Volume::Volume(const std::string& directoryPath, int desiredChannels) {
    try {
        // Ensure the path exists and is a directory
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
            std::vector<fs::directory_entry> entries;

            // Iterate over directory contents and store them in a vector
            for (const auto& entry : fs::directory_iterator(directoryPath)) {
                entries.push_back(entry);
            }

            // Sort the entries based on filenames
            sortFilenames(entries);

            // Load images in sorted order
            int i = 0;
            for (const auto& entry : entries) {
                // Check if the entry is a file
                if (entry.is_regular_file()) {
                    // Load the image
                    try {
                        Image img(entry.path().string(), desiredChannels);
                        images.push_back(img); // Move img into the vector
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Failed to load image " << entry.path() << ": " << e.what() << std::endl;
                    }
                }
                i++;
                std::cout << i << " number loaded" << std::endl;
            }
        }
        else {
            std::cerr << "Directory does not exist or is not a directory: " << directoryPath << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
}

/**
 * @details Finds all the files names in the specified directory and returns them in a vector.
 * @author Shengzhi Tian
 */
std::vector<fs::directory_entry> Volume::getFileEntries(const std::string& directoryPath) {
    std::vector<fs::directory_entry> entries;
    // Ensure the path exists and is a directory
        // Iterate over directory contents and store them in a vector
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        entries.push_back(entry);
    }
    return entries;
}

/**
 * @details Constructs a slab of Volume object from the images within the specified range in the directory.
 * The images are loaded in sorted order based on filenames.
 * @author Yunting Tao
 */
Volume::Volume(const std::string& directoryPath, int z1, int z2, int desiredChannels) {
    try {
        // Ensure the path exists and is a directory
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
            std::vector<fs::directory_entry> entries;

            entries = getFileEntries(directoryPath);

            // Sort the entries based on filenames
            sortFilenames(entries);

            // Ensure that z1 and z2 are within the range of image indices
            if (z1 < 1 || z1 > entries.size() || z2 < 1 || z2 > entries.size() || z1 >z2) {
                throw std::invalid_argument("Invalid z range");
            }

            // Load images in sorted order
            int i = 0;
            for (int i = z1 - 1; i < z2 && i <= entries.size(); ++i) {
                const auto& entry = entries[i];
                // Check if the entry is a file
                if (entry.is_regular_file()) {
                    // Load the image
                    try {
                        Image img(entry.path().string(), desiredChannels);
                        images.push_back(img); // Move img into the vector
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Failed to load image " << entry.path() << ": " << e.what() << std::endl;
                    }
                }
                std::cout << i << " number loaded" << entry.path() << std::endl;
            }
        }
        else {
            std::cerr << "Directory does not exist or is not a directory: " << directoryPath << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
}


Volume::~Volume()
{
    for (auto img : images)
    {
        stbi_image_free(img.get_data());
    }
    
}


/**
 * @details Retrieves the images in the volume.
 * @author Yunting Tao
 */
std::vector<Image> Volume::getImages() const {
    return images;
}

/**
 * @details Saves the volume to the specified directory.
 * The images are saved as PNG files with filenames image0.png, image1.png, etc.
 * The images are saved in the order they were loaded.
 * If the directory does not exist, an error message is printed to the console.
 * If an image fails to save, an error message is printed to the console.
 * @author Shengzhi Tian
 */
void Volume::save(const std::string& directoryPath) {
    try {
        // Ensure the path exists and is a directory
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
            // Iterate over images
            for (size_t i = 0; i < images.size(); ++i) {
                // Construct the file path
                std::string filePath = directoryPath + "/image" + std::to_string(i) + ".png";
                // Save the image
                try {
                    images[i].save(filePath);
                }
                catch (const std::exception& e) {
                    std::cerr << "Failed to save image " << i << ": " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Directory does not exist or is not a directory: " << directoryPath << std::endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
}

/**
 * @brief Swaps the values of two variables.
 * @tparam T The type of the variables to swap.
 * @param a The first variable.
 * @param b The second variable.
 */
template <typename T>
void custom_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/**
 * @details Helper function that partitions the directory entries for quicksort.
 * This function is used by the quicksort function to sort the directory entries based on filenames.
 * @author Shengzhi Tian
 */
size_t Volume::partition(std::vector<fs::directory_entry>& entries, size_t low, size_t high) {
    std::string pivot = entries[high].path().filename().string();
    size_t i = low - 1;
    for (size_t j = low; j < high; ++j) {
        if (entries[j].path().filename().string() < pivot) {
            ++i;
            custom_swap(entries[i], entries[j]);
        }
    }
    custom_swap(entries[i + 1], entries[high]);
    return i + 1;
}

/**
 * @details Helper function that sorts the filenames of directory entries.
 * This function implements the quicksort algorithm to sort the directory entries based on filenames.
 * @author Shengzhi Tian
 */
void Volume::quicksort(std::vector<fs::directory_entry>& entries, size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(entries, low, high);
        if (pi > 0) {
            quicksort(entries, low, pi - 1);
        }
        quicksort(entries, pi + 1, high);
    }
}

/**
 * @details Helper function that sorts the filenames of directory entries.
 * @author Shengzhi Tian
 */
void Volume::sortFilenames(std::vector<fs::directory_entry>& entries) {
    quicksort(entries, 0, entries.size() - 1);
}

