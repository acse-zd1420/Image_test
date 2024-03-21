#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "filter.h"
#include "utility.h"
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

int main() {
    std::vector<std::string> imagePaths = {
        "../test_image/test_image_100_pixels.png", // Replace these with the actual paths to your test images
        "../test_image/test_image_1000_pixels.png",
        "../test_image/test_image_10000_pixels.png",
        "../test_image/test_image_100000_pixels.png",
        "../test_image/test_image_1000000_pixels.png",
        "../test_image/test_image_10000000_pixels.png",
        "../test_image/test_image_100000000_pixels.png"
    };

    std::ofstream resultsFile("../filter_performance.csv");

    // Header for the CSV file
    resultsFile << "Image,AdjustBrightness,AutoAdjustBrightness,AddSaltAndPepper,MedianBlur,GaussianBlur,BoxBlur,Grayscale,HistogramGray,ThresholdGray,HistogramHSV,ThresholdHSV,HistogramHSL,ThresholdHSL,Sobel,Prewitt,Roberts,Scharr\n";

    for (const auto& imagePath : imagePaths) {
        Image img(imagePath); // Assume constructor loads the image

        // Duration of brightness adjustment
        long long adjustBrightnessDuration = 0;
        long long autoAdjustBrightnessDuration = 0;
        
        // Duration of grayscale conversion
        long long grayscaleDuration = 0;

        // Duration of histogram equalization
        long long histogram_hsv_duration = 0;
        long long histogram_hsl_duration = 0;
        long long histogram_gray_duration = 0;

        // Duration of thresholding
        long long thresh_gray_duration = 0;
        long long thresh_hsv_duration = 0;
        long long thresh_hsl_duration = 0;

        // Duration of blur filter
        long long median_blur_duration = 0;
        long long gaussian_blur_duration = 0;
        long long box_blur_duration = 0;

        // Duration of salt and pepper noise
        long long addSaltAndPepperDuration = 0;

        // Duration of edge detection
        long long sobel_duration = 0;
        long long prewitt_duration = 0;
        long long roberts_duration = 0;
        long long Scharr_duration = 0;

        //////////////////////////////////////////
        // Performance testing for each filter //
        //////////////////////////////////////////

        // Adjust brightness
        auto start = std::chrono::high_resolution_clock::now();
        Filter::adjust_brightness(img, 50);
        auto end = std::chrono::high_resolution_clock::now();
        adjustBrightnessDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Auto adjust brightness
        start = std::chrono::high_resolution_clock::now();
        Filter::auto_adjust_brightness(img);
        end = std::chrono::high_resolution_clock::now();
        autoAdjustBrightnessDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Salt and pepper noise
        start = std::chrono::high_resolution_clock::now();
        Filter::add_salt_and_pepper(img, 0.05);
        end = std::chrono::high_resolution_clock::now();
        addSaltAndPepperDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Blur filter
        start = std::chrono::high_resolution_clock::now();
        Filter::median_blur(img, 3);
        end = std::chrono::high_resolution_clock::now();
        median_blur_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();  
        Filter::gaussian_blur_2d(img, 3);
        end = std::chrono::high_resolution_clock::now();
        gaussian_blur_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        Filter::box_blur(img, 3);
        end = std::chrono::high_resolution_clock::now();
        box_blur_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Gray scale
        start = std::chrono::high_resolution_clock::now();
        Filter::RGB2Gray(img);
        end = std::chrono::high_resolution_clock::now();
        grayscaleDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Grayscale Histogram equalization
        start = std::chrono::high_resolution_clock::now();
        Filter::HistogramEqualization(img,0);
        end = std::chrono::high_resolution_clock::now();
        histogram_gray_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Thresholding
        start = std::chrono::high_resolution_clock::now();
        Filter::Tresholding(img, 128, 0);
        end = std::chrono::high_resolution_clock::now();
        thresh_gray_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();
        img = Image(imagePath);

        // HSV histogram equalization
        start = std::chrono::high_resolution_clock::now();
        Filter::HistogramEqualization(img,1);
        end = std::chrono::high_resolution_clock::now();
        histogram_hsv_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // HSV Thresholding
        start = std::chrono::high_resolution_clock::now();
        Filter::Tresholding(img, 128, 1);
        end = std::chrono::high_resolution_clock::now();
        thresh_hsv_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();
        img = Image(imagePath);

        // HSL histogram equalization
        start = std::chrono::high_resolution_clock::now();
        Filter::HistogramEqualization(img,2);
        end = std::chrono::high_resolution_clock::now();
        histogram_hsl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // HSL Thresholding
        start = std::chrono::high_resolution_clock::now();
        Filter::Tresholding(img, 128, 2);
        end = std::chrono::high_resolution_clock::now();
        thresh_hsl_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();
        
        // Sobel edge detection
        img = Image(imagePath);
        Filter::RGB2Gray(img);

        start = std::chrono::high_resolution_clock::now();
        Filter::apply_sobel_edge_detection(img);
        end = std::chrono::high_resolution_clock::now();
        sobel_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();

        // Prewitt edge detection
        img = Image(imagePath);
        Filter::RGB2Gray(img);

        start = std::chrono::high_resolution_clock::now();
        Filter::apply_prewitt_edge_detection(img);
        end = std::chrono::high_resolution_clock::now();
        prewitt_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();

        // Roberts edge detection
        img = Image(imagePath);
        Filter::RGB2Gray(img);

        start = std::chrono::high_resolution_clock::now();
        Filter::apply_roberts_edge_detection(img);
        end = std::chrono::high_resolution_clock::now();
        roberts_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();

        // Scharr edge detection
        img = Image(imagePath);
        Filter::RGB2Gray(img);

        start = std::chrono::high_resolution_clock::now();
        Filter::apply_scharr_edge_detection(img);
        end = std::chrono::high_resolution_clock::now();
        Scharr_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        img.~Image();

        // Write results to CSV file
        resultsFile << imagePath << "," << adjustBrightnessDuration << "," << autoAdjustBrightnessDuration << "," << addSaltAndPepperDuration << "," << median_blur_duration << "," << gaussian_blur_duration << "," << box_blur_duration << "," << grayscaleDuration << "," << histogram_gray_duration << "," << thresh_gray_duration << "," << histogram_hsv_duration << "," << thresh_hsv_duration << "," << histogram_hsl_duration << "," << thresh_hsl_duration << "," << sobel_duration << "," << prewitt_duration << "," << roberts_duration << "," << Scharr_duration << "\n";

        std::cout << "Performance testing for " << imagePath << " completed.\n";
    }

    resultsFile.close();
    std::cout << "Performance testing completed. Results are saved to filter_performance.csv\n";

    return 0;
}

