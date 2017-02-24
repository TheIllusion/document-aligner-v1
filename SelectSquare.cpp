//
// Created by Illusion on 2017. 2. 23..
//

#include "SelectSquare.h"
#include <algorithm>
#include <iterator>

std::vector<cv::Point2i> selectSquareInSquares(std::vector<std::vector<cv::Point2i>>& squares,
                                               cv::Mat &hsv_image)
{
    if (squares.size() == 1)
    {
        return squares[0];
    }

    //compare color difference between four points
    //iterate through squares
    double minimum_hue_difference = -1;
    std::vector<cv::Point2i> selected_square;

    for(std::vector<std::vector<cv::Point2i>>::iterator square = squares.begin(); square != squares.end(); square++)
    {
        cv::Vec3b hsv_pixel_0 = hsv_image.at<cv::Vec3b>((*square)[0] + cv::Point2i(15,15));
        cv::Vec3b hsv_pixel_1 = hsv_image.at<cv::Vec3b>((*square)[1] + cv::Point2i(-15,15));
        cv::Vec3b hsv_pixel_2 = hsv_image.at<cv::Vec3b>((*square)[2] + cv::Point2i(-15,-15));
        cv::Vec3b hsv_pixel_3 = hsv_image.at<cv::Vec3b>((*square)[3] + cv::Point2i(15,-15));

        cv::Vec3b hsv_pixel_4 = hsv_image.at<cv::Vec3b>((*square)[0] + cv::Point2i(25,25));
        cv::Vec3b hsv_pixel_5 = hsv_image.at<cv::Vec3b>((*square)[1] + cv::Point2i(-25,25));
        cv::Vec3b hsv_pixel_6 = hsv_image.at<cv::Vec3b>((*square)[2] + cv::Point2i(-25,-25));
        cv::Vec3b hsv_pixel_7 = hsv_image.at<cv::Vec3b>((*square)[3] + cv::Point2i(25,-25));

        double average_hue_value = (hsv_pixel_0[0] + hsv_pixel_1[0] + hsv_pixel_2[0] + hsv_pixel_3[0] +
                                    hsv_pixel_4[0] + hsv_pixel_5[0] + hsv_pixel_6[0] + hsv_pixel_7[0]) / 8;
        double sum_of_hue_variations = fabs(hsv_pixel_0[0] - average_hue_value) +
                                       fabs(hsv_pixel_1[0] - average_hue_value) +
                                       fabs(hsv_pixel_2[0] - average_hue_value) +
                                       fabs(hsv_pixel_3[0] - average_hue_value) +
                                       fabs(hsv_pixel_4[0] - average_hue_value) +
                                       fabs(hsv_pixel_5[0] - average_hue_value) +
                                       fabs(hsv_pixel_6[0] - average_hue_value) +
                                       fabs(hsv_pixel_7[0] - average_hue_value);

        if (minimum_hue_difference == -1 ||
            sum_of_hue_variations < minimum_hue_difference)
        {
            minimum_hue_difference = sum_of_hue_variations;
            selected_square = *square;
        }
    }

    return selected_square;

#if 0
    const double totalDistanceThreshold = 8.0;
    std::vector<int> similarSquareCounts;
    std::vector<std::vector<cv::Point2i>>diffSquares;

    int maxCountIndex = -1;
    int maxCount = 0;

    for (int i = 0; i < squares.size(); i++)
    {
        if (diffSquares.size() == 0)
        {
            diffSquares.push_back(squares[i]);
            similarSquareCounts.push_back(0);
        }
        else
        {
            bool foundSimilarSquare = false;
            for (int j = 0; j < diffSquares.size(); j++) {
                double totalDistance = 0;
                for (int sp = 0; sp < 4; sp++){
                    double minDistance = 200.0;
                    for (int dp = 0; dp < 4; dp++){
                        cv::Point2i sourcePoint = diffSquares[j][sp];
                        cv::Point2i destPoint = squares[i][dp];

                        double distance = cv::norm(sourcePoint-destPoint);
                        minDistance = MIN(distance, minDistance);
                    }

                    totalDistance += minDistance;
                }

                if (totalDistance < totalDistanceThreshold)
                {
                    foundSimilarSquare = true;

                    int newCount = similarSquareCounts[j] + 1;
                    similarSquareCounts[j] = newCount;

                    if (newCount >= maxCount)
                    {
                        maxCountIndex = j;
                        maxCount = newCount;
                    }

                    break;
                }
            }

            if (!foundSimilarSquare)
            {
                diffSquares.push_back(squares[i]);
                similarSquareCounts.push_back(0);
            }
        }
    }

    if (maxCountIndex >= 0)
    {
        return diffSquares[maxCountIndex];
    }
    else
    {
        return diffSquares[0];
    }
#endif
}

#define MINIMUM_LINE_LENGTH_FOR_SQUARE 100.0

//void generate_combinations(int N, int K)
std::vector<std::vector<cv::Point2i>> generate_combinations(std::vector<cv::Point2f> &corners)
{
    std::vector<std::vector<cv::Point2i>> squares;

    int N = corners.size();
    int K = 4;

    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    int count = 0;
    // print integers and permute bitmask
    do {
        std::vector<cv::Point2i> square;

        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i])
            {
                //std::cout << " " << i;
                square.push_back(corners[i]);
            }
        }

        sortCorners(square);

        if(distance_between_points(square[0], square[1]) > MINIMUM_LINE_LENGTH_FOR_SQUARE &&
           distance_between_points(square[1], square[2]) > MINIMUM_LINE_LENGTH_FOR_SQUARE &&
           distance_between_points(square[2], square[3]) > MINIMUM_LINE_LENGTH_FOR_SQUARE &&
           distance_between_points(square[3], square[0]) > MINIMUM_LINE_LENGTH_FOR_SQUARE)
        {
            //Check the angles
            double maxCosine = 0;

            for (int j = 1; j < 5; j++)
            {
                double cosine = fabs(angle(square[(j+1)%4], square[j-1], square[j%4]));
                maxCosine = MAX(maxCosine, cosine);
            }

//#define MAXIMUM_ALLOWED_COSINE_VALUE 0.3
#define MAXIMUM_ALLOWED_COSINE_VALUE 0.05

            if (maxCosine < MAXIMUM_ALLOWED_COSINE_VALUE)
                squares.push_back(square);
        };

        //std::cout << std::endl;
        count++;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    std::cout << "number of combinations: " << count << std::endl;
    std::cout << "selected combinations: " << squares.size() << std::endl;

    return squares;
}

void sortCorners(std::vector<cv::Point2i>& corners)
{
    std::vector<cv::Point2i> top, bot;

    /*
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }
    */

    // find the lowest point
    int max_y_index = 0;
    int temp_y = 0;
    for (int i = 0; i < corners.size(); i++)
    {
        if(corners[i].y > temp_y)
        {
            temp_y = corners[i].y;
            max_y_index = i;
        }
    }

    bot.push_back(corners[max_y_index]);

    // find the second lowest point
    int second_max_y_index = 0;
    temp_y = 0;
    for (int i = 0; i < corners.size(); i++)
    {
        if(i == max_y_index)
            continue;

        if(corners[i].y > temp_y)
        {
            temp_y = corners[i].y;
            second_max_y_index = i;
        }
    }

    bot.push_back(corners[second_max_y_index]);

    // top points
    for (int i = 0; i < corners.size(); i++)
    {
        if(i == max_y_index || i == second_max_y_index)
            continue;

        top.push_back(corners[i]);
    }


    cv::Point2i tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point2i tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point2i bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point2i br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}


double distance_between_points(cv::Point2f pointA, cv::Point2f pointB)
{
    double dist = cv::sqrt( (pointA.x - pointB.x) * (pointA.x - pointB.x) +
                            (pointA.y - pointB.y) * (pointA.y - pointB.y));

    return dist;
}


double angle(cv::Point2i pt1, cv::Point2i pt2, cv::Point2i pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}