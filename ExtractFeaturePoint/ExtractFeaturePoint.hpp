//
//  ExtractFeaturePoint.hpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/05/25.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#ifndef ExtractFeaturePoint_hpp
#define ExtractFeaturePoint_hpp

#include <stdio.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include "Rotate.hpp"
//#include "Transform.hpp"
#include "Core.hpp"

class ExtractFeaturePoint
{
public:
    ExtractFeaturePoint(const cv::Size& fs, const int divNum, const int mergin);
    
    /*
    // 低緯度の矩形部分の特徴点を抽出
    void extractRectRoiFeaturePoint
    (const cv::Mat& img, std::vector<cv::KeyPoint>& keyPoints,
     cv::Mat& descriptors) const;
     */
    
    // 全天球画像全体から特徴を抽出（比較用）
    void extFeatSimp
    (const cv::Mat& img, std::vector<cv::KeyPoint>& keyPoints,
     cv::Mat& descs) const;
    
    // 基準画像を回転させながらすべての領域の特徴を抽出
    void extFeat
    (const cv::Mat& img, std::vector<cv::KeyPoint>& keyPoints,
     cv::Mat& descs) const;
    
    // numberで指定された部分の特徴点を抽出（基準画像における座標）
    void extFeatRoi
    (const cv::Mat& img, const int number,
     std::vector<cv::KeyPoint>& roiKeyPoints, cv::Mat& roiDescriptors) const;
    
    // 回転前の座標の特徴点から回転後の座標の特徴点に変換
    void rotKeyPointCrd
    (std::vector<cv::KeyPoint>& keyPoints, const float angle) const;
    
    // ROIの座標から大域の座標に変換
    void getGlobalCrd(std::vector<cv::KeyPoint>& keyPoints) const {
        for (int i = 0; i < keyPoints.size(); i++)
            keyPoints[i].pt.y += roi.y;
    };
    
    // 特徴点を統合
    static void keyPointConcat
    (const std::vector<cv::KeyPoint>& src, std::vector<cv::KeyPoint>& dest) {
        std::copy(src.begin(), src.end(), std::back_inserter(dest));
    };
    
    // 記述子を統合
    static void descConcat(const cv::Mat& src, cv::Mat& dest) {
        cv::vconcat(dest, src, dest);
    };
    
    // 有効範囲にある特徴点のみを取り出す
    void filterLowLat
    (std::vector<cv::KeyPoint>& keyPoints, cv::Mat& descriptors) const;
    
    // 特徴点が有効範囲にあるか
    bool isInLowLat(const Equirect& equirect) const {
        Polar polar = Map::equirect2polar(equirect);
        
        float theta = polar.x;
        float phi = polar.y;
        
        if (cosf(theta) == 0.0) return false;
        float latitude = atanf(-tanf(phi)/cosf(theta));
        
        float latThre = M_PI / (divNum*2.0);
        
        if (std::abs(latitude) < latThre) return true;
        else return false;
    }
    
    // 特徴点の描写
    void drawKeyPoint
    (const cv::Mat& img, const std::vector<cv::KeyPoint>& keyPoints,
     cv::Mat& outImg) const;
    
    // 特徴点の描写（わかりやすく）
    void drawKeyPointClear
    (const cv::Mat& img, const std::vector<cv::KeyPoint>& keyPoints,
     cv::Mat& outImg);
    
    // 特徴抽出範囲のマスク
    void setLowLatMask(const cv::Size& fs);

private:
    const cv::Ptr<cv::Feature2D> feature; // 特徴クラス

    const int divNum;   // 全天球画像の分割数
    const cv::Rect roi; // 各球面回転画像の特徴抽出領域(矩形)
    
    cv::Mat mask;       // マスクを使って特徴抽出する際に必要
};

#endif /* ExtractFeaturePoint_hpp */
