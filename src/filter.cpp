/** 
* @file filter.cpp 
* @brief this file will contain all the filters (2D / 3D) that can be applied to the image and volume data
* @author Shengzhi Tian (edsml-st1123) 
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @date 19/03/2024
*/

#include "filter.h"
#include "volume.h"

/**
 * @details A simple helper function to swap two values.
 * @author Berat Yildizgorer
 */
template <typename T>
void custom_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/**
 * @details Manually adjust the brightness of the image by adding a constant value to each pixel.
 * The brightness value can be positive or negative.
 *
 * @author Berat Yildizgorer
 * @author Georgia Ray 
 */
void Filter::adjust_brightness(Image& img, int brightness) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();

    //calculating outside the loop for optimization 
    int size = width * height * channels;

    //access each pixel directly using pointer arithmetic to adjust brightness
    for (int i = 0; i < size; ++i) {
        // skip alpha
        if (channels == 4 && i % channels == 3)
            continue;

        int value = static_cast<int>(data[i]) + brightness;
        // Use memset to clip the pixel values between 0 and 255
        data[i] = static_cast<unsigned char>(std::min(255, std::max(0, value)));
    }
}

/**
 * @details Automatically adjust the brightness of the image to make it appear neither too bright nor too dark.
 * The average pixel value is calculated and the brightness is adjusted so that the average pixel value is 128.
 * @author Berat Yildizgorer
 */
void Filter::auto_adjust_brightness(Image& img) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    long long total = 0;
    int pixelCount = width * height * channels;

    // Calculate the total pixel value
    for(int i = 0; i < pixelCount; ++i) {
        if(channels == 4 && (i % channels == 3)) continue; // Skip alpha channel
        total += data[i];
    }

    // Calculate the average pixel value
    int pixelCountWithoutAlpha = (channels == 4) ? pixelCount / 4 * 3 : pixelCount; // Adjust for alpha channel if present
    int average = total / pixelCountWithoutAlpha;

    // Adjust the brightness to make the average pixel value 128
    int adjustment = 128 - average;

    // Apply the brightness adjustment
    adjust_brightness(img, adjustment);
}

/**
 * @details Helper function to sort the vector in ascending order using quick sort.
 * @author Berat Yildizgorer
 */
static int partition(std::vector<unsigned char>& vec, int low, int high) {
    unsigned char pivot = vec[high]; // Pivot
    int i = (low - 1); // Index of smaller element

    // Partition the vector
    for (int j = low; j < high; j++) {
        if (vec[j] <= pivot) {
            i++; // Increment index of smaller element
            custom_swap(vec[i], vec[j]);
        }
    }
    custom_swap(vec[i + 1], vec[high]);
    return (i + 1);
}

/**
 * @details Helper function of sorting used for finding the median
 * @author Berat Yildizgorer
 */
unsigned char Filter::quickSelect(std::vector<unsigned char>& arr, int low, int high, int k) {
    /* 
    Docstring: 
    QuickSelect algorithm to find the k-th smallest element in the vector.

    Args:
    arr: The vector to find the k-th smallest element in
    low: The lower index of the vector
    high: The higher index of the vector
    k: The k-th smallest element to find

    Returns:
    The k-th smallest element

    Author: 
    Berat Yildizgorer (asce-by1123)
    */
    if (low == high) return arr[low];

    int partitionIndex = partition(arr, low, high);

    if (partitionIndex == k) return arr[partitionIndex];
    else if (partitionIndex > k) return quickSelect(arr, low, partitionIndex - 1, k);
    else return quickSelect(arr, partitionIndex + 1, high, k);
}

/**
 * @details Helper function to find the median value from the given neighborhood.
 * The neighborhood is a vector of unsigned chars.
 * The median value is the middle value when the neighborhood is sorted in ascending order.
 * If the neighborhood size is even, the median is the average of the two middle values.
 * @author Berat Yildizgorer
 */
unsigned char Filter::findMedian(std::vector<unsigned char>& neighborhood) {
    /* 
    Docstring: 
    Find the median value in the neighborhood vector.

    Args:
    neighborhood: The vector to find the median value in

    Returns:
    The median value

    Author: 
    Berat Yildizgorer (asce-by1123)
    */
    size_t medianIndex = neighborhood.size() / 2;
    return quickSelect(neighborhood, 0, neighborhood.size() - 1, medianIndex);
}

/**
 * @details Apply median blur to the image using the specified kernel size.
 * The kernel size must be an odd number.
 * @author Berat Yildizgorer
 */
void Filter::median_blur(Image& img, int kernelSize) {
    const int edgeOffset = kernelSize / 2;
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();

    std::vector<unsigned char> originalImg(data, data + width * height * channels);
    std::vector<unsigned char> neighborhood;
    neighborhood.reserve(kernelSize * kernelSize);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                if (channels == 4 && c == 3) {
                    continue;
                }
                neighborhood.clear();
                for (int ky = -edgeOffset; ky <= edgeOffset; ++ky) {
                    for (int kx = -edgeOffset; kx <= edgeOffset; ++kx) {
                        int nx = std::min(std::max(x + kx, 0), width - 1);
                        int ny = std::min(std::max(y + ky, 0), height - 1);
                        neighborhood.push_back(originalImg[ny * width * channels + nx * channels + c]);
                    }
                }
                unsigned char medianValue = findMedian(neighborhood);
                data[y * width * channels + x * channels + c] = medianValue;
            }
        }
    }
}

/**
 * @details Apply box blur to the image using the specified kernel size. The kernel size must be an odd number.
 * @author Georgia Ray
 * @author Berat Yildizgorer
 */
void Filter::box_blur(Image& img, int kernelSize) {
    int width = img.width();
    int height = img.height();
    int channels = img.channels();

    std::vector<unsigned char> newImg(width * height * channels);
    std::vector<int> sum(channels, 0); // Use this to accumulate sums

    const int edgeOffset = kernelSize / 2;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::fill(sum.begin(), sum.end(), 0); // Reset sum for each pixel
            int count = 0;

            for (int ky = -edgeOffset; ky <= edgeOffset; ++ky) {
                int ny = y + ky;
                // Check if the ny is within the image bounds
                if (ny < 0 || ny >= height) continue; // Skip this ky iteration if ny is out of bounds
                
                for (int kx = -edgeOffset; kx <= edgeOffset; ++kx) {
                    int nx = x + kx;
                    // Check if the nx is within the image bounds
                    if (nx < 0 || nx >= width) continue; // Skip this kx iteration if nx is out of bounds

                    for (int c = 0; c < channels; ++c) {
                        if (channels == 4 && c == 3) { // Copy alpha channel unchanged
                            newImg[(y * width + x) * channels + c] = img.get_data()[(ny * width + nx) * channels + c];
                        } else {
                            sum[c] += img.get_data()[(ny * width + nx) * channels + c];
                        }
                    }
                    count++;
                }
            }

            for (int c = 0; c < channels; ++c) {
                if (channels != 4 || c != 3) { // Skip alpha channel processing
                    newImg[(y * width + x) * channels + c] = sum[c] / count;
                }
            }
        }
    }

    // Now, copy the blurred image back to the original data buffer.
    std::copy(newImg.begin(), newImg.end(), img.get_data());
}

/**
 * @details Convert the image from RGB to grayscale.
 * 
 * @author Zhikang Dong
 */
void Filter::RGB2Gray(Image& img) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    unsigned char* grayData = new unsigned char[width * height];

    for(int j = 0; j < height; ++j) {
        for(int i = 0; i < width; ++i) {
            int index = j * width * channels + i * channels;
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];
            unsigned char gray = static_cast<unsigned char>(0.2126 * r + 0.7152 * g + 0.0722 * b);
            
            grayData[j * width + i] = gray;
        }
    }

    img.set_data(grayData);
    img.set_channels(1);
}

/**
 * @details Convert the image from RGB to HSV.
 * @author Zhikang Dong
 */
void Filter::RGB2HSV(Image& img) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    for(int j = 0; j < height; ++j) {
        for(int i = 0; i < width; ++i) {
            int index = j * width * channels + i * channels;
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];
            
            float R = r / 255.0f;
            float G = g / 255.0f;
            float B = b / 255.0f;
            
            float Cmax = std::max(std::max(R, G), B);
            float Cmin = std::min(std::min(R, G), B);
            float delta = Cmax - Cmin;
            
            float H = 0;
            if (delta == 0) {
                H = 0;
            } else if (Cmax == R) {
                H = 60 * fmod((G - B) / delta, 6);
            } else if (Cmax == G) {
                H = 60 * ((B - R) / delta + 2);
            } else if (Cmax == B) {
                H = 60 * ((R - G) / delta + 4);
            }
            
            float S = (Cmax == 0) ? 0 : delta / Cmax;
            float V = Cmax;
            
            data[index] = static_cast<unsigned char>(H / 360 * 255);
            data[index + 1] = static_cast<unsigned char>(S * 255);
            data[index + 2] = static_cast<unsigned char>(V * 255);
        }
    }
}

/**
 * @details Convert the image from HSV to RGB.
 * @author Zhikang Dong
 */
void Filter::HSV2RGB(Image& img){
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    for(int j = 0; j < height; ++j) {
        for(int i = 0; i < width; ++i) {
            int index = j * width * channels + i * channels;
            float H = data[index] / 255.0f * 360;
            float S = data[index + 1] / 255.0f;
            float V = data[index + 2] / 255.0f;
            
            float C = V * S;
            float X = C * (1 - fabs(fmod(H / 60, 2) - 1));
            float m = V - C;
            
            float R, G, B;
            if (H >= 0 && H < 60) {
                R = C;
                G = X;
                B = 0;
            } else if (H >= 60 && H < 120) {
                R = X;
                G = C;
                B = 0;
            } else if (H >= 120 && H < 180) {
                R = 0;
                G = C;
                B = X;
            } else if (H >= 180 && H < 240) {
                R = 0;
                G = X;
                B = C;
            } else if (H >= 240 && H < 300) {
                R = X;
                G = 0;
                B = C;
            } else {
                R = C;
                G = 0;
                B = X;
            }
            
            data[index] = static_cast<unsigned char>((R + m) * 255);
            data[index + 1] = static_cast<unsigned char>((G + m) * 255);
            data[index + 2] = static_cast<unsigned char>((B + m) * 255);
        }
    }
}

/**
 * @details Convert the image from RGB to HSL.
 * @author Zhikang Dong
 */
void Filter::RGB2HSL(Image& img) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int index = j * width * channels + i * channels;
            float r = data[index] / 255.0f;
            float g = data[index + 1] / 255.0f;
            float b = data[index + 2] / 255.0f;
            
            float max = std::max({r, g, b});
            float min = std::min({r, g, b});
            float delta = max - min;
            
            float L = (max + min) / 2;
            float H = 0;
            float S = 0;
            
            if (delta != 0) {
                S = (L < 0.5) ? (delta / (max + min)) : (delta / (2 - max - min));
                
                if (max == r) {
                    H = 60 * fmod(((g - b) / delta), 6);
                } else if (max == g) {
                    H = 60 * ((b - r) / delta + 2);
                } else if (max == b) {
                    H = 60 * ((r - g) / delta + 4);
                }
            }
            
            data[index] = static_cast<unsigned char>((H / 360) * 255); // H
            data[index + 1] = static_cast<unsigned char>(S * 255); // S
            data[index + 2] = static_cast<unsigned char>(L * 255); // L
        }
    }
}

/**
 * @details Convert the image from HSL to RGB.
 * @author Zhikang Dong
 */
void Filter::HSL2RGB(Image& img) {
    unsigned char* data = img.get_data();
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int index = j * width * channels + i * channels;
            float H = data[index] / 255.0f * 360;
            float S = data[index + 1] / 255.0f;
            float L = data[index + 2] / 255.0f;
            
            float C = (1 - fabs(2 * L - 1)) * S;
            float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
            float m = L - C / 2;
            
            float r = 0, g = 0, b = 0;
            if (0 <= H && H < 60) {
                r = C; g = X; b = 0;
            } else if (60 <= H && H < 120) {
                r = X; g = C; b = 0;
            } else if (120 <= H && H < 180) {
                r = 0; g = C; b = X;
            } else if (180 <= H && H < 240) {
                r = 0; g = X; b = C;
            } else if (240 <= H && H < 300) {
                r = X; g = 0; b = C;
            } else if (300 <= H && H < 360) {
                r = C; g = 0; b = X;
            }
            
            data[index] = static_cast<unsigned char>((r + m) * 255);
            data[index + 1] = static_cast<unsigned char>((g + m) * 255);
            data[index + 2] = static_cast<unsigned char>((b + m) * 255);
        }
    }
}

/**
 * @details Apply histogram equalization to the image.
 * If the image is grayscale, the histogram equalization is applied to the intensity values.
 * If the image is in RGB format, the histogram equalization is applied to the V channel in HSV format or the L channel in HSL format.
 * @author Zhikang Dong
 * @author Georgia Ray
 */
void Filter::HistogramEqualization(Image& img, int transform) {
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    if (channels == 1) 
    {   
        unsigned char* data = img.get_data();
        std::vector<int> histogram(256, 0);
        for (int i = 0; i < width * height; ++i) {
            histogram[data[i]]++;
        }
        
        std::vector<int> cumulativeHistogram(256, 0);
        cumulativeHistogram[0] = histogram[0];
        for (int i = 1; i < 256; ++i) {
            cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
        }
        
        for (int i = 0; i < width * height; ++i) {
            data[i] = static_cast<unsigned char>(255 * cumulativeHistogram[data[i]] / (width * height));
        }
    }

    else if (channels == 3 || channels == 4) 
    {
        if(transform == 1) RGB2HSV(img); // Convert RGB to HSV
        else if(transform == 2) RGB2HSL(img); // Convert RGB to HSL

        unsigned char* data = img.get_data(); // Now data is in HSV format
        int pixelCount = width * height ;

        // Step 1: Build a histogram for the V channel only
        std::vector<int> histogram(256, 0);
        for (int i = 0; i < pixelCount; ++i) {
            int vIndex = i * channels + 2; // V channel index in HSV
            unsigned char v = data[vIndex];
            histogram[v]++;
        }

        // Step 2: Calculate the CDF
        std::vector<int> cdf(256, 0);
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; ++i) {
            cdf[i] = cdf[i - 1] + histogram[i];
        }

        // Normalize the CDF
        float cdfMin = cdf[0];
        float denom = static_cast<float>(pixelCount - cdfMin); // Avoid division by zero
        for (int i = 0; i < 256; ++i) {
            if (denom != 0) {
                cdf[i] = round(((cdf[i] - cdfMin) / denom) * 255);
            } else {
                cdf[i] = 0;
            }
        }

        // Step 3: Apply the equalized CDF to the V channel
        for (int i = 0; i < pixelCount; ++i) {
            int vIndex = i * channels + 2; // V channel index in HSV
            data[vIndex] = static_cast<unsigned char>(cdf[data[vIndex]]);
        }
        
        // Convert back to RGB (assuming you have an HSV2RGB function defined)
        if (transform == 1) HSV2RGB(img);
        else if (transform == 2) HSL2RGB(img);
    }
}

/**
 * @details Apply thresholding to the image.
 * If the image is grayscale, the thresholding is applied to the intensity values.
 * If the image is in RGB format, the thresholding is applied to the V channel in HSV format or the L channel in HSL format.
 * @author Zhikang Dong
 * @author Georgia Ray
 */
void Filter::Tresholding(Image& img, int threshold, int transform) {
    int width = img.width();
    int height = img.height();
    int channels = img.channels();
    
    if (channels == 1) 
    {
        unsigned char* data = img.get_data();
        for (int i = 0; i < width * height; ++i) {
            data[i] = (data[i] > threshold) ? 255 : 0;
        }
    }
    else if (channels == 3 || channels == 4) 
    {
        if(transform == 1) RGB2HSV(img); // Convert RGB to HSV
        else if(transform == 2) RGB2HSL(img); // Convert RGB to HSL

        unsigned char* data = img.get_data();
        unsigned char* TreshData = new unsigned char[width * height];

        for (int i = 0; i < width * height; ++i) {
            int vIndex = i * channels + 2; // V channel index in HSV
            TreshData[i] = (data[vIndex] > threshold) ? 255 : 0;
        }

        img.set_data(TreshData);
        img.set_channels(1);
    }
}

/**
 * @details Add salt and pepper noise to the image.
 * @author Shengzhi Tian
 */
void Filter::add_salt_and_pepper(Image& img, float density) {
    int w = img.width();
    int h = img.height();
    int c = img.channels();
    unsigned char* data = img.get_data();

    int num_pixels = w * h;
    int num_salt = static_cast<int>(num_pixels * density);
    std::unordered_set<int> uniqueIdxs;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist_width(0,w-1);
    std::uniform_int_distribution<int> dist_height(0,h-1);
    std::uniform_int_distribution<int> dist_value(0,1);

    while (uniqueIdxs.size() < num_salt) {
        int x = dist_width(rng);
        int y = dist_height(rng);
        int pixel_index = y * w * c + x * c;
        if (uniqueIdxs.find(pixel_index) == uniqueIdxs.end()) {
            uniqueIdxs.insert(pixel_index);
            // Randomly set the pixel to either black (0) or white (255)
            int value = dist_value(rng) * 255;

            if (c == 1) {
                // Grayscale image
                data[pixel_index] = value;
            } else if (c == 4 || c == 3) {
                // RGB image
                data[pixel_index] = value;         // Red
                data[pixel_index + 1] = value; // Green
                data[pixel_index + 2] = value; // Blue
            } else {
                // Unsupported number of channels
                std::cerr << "Unsupported number of channels: " << c << std::endl;
                return; // No need to continue if unsupported channels
            }
        }
    }
}

/**
 * @details Apply 2D Gaussian blur to the image using the specified kernel size.
 * The kernel size must be an odd number.
 * @author Shengzhi Tian
 */

void Filter::gaussian_blur_2d(Image &img, int kernelSize, double sigma) {
    int w = img.width();
    int h = img.height();
    int c = img.channels();
    unsigned char* data = img.get_data();

    double *gaussianArray = Filter::getGaussianKernel(kernelSize, sigma);
    auto *temp = new unsigned char[w * h * c];
    Filter::GaussBlur_x(data, temp, w, h, kernelSize, gaussianArray, c);
    Filter::GaussBlur_y(temp, data, w, h, kernelSize, gaussianArray, c);

    stbi_image_free(temp);
    stbi_image_free(gaussianArray);
}

/**
 * @details Helper function to get the 1D Gaussian kernel.
 * @author Shengzhi Tian
 */
double* Filter::getGaussianKernel(int kernel_size, double sigma)
{
    double *array = new double[kernel_size];
    int center_i = kernel_size / 2;
    double sum = 0.0f;
    double sigma2 = (2.0f*sigma*sigma);
    for (int i = 0; i < kernel_size; i++)
    {
        array[i] = exp(-(1.0f)* (((i - center_i)*(i - center_i)) / sigma2));
        sum += array[i];
    }

    for (int i = 0; i < kernel_size; i++)
    {
        array[i] /= sum;
    }
    return array;
}

/**
 * @details Helper function to apply 1D Gaussian blur to the image in the horizontal direction.
 * @author Shengzhi Tian
 */
void Filter::GaussBlur_x(unsigned char* src, unsigned char* dst, int w, int h, int kernelSize, double *gaussianArray, int nc) {
    /* 
    Docstring: 
    Helper function to apply 1D Gaussian blur to the image in the horizontal direction.

    Args:
    src: The source image data.
    dst: The destination image data.
    w: The width of the image.
    h: The height of the image.
    kernelSize: The size of the kernel to use for the blur.
    gaussianArray: The 1D Gaussian kernel array.
    nc: The number of channels in the image.

    Returns:
    None

    Author: 
    Shengzhi Tian (edsml-st1123)
    */
    int center = kernelSize / 2;
    int ind = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int k = -center; k <= center; k++) {
                // mirroring if exceed boundary
                if (j + k < 0 || j + k >= w) {
                    ind = (i * w + j - k) * nc;
                } else {
                    ind = (i * w + j + k) * nc;
                }
                sumR += src[ind] * gaussianArray[k + center];
                if (nc == 4) {
                    sumG += src[ind + 1] * gaussianArray[k + center];
                    sumB += src[ind + 2] * gaussianArray[k + center];
                }
            }
            // Store result in the destination
            ind = (i * w + j)*nc;
            dst[ind] = std::round(std::max(std::min(sumR, 255.0), 0.0));

            if (nc == 4) {
                dst[ind + 1] = std::round(std::max(std::min(sumG, 255.0), 0.0));
                dst[ind + 2] = std::round(std::max(std::min(sumB, 255.0), 0.0));
            }
        }
    }
}

/**
 * @details Helper function to apply 1D Gaussian blur to the image in the vertical direction.
 * @author Shengzhi Tian
 */
void Filter::GaussBlur_y(unsigned char* src, unsigned char* dst, int w, int h, int kernelSize, double *gaussianArray, int nc)
{
    /* 
    Docstring: 
    Helper function to apply 1D Gaussian blur to the image in the vertical direction.

    Args:
    src: The source image data.
    dst: The destination image data.
    w: The width of the image.
    h: The height of the image.
    kernelSize: The size of the kernel to use for the blur.
    gaussianArray: The 1D Gaussian kernel array.
    nc: The number of channels in the image.

    Returns:
    None

    Author: 
    Shengzhi Tian (edsml-st1123)
    */
    int center = kernelSize / 2;
    int ind = 0;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            double sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for (int k = -center; k <= center; k++)
            {
                if (i + k < 0 || i + k >= h) {
                    ind = ((i - k) * w + j) * nc;
                } else {
                    ind = ((i + k) * w + j) * nc;
                }
                sumR += src[ind] * gaussianArray[k + center];
                if (nc == 4) {
                    sumG += src[ind + 1] * gaussianArray[k + center];
                    sumB += src[ind + 2] * gaussianArray[k + center];
                }
            }
            ind = (i*w + j) * nc;
            dst[ind] = std::round(std::max(std::min(sumR, 255.0), 0.0));

            if (nc == 4){
                dst[ind + 1] = std::round(std::max(std::min(sumG, 255.0), 0.0));
                dst[ind + 2] = std::round(std::max(std::min(sumB, 255.0), 0.0));
            }
        }
    }
}

/**
 * @details Apply 3D median blur to the volume using the specified kernel size.
 * @author Berat Yildizgorer
 */
void Filter::median_blur_3d(Volume &vol, int kernelSize) {
    std::vector<Image> imgs = vol.getImages();
    int num_imgs = imgs.size();
    if (num_imgs == 0) return;

    int w = imgs[0].width();
    int h = imgs[0].height();
    int nc = imgs[0].channels();

    // Create new data storage
    std::vector<unsigned char*> new_data(num_imgs);
    for (int i = 0; i < num_imgs; ++i) {
        new_data[i] = new unsigned char[w * h * nc];
    }

    for (int z = 0; z < num_imgs; ++z) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                for (int c = 0; c < nc; ++c) { // Iterate through each channel
                    if (!(nc == 4 && c == 3)) { // Skip alpha channel for RGBA images
                        int histogram[256] = {0};
                        int totalPixels = 0;

                        // Populate histogram for the neighborhood in 3D
                        for (int zz = std::max(0, z - kernelSize / 2); zz <= std::min(z + kernelSize / 2, num_imgs - 1); ++zz) {
                            for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ++ky) {
                                for (int kx = -kernelSize / 2; kx <= kernelSize / 2; ++kx) {
                                    int nx = std::max(0, std::min(x + kx, w - 1));
                                    int ny = std::max(0, std::min(y + ky, h - 1));
                                    unsigned char pixelValue = imgs[zz].get_data()[(ny * w + nx) * nc + c];
                                    histogram[pixelValue]++;
                                    totalPixels++;
                                }
                            }
                        }

                        // Find median from histogram
                        int sum = 0;
                        int median = 0;
                        for (int i = 0; i < 256; ++i) {
                            sum += histogram[i];
                            if (sum >= (totalPixels / 2)) {
                                median = i;
                                break;
                            }
                        }

                        new_data[z][(y * w + x) * nc + c] = median;
                    }
                }
            }
        }
    }

    // Copy new data to original images and clean up
    for (int i = 0; i < num_imgs; ++i) {
        memcpy(imgs[i].get_data(), new_data[i], w * h * nc);
        delete[] new_data[i];
    }
}


/**
 * @details Apply 3D gaussian blur to the volume using the specified kernel size.
 * @author Shengzhi Tian
 */

void Filter::gaussian_blur_3d(Volume &vol, int kernelSize, double sigma) {
    std::vector<Image> imgs = vol.getImages();
    int num_imgs = imgs.size();

    // get 1d gaussian kernel
    double *gaussianArray = Filter::getGaussianKernel(kernelSize, sigma);

    // apply to x and y direction
    for (int i = 0; i < num_imgs; i++) {
        unsigned char* data = imgs[i].get_data();
        int w = imgs[i].width();
        int h = imgs[i].height();
        int nc = imgs[i].channels();

        auto *temp_x = new unsigned char[w * h * nc];
        Filter::GaussBlur_x(data, temp_x, w, h, kernelSize, gaussianArray, nc);
        Filter::GaussBlur_y(temp_x, data, w, h, kernelSize, gaussianArray, nc);

        stbi_image_free(temp_x);
    }

    // apply to z direction
    int w = imgs[0].width();
    int h = imgs[0].height();
    int nc = imgs[0].channels();

    for (int z = 0; z < num_imgs; z++) {
        unsigned char* data = imgs[z].get_data();

        int center = kernelSize / 2;
        int ind = 0;
        int img_ind = 0;
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                double sumR = 0.0, sumG = 0.0, sumB = 0.0;
                for (int k = -center; k <= center; k++)
                {
                    if (z + k < 0 || z + k >= num_imgs) {
                        img_ind = z - k;
                    } else {
                        img_ind = z + k;
                    }
                    unsigned char * src = imgs[img_ind].get_data();
                    ind = (i * w + j) * nc;
                    sumR += src[ind] * gaussianArray[k + center];
                    if (nc == 4) {
                        sumG += src[ind + 1] * gaussianArray[k + center];
                        sumB += src[ind + 2] * gaussianArray[k + center];
                    }
                }
                ind = (i*w + j) * nc;
                data[ind] = std::max(std::min(sumR, 255.0), 0.0);

                if (nc == 4){
                    data[ind + 1] = std::max(std::min(sumG, 255.0), 0.0);
                    data[ind + 2] = std::max(std::min(sumB, 255.0), 0.0);
                }
            }
        }
    }

    stbi_image_free(gaussianArray);
}

/**
 * @details Apply edge detection to the image using the specified kernels.
 * @author Yunting Tao
 */
void Filter::apply_edge_detection(Image& img, const int horizontal_kernel[3][3], const int vertical_kernel[3][3]) {
    int width = img.width();
    int height = img.height();

    // Temporary array to store the result of edge detection
    unsigned char* edge_pixels = new unsigned char[width * height];

    // Apply edge detection operator
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double gradient_x = 0.0;
            double gradient_y = 0.0;
            // Apply kernel
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    // Calculate pixel coordinates, handling edge cases by replicating border pixels
                    int pixelX = std::min(std::max(x + i, 0), width - 1);
                    int pixelY = std::min(std::max(y + j, 0), height - 1);
                    // Calculate the gradient in horizontal direction
                    gradient_x += horizontal_kernel[j + 1][i + 1] * img.get_data()[(pixelY * width + pixelX)];
                    // Calculate the gradient in vertical direction
                    gradient_y += vertical_kernel[j + 1][i + 1] * img.get_data()[(pixelY * width + pixelX)];
                }
            }
            // Calculate the magnitude of gradient
            double magnitude = sqrt(gradient_x * gradient_x + gradient_y * gradient_y);
            magnitude = (magnitude > 255.0f) ? 255.0f : magnitude;
            // Threshold the magnitude to obtain binary edges
            edge_pixels[y * width + x] = magnitude;
        }
    }

    // Copy the result back to the image
    for (int i = 0; i < width * height; ++i) {
        img.get_data()[i] = edge_pixels[i];
    }

    // Free memory allocated for temporary array
    delete[] edge_pixels;
}

/**
 * @details Apply Sobel edge detection to the image.
 * @author Yunting Tao
 */
void Filter::apply_sobel_edge_detection(Image& img) {
    int sobel_horizontal[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int sobel_vertical[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
    apply_edge_detection(img, sobel_horizontal, sobel_vertical);
}

/**
 * @details Apply Prewitt edge detection to the image.
 * @author Yunting Tao
 */
void Filter::apply_prewitt_edge_detection(Image& img) {
    int prewitt_horizontal[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
    int prewitt_vertical[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    apply_edge_detection(img, prewitt_horizontal, prewitt_vertical);
}

/**
 * @details Apply Scharr edge detection to the image.
 * @author Chuhan Li
 * @author Yunting Tao
 */
void Filter::apply_scharr_edge_detection(Image& img) {
    int scharr_horizontal[3][3] = { {-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3} };
    int scharr_vertical[3][3] = { {-3, -10, -3}, {0, 0, 0}, {3, 10, 3} };
    apply_edge_detection(img, scharr_horizontal, scharr_vertical);
}

/**
 * @details Apply Roberts' Cross edge detection to the image using the specified kernel.
 * @author Chuhan Li
 * @author Yunting Tao
 * @author Georgia Ray
 */
void Filter::apply_roberts_edge_detection(Image& img) {
    // Roberts' Cross edge detection kernels
    int width = img.width();
    int height = img.height();

    // Temporary array to store the result of edge detection
    unsigned char* edge_pixels = new unsigned char[width * height];

    // Apply edge detection operator
    for (int y = 0; y < height - 1; ++y) {
        for (int x = 0; x < width - 1; ++x) {
            // Apply Roberts' Cross operator
            int gx = img.get_data()[y * width + x] - img.get_data()[(y + 1) * width + (x + 1)];
            int gy = img.get_data()[(y + 1) * width + x] - img.get_data()[y * width + (x + 1)];
            // Calculate the magnitude of gradient
            double magnitude = sqrt(gx * gx + gy * gy);
            magnitude = (magnitude > 255.0f) ? 255.0f : magnitude;
            // Threshold the magnitude to obtain binary edges
            edge_pixels[y * width + x] = magnitude;
        }
    }

    // Handle the last row and column of the image
    // We replicate the last row and column of the image to handle the edge cases
    for (int x = 0; x < width - 1; ++x) {
        int gx = img.get_data()[(height - 1) * width + x] - img.get_data()[(height - 2) * width + (x + 1)];
        int gy = img.get_data()[height * width - 1] - img.get_data()[(height - 1) * width + (x + 1)];
        double magnitude = sqrt(gx * gx + gy * gy);
        magnitude = (magnitude > 255.0f) ? 255.0f : magnitude;
        edge_pixels[(height - 1) * width + x] = magnitude;
    }
    for (int y = 0; y < height - 1; ++y) {
        int gx = img.get_data()[y * width + (width - 1)] - img.get_data()[(y + 1) * width + (width - 1)];
        int gy = img.get_data()[(y + 1) * width + (width - 1)] - img.get_data()[y * width + (width - 1)];
        double magnitude = sqrt(gx * gx + gy * gy);
        magnitude = (magnitude > 255.0f) ? 255.0f : magnitude;
        edge_pixels[y * width + (width - 1)] = magnitude;
    }

    // The last pixel in the image
    int gx = 0 - img.get_data()[(height - 1) * width + (width - 1)];
    int gy = img.get_data()[height * width - 1] - img.get_data()[(height - 1) * width + (width - 1)];
    double magnitude = sqrt(gx * gx + gy * gy);
    magnitude = (magnitude > 255.0f) ? 255.0f : magnitude;
    edge_pixels[(height - 1) * width + (width - 1)] = magnitude;

    // Copy the result back to the image
    for (int i = 0; i < width * height; ++i) {
        img.get_data()[i] = edge_pixels[i];
    }

    // Free memory allocated for temporary array
    delete[] edge_pixels;
}

