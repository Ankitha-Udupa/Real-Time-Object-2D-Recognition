//
//  preprocess.hpp
//  project-3
//
//  Created by Anish Satalkar on 2/26/22.
//

#ifndef preprocess_hpp
#define preprocess_hpp



#include <stdio.h>
#include <opencv2/core.hpp>
#include <iostream>

/// Grows the elements in an image using the 4-connected approach.
/// @param src Input cv::Mat
/// @param dst Output cv::Mat
int grow(cv::Mat &src, cv::Mat &dst);

/// Shrinks the elements in an image using the 4-connected approach
/// @param src Input cv::Mat
/// @param dst Output cv::Mat
int shrink(cv::Mat &src, cv::Mat &dst);

/// Creates a binary image by applying a threshold logic on the input image.
/// @param src Input color image
/// @param dst Output binary image
int thresholding(cv::Mat &src, cv::Mat &dst);

/// Applies blur filter over the image.
/// @param src Input color image.
/// @param dst Output blurred image.
int blur5x5( cv::Mat &src, cv::Mat &dst);

#endif /* preprocess_hpp */
