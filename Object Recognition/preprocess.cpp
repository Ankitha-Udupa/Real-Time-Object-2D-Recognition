//
//  preprocess.cpp
//  project-3
//
//  Created by Anish Satalkar on 2/26/22.
//

#include "preprocess.hpp"

#define B 0
#define G 1
#define R 2
#define FG_COLOR 255
#define BG_COLOR 0

/*
 Performs convolution between filter and image data.
 */
cv::Vec3b applyFilter(const cv::Mat &src, int i, int j, const cv::Mat &filter){

    int c,x,y;
    cv::Vec3b result={0,0,0};
    cv::Vec3b finalresult;
    
   
    int halfrow = filter.rows/2;
    int halfcol = filter.cols/2;

    for(y=0;y<filter.rows;y++){
        for(x=0;x<filter.cols;x++){
            for(c=0;c<3;c++){
                result[c] += (filter.at<float>(y,x) * src.at<cv::Vec3b>(i-halfrow+y,j-halfcol+x)[c]/10);
    
            }
        }
    }

    for(c=0;c<3;c++){
        finalresult[c] = (unsigned char) result[c];
    }
    
    return (finalresult);
}

int paint_pixel(cv::Vec3b &pixel, int color){
    for (int c = 0; c < 3; c++) {
        pixel[c] = (unsigned char) color;
    }
    return 0;
}

int growBorderRows(cv::Mat &src, cv::Mat &dst){
    int row = 0;
    int last_row = src.rows - 1;
    for (int j = 1; j < src.cols-1; j++) {
        if(src.at<cv::Vec3b>(row, j)[R] == FG_COLOR){
            // Lower cell
            if(src.at<cv::Vec3b>(row + 1, j)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(row + 1, j), FG_COLOR);
            }
            // Right cell
            if(src.at<cv::Vec3b>(row, j + 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(row, j + 1), FG_COLOR);
            }
            // Left cell
            if(src.at<cv::Vec3b>(row, j - 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(row, j - 1), FG_COLOR);
            }
        }
        paint_pixel(dst.at<cv::Vec3b>(row,j), src.at<cv::Vec3b>(row,j)[R]);
        
        if(src.at<cv::Vec3b>(last_row, j)[R] == FG_COLOR){
            // Upper cell
            if(src.at<cv::Vec3b>(last_row - 1, j)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(last_row - 1, j), FG_COLOR);
            }
            // Right cell
            if(src.at<cv::Vec3b>(last_row, j + 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(last_row, j + 1), FG_COLOR);
            }
            // Left cell
            if(src.at<cv::Vec3b>(last_row, j - 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(last_row, j - 1), FG_COLOR);
            }
        }
        paint_pixel(dst.at<cv::Vec3b>(last_row,j), src.at<cv::Vec3b>(last_row,j)[R]);
    }
    return 0;
}

int growBorderColumns(cv::Mat &src, cv::Mat &dst){
    int first_col = 0;
    int last_col = src.cols - 1;
    for (int i = 1; i < src.rows-1; i++) {
        if(src.at<cv::Vec3b>(i, first_col)[R] == FG_COLOR){
            // Upper cell
            if(src.at<cv::Vec3b>(i - 1, first_col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i - 1, first_col), FG_COLOR);
            }
            // Lower cell
            if(src.at<cv::Vec3b>(i + 1, first_col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i, first_col + 1), FG_COLOR);
            }
            // Right cell
            if(src.at<cv::Vec3b>(i, first_col + 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i, first_col + 1), FG_COLOR);
            }
        }
        paint_pixel(dst.at<cv::Vec3b>(i, first_col), src.at<cv::Vec3b>(i, first_col)[R]);
        
        if(src.at<cv::Vec3b>(i, last_col)[R] == FG_COLOR){
            // Left cell
            if(src.at<cv::Vec3b>(i, last_col - 1)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i, last_col), FG_COLOR);
            }
            // Lower cell
            if(src.at<cv::Vec3b>(i + 1, last_col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i + 1, last_col), FG_COLOR);
            }
            // Left cell
            if(src.at<cv::Vec3b>(i - 1, last_col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(i - 1, last_col), FG_COLOR);
            }
        }
        paint_pixel(dst.at<cv::Vec3b>(i, last_col), src.at<cv::Vec3b>(i, last_col)[R]);
    }
    return 0;
}

int growCorners(cv::Mat &src, cv::Mat &dst){
    int borderPixels[] = {
        0,0,
        0,src.cols-1,
        src.rows - 1, src.cols - 1,
        src.rows - 1, 0
    };
    
    int adjacent[] = {
        0,1,1,0,
        0,-1,1,0,
        0,-1,-1,0,
        -1,0,0,1
    };
    int row,col;
    for (int i = 0 ; i < 4; i++) {
        row = borderPixels[i*2];
        col = borderPixels[i*2 + 1];
        if(src.at<cv::Vec3b>(row, col)[R] == FG_COLOR){
            if(src.at<cv::Vec3b>(adjacent[i*4] + row, adjacent[i*4 + 1] + col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(adjacent[i*4] + row, adjacent[i*4 + 1] + col), FG_COLOR);
            }
            if(src.at<cv::Vec3b>(adjacent[i*4 + 2] + row, adjacent[i*4 + 3] + col)[R] == BG_COLOR){
                paint_pixel(dst.at<cv::Vec3b>(adjacent[i*4 + 2] + row, adjacent[i*4 + 3] + col), FG_COLOR);
            }
            paint_pixel(dst.at<cv::Vec3b>(row, col), src.at<cv::Vec3b>(row, col)[R]);
        }
    }
    return 0;
}

int grow(cv::Mat &src, cv::Mat &dst){
    growBorderRows(src,dst);
    growBorderColumns(src, dst);
    growCorners(src, dst);
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++) {
            if(src.at<cv::Vec3b>(i,j)[R] == FG_COLOR){
                // Upper cell
                if(src.at<cv::Vec3b>(i - 1, j)[R] == BG_COLOR){
                    paint_pixel(dst.at<cv::Vec3b>(i - 1,j), FG_COLOR);
                }
                // Lower cell
                if(src.at<cv::Vec3b>(i + 1, j)[R] == BG_COLOR){
                    paint_pixel(dst.at<cv::Vec3b>(i + 1, j), FG_COLOR);
                }
                // Right cell
                if(src.at<cv::Vec3b>(i, j + 1)[R] == BG_COLOR){
                    paint_pixel(dst.at<cv::Vec3b>(i, j + 1), FG_COLOR);
                }
                // Left cell
                if(src.at<cv::Vec3b>(i, j - 1)[R] == BG_COLOR){
                    paint_pixel(dst.at<cv::Vec3b>(i, j - 1), FG_COLOR);
                }
            }
            paint_pixel(dst.at<cv::Vec3b>(i,j), src.at<cv::Vec3b>(i,j)[R]);
        }
    }
    
    return 0;
}

int shrinkBorderRows(cv::Mat &src, cv::Mat &dst){
    int row = 0;
    int last_row = src.rows - 1;
    for (int j = 1; j < src.cols-1; j++) {
        if(src.at<cv::Vec3b>(row + 1, j)[R] == BG_COLOR || // Lower cell
           src.at<cv::Vec3b>(row, j + 1)[R] == BG_COLOR || // Right cell
           src.at<cv::Vec3b>(row, j - 1)[R] == BG_COLOR){ // Left cell
            paint_pixel(dst.at<cv::Vec3b>(row,j), BG_COLOR);
        }
        
        if(src.at<cv::Vec3b>(last_row - 1, j)[R] == BG_COLOR || // Lower cell
           src.at<cv::Vec3b>(last_row, j + 1)[R] == BG_COLOR || // Right cell
           src.at<cv::Vec3b>(last_row, j - 1)[R] == BG_COLOR){ // Left cell
            paint_pixel(dst.at<cv::Vec3b>(last_row,j), BG_COLOR);
        }
    }
    return 0;
}

int shrinkBorderColumns(cv::Mat &src, cv::Mat &dst){
    int first_col = 0;
    int last_col = src.cols - 1;
    for (int i = 1; i < src.rows-1; i++) {
        if(src.at<cv::Vec3b>(i, first_col + 1)[R] == BG_COLOR || // Right cell
           src.at<cv::Vec3b>(i - 1, first_col)[R] == BG_COLOR || // Upper cell
           src.at<cv::Vec3b>(i + 1, first_col)[R] == BG_COLOR){ // Lower cell
            paint_pixel(dst.at<cv::Vec3b>(i, first_col), BG_COLOR);
        }
        
        if(src.at<cv::Vec3b>(i, last_col - 1)[R] == BG_COLOR || // Lower cell
           src.at<cv::Vec3b>(i - 1, last_col)[R] == BG_COLOR || // Right cell
           src.at<cv::Vec3b>(i + 1, last_col)[R] == BG_COLOR){ // Left cell
            paint_pixel(dst.at<cv::Vec3b>(i, last_col), BG_COLOR);
        }
    }
    return 0;
}

int shrinkCorners(cv::Mat &src, cv::Mat &dst){
    int borderPixels[] = {0,0,
        0,src.cols-1,
        src.rows - 1, src.cols - 1,
        src.rows - 1, 0
    };
    
    int adjacent[] = {
        0,1,1,0,
        0,-1,1,0,
        0,-1,-1,0,
        -1,0,0,1
    };
    int row,col;
    for (int i = 0 ; i < 4; i++) {
        row = borderPixels[i*2];
        col = borderPixels[i*2 + 1];
        if(src.at<cv::Vec3b>(adjacent[i*4] + row, adjacent[i*4 + 1] + col)[R] == BG_COLOR || // Right cell
           src.at<cv::Vec3b>(adjacent[i*4 + 2] + row, adjacent[i*4 + 3] + col)[R] == BG_COLOR){ // Lower cell
            paint_pixel(dst.at<cv::Vec3b>(row, col), BG_COLOR);
        }
    }
    return 0;
}

int shrink(cv::Mat &src, cv::Mat &dst){
    shrinkBorderRows(src, dst);
    shrinkBorderColumns(src, dst);
    shrinkCorners(src, dst);
    for (int i = 1; i < src.rows-1; i++) {
        for (int j = 1; j < src.cols-1; j++) {
            paint_pixel(dst.at<cv::Vec3b>(i,j), src.at<cv::Vec3b>(i,j)[R]);
            if(src.at<cv::Vec3b>(i,j)[R] == FG_COLOR){
                // If any adjacent pixel to a pixel with FG_COLOR is found to have BG_COLOR, change it to BG_COLOR
                if(src.at<cv::Vec3b>(i - 1, j)[R] == BG_COLOR || // Upper cell
                   src.at<cv::Vec3b>(i + 1, j)[R] == BG_COLOR || // Lower cell
                   src.at<cv::Vec3b>(i, j + 1)[R] == BG_COLOR || // Right cell
                   src.at<cv::Vec3b>(i, j - 1)[R] == BG_COLOR){  // Left cell
                    paint_pixel(dst.at<cv::Vec3b>(i,j), BG_COLOR);
                }
            }
        }
    }
    return 0;
}

int thresholding(cv::Mat &src, cv::Mat &dst){
    
    for(int i=0; i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            cv::Vec3b color = src.at<cv::Vec3b>(i,j); //bgr
            
            //calculating chromaticity
            float r = ((float)color[R])/(color[B]+color[G]+color[R]+1);
            float g = ((float)color[G])/(color[B]+color[G]+color[R]+1);
            float b = 1-r-g;
        
            //white point (0.3127,0.3290)
            if((r<=0.369999 && r>=0.305555) && (g<=0.359999 && g>=0.305555)&& (b<=0.359999 && b>=0.290000)){
                dst.at<cv::Vec3b>(i,j)[B]=0;
                dst.at<cv::Vec3b>(i,j)[G]=0;
                dst.at<cv::Vec3b>(i,j)[R]=0;
            }
            else{
                dst.at<cv::Vec3b>(i,j)[B]=255;
                dst.at<cv::Vec3b>(i,j)[G]=255;
                dst.at<cv::Vec3b>(i,j)[R]=255;
            }
        
        }
    }
    return 0;
}

/*
 Applies 5x5 gaussian blur filter to an image.
 */
int blur5x5( cv::Mat &src, cv::Mat &dst )
{

    
    float data[10] = { 1, 2, 4, 2 ,1 } ;
    cv::Mat filter1 = cv::Mat(1, 5, CV_32F, data);
    cv::Mat filter2 = cv::Mat(5, 1, CV_32F, data);
    
    for(int i=1; i<src.rows-1;i++){
        for(int j=0;j<src.cols-1;j++){
            dst.at<cv::Vec3b>(i,j) = applyFilter(src,i,j,filter1);
            dst.at<cv::Vec3b>(i,j) = applyFilter(dst,i,j,filter2);
        }
    }
    
    return 0;
}
