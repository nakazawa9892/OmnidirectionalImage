//
//  IMUProcess.cpp
//  OmnidirectionalImage
//
//  Created by masakazu nakazawa on 2016/11/22.
//  Copyright © 2016年 masakazu. All rights reserved.
//

#include "IMUProcess.hpp"

IMUProcess::IMUProcess(IMU& imu):
imu(imu), quatCur(1, 0, 0, 0), matCur(cv::Mat::eye(3, 3, CV_32FC1))
{
    
}

bool IMUProcess::isMagZero(const IMUData &data)
{
    if (data.mag_x==0 && data.mag_y==0 && data.mag_z==0) {
        return true;
    } else {
        return false;
    }
}

void IMUProcess::renewPose(const IMUData &data)
{
    // コンパスが０でないならデータは正しくない
    if (!isMagZero(data)) return;
    
    // 角速度ベクトル
    cv::Vec3f angVel(IMU::getGyroValue(filterGyroDrift(data.gyro_x)),
                     IMU::getGyroValue(filterGyroDrift(data.gyro_y)),
                     IMU::getGyroValue(filterGyroDrift(data.gyro_z)));
    
    // 単位行列 + 無限小回転行列
    cv::Mat infEyeMat = cv::Mat::eye(3, 3, CV_32FC1) +
                            Rotation::getInfRot(angVel);
    
    // 更新
    matCur = infEyeMat * matCur;
}

void IMUProcess::printCurPose()
{
    std::cout << matCur << std::endl;
}
