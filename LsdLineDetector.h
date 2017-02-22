//
// Created by illusion on 17. 2. 21.
//

#ifndef DOCUMENT_ALIGNER_V1_LSDLINEDETECTOR_H
#define DOCUMENT_ALIGNER_V1_LSDLINEDETECTOR_H

#include "opencv2/opencv.hpp"

class LsdLineDetector
{
public:
    LsdLineDetector()
    {
        ls = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
    }

    std::vector<cv::Vec4f> DetectLines(cv::Mat &image);

private:
    cv::Ptr<cv::LineSegmentDetector> ls;
};
#endif //DOCUMENT_ALIGNER_V1_LSDLINEDETECTOR_H
