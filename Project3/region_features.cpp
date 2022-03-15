//
//  region_features.cpp
//  project-3
//
//  Created by Anish Satalkar on 3/6/22.
//

#include "region_features.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stack>
#include <iostream>
#include <map>
#include "opencv2/opencv.hpp"
#define R 2
#define G 1
#define B 0

int calculateMoment_PQ(cv::Mat &src, int p, int q, cv::Mat &regionMap, int regionId){
    int m_pq = 0;
    int x = 1;
    int y = 1;
    int last_row_idx = src.rows - 1;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            x = j;
            y = last_row_idx - i;
            if(regionMap.at<cv::Vec3b>(y,x)[R] == regionId){
                m_pq += pow(x,p) * pow(y,q) * 1;
            }

        }
    }
    return m_pq;
}

int getCentroid(cv::Mat &srcRegion, int &x, int &y, cv::Mat &regionMap, int regionId){
    int m_00 = calculateMoment_PQ(srcRegion, 0, 0, regionMap, regionId);
    x = calculateMoment_PQ(srcRegion, 1, 0, regionMap, regionId) / m_00;
    y = calculateMoment_PQ(srcRegion, 0, 1, regionMap, regionId) / m_00;
    return 0;
}

int initialiseRegionMap(cv::Mat &regionMap){
    for(int i=0; i<regionMap.rows; i++){
        for(int j=0; j<regionMap.cols;j++){
            regionMap.at<int>(i,j)=-1;
        }
    }
    return 0;
}

std::vector<int> findSeed(cv::Mat srcCopy){
    std::vector<int> index={0,0};
    for(int x=0;x<srcCopy.rows;x++){
        for(int y=0;y<srcCopy.cols;y++){
            if(srcCopy.at<cv::Vec3b>(x,y)[2]==255){
                //cout<<"seed x "<<x<<"y "<<y<<endl;
                index[0]=x;
                index[1]=y;
                return index;
            }
            
        }
    }
    index[0]=-1;
    index[1]=-1;
    return index;
}

std::vector<int> largestRegions(cv::Mat regionMap){
    std::vector<int> count(100);
    int ibuff=0.20*regionMap.rows;
    int jbuff=0.20*regionMap.cols;
    
    for(int i=ibuff; i<regionMap.rows-ibuff;i++){
        for(int j=jbuff;j<regionMap.cols-jbuff; j++){
            if(regionMap.at<int>(i,j)>0){
                count[regionMap.at<int>(i,j)]+=1;
            }
        }
    }
    
    //Index value for each region which is also the regionID
    std::vector<int> indexFinal(100);
    
    for(int i=0; i<100;i++){
        indexFinal[i]=i;
    }
    
    //Sorting to get the idnex/region ID in descending order
    for(int i=0; i<count.size()-1; i++){
        for(int j=i+1; j<count.size();j++){
            if(count[j]>count[i]){
                std::swap(count[i], count[j]);
                std::swap(indexFinal[i], indexFinal[j]);
            }
        }
    }
    
    return indexFinal;
}

std::vector<float> computeFeatures(cv::Mat &regionMap, int regionID, cv::Mat &src){
    long M_00=0;
    long M_10=0;
    long M_01=0;
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                int x=j;
                int y=(src.rows-1)-i;
                int pixel=src.at<cv::Vec3b>(i,j)[R];
                M_00+= pow(x,0)*pow(y,0)*pixel;
                M_10+= pow(x,1)*pow(y,0)*pixel;
                M_01+= pow(x,0)*pow(y,1)*pixel;
            }
        }
    }
    
    //    cout<<"M_00 "<<M_00<<endl;
    //    cout<<"M_01 "<<M_01<<endl;
    //    cout<<"M_10 "<<M_10<<endl;
    
    float x_centroid = round(M_10/M_00);
    float y_centroid = round(M_01/M_00);
    
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                int x=j;
                int y=(src.rows-1)-i;
                if((x<x_centroid+2 && x>x_centroid-2) && (y<y_centroid+2 && y>y_centroid-2)){
                    int k=x;
                    int l=(src.rows-1)-y;
                    src.at<cv::Vec3b>(l,k)[R]=255;
                    src.at<cv::Vec3b>(l,k)[G]=255;
                    src.at<cv::Vec3b>(l,k)[B]=0;
                }
            }
        }
    }
    
    long mu_11=0;
    long mu_20=0;
    long mu_02=0;
    
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                int x=j;
                int y=(src.rows-1)-i;
                int pixel=src.at<cv::Vec3b>(i,j)[R];
                mu_11+= pow(x-x_centroid,1)*pow(y-y_centroid,1)*pixel;
                mu_20+= pow(x-x_centroid,2)*pow(y-y_centroid,0)*pixel;
                mu_02+= pow(x-x_centroid,0)*pow(y-y_centroid,2)*pixel;
            }
        }
    }
    
    double alpha = atan((float)(2*mu_11)/(float)(mu_20-mu_02))/2;
    int x2 = x_centroid+200*cos(alpha);
    int y2=y_centroid+200*sin(alpha);
    
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                src.at<cv::Vec3b>(i,j)[0]=255;
                int x=j;
                int y=(src.rows-1)-i;
                if(x==x_centroid && y==y_centroid){
                    int p1i = (src.rows-1)-y_centroid;
                    int p1j = x_centroid;
                    int p2i = (src.rows-1)-y2;
                    int p2j = x2;
                    cv::Point p1(p1j, p1i), p2(p2j, p2i);
                    cv::line(src, p1, p2, cv::Scalar(255, 0, 0),2);
                }
            }
        }
    }
    
    
    float max_X=0, min_X=10000, max_Y=0, min_Y=10000;
    float max_X_Y=0, min_X_Y=0, max_Y_X=0, min_Y_X=0;
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                float x=j;
                float y=(src.rows-1)-i;
                float x_prime = (x-x_centroid)*cos(alpha)+(y-y_centroid)*sin(alpha);
                float y_prime = (x-x_centroid)*(-1*sin(alpha))+(y-y_centroid)*cos(alpha);
                if(x_prime>max_X){
                    max_X=x_prime;
                    max_X_Y=y_prime;
                }
                if(x_prime<min_X){
                    min_X=x_prime;
                    min_X_Y=y_prime;
                }
                if(y_prime>max_Y){
                    max_Y=y_prime;
                    max_Y_X=x_prime;
                }
                if(y_prime<min_Y){
                    min_Y=y_prime;
                    min_Y_X=x_prime;
                }
            }
        }
    }
    
    float rot_xmax=max_X+x_centroid;
        float rot_ymax=max_Y+y_centroid;
        float rot_xmin=min_X+x_centroid;
        float rot_ymin= min_Y+y_centroid;

//    float rot_xmax=round(max_X*cos(alpha)-max_X_Y*sin(alpha)+x_centroid);
//    float rot_ymax=round(max_Y_X* sin(alpha)+ max_Y*cos(alpha)+y_centroid);
//    float rot_xmin=round(min_X*cos(alpha)-min_X_Y*sin(alpha)+x_centroid);
//    float rot_ymin= round(min_Y_X* sin(alpha)+ min_Y*cos(alpha)+y_centroid);
    
    float aspect_ratio=0.0;
    float total_area=0.0, filled_area=0.0,perc_filled=0.0;
    for(int i=0; i<regionMap.rows;i++){
        for(int j=0;j< regionMap.cols;j++){
            if(regionMap.at<int>(i,j)==regionID){
                int x=j;
                int y=(src.rows-1)-i;
                if(x==x_centroid && y==y_centroid){
                    cv::Point up_left(rot_xmax,(src.rows-1)-rot_ymax), up_right(rot_xmax,(src.rows-1)-rot_ymin), down_right(rot_xmin,(src.rows-1)-rot_ymin), down_left(rot_xmin,(src.rows-1)-rot_ymax);
                    cv::line(src, up_left, up_right, cv::Scalar(255, 0, 0),2);
                    cv::line(src, up_right, down_right, cv::Scalar(255, 0, 0),2);
                    cv::line(src, down_right, down_left, cv::Scalar(255, 0, 0),2);
                    cv::line(src, down_left, up_left, cv::Scalar(255, 0, 0),2);
                    aspect_ratio= cv::norm(up_left-up_right)/cv::norm(up_right-down_right);
                }
            }
        }
        
    }
    
    //Get percentage filled
    for(int i=rot_ymin; i<=rot_ymax;i++){
        int k=(src.rows-1)-i;
        for(int j=rot_xmin; j<=rot_xmax;j++){
            total_area+=1;
            if(regionMap.at<int>(k,j)==regionID){
                filled_area+=1;
            }
        }
    }
    perc_filled=(filled_area/total_area)*100;
    //    cout<<"aspect ratio "<<aspect_ratio<<endl;
    //    cout<<"Perc filled "<<perc_filled<<endl;
    //    cout<<"mu_20  "<<mu_20/M_00<<endl;
    //    cout<<"mu_02 "<<mu_02/M_00<<endl;

    std::vector<float> features;
    features.push_back(mu_02/M_00);
    features.push_back(mu_20/M_00);
    features.push_back(aspect_ratio);
    features.push_back(perc_filled);
    return features;
}



cv::Mat getRegionMap(cv::Mat &src){
    cv::Mat regionMap(src.size(),CV_16SC3);
    cv::Mat srcCopy;
    src.copyTo(srcCopy);
    
    //Sets all the values in the region map to -1.
    initialiseRegionMap(regionMap);
    int currRedionId=1;
    std::stack<int> seedStack;
    
    //Scans through the source copy and finds the first foreground pixel.
    std::vector<int> index=findSeed(srcCopy);
    
    //Pushed the j,i value if the seed pixel onto the stack
    seedStack.push(index[1]);
    seedStack.push(index[0]);
    
    //Sets the seed pixel to 0 so findSeed does not reconsider the pixel as seed.
    srcCopy.at<cv::Vec3b>(index[0],index[1])[R]=0;
    
    //Gives the seed pixel current region ID.
    regionMap.at<int>(index[0],index[1])=currRedionId;
    
    //While there still exists a seed pixel.
    while(index[0]!=-1 && index[1]!=-1){
        //Checks for neighbours for a given region
        while(!seedStack.empty()){
            int k=seedStack.top();   //pixel column value
            seedStack.pop();
            int l=seedStack.top(); //pixel row value
            seedStack.pop();
            //Checks for 8 pixel neighbours around the current pixel.
            for(int i=-1;i<2;i++){
                for(int j=-1;j<2;j++){
                    //If the pixel has not been assigned a region and its a FG pixel.
                    if(regionMap.at<int>(k+i,l+j)==-1 && srcCopy.at<cv::Vec3b>(k+i,l+j)[R]<=255 && srcCopy.at<cv::Vec3b>(k+i,l+j)[R]>=254) {
                        regionMap.at<int>(k+i,l+j)=currRedionId;  //assign the pixel the region ID
                        srcCopy.at<cv::Vec3b>(k+i,l+j)[R]=0;  //Set the pixel value to 0 so findSeed would consider it
                        seedStack.push(l+j);
                        seedStack.push(k+i);
                    }
                }
            }
        }
        
        currRedionId++;
        index=findSeed(srcCopy);
        seedStack.push(index[1]);
        seedStack.push(index[0]);
        regionMap.at<int>(index[0],index[1])=currRedionId;
        srcCopy.at<cv::Vec3b>(index[0],index[1])[R]=0;
    }
    return regionMap;
}


int getImportantRegionId(cv::Mat &regionMap){
    // Region with max frequency in a centre window of the regionMap.
    int regionWithMaxFrequency = -1;
    
    // Define a window that is centered on the region map. Starts at 1/3 of rows and columns and ends at 2/3 of rows and columns.
    std::map<int, int> regionFrequency;
    int xStart = regionMap.cols / 3;
    int xEnd = xStart * 2;
    int yStart = regionMap.rows / 3;
    int yEnd = yStart * 2;
    int regionId = -1;
    for (int i = yStart; i < yEnd; i++) {
        for (int j = xStart; j < xEnd; j++) {
            regionId = regionMap.at<cv::Vec3b>(i,j)[R];
            if (regionFrequency.count(regionId) == 0){
                regionFrequency[regionId] = 1;
            }else{
                regionFrequency[regionId] += 1;
            }
        }
    }
    
    int maxFrequency = -1;
    for (auto it = regionFrequency.begin(); it != regionFrequency.end(); ++it) {
        if(it->second > maxFrequency){
            maxFrequency = it->second;
            regionWithMaxFrequency = it->first;
        }
    }
    
    return regionWithMaxFrequency;
}
