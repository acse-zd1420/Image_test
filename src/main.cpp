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

vector<string> imagePaths = {
        "../test_volumes/test_volume_1000_pixels/", // Replace these with the actual paths to your test images
        "../test_volumes/test_volume_9261_pixels/",
        "../test_volumes/test_volume_97336_pixels/",
        "../test_volumes/test_volume_1000000_pixels/",
        "../test_volumes/test_volume_99897344_pixels/",
        "../test_volumes/test_volume_1000000000_pixels/"
    };

int main() {
    
    ofstream resultsFile("../3D_filter_performance.csv");

    // Header for the CSV file
    resultsFile << "Image,GaussianBlur,MedianBlur,MIP,MinIP,AIP,XZ_Slice,YZ_Slice,Slab\n";

    for (const auto& imagePath : imagePaths) {
        Volume* v = new Volume(imagePath); // Assume constructor loads the image

        // Duration of 3D_gaussian_blur filter
        long long gaussian_blur_duration = 0;
        // Duration of 3D_median_blur filter
        long long median_blur_duration = 0;

        // Duration of Projections
        long long MIP_duration = 0;
        long long MinIP_duration = 0;
        long long AIP_duration = 0;
        
        // Duration of Slice
        long long XZ_Slice_duration = 0;
        long long YZ_Slice_duration = 0;

        // Duration of Slab
        long long Slab_duration = 0;

        ////////////////////////////////////////////
        //////////////   Testing   /////////////////
        ////////////////////////////////////////////

        // Gaussian Blur
        auto start = chrono::high_resolution_clock::now();
        Filter::gaussian_blur_3d(*v, 3);
        auto end = chrono::high_resolution_clock::now();
        gaussian_blur_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout<<"Gaussian Blur Done"<<endl;

        // Median Blur
        start = chrono::high_resolution_clock::now();
        Filter::median_blur_3d(*v, 3);
        end = chrono::high_resolution_clock::now();
        median_blur_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout<<"Median Blur Done"<<endl;
        
        // Maximum Intensity Projection
        start = chrono::high_resolution_clock::now();
        Projection::MIP(*v);
        end = chrono::high_resolution_clock::now();
        MIP_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Minimum Intensity Projection
        start = chrono::high_resolution_clock::now();
        Projection::MinIP(*v);
        end = chrono::high_resolution_clock::now();
        MinIP_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Average Intensity Projection
        start = chrono::high_resolution_clock::now();
        Projection::AIP(*v);
        end = chrono::high_resolution_clock::now();
        AIP_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // XZ Slice
        start = chrono::high_resolution_clock::now();
        Slice::slice(*v, 50, SliceType::XZ);
        end = chrono::high_resolution_clock::now();
        XZ_Slice_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // YZ Slice
        start = chrono::high_resolution_clock::now();
        Slice::slice(*v, 50, SliceType::YZ);
        end = chrono::high_resolution_clock::now();
        YZ_Slice_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Slab
        start = chrono::high_resolution_clock::now();
        Volume Volume(imagePath, 50, 100);
        end = chrono::high_resolution_clock::now();
        Slab_duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Write results to CSV file
        resultsFile << imagePath << "," << gaussian_blur_duration << "," << median_blur_duration << "," << MIP_duration << "," << MinIP_duration << "," << AIP_duration << "," << XZ_Slice_duration << "," << YZ_Slice_duration << "," << Slab_duration << "\n";

        std::cout << "Performance testing for " << imagePath << " completed.\n";

        delete v;

    }

    resultsFile.close();
    
    return 0;
}

