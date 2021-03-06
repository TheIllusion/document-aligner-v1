//
// Created by Illusion on 2017. 2. 23..
//

#include "SelectSquare.h"
#include <algorithm>
#include <iterator>

std::vector<cv::Point2i> selectSquareInSquares(std::vector<std::vector<cv::Point2i>>& squares,
                                               cv::Mat &lab_image)
{
    if (squares.size() == 1)
    {
        return squares[0];
    }

    //compare color difference between four points
    //iterate through squares
    double minimum_lab_difference = -1;
    std::vector<cv::Point2i> selected_square;

    for(std::vector<std::vector<cv::Point2i>>::iterator square = squares.begin(); square != squares.end(); square++)
    {
        // sample colors from the neighbouring points of each corner
        cv::Vec3b lab_pixel[4];
        cv::Vec3b average_lab_value;
        double    average_lab_l, average_lab_a, average_lab_b;

#define MIN_SAMPLING_DEPTH_IN_DIST 100
#define MAX_SAMPLING_DEPTH_IN_DIST 150
#define SAMPLING_STARTING_POINT_IN_DIST 2
#define SAMPLING_DEPTH_IN_DIST 40

        // calculate the average LAB value
        average_lab_l = 0;
        average_lab_a = 0;
        average_lab_b = 0;

        for(int i = MIN_SAMPLING_DEPTH_IN_DIST; i < MAX_SAMPLING_DEPTH_IN_DIST; i++)
        {
            lab_pixel[0] = lab_image.at<cv::Vec3b>((*square)[0] + cv::Point2i(i, i));
            lab_pixel[1] = lab_image.at<cv::Vec3b>((*square)[1] + cv::Point2i(-i, i));
            lab_pixel[2] = lab_image.at<cv::Vec3b>((*square)[2] + cv::Point2i(-i, -i));
            lab_pixel[3] = lab_image.at<cv::Vec3b>((*square)[3] + cv::Point2i(i, -i));

            //debug
            /*
            std::cout << "L lab_pixel[0][0]: " << (double)lab_pixel[0][0] << std::endl;
            std::cout << "a lab_pixel[0][1]: " << (double)lab_pixel[0][1] << std::endl;
            std::cout << "b lab_pixel[0][2]: " << (double)lab_pixel[0][2] << std::endl;
            */

            //debug
            cv::circle(lab_image, (*square)[0] + cv::Point2i(i, i) , 1, cv::Scalar(255, 0, 0), 2);
            cv::circle(lab_image, (*square)[1] + cv::Point2i(-i, i) , 1, cv::Scalar(255, 0, 0), 2);
            cv::circle(lab_image, (*square)[2] + cv::Point2i(-i, -i) , 1, cv::Scalar(255, 0, 0), 2);
            cv::circle(lab_image, (*square)[3] + cv::Point2i(i, -i) , 1, cv::Scalar(255, 0, 0), 2);

            average_lab_l += (lab_pixel[0][0] + lab_pixel[1][0] + lab_pixel[2][0] + lab_pixel[3][0]);
            average_lab_a += (lab_pixel[0][1] + lab_pixel[1][1] + lab_pixel[2][1] + lab_pixel[3][1]);
            average_lab_b += (lab_pixel[0][2] + lab_pixel[1][2] + lab_pixel[2][2] + lab_pixel[3][2]);
        }

        average_lab_l = (double)average_lab_l / (4*(MAX_SAMPLING_DEPTH_IN_DIST-MIN_SAMPLING_DEPTH_IN_DIST));
        average_lab_a = (double)average_lab_a / (4*(MAX_SAMPLING_DEPTH_IN_DIST-MIN_SAMPLING_DEPTH_IN_DIST));
        average_lab_b = (double)average_lab_b / (4*(MAX_SAMPLING_DEPTH_IN_DIST-MIN_SAMPLING_DEPTH_IN_DIST));

        //debug
        std::cout << "average_lab_l: " << average_lab_l << std::endl;
        std::cout << "average_lab_a: " << average_lab_a << std::endl;
        std::cout << "average_lab_b: " << average_lab_b << std::endl;

        average_lab_value[0] = average_lab_l;
        average_lab_value[1] = average_lab_a;
        average_lab_value[2] = average_lab_b;

        //debug
        /*
        std::cout << "average Lab value(L): " << (double)average_lab_value[0] << std::endl;
        std::cout << "average Lab value(a): " << (double)average_lab_value[1] << std::endl;
        std::cout << "average Lab value(b): " << (double)average_lab_value[2] << std::endl;
        */

        double sum_of_lab_variations = 0;

        for(int i = SAMPLING_STARTING_POINT_IN_DIST; i < SAMPLING_DEPTH_IN_DIST; i++)
        {
            lab_pixel[0] = lab_image.at<cv::Vec3b>((*square)[0] + cv::Point2i(i, i));
            lab_pixel[1] = lab_image.at<cv::Vec3b>((*square)[1] + cv::Point2i(-i, i));
            lab_pixel[2] = lab_image.at<cv::Vec3b>((*square)[2] + cv::Point2i(-i, -i));
            lab_pixel[3] = lab_image.at<cv::Vec3b>((*square)[3] + cv::Point2i(i, -i));

#if 0
            sum_of_lab_variations += cv::norm(lab_pixel[0], average_lab_value) +
                                     cv::norm(lab_pixel[1], average_lab_value) +
                                     cv::norm(lab_pixel[2], average_lab_value) +
                                     cv::norm(lab_pixel[3], average_lab_value);
#else
            //for HSV
            sum_of_lab_variations += (lab_pixel[0][0] - average_lab_value[0]) * (lab_pixel[0][0] - average_lab_value[0]) +
                                     (lab_pixel[1][0] - average_lab_value[0]) * (lab_pixel[1][0] - average_lab_value[0]) +
                                     (lab_pixel[2][0] - average_lab_value[0]) * (lab_pixel[2][0] - average_lab_value[0]) +
                                     (lab_pixel[3][0] - average_lab_value[0]) * (lab_pixel[3][0] - average_lab_value[0]) +
                                     (lab_pixel[0][1] - average_lab_value[1]) * (lab_pixel[0][1] - average_lab_value[1]) +
                                     (lab_pixel[1][1] - average_lab_value[1]) * (lab_pixel[1][1] - average_lab_value[1]) +
                                     (lab_pixel[2][1] - average_lab_value[1]) * (lab_pixel[2][1] - average_lab_value[1]) +
                                     (lab_pixel[3][1] - average_lab_value[1]) * (lab_pixel[3][1] - average_lab_value[1]);
#endif
        }

        std::cout << "sum_of_lab_variations: " << sum_of_lab_variations << std::endl;

        if (minimum_lab_difference == -1 ||
            sum_of_lab_variations < minimum_lab_difference)
        {
            minimum_lab_difference = sum_of_lab_variations;
            selected_square = *square;
        }
    }

    std::cout << "minimum_lab_difference: " << minimum_lab_difference << std::endl;

    cv::namedWindow("Lab_image");
    cv::imshow("Lab_image", lab_image);

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

#define MINIMUM_LINE_LENGTH_FOR_SQUARE 150.0

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
#define MAXIMUM_ALLOWED_COSINE_VALUE 0.3

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

// using inner product between two vectors
double angle(cv::Point2i pt1, cv::Point2i pt2, cv::Point2i pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}