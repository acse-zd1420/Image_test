/** 
* @file Image.cpp 
* @brief Image class implementation file, which contains the implementation of the Image class and its member functions.
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#include "Image.h"

/**
 * @brief Constructs an Image object from the given file.
 * @author Shengzhi Tian
 */
Image::Image(std::string const& fileName, int desiredChannels) {
    data = stbi_load(fileName.c_str(), &w, &h, &c, 0);
    if (data == nullptr) {
        throw std::runtime_error("Failed to load image: " + fileName);
    }
    assert(c > 0 && "The number of channels should be greater than 0.");
    std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
}

/**
 * @brief Constructs an Image object from the given data.
 * @author Shengzhi Tian
 */
Image::Image(unsigned char* data, int w, int h, int c) : data(data), w(w), h(h), c(c) {
    if (data == nullptr) {
        throw std::runtime_error("Invalid image data.");
    }
    assert(c > 0 && "The number of channels should be greater than 0.");
}

Image::Image()
{
}

Image::~Image() {
    //stbi_image_free(data);
}

int Image::width() const {
    return w;
}

int Image::height() const {
    return h;
}

int Image::channels() const {
    return c;
}

unsigned char* Image::get_data() const {
    return data;
}

/**
 * @brief Saves the image to the specified file.
 * @author Shengzhi Tian
 */
void Image::save(std::string const& fileName) {
    if (!stbi_write_png(fileName.c_str(), w, h, c, data, w * c)) {
        throw std::runtime_error("Failed to save image: " + fileName);
    }
    std::cout << "Image saved to " << fileName << std::endl;
}

/**
 * @brief Sets the image data.
 * @author Zhikang Dong
 */
void Image::set_data(unsigned char* NewData) {
    stbi_image_free(data);
    data = NewData;
}

/**
 * @brief Sets the number of channels of the image.
 * @author Zhikang Dong
 */
void Image::set_channels(int NewChannels) {
    c = NewChannels;
}

/**
 * @brief Saves the image to the specified file (deprecated).
 * @author Georgia Ray 
 */
void Image::save_old(std::string const& fileName) {
    int result = 0;
    if (c == 1) {
        result = stbi_write_png(fileName.c_str(), w, h, c, data, w * c);
    }
    else if (c == 3) {
        result = stbi_write_png(fileName.c_str(), w, h, c, data, w * c);
    }
    else {
        std::cerr << "Unsupported number of channels. Unable to save image." << std::endl;
        return;
    }

    if (!result) {
        std::cerr << "Failed to save image to file: " << fileName << std::endl;
    }
    else {
        std::cout << "Image saved successfully to file: " << fileName << std::endl;
    }
}