//
//  MatchFeaturePoint.cpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/05/27.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#include "MatchFeaturePoint.hpp"

MatchFeaturePoint::MatchFeaturePoint
(const Transform& transform, const float distThreshold,
 const float coordThreshold):
transform(transform),
distThreshold(distThreshold), coordThreshold(coordThreshold),
matcher(cv::DescriptorMatcher::create("BruteForce"))
{
}

void MatchFeaturePoint::match
(const cv::Mat descriptors1, const cv::Mat descriptors2,
 std::vector<cv::DMatch> &dMatches) const
{
    std::vector<cv::DMatch> dMatches1, dMatches2;
    
    matcher->match(descriptors1, descriptors2, dMatches1);
    matcher->match(descriptors2, descriptors1, dMatches2);
    
    crossMatch(dMatches1, dMatches2, dMatches);
}

void MatchFeaturePoint::crossMatch
(const std::vector<cv::DMatch> &dMatches1,
 const std::vector<cv::DMatch> &dMatches2,
 std::vector<cv::DMatch> &dMatches)
{
    dMatches.clear();
    
    for (int i = 0; i < dMatches1.size(); i++) {
        cv::DMatch forward = dMatches1[i];
        cv::DMatch backward = dMatches2[forward.trainIdx];
        
        if (forward.queryIdx == backward.trainIdx)
            dMatches.push_back(forward);
    }
}

void MatchFeaturePoint::filterMatchDistance
(std::vector<cv::DMatch> &dMatches) const
{
    // .eraseに注意
    for (int i = 0; i < dMatches.size();  ) {
        if (dMatches[i].distance > distThreshold)
            dMatches.erase(dMatches.begin() + i);
        else
            i++;
    }
}

void MatchFeaturePoint::filterCoordDistance
(std::vector<Sphere> &forSpheres, std::vector<Sphere> &latSpheres) const
{
    // .eraseに注意
    for (int i = 0; i < forSpheres.size(); ) {
        if (cv::norm(forSpheres[i] - latSpheres[i]) > coordThreshold) {
            forSpheres.erase(forSpheres.begin() + i);
            latSpheres.erase(latSpheres.begin() + i);
        } else {
            i++;
        }
    }
}

void MatchFeaturePoint::drawMatchVert
(const cv::Mat &img1, const std::vector<Equirect> &forEquirects,
 const cv::Mat &img2, const std::vector<Equirect> &latEquirects,
 cv::Mat &outImg)
{
    cv::vconcat(img1, img2, outImg);
    
    for (int i=0; i<forEquirects.size(); i++) {
        cv::Point2f forEquirect = forEquirects[i];
        cv::Point2f latEquirect = latEquirects[i];
        
        latEquirect.y = latEquirect.y + img1.rows;
        
        cv::line
        (outImg, forEquirect, latEquirect,
         cv::Scalar(rand()%256, rand()%256, rand()%256), 2, CV_AA);
    }
}

void MatchFeaturePoint::getMatchKeyPoint
(const std::vector<cv::KeyPoint> &forKeyPointsSet,
 const std::vector<cv::KeyPoint> &latKeyPointsSet,
 const std::vector<cv::DMatch> &matchs,
 std::vector<cv::KeyPoint> &forKeyPoints,
 std::vector<cv::KeyPoint> &latKeyPoints) const
{
    for (int i = 0; i < matchs.size(); i++) {
        forKeyPoints.push_back(forKeyPointsSet[matchs[i].queryIdx]);
        latKeyPoints.push_back(latKeyPointsSet[matchs[i].trainIdx]);
    }
}
