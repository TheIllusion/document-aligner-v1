#include <iostream>

#include "opencv2/opencv.hpp"
#include "LsdLineDetector.h"
#include "GfttCornerDetector.h"

#define TEST_SET_ROOT_PATH "/home/illusion/ClionProjects/line-detection-experiments-git/Document_Aligner_TestSet/Document_Aligner_TestSet/normal_set/original/"
//#define TEST_SET_ROOT_PATH "/Users/Illusion/Documents/Data/Document_Aligner_TestSet/normal_set/original/"

static char filename_buff[100];

static const char* file_name =
        //"IMG_1415_iphone.jpg";
        //"IMG_1415_iphone.jpg";
        //"IMG_1416_iphone.jpg";
        //"IMG_1417_iphone.jpg";
        //"IMG_1418_iphone.jpg";
        //"IMG_1419_iphone.jpg";
        //"IMG_1420_iphone.jpg";
        //"IMG_1421_iphone.jpg";
        //"20170125_152406_galaxy.jpg";
        "20170125_152538_galaxy.jpg";
        //"20170125_153220_galaxy.jpg";
        //"20170125_153230_galaxy.jpg";
        //"20170125_153242_galaxy.jpg";

double distance_between_points(cv::Point2f pointA, cv::Point2f pointB);

std::vector<cv::Point2f> find_candidate_corners(std::vector<cv::Point2f> corners, std::vector<cv::Vec4f> lines);

int main() {
    std::cout << "Hello, World!" << std::endl;

    strcpy(filename_buff, TEST_SET_ROOT_PATH);
    strcat(filename_buff, file_name);

    // open the image file
    cv::Mat color_image = cv::imread(filename_buff, cv::IMREAD_COLOR);
    cv::Mat gray_image = cv::imread(filename_buff, cv::IMREAD_GRAYSCALE);

    if (color_image.data == NULL || gray_image.data == NULL)
    {
        std::cout << "Error loading input image";
        return -1;
    }

#define RESIZED_IMAGE_WIDTH 500
    // resize the frame size if necessary
    if (color_image.cols > RESIZED_IMAGE_WIDTH)
    {
        cv::resize(color_image, color_image, cv::Size(), (double) RESIZED_IMAGE_WIDTH / color_image.cols,
                   (double) RESIZED_IMAGE_WIDTH / color_image.cols, cv::INTER_CUBIC);
    }

    // resize the frame size if necessary
    if (gray_image.cols > RESIZED_IMAGE_WIDTH)
    {
        cv::resize(gray_image, gray_image, cv::Size(), (double) RESIZED_IMAGE_WIDTH / gray_image.cols,
                   (double) RESIZED_IMAGE_WIDTH / gray_image.cols, cv::INTER_CUBIC);
    }

    Gftt gftt_detector;

    std::vector<cv::Point2f> corners = gftt_detector.DetectCorners(gray_image);

    LsdLineDetector lsd_detector;

    std::vector<cv::Vec4f> lines = lsd_detector.DetectLines(gray_image);

    std::vector<cv::Point2f> candidate_corners = find_candidate_corners(corners, lines);

    cv::waitKey();

    return 0;
}

std::vector<cv::Point2f> find_candidate_corners(std::vector<cv::Point2f> corners, std::vector<cv::Vec4f> lines)
{
    std::vector<cv::Point2f> candidate_corners;

    return candidate_corners;
}

double distance_between_points(cv::Point2f pointA, cv::Point2f pointB)
{
    double dist = cv::sqrt( (pointA.x - pointB.x) * (pointA.x - pointB.x) +
                            (pointA.y - pointB.y) * (pointA.y - pointB.y));

    return dist;
}