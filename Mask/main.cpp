//
//  main.cpp
//  Mask
//
//  Created by masakazu nakazawa on 2016/07/08.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <math.h>
#include "Core.hpp"
#include "Transform.hpp"
#include "ExtractFeaturePoint.hpp"
#include "Quaternion.hpp"
#include "Rotation.hpp"

int main(int argc, const char * argv[])
{
    const std::string path = "/Users/masakazu/Desktop/rot/";
    const std::string inputName = path + "hall4.jpg";
    const std::string rotColorName = path + "hall5rot.jpg";
    const std::string oriColorName = path + "hall5.jpg";

    const cv::Size frameSize(5376, 2688);
//    const cv::Size frameSize(960, 480);
    cv::Mat input = cv::imread(inputName);
    cv::Mat img;
    cv::resize(input, img, frameSize);
    
    Transform transform(frameSize);
    
    float angle =  - M_PI/2.0 + M_PI / 6 * 5;
    cv::Vec3f axis(1, 0, 0);
    cv::Mat rotMat = Rotation::RotVec2RotMat(angle * axis);
    
    const int divNum = 6;
    
    ExtractFeaturePoint efp(frameSize, transform, divNum);
    
    cv::Mat rotImg;
    transform.rotateImg(img, rotImg, rotMat);
    
//    for (int u = 0; u < frameSize.width; u++) {
        
  //      for (int v = 0; v < frameSize.height; v++) {
    for (int v = 0; v < frameSize.height; v++) {
        cv::Vec3b* row = rotImg.ptr<cv::Vec3b>(v);
        
        for (int u = 0; u < frameSize.width; u++) {

                /*
                cv::Vec3b tmpVec = rotImg.at<cv::Vec3b>(v, u);
                uchar R = tmpVec[0];
                uchar G = tmpVec[1];
                uchar B = tmpVec[2];
                
                
                uchar umax = MAX(MAX(R, G), B);
                uchar umin = MIN(MIN(R, G), B);
                
                uchar gray = umax/2 + umin/2;
                
                rotImg.at<cv::Vec3b>(v, u) = cv::Vec3b(gray, gray, gray);
            //} else {
            //    img.at<cv::Vec3b>(v, u) = cv::Vec3b(0, 0, 0);
                 */
            
            if (efp.isInLowLat(Equirect(u, v))) {
                row[u] += cv::Vec3b(100, 100, 0);
            }
        }
    }
    
    cv::Mat rotInvImg;
    transform.rotateImg(rotImg, rotInvImg, rotMat.inv());
    
    //cv::Mat resizeImg;
    //cv::resize(img, resizeImg, frameSizeRe);
    
    cv::namedWindow("rot img");
    cv::imshow("rot img", rotImg);

    cv::namedWindow("rot inv img");
    cv::imshow("rot inv img", rotInvImg);
    
    cv::waitKey(-1);
    
    cv::imwrite(rotColorName, rotImg);
    cv::imwrite(oriColorName, rotInvImg);
    
//    cv::namedWindow("rot");
//    cv::imshow("rot", rotImg);
    
    
    
    return 0;
}
