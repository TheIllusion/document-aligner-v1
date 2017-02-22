//
// Created by illusion on 17. 2. 22.
//

#include "LsdLineDetector.h"

#define MIN_LINE_SEG_LENGTH_LSD 50.0

std::vector<cv::Vec4f> LsdLineDetector::DetectLines(cv::Mat &image)
{
    std::vector<cv::Vec4f> lines_std;
    std::vector<float> widths;

    cv::Mat processing_image = image.clone();

    double start = double(cv::getTickCount());

    // Detect the lines
    ls->detect(processing_image, lines_std, widths);

    double duration_ms = (double(cv::getTickCount()) - start) * 1000 / cv::getTickFrequency();
    std::cout << "LSD: It took " << duration_ms << " ms." << std::endl;
    std::cout << "Number of line segments " << lines_std.size() << std::endl;

    for(std::vector<cv::Vec4f>::iterator it = lines_std.begin(); it != lines_std.end(); it++)
    {
        double dist = cv::sqrt( (it->val[0] - it->val[2]) * (it->val[0] - it->val[2]) +
                                (it->val[1] - it->val[3]) * (it->val[1] - it->val[3]));
        if(dist < MIN_LINE_SEG_LENGTH_LSD)
        {
            it = lines_std.erase(it);
            --it;
        }
        else
        {
            //std::cout << "dist = " << dist << std::endl;
        }
    }

    std::cout << "Number of line segments(after deletion) " << lines_std.size() << std::endl;

    // Show found lines
    ls->drawSegments(image, lines_std);

    cv::namedWindow("Display");
    cv::imshow("Display", image);

    return lines_std;
}