//
// Created by Illusion on 2017. 2. 23..
//

#include "SelectSquare.h"
#include <algorithm>
#include <iterator>

std::vector<cv::Point2i> selectSquareInSquares(std::vector<std::vector<cv::Point2i>>& squares)
{
    if (squares.size() == 1)
    {
        return squares[0];
    }

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
}

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

        squares.push_back(square);

        //std::cout << std::endl;
        count++;
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    std::cout << "number of combinations: " << count << std::endl;

    return squares;
}
