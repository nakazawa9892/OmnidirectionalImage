//
//  CalcOpticalFlow.cpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/10/27.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#include "CalcOpticalFlow.hpp"

void CalcOpticalFlow::getNormalPair
(const cv::Mat &forPersImg, const cv::Mat &latPersImg,
 const cv::Mat &froChgMat, std::vector<cv::Point2f> &forNormals,
 std::vector<cv::Point2f> &latNormals) const
{
    // 特徴点を一時的に格納
    std::vector<cv::Point2f> forNormalsTmp, latNormalsTmp;
    
    // 正面の特徴点
    bool isFront = true;
    getNormalPairOneDir
    (forPersImg, latPersImg, froChgMat, isFront,
     forNormalsTmp, latNormalsTmp);
    
    // コピー
    std::copy
    (forNormalsTmp.begin(), forNormalsTmp.end(),
     std::back_inserter(forNormals));
    std::copy
    (latNormalsTmp.begin(), latNormalsTmp.end(),
     std::back_inserter(latNormals));
    
    /*
     // 背面の特徴点
     isFront = false;
     getNormalPairOneDir
     (forImg, latImg, rotMat, forNormalsTmp, latNormalsTmp, isFront);
     
     // コピー
     std::copy
     (forNormalsTmp.begin(), forNormalsTmp.end(),
     std::back_inserter(forNormals));
     std::copy
     (latNormalsTmp.begin(), latNormalsTmp.end(),
     std::back_inserter(latNormals));
     */
}

void CalcOpticalFlow::getNormalPairOneDir
(const cv::Mat &forPersImg, const cv::Mat &latPersImg,
 const cv::Mat &froChgMat, const bool isFront,
 std::vector<cv::Point2f> &forNormals,
 std::vector<cv::Point2f> &latNormals) const
{
    // オプティカルフローを抽出
    std::vector<cv::Point2f> forPerss, latPerss;
    getOpticalFlow(forPersImg, latPersImg, forPerss, latPerss);
    
    
}

void CalcOpticalFlow::getOpticalFlow
(const cv::Mat &forPersImg, const cv::Mat &latPersImg,
 std::vector<cv::Point2f> &forPerss,
 std::vector<cv::Point2f> &latPerss) const
{
    // グレースケールに変換
    cv::Mat forGrayPersImg;
    cv::cvtColor(forPersImg, forGrayPersImg, CV_BGR2GRAY);
    
    // 前透視投影画像からトラッキング用の特徴点を抽出
    cv::goodFeaturesToTrack
    (forGrayPersImg, forPerss, 300, 0.01, 2, optflowMask);
    
    // グレースケールに変換
    cv::Mat latGrayPersImg;
    cv::cvtColor(latPersImg, latGrayPersImg, CV_BGR2GRAY);
    
    // オプティカルフローを計算
    std::vector<uchar> statuss;
    std::vector<float> errors;
    cv::calcOpticalFlowPyrLK
    (forGrayPersImg, latGrayPersImg, forPerss, latPerss, statuss, errors);
    
    // エラーを取り除く
    for (int i = 0; i < forPerss.size(); ) {
        if (statuss[i] == 0) {
            forPerss.erase(forPerss.begin() + i);
            latPerss.erase(latPerss.begin() + i);
            statuss.erase(statuss.begin() + i);
        } else {
            i++;
        }
    }
}

void CalcOpticalFlow::remOrthOutlier
(std::vector<cv::Point2f> &forNormals,
 std::vector<cv::Point2f> &latNormals) const
{
    for (int i = 0; i < forNormals.size(); ) {
        if (!isOrthCond(forNormals[i], latNormals[i])) {
            forNormals.erase(forNormals.begin() + i);
            latNormals.erase(latNormals.begin() + i);
        } else {
            i++;
        }
    }
}
