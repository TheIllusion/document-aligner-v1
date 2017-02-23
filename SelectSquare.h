//
// Created by Illusion on 2017. 2. 23..
//

#ifndef DOCUMENT_ALIGNER_V1_SELECTSQUARE_H
#define DOCUMENT_ALIGNER_V1_SELECTSQUARE_H

#include "opencv2/opencv.hpp"

std::vector<cv::Point2i> selectSquareInSquares(std::vector<std::vector<cv::Point2i>>& squares);

//void generate_combinations(int N, int K);
std::vector<std::vector<cv::Point2i>> generate_combinations(std::vector<cv::Point2f> &corners);

#endif //DOCUMENT_ALIGNER_V1_SELECTSQUARE_H
