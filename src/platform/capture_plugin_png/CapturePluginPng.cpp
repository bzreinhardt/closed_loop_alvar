/*
 * This file is part of ALVAR, A Library for Virtual and Augmented Reality.
 *
 * Copyright 2007-2012 VTT Technical Research Centre of Finland
 *
 * Contact: VTT Augmented Reality Team <alvar.info@vtt.fi>
 *          <http://www.vtt.fi/multimedia/alvar.html>
 *
 * ALVAR is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALVAR; if not, see
 * <http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html>.
 */

#include "CapturePluginPng.h"

namespace alvar {
namespace plugins {

bool Timestamp2TV(std::string ts, struct timeval* t) 
{
  std::size_t start = ts.find(",");
  if(start==std::string::npos)
    return false;  
  std::size_t usec = ts.find(".");
  if(usec==std::string::npos)
    return false;  
  t->tv_sec = atol(ts.substr(start+1,usec-1).c_str());
  t->tv_usec = atol(ts.substr(usec+1,ts.size()-1).c_str());
  return true;
}

CapturePng::CapturePng(const CaptureDevice captureDevice)
    : Capture(captureDevice)
    , mImage()
    , mHasTimestamps(false)
    , mTimestamps()
    , mPicIdx(0)
{
  mLastTS.tv_sec = 0;
  mLastTS.tv_usec = 0;
  mLastTime.tv_sec = 0;
  mLastTime.tv_usec = 0;
}

CapturePng::~CapturePng()
{
    stop();
}

bool CapturePng::start()
{
    if (isCapturing()) {
        return isCapturing();
    }

    std::string picPath = captureDevice().id() + std::string("/m000000000.png");
    IplImage* pic = cvLoadImage(picPath.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
    if(pic != NULL) {
      mXResolution = (int)pic->width;
      mYResolution = (int)pic->height;
      mIsCapturing = true;
      std::string ts = captureDevice().id() + "/timestamps.txt"; 
      try {
        mTimestamps.open(ts.c_str());
        mHasTimestamps = true;
      } catch (std::ifstream::failure e) {
        mHasTimestamps = false;
      }
    }
    
    return isCapturing();
}

void CapturePng::stop()
{
    if (isCapturing()) {
        if(mHasTimestamps)
          mTimestamps.close();
        mIsCapturing = false;
    }
}

IplImage *CapturePng::captureImage()
{
    if (!isCapturing()) {
        return NULL;
    }

    std::stringstream snum;
    snum << std::setfill('0') << std::setw(9) << mPicIdx;
    std::string picPath = captureDevice().id() + "/m" + snum.str() + ".png";
    IplImage* pic = cvLoadImage(picPath.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
    mPicIdx++;

    if(pic != NULL) {
      mImage = *pic;
    } else {
      mIsCapturing = false;
      return NULL;
    }
     
    return &mImage;
}

IplImage *CapturePng::captureImage(struct timeval* timestamp)
{
    if (!isCapturing()) {
        return NULL;
    }
    std::stringstream snum;
    snum << std::setfill('0') << std::setw(9) << mPicIdx;
    std::string picPath = captureDevice().id() + "/m" + snum.str() + ".png";
    IplImage* pic = cvLoadImage(picPath.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
    mPicIdx++;

    struct timeval curTime;
    gettimeofday(&curTime,NULL);
    if(pic != NULL) {
      mImage = *pic;
      if(mHasTimestamps) {
        std::string ts;
        std::getline(mTimestamps, ts);
        if(Timestamp2TV(ts,timestamp)) {
          if(mLastTime.tv_sec != 0 ) 
          {
            useconds_t rt = (curTime.tv_sec - mLastTime.tv_sec)*1000000 + (curTime.tv_usec - mLastTime.tv_usec); 
            useconds_t slp = (timestamp->tv_sec - mLastTS.tv_sec)*1000000 + (timestamp->tv_usec - mLastTS.tv_usec); 
            if(slp > rt) {
              slp = slp - rt;
              usleep(slp);
            }
          }
          mLastTS = *timestamp;
          gettimeofday(timestamp,NULL);
        } else {
          gettimeofday(timestamp,NULL);
        }
      }
    } else {
      mIsCapturing = false;
      return NULL;
    }
    
    mLastTime = curTime; 
    return &mImage;
}

bool CapturePng::showSettingsDialog()
{
    // TODO: implement this method
    return false;
}

std::string CapturePng::SerializeId()
{
    return "CapturePng";
}

bool CapturePng::Serialize(Serialization *serialization)
{
    return false;
}

CapturePluginPng::CapturePluginPng(const std::string &captureType)
    : CapturePlugin(captureType)
{
}

CapturePluginPng::~CapturePluginPng()
{
}

CapturePlugin::CaptureDeviceVector CapturePluginPng::enumerateDevices()
{
    CaptureDeviceVector devices;
    return devices;
}

Capture *CapturePluginPng::createCapture(const CaptureDevice captureDevice)
{
    return new CapturePng(captureDevice);
}

void registerPlugin(const std::string &captureType, alvar::CapturePlugin *&capturePlugin)
{
    capturePlugin = new CapturePluginPng(captureType);
}

} // namespace plugins
} // namespace alvar
