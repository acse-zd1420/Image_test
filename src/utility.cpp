/**
* @file utility.h
* @brief this header file contains the declarations of the Utility class that is used to help build the UI interface in main.
* @author Shengzhi Tian (edsml-st1123)
* @author Berat Yildizgorer (asce-by1123)
* @author Georgia Ray (edsml-ger23)
* @author Zhikang Dong (acse-zd1420)
* @author Yunting Tao (acse-yt2323)
* @author Chuhan Li (edsml-ll423)
* @note all functions in this class are created by Georgia Ray (edsml-ger23)
* @date 19/03/2024
*/

#include "utility.h"

/**
 * @details This function allows users to try again if they select a invalid option.
 * @author Georgia Ray
 */
void Utility::try_again(std::string message) {
    /*
    Docstring:
    This function is used to ask the user if they would like to try again after an invalid input.

    Parameters:
    None

    Returns:
    void
    */
    std::cerr << message;
    std::cin.clear(); // Clear the fail state
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the input line

}

/**
 * @details This function allows the user to choose between auto brightness and manual brightness adjustment.
 * @author Georgia Ray
 */
void Utility::apply_brightness(Image& img) {
    while (true) {

        //ask the user to choose between auto and manual brightness
        std::cout << "Choose brightness adjustment mode:\n";
        std::cout << "1) Auto Brightness\n";
        std::cout << "2) Manual Brightness\n";
        std::cout << "Mode: ";

        //get the user's input
        int brightnessOption;
        std::cin >> brightnessOption;

        //switch statement to handle the user's input
        switch (brightnessOption) {

            case 1: // Auto Brightness
                Filter::auto_adjust_brightness(img);
                return; // Exit the method after applying auto brightness

            case 2: { // Manual Brightness
                //ask the user to enter a brightness value
                int brightness;
                bool validBrightness = false;

                //enter a loop so the user can enter a valid brightness value, and if they enter an invalid value, they can try again
                while (!validBrightness) {
                    std::cout << "Enter brightness adjustment value (-255 to 255): ";
                    std::cin >> brightness;

                    if (std::cin.fail() || brightness < -255 || brightness > 255) {
                        //if the user enters an invalid value, they are asked to try again
                        try_again("Invalid brightness value. Please enter a value between -255 and 255.\n");
                        continue;
                    } else {
                        validBrightness = true;
                        //apply the filter with valid input
                        Filter::adjust_brightness(img, brightness);
                        return; // Exit the method after applying manual brightness
                    }
                }
                break;
            }
            default:
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid brightness adjustment mode selected.\n");
                continue;
        }
    }
}

/**
 * @details This function adjusts the kernel size to be an odd number if it is not already, where the kernel size is the size of the filter applied to the image.
 * @author Georgia Ray
 */
int Utility::adjust_kernel_size(int kernelSize) {
    /*
    Docstring:
    This function adjusts the kernel size to be an odd number if it is not already.

    Parameters:
    int kernelSize: The kernel size to be adjusted.

    Returns:
    int: The adjusted kernel size.
    */
    if (kernelSize % 2 == 0) {
        std::cerr << "Invalid kernel size. Using kernel size " << kernelSize - 1 << " instead.\n";
        return kernelSize - 1;
    }
    return kernelSize;
}

/**
 * @details This function allows the user to choose between median blur, box blur, and Gaussian blur.
 * @author Georgia Ray
 */
void Utility::apply_blur(Image& img) {
    //initiate a loop so the user can select their blur method, and if they enter an invalid option, they can try again
    while (true) {
        //ask the user to choose between median, box, and gaussian blur
        std::cout << "Select type of blur:\n";
        std::cout << "1) Median Blur\n";
        std::cout << "2) Box Blur\n";
        std::cout << "3) Gaussian Blur\n";
        std::cout << "Blur option: ";

        //get the user's input
        int blurOption;
        std::cin >> blurOption;

        //switch statement to handle the user's input
        int kernelSize;
        double sigma;
        switch (blurOption) {
            case 1: // Median Blur
                kernelSize = get_kernel_size();
                Filter::median_blur(img, kernelSize);
                return;
            case 2: // Box Blur
                kernelSize = get_kernel_size();
                Filter::box_blur(img, kernelSize);
                return;
            case 3: // Gaussian Blur
                kernelSize = get_kernel_size();
                while(true) { 
                //ask the user if they want to apply a sigma value and default to 2 otherwise 
                std::cout << "Would you like to apply a sigma value?\n";
                std::cout << "If you choose no, you will use the default sigma value of 2.\n";
                std::cout << "Apply sigma? (y/n): ";
                char applySigma;
                std::cin >> applySigma;
                if (applySigma == 'y') {
                    sigma = apply_sigma();
                    break;
                } else if (applySigma == 'n'){
                    sigma = 2.0;
                    break;
                }
                else {
                    //if the user enters an invalid option, they are asked to try again
                    try_again("Invalid option. Please enter 'y' or 'n'.\n");
                    continue;
                }
                }
                Filter::gaussian_blur_2d(img, kernelSize, sigma);
                return;
            default:
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid blur option selected. Please select a number between 1 and 3.\n");
                continue;
        }
    }
}

/** 
 * @details This function allows the user to apply a customized sigma for guassian blur 
 * @return double: The sigma value entered by the user
 * @note This function is used in apply_blur() function
 * @author Georgia Ray
*/
double Utility::apply_sigma() { 

    double sigma;
    //enter a loop so the user can enter a valid sigma value, and if they enter an invalid value, they can try again
    while (true) {
        //ask the user to enter a sigma value
        std::cout << "Enter the sigma value: ";
        std::cin >> sigma;
        if (std::cin.fail() || sigma < 0) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid sigma value. Please enter a value greater than 0.\n");
            continue;
        }
        break;
    }
    //return the valid sigma value
    return sigma;
}

/**
 * @details This function helps the user apply salt and pepper noise to the image.
 * @author Georgia Ray
 */
void Utility::apply_salt_and_pepper(Image& img) {
    //ask the user to enter a salt and pepper value
    std::cout << "Give a value between 0.0 and 1.0 to apply salt and pepper:\n";
    double density;

    //enter a loop so the user can enter a valid density value, and if they enter an invalid value, they can try again
    while (true) {
        std::cin >> density;
        if (std::cin.fail() || density < 0.0 || density > 1.0) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid density value. Please enter a value between 0.0 and 1.0.\n");
            continue;
        }
        break;
    }
    //apply the filter with the valid input
    Filter::add_salt_and_pepper(img, density);
}

/**
 * @details This function helps the user apply thresholding to the image, allowing them to choose between RGB to HSL or HSV transformations.
 * @author Georgia Ray
 */
void Utility::apply_thresholding(Image& img) {
    /*
    Docstring:
    This function helps the user apply thresholding to the image.

    Parameters:
    Image& img: The image to apply thresholding to.

    Returns:
    void
    */

    //ask the user to enter a threshold value
    std::cout << "Enter the threshold value (0 to 255): ";
    int threshold;

    //enter a loop so the user can enter a valid threshold value, and if they enter an invalid value, they can try again
    while(true) {
        std::cin >> threshold;
        if (std::cin.fail() || threshold < 0 || threshold > 255) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid threshold value. Please enter a value between 0 and 255.\n");
            continue;
        }
        break;
    }

    //check how many channels the image has, and if it is a grayscale image, apply the filter with a placeholder value of 1
    int channels = img.channels();
    //case to handle grayscale images
    if (channels == 1) {
        //will not use transform for grayscale images, so 1 is just placeholder for function call
        Filter::Tresholding(img, threshold, 1);
        return;
    }

    //if not grayscale, ask the user to choose between HSL and HSV
    std::cout << "Choose whether you want to transform RGB to HSL or HSV:\n";
    std::cout << "1) RGB to HSV\n";
    std::cout << "2) RGB to HSL\n";
    std::cout << "Transformation option: ";

    //enter a loop so the user can enter a valid transformation value, and if they enter an invalid value, they can try again
    int transform;
    while(true) {
        std::cin >> transform;
        if (std::cin.fail() || transform != 1 && transform != 2) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid transformation option. Please enter 1 or 2: ");
            continue;
        }
        break;
    }
    //apply the filter with the valid inputs
    Filter::Tresholding(img, threshold, transform);
}

/**
 * @details This function helps the user apply histogram equalization to the image, allowing them to choose between RGB to HSL or HSV transformations.
 * @author Georgia Ray
 */
void Utility::apply_histogram(Image& img) {
    int channels = img.channels();
    //case to handle grayscale images
    if (channels == 1) {
        //will not use transform for grayscale images, so 1 is just placeholder for function call
        Filter::HistogramEqualization(img, 1);
        return;
    }

    //if not grayscale, ask the user to choose between HSL and HSV
    std::cout << "Choose whether you want to transform RGB to HSL or HSV:\n";
    std::cout << "1) RGB to HSV\n";
    std::cout << "2) RGB to HSL\n";
    std::cout << "Transformation option: ";

    //enter a loop so the user can enter a valid transformation value, and if they enter an invalid value, they can try again
    int transform;
    while(true) {
        std::cin >> transform;
        if (std::cin.fail() || transform != 1 && transform != 2) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid transformation option. Please enter 1 or 2: ");
            continue;
        }
        break;
    }
    //apply the filter with the valid inputs
    Filter::HistogramEqualization(img, transform);
}

/**
 * @details This function helps the user apply edge detection to the image, allowing them to choose between Sobel, Prewitt, Scharr, and Roberts edge detection methods.
 * @author Georgia Ray
 */
void Utility::apply_edge_detection(Image& img) {
    // First convert image to grayscale
    Filter::RGB2Gray(img);
    int edgeDetectionOption;

    //enter a loop so the user can select their edge detection method, and if they enter an invalid option, they can try again
    while (true) {
        std::cout << "Choose an edge detection method:\n";
        std::cout << "1) Sobel\n";
        std::cout << "2) Prewitt\n";
        std::cout << "3) Scharr\n";
        std::cout << "4) Roberts\n";
        std::cout << "Edge detection method: ";

        std::cin >> edgeDetectionOption;

        //switch statement to handle the user's input
        switch (edgeDetectionOption) {
            case 1: // Sobel
                Filter::apply_sobel_edge_detection(img);
                return;
            case 2: // Prewitt
                Filter::apply_prewitt_edge_detection(img);
                return;
            case 3: // Scharr
                Filter::apply_scharr_edge_detection(img);
                return;
            case 4: // Roberts
                Filter::apply_roberts_edge_detection(img);
                return;
            default:
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid edge detection method selected. Please enter a number between 1 and 4.\n");
                continue;
        }
    }
}

/**
 * @details This function helps the user apply projection to the volume, allowing them to choose between Maximum Intensity Projection, Minimum Intensity Projection, and Mean Intensity Projection.
 * The user can also choose between Gaussian and Median filters, or no filter at all. The user can also choose the kernel size for the filter.
 * The function returns the image after applying the projection.
 * @author Georgia Ray
 */
Image Utility::apply_projection(Volume& vol) {
    //ask the user to choose between different projection methods
    int projectionOption;
    //get the projection type and kernel size, using helper methods
    double sigma = 2;
    int projectionType = get_projection_type(sigma);

    int kernelSize;
    if (projectionType == 1 || projectionType == 2) { 
        kernelSize = get_kernel_size();
    }
    else { 
        kernelSize = 0;
    }

    //enter a loop so the user can select their projection method, and if they enter an invalid option, they can try again
    while (true) {
        std::cout << "Choose a projection method:\n";
        std::cout << "1) Maximum Intensity Projection\n";
        std::cout << "2) Minimum Intensity Projection\n";
        std::cout << "3) Mean Intensity Projection\n";
        std::cout << "Projection method: ";

        std::cin >> projectionOption;

        Image img;

        //switch statement to handle the user's input
        switch (projectionOption) {
            case 1: // Maximum Intensity Projection
                img = Projection::MIP(vol, projectionType, kernelSize, sigma);
                return img;
            case 2: // Minimum Intensity Projection
                img = Projection::MinIP(vol, projectionType, kernelSize, sigma);
                return img;
            case 3: // Mean Intensity Projection
                img = Projection::AIP(vol, projectionType, kernelSize, sigma);
                return img;
            default:
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid projection method selected. Please enter a number between 1 and 3.\n");
                continue;
        }
    }
}

/**
 * @details This function helps the user apply projection to the volume, allowing them to choose between Maximum Intensity Projection, Minimum Intensity Projection, and Mean Intensity Projection.
 * The user can also choose between Gaussian and Median filters, or no filter at all. The user can also choose the kernel size for the filter.
 * The function returns the image after applying the projection.
 * @author Georgia Ray
 */
int Utility::get_projection_type(double& sigma) {
    int filter_option;
    while (true) {
    std::cout << "Choose a filter method for projection:\n";
    std::cout << "1) Gaussian\n";
    std::cout << "2) Median\n";
    std::cout << "3) None\n";
    std::cout << "Filter method: ";
    std::cin >> filter_option;

    if (filter_option == 1) { 
        char applySigma;
        while (true) { 
            std::cout << "Would you like to apply a sigma value?\n";
            std::cout << "If you choose no, you will use the default sigma value of 2.\n";
            std::cout << "Apply sigma? (y/n): ";
            std::cin >> applySigma;
            if (applySigma == 'y') {
                sigma = apply_sigma();
                break;
            } else if (applySigma == 'n'){
                sigma = 2.0;
                break;
            }
            else {
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid option. Please enter 'y' or 'n'.\n");
                continue;
            }
        }
    }
    else if (std::cin.fail() || filter_option < 1 || filter_option > 3) {
        //if the user enters an invalid option, they are asked to try again
        try_again("Invalid filter method selected. Please enter 1, 2, or 3.\n");
        continue;
    }
    break;
    }
    return filter_option;
};

int Utility::get_kernel_size() {
    /*
    Docstring:
    This function allows the user to choose the kernel size for the blur.

    Parameters:
    None

    Returns:
    int: The kernel size selected.
    */

    //ask the user to enter a kernel size
    std::cout << "Enter kernel size (odd number): ";
    int kernelSize;

    //enter a loop so the user can enter a valid kernel size, and if they enter an invalid value, they can try again
    while (true) {
        std::cin >> kernelSize;
        if (std::cin.fail() || kernelSize < 0) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid kernel size. Please enter an odd number greater than 0: ");
            continue;
        }
        break;
    }
    //adjust the kernel size to be an odd number if it is not already
    kernelSize = adjust_kernel_size(kernelSize);
    //return the adjusted kernel size
    return kernelSize;
}

/**
 * @details This function helps the user choose between working with the whole volume or a slab of the volume.
 * @author Georgia Ray
 * @author Shengzhi Tian
 */
Volume Utility::slab_or_whole(std::string volPath) {
    // Making a whole volume object before beginning, so I can access its total size
    std::vector<fs::directory_entry> entries = Volume::getFileEntries(volPath);
    //Enter a loop so the user can select whether they want to work with the whole volume or a slab
    while (true) {
        std::cout << "Would you like to work with the whole volume or a slab?\n";
        std::cout << "1) Whole Volume\n";
        std::cout << "2) Slab\n";
        std::cout << "Option: ";

        int slab_or_whole;
        std::cin >> slab_or_whole;

        if (slab_or_whole == 1) {
            // Create a volume object with the whole volume
            Volume vol(volPath);
            return vol; // Return the whole volume
        }

            //if the user wants to work with a slab, ask them for the start and end values
        else if (slab_or_whole == 2) {
            // Get the total number of images in the volume, specifying the possible range of start and end values
            int maxZ = entries.size();
            int start, end;

            // Ask the user for the start value and enter a loop so the user can enter a valid start value if incorrect the first time
            while (true) {
                std::cout << "Enter the value at which you want the slab to start: ";
                std::cin >> start;

                // Check the validity of the start value
                //using maxZ as stand-in for end value here because we don't have an end value yet
                bool validStart = checkZValidity(start, maxZ, 1, maxZ);

                if (std::cin.fail() || !validStart) {
                    //if the user enters an invalid value, they are asked to try again
                    try_again("Invalid start value. Please try again.\n");
                    continue;
                } else {
                    break; // Break the loop if the start value is valid
                }
            }

            // Ask the user for the end value and enter a loop so the user can enter a valid end value if incorrect the first time
            while (true) {
                std::cout << "Enter the value at which you want the slab to end: ";
                std::cin >> end;

                // Check the validity of the end value
                bool validEnd = checkZValidity(start, end, 1, maxZ);

                if (std::cin.fail() || !validEnd) {
                    //if the user enters an invalid value, they are asked to try again
                    try_again("Invalid end value. Please try again.\n");
                    continue;
                } else {
                    break; // Break the loop if the end value is valid
                }
            }

            // Create a volume object with the specified start and end values
            Volume vol(volPath, start, end);
            return vol;
        }
        else {
            //if the user enters an invalid slab or whole option, they are asked to try again
            try_again("Invalid option. Please choose 1 for the whole volume or 2 for a slab.\n");
            continue;
        }
    }
}

/**
 * @details This function checks if the given range of z values is valid. If the range is invalid, the function returns false. Otherwise, it returns true.
 * @author Georgia Ray
 */
bool Utility::checkZValidity(int z1, int z2, int minZ, int maxZ) {
    return !(z1 < minZ || z1 > maxZ || z2 < minZ || z2 > maxZ || z1 > z2);
}

/**
 * @details This function allows the user to take a slice of the volume and returns the slice as an image. The user can choose between XZ and YZ slices, and specify the slice number.
 * @author Georgia Ray
 */
Image Utility::take_a_slice(Volume vol) {
    //ask the user what kind of slice they want to take, either xz or yz
    int sliceType;
    //enter a loop so the user can select their slice type, and if they enter an invalid option, they can try again
    while (true) {
        std::cout << "Choose a slice type:\n";
        std::cout << "1) XZ\n";
        std::cout << "2) YZ\n";
        std::cout << "Slice type: ";
        std::cin >> sliceType;

        if (std::cin.fail() || (sliceType != 1 && sliceType != 2)) {
            //if the user enters an invalid option, they are asked to try again
            try_again("Invalid slice type. Please enter 1 for XZ or 2 for YZ.\n");
            continue;
        } else {
            break; // Valid input, exit the loop
        }
    }

    //cast the int to a SliceType and get max values for the slice
    int max;
    SliceType type;

    //this will be used to get the max value for the slice
    std::vector<Image> images = vol.getImages();

    //if the user wants an XZ slice
    if (sliceType == 1) {
        SliceType type = SliceType::XZ;
        max = images[0].height();
    }

        //if the user wants a YZ slice
    else {
        SliceType type = SliceType::YZ;
        max = images[0].width();
    }

    //ask the user for the slice number they wish to use
    int n;

    //enter a loop so the user can enter a valid slice number, and if they enter an invalid value, they can try again
    while (true) {
        std::cout << "Enter the slice number (1 to " << max << "): ";
        std::cin >> n;

        if (std::cin.fail() || n < 1 || n > max) {
            //if the user enters an invalid value, they are asked to try again
            try_again("Invalid slice number. Please enter a number between 1 and " + std::to_string(max) + ".\n");
        } else {
            break; // Valid input, exit the loop
        }
    }

    //take the slice and return it, based on valid user input
    Image slice = Slice::slice(vol, n, type);
    return slice;

}

/**
 * @details This function allows the user to choose all the operations with a 2D image. The user can choose to upload their own image or select a default image.
 * @author Georgia Ray
 */
Image Utility::twoDImageProcessing(std::string imagePath) {
    //if no path is passed (will only be passed when called from 3d imaging, otherwise we enter this 'if' for default or user input)
    if (imagePath == "None") {
        int width, height, channels;

        /*
        if image path does not exist
            - default to gracehopper
        else we continue
            and load user image
        */

        std::cout << "Enter the path to the image (or press Enter to use default, a color image of Grace Hopper): ";
        std::getline(std::cin, imagePath);

        if (!imagePath.empty() && std::filesystem::exists(imagePath)) {
            imagePath = imagePath;
        }
        else {
            //automatically tell the user they will be using the Grace Hopper image if they input a wrong path
            //taking some option away from the user, so we can show the program to work if they can't figure out how to input a path
            std::cerr << "Error loading image: " << imagePath << std::endl;
            std::cerr << "You will be using the default image. It is a color image of Grace Hopper.\n";
            imagePath = "Images/gracehopper.png";
        }
    }

    //load the image using the path (either passed in, default, or user input)
    Image img(imagePath);

    //setting some global variables for use later
    std::string outputPath;
    int count = 0;
    bool exitProgram = false;
    bool failed = false;

    //enter a loop so the user can select their filter, and if they enter an invalid option, they can try again
    while (!exitProgram) {
        //message for when the program restarts
        if (count > 0 && !failed) {
            std::cout << "Your filter has been applied! Please choose another filter to apply or select '8' to save your image and exit the program:\n";
        }
            //message for when the program starts for the first time
        else if (count == 0 || failed) {
            std::cout << "Choose an option:\n";
        }

        //ask the user to choose between different filters
        std::cout << "1) Adjust Brightness\n";
        std::cout << "2) Apply Blur\n";
        std::cout << "3) Apply Salt and Pepper\n";
        std::cout << "4) Make Image Grayscale\n";
        std::cout << "5) Equalize Image Histogram\n";
        std::cout << "6) Apply Thresholding\n";
        std::cout << "7) Apply Edge Detection\n";
        std::cout << "8) Save and Exit\n";
        std::cout << "Option: ";
        int option;
        std::cin >> option;

        //switch statement to handle the user's input
        switch (option) {
            case 1: {
                apply_brightness(img);
                count++;
                failed = false;
                break;
            }
            case 2: {
                apply_blur(img);
                count++;
                failed = false;
                break;
            }
            case 3: { // Salt and Pepper
                apply_salt_and_pepper(img);
                count++;
                failed = false;
                break;
            }
            case 4: { // Grayscale
                Filter::RGB2Gray(img);
                count++;
                failed = false;
                break;
            }
            case 5: { // Histogram Equalization
                apply_histogram(img);
                count++;
                failed = false;
                break;
            }
            case 6: { // Thresholding
                apply_thresholding(img);
                count++;
                failed = false;
                break;
            }
            case 7: { // Edge Detection
                apply_edge_detection(img);
                count++;
                failed = false;
                break;
            }
            case 8: { // Save and Exit
                exitProgram = true;
                break;
            }
            default:
                //if the user enters an invalid option, they are asked to try again
                try_again("Invalid filter option selected. Try again.\n");
                failed = true; //mark as failed to get correct starting message
                continue;
        }
    }
    //save the image and return it
    return img;
}

/**
 * @details This function allows the user to choose all the operations with a 3D volume. The user can choose to specific their own path to volume or select a default one.
 * @author Georgia Ray
 */
Image Utility::threeDImageProcessing() {
    //global variables for later use
    int images_to_use;
    std::string volPath;
    bool chooseYourOwn = false;

    //enter a loop so the user can select their scan, and if they enter an invalid option, they can try again
    while (true) {
        // Ask the user which scans they would like to work with (Confuciusornis, Fracture, or upload their own scan)
        std::cout << "Please choose which scans you would like to work with: \n";
        std::cout << "1) Confuciusornis\n";
        std::cout << "2) Fracture\n";
        std::cout << "3) Upload your own scan\n";
        std::cout << "Enter your choice: ";
        std::cin >> images_to_use;

        if (images_to_use == 1) {
            volPath = "Scans/confuciusornis";
            break;
        }
        else if (images_to_use == 2) {
            volPath = "Scans/fracture";
            break;
        }
        else if (images_to_use == 3) {
            chooseYourOwn = true;
            break; // Exit the loop to proceed to entering the scan folder path
        }
        else {
            //if the user enters an invalid scan option, they are asked to try again
            try_again("Invalid option selected. Try again.\n");
            continue;
        }
    }

    // Loop for entering the scan folder path; only entered via this boolean if the user selected 3
    while (chooseYourOwn) {
        // Ask the user to enter the path to their scan folder
        std::cout << "Enter the path to your scan folder: ";
        std::cin >> volPath;

        //if the path is valid, store it and move on
        if (std::filesystem::is_directory(volPath)) {
            break; // Valid directory path, exit the loop
        } else {
            //if the user enters an invalid path, they are asked to try again
            try_again("Directory does not exist or is not a directory. Please try again.\n");
            continue;
        }
    }

    //ask the user if they would like a slab or the whole volume
    Volume vol = Utility::slab_or_whole(volPath);

    //ask the user if they would like to take a slice or a projection
    int slice_or_projection;

    // Loop until a valid choice is made on slice or projection
    while (true) {
        std::cout << "Would you like to take a slice or a projection?\n";
        std::cout << "1) Slice\n";
        std::cout << "2) Projection\n";
        std::cout << "Option: ";
        std::cin >> slice_or_projection;

        // Check if input is valid
        if (std::cin.fail() || (slice_or_projection != 1 && slice_or_projection != 2)) {
            //if the user enters an invalid option, they are asked to try again
            try_again("Invalid choice. Please enter 1 for Slice or 2 for Projection.\n");
            continue;
        } else {
            break; // Valid input, exit the loop
        }
    }

    // If the user wants to take a slice

    Image to_save;
    if (slice_or_projection == 1) {
        to_save = Utility::take_a_slice(vol);
    }
    // If the user wants to take a projection, apply the projection
    else if(slice_or_projection == 2) {
        to_save = Utility::apply_projection(vol);
    }

    //ask user if they want to apply filters to their slice or projection image 
    int filter_option;

    // Loop until a valid choice is made on applying filters to the slice
    while (true) {
        std::cout << "Would you like to treat your new slice object as a 2D png and apply filters?\n";
        std::cout << "1) Yes\n";
        std::cout << "2) No\n";
        std::cout << "Option: ";
        std::cin >> filter_option;
        if (std::cin.fail() || (filter_option != 1 && filter_option != 2)) {
            //if the user enters an invalid option, they are asked to try again
            try_again("Invalid choice. Please enter 1 for Yes or 2 for No.\n");
            continue;
        } else {
            break; // Valid input, exit the loop
        }
    }

    if (filter_option == 1) {
        std::string outputPath = "Images/slice.png";
        to_save.save(outputPath);
        to_save = Utility::twoDImageProcessing(outputPath);
    }
    for (auto img_v : vol.getImages())
    {
        stbi_image_free(img_v.get_data());
    }

    return to_save;
}