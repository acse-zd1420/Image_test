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
    
    ofstream resultsFile_2D("../kernal_performance_2D.csv");

    // Header for the CSV file
    resultsFile_2D << "Image,GaussianBlur,BoxBlur,MedianBlur\n";
    
    for (int i = 3; i < 16; i+=2) {
        Image* img = new Image("../test_image/test_image_1000000_pixels.png");
        
        auto start = chrono::high_resolution_clock::now();
        Filter::gaussian_blur_2d(*img, i);
        auto end = chrono::high_resolution_clock::now();
        long long gaussian_blur_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        auto start = chrono::high_resolution_clock::now();
        Filter::box_blur(*img, i);
        auto end = chrono::high_resolution_clock::now();
        long long box_blur_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        auto start = chrono::high_resolution_clock::now();
        Filter::median_blur(*img, i);
        auto end = chrono::high_resolution_clock::now();
        long long median_blur_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();


        resultsFile_2D << "test_image_1000000_pixels.png," << gaussian_blur_duration << "," << box_blur_duration << "," << median_blur_duration << "\n";
        img->~Image();
    }
    resultsFile_2D.close();
    
    ofstream resultsFile_3D("../kernal_performance_3D.csv");


    return 0;
}

