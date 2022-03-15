//
//  main.cpp
//  Project3
//
//  Created by Ankitha Udupa on 2/27/22.
//

#include <iostream>
#include<string>
#include "opencv2/opencv.hpp"
#include "region_features.hpp"
#include "preprocess.hpp"
#include "csv_util.h"
using namespace cv;
using namespace std;

float calculateSD(float data[], int len) {
    float sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    
    for(i = 0; i < len; ++i) {
        sum += data[i];
    }
    
    mean = sum / len;
    
    for(i = 0; i < len; ++i) {
        standardDeviation += pow(data[i] - mean, 2);
    }
    
    float finalSD=sqrt((standardDeviation / (len-1)));
    return finalSD;
}
int main(int argc, const char * argv[]) {
    char filename[256]="data/features.csv";
    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    vector<float> finalDistance;
    
    cv::Mat img= imread("img/mask2.png");
    
    cv::Mat dst(img.size(), CV_8UC3);
    //    blur5x5( img, dst);
    thresholding(img, dst);
    //    grow(img, dst);
    grow(dst, img);
    grow(img, dst);
    grow(dst, img);
    grow(img, dst);
    grow(dst, img);
    shrink(img, dst);
    shrink(dst, img);
    
    //compute target image features
    cv::Mat regionMap = getRegionMap(img);
    std::vector<int> largestRegionsList = largestRegions(regionMap);
    int importantRegionId = largestRegionsList[0];
    std::vector<float> targetFeatures = computeFeatures(regionMap, importantRegionId, img);
    
    //read image features from the csv file
    read_image_data_csv(filename, filenames, data, 0 );
    
    //Compute distances for each image in database with target image
    float aspectRatio[data.size()], perc_fill[data.size()], mu20[data.size()], mu02[data.size()];
    for(int i=0;i<data.size();i++){
        aspectRatio[i]=data[i][0];
        perc_fill[i]=data[i][1];
        mu20[i]=data[i][2];
        mu02[i]=data[i][3];
    }
    
    
    float sd_aspectRatio = calculateSD(aspectRatio,data.size());
    float sd_perc_fill = calculateSD(perc_fill,data.size());
    float sd_mu20 = calculateSD(mu20,data.size());
    float sd_mu02 = calculateSD(mu02,data.size());
    
    
    for(int i=0;i<data.size();i++){
        float distance=0.0;
        distance+=(targetFeatures[0]-data[i][0])*(targetFeatures[0]-data[i][0])/sd_aspectRatio;
        distance+=(targetFeatures[1]-data[i][1])*(targetFeatures[1]-data[i][1])/sd_perc_fill;
        distance+=(targetFeatures[2]-data[i][2])*(targetFeatures[2]-data[i][2])/sd_mu20;
        distance+=(targetFeatures[3]-data[i][3])*(targetFeatures[3]-data[i][3])/sd_mu02;
        finalDistance.insert(finalDistance.end(),distance);
    }
    
    //sort distance metric array and filenames
    for(int i=0; i<data.size()-1; i++){
        for(int j=i+1; j<data.size();j++){
            if(finalDistance[j]<finalDistance[i]){
                std::swap(finalDistance[i], finalDistance[j]);
                std::swap(filenames[i], filenames[j]);
            }
        }
    }
    
    cout<<"Final Label "<<filenames[0]<<" dist "<<finalDistance[0]<<endl;
    
    return 0;
}
