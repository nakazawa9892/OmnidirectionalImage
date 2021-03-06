//
//  FeatureMatchEstimator.hpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/10/27.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#ifndef FeatureMatchEstimator_hpp
#define FeatureMatchEstimator_hpp

#include <stdio.h>

#include <opencv2/core.hpp>
#include <sys/time.h>

#include "Estimator.hpp"
#include "ExtractFeaturePoint.hpp"
#include "MatchFeaturePoint.hpp"
#include "Range.hpp"
#include "Estimate.hpp"
#include "Epipolar.hpp"
#include "Core.hpp"

class FeatureMatchEstimator: public Estimator {
public:
    FeatureMatchEstimator
    (const ExtractFeaturePoint& efp, const MatchFeaturePoint& mfp,
     const Epipolar& epi, const Range& ran);
    
    // インタフェース
    cv::Mat getRotMat
    (const cv::Mat& forImg, const cv::Mat& latImg, const State& state);
    
    // 実体
    cv::Mat getRotMat
    (const cv::Mat& forImg, const cv::Mat& latImg, const int frameNum);
    
    // 指定した方向の特徴点で回転行列を推定
    cv::Mat getRotMatSpecDir
    (const std::vector<Sphere>& forSpheres,
     const std::vector<Sphere>& latSpheres,
     const cv::Mat& froChgMat, float &weight) const;
    
    // 最大重みの回転行列
    cv::Mat getRotMatWeightMax
    (const std::vector<Sphere>& forSpheres,
     const std::vector<Sphere>& latSpheres, int& maxIdx) const;
    
private:
    const ExtractFeaturePoint& efp;
    const MatchFeaturePoint& mfp;
    const Epipolar& epi;
    const Range& ran;
    
    // カメラ座標系変更のための行列
    std::vector<cv::Mat> froChgMats;
    
    int prevFrameNum; // 直前の特徴マッチ法で処理したフレーム番号
    std::vector<cv::KeyPoint> curKeyPointSet; // 直前フレームの特徴点
    cv::Mat curDescSet; // 直前フレームの特徴量
};

#endif /* FeatureMatchEstimator_hpp */
