////
////  readImages.cpp
////  Project2
////
////  Created by Ankitha Udupa on 2/16/22.
////
//
//#include "readImages.hpp"
//#include <cstdio>
//#include <cstring>
//#include <cstdlib>
//#include <dirent.h>
//#include "opencv2/opencv.hpp"
//using namespace cv;
//using namespace std;
//#include "objectRecognition.hpp"
//#include "csv_util.h"
//#include "preprocess.hpp"
//#include "region_features.hpp"
//
///*
//  Given a directory on the command line, scans through the directory for image files.
//
//  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
// */
//
//int main(int argc, char *argv[]) {
//
//  char dirname[256];
//  char buffer[256];
//  FILE *fp;
//  DIR *dirp;
//  struct dirent *dp;
//  int i;
//
//  // check for sufficient arguments
////  if( argc < 2) {
////    printf("usage: %s <directory path>\n", argv[0]);
////    exit(-1);
////  }
//
//  // get the directory path
//  strcpy(dirname, "img/");
//
//  // open the directory
//  dirp = opendir( dirname );
//  if( dirp == NULL) {
//    printf("Cannot open directory %s\n", dirname);
//    exit(-1);
//  }
//
//  // loop over all the files in the image file listing
//  while( (dp = readdir(dirp)) != NULL ) {
//
//    // check if the file is an image
//    if( strstr(dp->d_name, ".JPG") ||
//    strstr(dp->d_name, ".png") ||
//    strstr(dp->d_name, ".ppm") ||
//    strstr(dp->d_name, ".tif") ) {
//
//      // build the overall filename
//      strcpy(buffer, dirname);
//      strcat(buffer, "/");
//      strcat(buffer, dp->d_name);
//
//
//        cout<<"buffer "<<buffer<<endl;
//      Mat img= imread(buffer);
//      cv::Mat dst(img.size(), CV_8UC3);
//        //    blur5x5( img, dst);
//        thresholding(img, dst);
//        //    grow(img, dst);
//        grow(dst, img);
//        grow(img, dst);
//        grow(dst, img);
//        grow(img, dst);
//        grow(dst, img);
//        shrink(img, dst);
//        shrink(dst, img);
//        
//        imshow(dp->d_name, img);
//        waitKey();
//        //compute target image features
//        cv::Mat regionMap = getRegionMap(img);
//        std::vector<int> largestRegionsList = largestRegions(regionMap);
//        int importantRegionId = largestRegionsList[0];
//        std::vector<float> targetFeatures = computeFeatures(regionMap, importantRegionId, img);
//
//        
//        char filename[256]="";
//
//
//        strcat(filename, "data/features.csv");
//        
//        char label[30];
//        cout<<dp->d_name<<endl;
//        cin>>label;
//
//        append_image_data_csv(filename, label, targetFeatures, 0 );
//    }
//  }
//    return 0;
//}
//
//
//
//
//
