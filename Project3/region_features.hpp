//
//  region_features.hpp
//  project-3
//
//  Created by Anish Satalkar on 3/6/22.
//

#ifndef region_features_hpp
#define region_features_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <vector>

int calculateMoment_PQ(cv::Mat &src, int p, int q, cv::Mat &regionMap, int regionId);

int getCentroid(cv::Mat &srcRegion, int &x, int &y, cv::Mat &regionMap, int regionId);

/// Calculates the region map of the input image.
/// @param src Input image.
cv::Mat getRegionMap(cv::Mat &src);

int getImportantRegionId(cv::Mat &regionMap);

/// Computes a list of features for a given input image based on the region map.
/// @param regionMap Region map of the input image.
/// @param regionId Input image.
/// @param dst Output image.
std::vector<float> computeFeatures(cv::Mat &regionMap ,int regionId, cv::Mat &dst);

/// Gives a list of regions in descending order of their frequencies.
/// @param regionMap Region map of an image.
std::vector<int> largestRegions(cv::Mat regionMap);

#endif /* region_features_hpp */
