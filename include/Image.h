/** 
* @file Image.h 
* @brief this header file contains all required functions and definitions for the Image class, which is used to load, save, and store image data.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/


#ifndef ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_IMAGE_H
#define ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_IMAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>

#include "stb_image.h"
#include "stb_image_write.h"


/**
 * @brief The Image class for loading, saving, and storing image data.
 */
class Image {
public:
    /**
     * @brief Constructs an Image object from the given file.
     * @param fileName The name of the file to load the image from.
     * @param desiredChannels The desired number of channels in the loaded image (default is 0).
     */
    Image(std::string const& fileName, int desiredChannels = 0);

    /**
     * @brief Constructs an Image object from the given data.
     * @param data The image data.
     * @param w The width of the image.
     * @param h The height of the image.
     * @param c The number of channels of the image.
     */
    Image(unsigned char* data, int w, int h, int c);

     /**
     * @brief Default constructor for Image object.
     */
    Image();

    /**
     * @brief Destructor for Image object.
     */
    ~Image();

    /**
     * @brief Gets the width of the image.
     * @return The width of the image.
     */
    int width() const;

    /**
     * @brief Gets the height of the image.
     * @return The height of the image.
     */
    int height() const;

    /**
     * @brief Gets the number of channels of the image.
     * @return The number of channels of the image.
     */
    int channels() const;

    /**
     * @brief Gets the raw image data.
     * @return Pointer to the raw image data.
     */
    unsigned char* get_data() const;

    /**
     * @brief Saves the image to the specified file.
     * @param fileName The name of the file to save the image to.
     */
    void save(std::string const& fileName);

    /**
     * @brief Saves the image to the specified file (deprecated).
     * @param fileName The name of the file to save the image to.
     */
    void save_old(std::string const& fileName);

    /**
     * @brief Sets the image data.
     * @param NewData The new image data.
     */
    void set_data(unsigned char* NewData);

    /**
     * @brief Sets the number of channels of the image.
     * @param NewChannels The new number of channels.
     */
    void set_channels(int NewChannels);
    
private:
    int w{}; /**< The width of the image. */
    int h{}; /**< The height of the image. */
    int c{}; /**< The number of channels of the image. */
    unsigned char* data; /**< The pointer to raw image data. */
};

#endif //ADVANCED_PROGRAMMING_GROUP_LINEAR_REGRESSION_IMAGE_H
