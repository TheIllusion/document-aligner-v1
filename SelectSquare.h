//
// Created by Illusion on 2017. 2. 23..
//

#ifndef DOCUMENT_ALIGNER_V1_SELECTSQUARE_H
#define DOCUMENT_ALIGNER_V1_SELECTSQUARE_H

#include "opencv2/opencv.hpp"

std::vector<cv::Point2i> selectSquareInSquares(std::vector<std::vector<cv::Point2i>>& squares, cv::Mat &lab_image);

//void generate_combinations(int N, int K);
std::vector<std::vector<cv::Point2i>> generate_combinations(std::vector<cv::Point2f> &corners);

//void sortCorners(std::vector<cv::Point2i>& corners, cv::Point2f center);
void sortCorners(std::vector<cv::Point2i>& corners);

double distance_between_points(cv::Point2f pointA, cv::Point2f pointB);

double angle(cv::Point2i pt1, cv::Point2i pt2, cv::Point2i pt0);

#endif //DOCUMENT_ALIGNER_V1_SELECTSQUARE_H
