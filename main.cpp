#include <iostream>

#include "opencv2/opencv.hpp"
#include "LsdLineDetector.h"
#include "GfttCornerDetector.h"
#include "SelectSquare.h"

//#define TEST_SET_ROOT_PATH "/home/illusion/ClionProjects/line-detection-experiments-git/Document_Aligner_TestSet/Document_Aligner_TestSet/normal_set/original/"
#define TEST_SET_ROOT_PATH "/Users/Illusion/Documents/Data/Document_Aligner_TestSet/normal_set/original/"

static char filename_buff[100];

static const char* file_name =
        //"IMG_1415_iphone.jpg";
        //"IMG_1415_iphone.jpg";
        //"IMG_1416_iphone.jpg";
        //"IMG_1417_iphone.jpg";
        //"IMG_1418_iphone.jpg";
        //"IMG_1419_iphone.jpg";
        //"IMG_1420_iphone.jpg";
        "IMG_1421_iphone.jpg";
        //"20170125_152406_galaxy.jpg";
        //"20170125_152538_galaxy.jpg";
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

    // Convert BGR to HSV
    cv::Mat hsv_image;
    cv::cvtColor(color_image, hsv_image, CV_BGR2HSV);

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

    cv::Mat candidate_corners_img = color_image.clone();
    cv::Mat result_corners_img = color_image.clone();

    Gftt gftt_detector;

    std::vector<cv::Point2f> corners = gftt_detector.DetectCorners(gray_image);

    LsdLineDetector lsd_detector;

    std::vector<cv::Vec4f> lines = lsd_detector.DetectLines(gray_image);

    std::vector<cv::Point2f> candidate_corners = find_candidate_corners(corners, lines);

    for(std::vector<cv::Point2f>::iterator it = candidate_corners.begin(); it != candidate_corners.end(); it++)
    {
        cv::circle(candidate_corners_img, *it, 3, cv::Scalar(0, 0, 255), 3);
    }

    cv::namedWindow("Candidate_Corners");
    cv::imshow("Candidate_Corners", candidate_corners_img);

    //generate_combinations(30, 4);
    std::vector<std::vector<cv::Point2i>> squares;

    if(candidate_corners.size() >= 4)
        squares = generate_combinations(candidate_corners);

#if 1
    std::vector<cv::Point2i> selected_square = selectSquareInSquares(squares, hsv_image);

    for(std::vector<cv::Point2i>::iterator it = selected_square.begin(); it != selected_square.end(); it++)
    {
        cv::circle(result_corners_img, *it, 3, cv::Scalar(0, 0, 255), 3);
    }

    //debug. show the color sampling points
    /*
    cv::circle(result_corners_img, selected_square[0] + cv::Point2i(20,20) , 3, cv::Scalar(0, 255, 0), 2);
    cv::circle(result_corners_img, selected_square[1] + cv::Point2i(-20,20) , 3, cv::Scalar(0, 255, 0), 2);
    cv::circle(result_corners_img, selected_square[2] + cv::Point2i(-20,-20) , 3, cv::Scalar(0, 255, 0), 2);
    cv::circle(result_corners_img, selected_square[3] + cv::Point2i(20,-20) , 3, cv::Scalar(0, 255, 0), 2);
    */

    cv::namedWindow("Result_Corners");
    cv::imshow("Result_Corners", result_corners_img);
#endif

    cv::waitKey();

    return 0;
}

#define MAXIMUM_DISTANCE_THRESHOLD 10

std::vector<cv::Point2f> find_candidate_corners(std::vector<cv::Point2f> corners, std::vector<cv::Vec4f> lines)
{
    std::vector<cv::Point2f> candidate_corners;
    cv::Point2f temp_point1, temp_point2;
    double dist, minimum_dist;
    //cv::Point2f closest_corner;
    std::vector<cv::Point2f>::iterator closest_corner;

    //iterate through lines
    for(std::vector<cv::Vec4f>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        temp_point1.x = it->val[0];
        temp_point1.y = it->val[1];

        temp_point2.x = it->val[2];
        temp_point2.y = it->val[3];

        //iterate through corners
        minimum_dist = MAXIMUM_DISTANCE_THRESHOLD;
        for(std::vector<cv::Point2f>::iterator corner = corners.begin(); corner != corners.end(); corner++)
        {
            dist = distance_between_points(temp_point1, *corner);

            if( (dist < minimum_dist) && (dist < MAXIMUM_DISTANCE_THRESHOLD) )
            {
                //this means that the two points are located in similar locations
                minimum_dist = dist;
                closest_corner = corner;
            }
        }

        //save the closest corner
        if( (minimum_dist > 0) && (minimum_dist < MAXIMUM_DISTANCE_THRESHOLD) )
        {
            candidate_corners.push_back(*closest_corner);
            corners.erase(closest_corner);
        }

        //iterate through corners
        minimum_dist = MAXIMUM_DISTANCE_THRESHOLD;
        for(std::vector<cv::Point2f>::iterator corner = corners.begin(); corner != corners.end(); corner++)
        {
            dist = distance_between_points(temp_point2, *corner);

            if( (dist < minimum_dist) && (dist < MAXIMUM_DISTANCE_THRESHOLD) )
            {
                //this means that the two points are located in similar locations
                minimum_dist = dist;
                closest_corner = corner;
            }
        }

        //save the closest corner
        if( (minimum_dist > 0) && (minimum_dist < MAXIMUM_DISTANCE_THRESHOLD) )
        {
            candidate_corners.push_back(*closest_corner);
            corners.erase(closest_corner);
        }
    }

    std::cout << "number of candidate corners: " << candidate_corners.size() << std::endl;
    return candidate_corners;
}
