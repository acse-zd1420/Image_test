// Created by Shengzhi Tian on 2024/3/16.
//
#include "Image/Image.h"
#include "filter/filter.h"
#include "volume/volume.h"
#include "projection/projection.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    //test gaussian_blur_3d
    //Volume volume("Scans/confuciusornis");
    //Filter::gaussian_blur_3d(volume, 10);
    //std::cout << "gaussian_blur_3d test passed" << std::endl;
    //volume.save("test_output");

//     test Max Intensity Projection (MIP) - no filters
    Volume volume0("Scans/confuciusornis", 0);
    Image img0 = Projection::MIP(volume0);
    img0.save("test_projections/confuciusornis-MIP.png");

    Volume volume("Scans/confuciusornis", 0);
    Image img = Projection::MIP(volume, "gaussian", 5);
    img.save("test_projections/confuciusornis-MIPg.png");

    Volume volume1("Scans/confuciusornis", 0);
    Image img1 = Projection::MIP(volume1, "median", 5);
    img1.save("test_projections/confuciusornis-MIPm.png");

    // test Max Intensity Projection (MIP) - gaussian filters
//    Volume volume("../Scans/confuciusornis", 0);
//    Image img = Projection::MIP(volume);
//    img.save("../test_projections/confuciusornis-MIP-gaussian.png");

////    // test Min Intensity Projection (MinIP) - no filters
//    Volume volume("../Scans/confuciusornis", 0);
//    Image img = Projection::MinIP(volume);
//    img.save("../test_projections/confuciusornis-MinIP.png");

////    // test Min Intensity Projection (MinIP) - gassuian
//    Volume volume("../Scans/confuciusornis", 0);
//    Image img = Projection::MinIP(volume, "gaussian");
//    img.save("../test_projections/confuciusornis-MinIP-gaussian.png");

// //    test Average Intensity Projection (AIP) - no filters
//    Volume volume("../Scans/confuciusornis", 0);
//    Image img = Projection::AIP(volume);
//    img.save("../test_projections/confuciusornis-AIP.png");

//    // test Average Intensity Projection (AIP) - gaussian
//    Volume volume("../Scans/confuciusornis", 0);
//    Image img = Projection::AIP(volume, "gaussian");
//    img.save("../test_projections/confuciusornis-AIP-gaussian.png");

//    // histogram equalization - AIP
//    Image img("../test_projections/confuciusornis-AIP.png", 0);
//    Filter::HistogramEqualization(img, 0);
//    img.save("../test_projections/confuciusornis-AIP-histogram.png");

//    // histogram equalization - AIP gaussian
//    Image img("../test_projections/confuciusornis-AIP-gaussian.png", 0);
//    Filter::HistogramEqualization(img, 0);
//    img.save("../test_projections/confuciusornis-AIP-histogram-gaussian.png");

    return 0;
}