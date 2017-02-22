//
// Created by illusion on 17. 2. 21.
//

#ifndef DOCUMENT_ALIGNER_V1_GFTTCORNERDETECTOR_H
#define DOCUMENT_ALIGNER_V1_GFTTCORNERDETECTOR_H

#include "opencv2/opencv.hpp"

class Gftt
{
public:
    std::vector<cv::Point2f> DetectCorners(cv::Mat &image);

private:
};
#endif //DOCUMENT_ALIGNER_V1_GFTTCORNERDETECTOR_H
