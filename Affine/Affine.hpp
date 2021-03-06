//
//  Affine.hpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/06/09.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#ifndef Affine_hpp
#define Affine_hpp

#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "Transform.hpp"

class Affine
{
public:
    Affine(const Transform& transform);
    ~Affine();
    
    void estimate3DRotMat
    (std::vector<cv::Point3f>& forPoints, std::vector<cv::Point3f>& latPoints,
     cv::Mat& estRotMat) const;
    // アフィン行列から回転行列を取り出す
    void extractRotMatFromAffineMat
    (const cv::Mat& affMat, cv::Mat& rotMat) const;
    // アフィン行列を推定する
    void estimate3DAffineMat
    (std::vector<cv::Point3f>& forPoints, std::vector<cv::Point3f>& latPoints,
     cv::Mat& affMat) const;
    // 誤差の乗った回転行列をシュミッド直交化法で正規化
    void normalizeRotMat(cv::Mat& rotMat) const;
    
    // 特異値分解により回転行列を推定する [forP] = [R]*[latP]
    void estimate3DRotMatSVD
    (std::vector<cv::Point3f>& forPoints, std::vector<cv::Point3f>& latPoints,
     cv::Mat& estRotMat) const;
    // ランダムに点を取り出す
    void getRondomPoint
    (const std::vector<cv::Point3f>& forPoints,
     const std::vector<cv::Point3f>& latPoints,
     std::vector<cv::Point3f>& forPointx, std::vector<cv::Point3f>& latPointx,
     size_t getSize) const;
    // ランダムに取り出した点について回転行列を推定
    void estimate3DRotMatSVDPartial
    (const std::vector<cv::Point3f>& forPointx,
     const std::vector<cv::Point3f>& latPointx, cv::Mat& estRotMatPart) const;
    // 推定した回転行列を評価
    float evalEstRotMat
    (const std::vector<cv::Point3f>& forPoints,
     const std::vector<cv::Point3f>& latPoints,
     const cv::Mat& estRotMat) const;
    // 閾値以下のサンプルを取り除く
    void removeOutlier
    (const std::vector<cv::Point3f>& forPoints,
     const std::vector<cv::Point3f>& latPoints,
     std::vector<cv::Point3f>& inForPoints,
     std::vector<cv::Point3f>& inLatPoints,
     float normThre, const cv::Mat& estRotMat) const;
    
private:
    const Transform& transform;
};

#endif /* Affine_hpp */
