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

#include "CapturePluginFile.h"

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

CaptureFile::CaptureFile(const CaptureDevice captureDevice)
    : Capture(captureDevice)
    , mVideoCapture()
    , mMatrix()
    , mImage()
    , mHasTimestamps(false)
{
  mLastTS.tv_sec = 0;
  mLastTS.tv_usec = 0;
  mLastTime.tv_sec = 0;
  mLastTime.tv_usec = 0;
 
}

CaptureFile::~CaptureFile()
{
    stop();
}

bool CaptureFile::start()
{
    if (isCapturing()) {
        return isCapturing();
    }
    
    //find video directory
    std::size_t lPos=0;
    std::size_t pos=0;
    bool first = true;
    while(pos != std::string::npos) {
      lPos = pos;
      if(first) {
        pos = captureDevice().id().find("/",lPos);
        first = false;
      } else 
        pos = captureDevice().id().find("/",lPos+1);
    }
    std::string tsFile;
    if(lPos == 0) {
      tsFile = "./timestamps.txt";
    } else {
      tsFile = captureDevice().id().substr(0,lPos) + std::string("/timestamps.txt");
      try {
        mTimestamps.open(tsFile.c_str());
        mHasTimestamps = true;
      } catch (std::ifstream::failure e) {
        mHasTimestamps = false;
      }
    }

    mVideoCapture.open(captureDevice().id().c_str());
    if (mVideoCapture.isOpened()) {
        mXResolution = (int)mVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
        mYResolution = (int)mVideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
        mIsCapturing = true;
    }

    return isCapturing();
}

void CaptureFile::stop()
{
    if (isCapturing()) {
        mVideoCapture.release();
        mIsCapturing = false;
    }
}

IplImage *CaptureFile::captureImage()
{
    if (!isCapturing()) {
        return NULL;
    }

    if (!mVideoCapture.grab()) {
        // try to restart the capturing when end of file is reached
        mVideoCapture.release();
        mVideoCapture.open(captureDevice().id().c_str());
        if (!mVideoCapture.isOpened()) {
            mIsCapturing = false;
            return NULL;
        }
        if (!mVideoCapture.grab()) {
            return NULL;
        }
    }
    mVideoCapture.retrieve(mMatrix);
    mImage = mMatrix;
    return &mImage;
}

IplImage *CaptureFile::captureImage(struct timeval* timestamp)
{
    if (!isCapturing()) {
        return NULL;
    }

    if (!mVideoCapture.grab()) {
        // try to restart the capturing when end of file is reached
        mVideoCapture.release();
        mVideoCapture.open(captureDevice().id().c_str());
        if (!mVideoCapture.isOpened()) {
            mIsCapturing = false;
            return NULL;
        }
        if (!mVideoCapture.grab()) {
            return NULL;
        }
    }
    struct timeval curTime;
    gettimeofday(&curTime,NULL);
    if(mHasTimestamps) {
      std::string ts;
      std::getline(mTimestamps, ts);
      if(Timestamp2TV(ts,timestamp)) {
        if(mLastTS.tv_sec != 0 )  
        {
          useconds_t real = (curTime.tv_sec - mLastTime.tv_sec)*1000000 + (curTime.tv_usec - mLastTime.tv_usec);
          useconds_t slp = (timestamp->tv_sec - mLastTS.tv_sec)*1000000 + (timestamp->tv_usec - mLastTS.tv_usec);
          if(slp > real) {
            slp = slp - real; 
            usleep(slp);
          }
        }
        mLastTS = *timestamp;
        gettimeofday(timestamp,NULL);
      } else {
        gettimeofday(timestamp,NULL);
      }
    }
    mVideoCapture.retrieve(mMatrix);
    mImage = mMatrix;
    mLastTime = curTime;
    return &mImage;
}

bool CaptureFile::showSettingsDialog()
{
    // TODO: implement this method
    return false;
}

std::string CaptureFile::SerializeId()
{
    return "CaptureFile";
}

bool CaptureFile::Serialize(Serialization *serialization)
{
    return false;
}

CapturePluginFile::CapturePluginFile(const std::string &captureType)
    : CapturePlugin(captureType)
{
}

CapturePluginFile::~CapturePluginFile()
{
}

CapturePlugin::CaptureDeviceVector CapturePluginFile::enumerateDevices()
{
    CaptureDeviceVector devices;
    return devices;
}

Capture *CapturePluginFile::createCapture(const CaptureDevice captureDevice)
{
    return new CaptureFile(captureDevice);
}

void registerPlugin(const std::string &captureType, alvar::CapturePlugin *&capturePlugin)
{
    capturePlugin = new CapturePluginFile(captureType);
}

} // namespace plugins
} // namespace alvar
