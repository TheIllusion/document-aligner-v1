//
// Created by illusion on 17. 2. 21.
//

#include "GfttCornerDetector.h"

std::vector<cv::Point2f> Gftt::DetectCorners(cv::Mat &image)
{
    std::vector<cv::Point2f> corners;

    cv::goodFeaturesToTrack(image, corners, 500, 0.01, 10);

    cv::Mat gftt_corners = image.clone();
    for(std::vector<cv::Point2f>::iterator it = corners.begin(); it != corners.end(); it++)
    {
        cv::circle(gftt_corners, *it, 3, cv::Scalar(0, 0, 255), 3);
    }

    cv::namedWindow("GFTT");
    cv::imshow("GFTT", gftt_corners);

    return corners;
}
