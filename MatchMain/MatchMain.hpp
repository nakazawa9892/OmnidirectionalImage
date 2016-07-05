//
//  MatchMain.hpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/06/15.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#ifndef MatchMain_hpp
#define MatchMain_hpp

#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>

#include "Transform.hpp"
#include "ExtractFeaturePoint.hpp"
#include "MatchFeaturePoint.hpp"
#include "Rotation.hpp"
#include "VideoReader.hpp"
#include "VideoWriter.hpp"

class MatchMain
{
public:
    MatchMain
    (const Transform& transform, const ExtractFeaturePoint& extractFeature,
     const MatchFeaturePoint& matchFeature, const Rotation& rotation);
    ~MatchMain();
    
    // １つ目の画像に合わせて２つ目の画像を修正する
    void ModifylatterImg
    (const cv::Mat& forImg, const cv::Mat& latImg, cv::Mat& modLatImg);
    // 初期フレームに合わせて動画を修正する
    void ModifyVideo(VideoReader& vr, VideoWriter& vw);
    
private:
    const Transform& transform;
    const ExtractFeaturePoint& extractFeature;
    const MatchFeaturePoint& matchFeature;
    const Rotation& rotation;
    cv::Mat accMat;
};

#endif /* MatchMain_hpp */