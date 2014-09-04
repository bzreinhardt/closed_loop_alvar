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

#include "CapturePluginPtgrey.h"
#include <flycapture/Camera.h>
#include <flycapture/Image.h>
#include <flycapture/Error.h>
#include <flycapture/BusManager.h>

#include <sstream>

using namespace std;

namespace alvar {
namespace plugins {

std::string DecoderRing(int code) {
  switch(code) {
    case FlyCapture2::VIDEOMODE_160x120YUV444:
     return "VIDEOMODE_160x120YUV444";
    case FlyCapture2::VIDEOMODE_320x240YUV422:
      return "VIDEOMODE_320x240YUV422";
    case FlyCapture2::VIDEOMODE_640x480YUV411:
      return "VIDEOMODE_640x480YUV411";
    case FlyCapture2::VIDEOMODE_640x480YUV422:
      return "VIDEOMODE_640x480YUV422";
    case FlyCapture2::VIDEOMODE_640x480RGB:
      return "VIDEOMODE_640x480RGB";
    case FlyCapture2::VIDEOMODE_640x480Y8:
      return "VIDEOMODE_640x480Y8";
    case FlyCapture2::VIDEOMODE_640x480Y16:
      return "VIDEOMODE_640x480Y16";
    case FlyCapture2::VIDEOMODE_800x600YUV422:
      return "VIDEOMODE_800x600YUV422";
    case FlyCapture2::VIDEOMODE_800x600RGB:
      return "VIDEOMODE_800x600RGB";
    case FlyCapture2::VIDEOMODE_800x600Y8:
      return "VIDEOMODE_800x600Y8";
    case FlyCapture2::VIDEOMODE_800x600Y16:
      return "VIDEOMODE_800x600Y16";
    case FlyCapture2::VIDEOMODE_1024x768YUV422:
      return "VIDEOMODE_1024x768YUV422"; 
    case FlyCapture2::VIDEOMODE_1024x768RGB:
      return "VIDEOMODE_1024x768RGB";
    case FlyCapture2::VIDEOMODE_1024x768Y8:
      return "VIDEOMODE_1024x768Y8";
    case FlyCapture2::VIDEOMODE_1024x768Y16:
      return "VIDEOMODE_1024x768Y16";
    case FlyCapture2::VIDEOMODE_1280x960YUV422:
      return "VIDEOMODE_1280x960YUV422";
    case FlyCapture2::VIDEOMODE_1280x960RGB:
      return "VIDEOMODE_1280x960RGB";
    case FlyCapture2::VIDEOMODE_1280x960Y8:
      return "VIDEOMODE_1280x960Y8";
    case FlyCapture2::VIDEOMODE_1280x960Y16:
      return "VIDEOMODE_1280x960Y16";
    case FlyCapture2::VIDEOMODE_1600x1200YUV422:
      return "VIDEOMODE_1600x1200YUV422";
    case FlyCapture2::VIDEOMODE_1600x1200RGB:
      return "VIDEOMODE_1600x1200RGB";
    case FlyCapture2::VIDEOMODE_1600x1200Y8:
     return "VIDEOMODE_1600x1200Y8";
    case FlyCapture2::VIDEOMODE_1600x1200Y16:
     return "VIDEOMODE_1600x1200Y16";
    case FlyCapture2::VIDEOMODE_FORMAT7:
     return "VIDEOMODE_FORMAT7";
    case FlyCapture2::NUM_VIDEOMODES:
      return "NUM_VIDEOMODES";
    case FlyCapture2::VIDEOMODE_FORCE_32BITS:
      return "VIDEOMODE_FORCE_32BITS";
    default:
      return "UNKNOWN";
  }
}

CapturePtgrey::CapturePtgrey(const CaptureDevice captureDevice)
    : Capture(captureDevice)
    , mCamera(new FlyCapture2::Camera)
    , mImage(new FlyCapture2::Image)
    , mChannels(-1)
    , mReturnFrame(NULL)
{
}

CapturePtgrey::~CapturePtgrey()
{
    stop();
    delete mCamera;
    delete mImage;
}

void CapturePtgrey::setResolution(const unsigned long xResolution, const unsigned long yResolution) {

    std::cout << "Set resolution to " << xResolution << "x" << yResolution << std::endl;
    FlyCapture2::Format7Info   info;
    bool supported;
    bool wasCapturing;
    if (isCapturing()) {
      wasCapturing = true;
    } else {
      start();
    }
    if (mCamera->GetFormat7Info (&info, &supported) != FlyCapture2::PGRERROR_OK) {
          std::cout << "CapturePtgrey: Can't get format information" << std::endl;
          return;
    }
    mCamera->StopCapture();
    

    FlyCapture2::Format7ImageSettings settings;
    unsigned int packetSize;

    if(xResolution > info.maxWidth) {
      std::cout << "CapturePtgrey: Width is too big. Setting to " << info.maxWidth << std::endl;
      settings.width = info.maxWidth;
    } else {
      settings.width = xResolution;
    }
    if(yResolution > info.maxHeight) {
      std::cout << "CapturePtgrey: Height is too big. Setting to " << info.maxHeight << std::endl;
      settings.height = info.maxHeight;
    } else {
      settings.height = yResolution;
    }

    settings.mode = FlyCapture2::MODE_0;
    settings.offsetX = (info.maxWidth-xResolution)/2;
    settings.offsetY = (info.maxHeight-yResolution)/2;
    settings.pixelFormat = FlyCapture2::PIXEL_FORMAT_MONO8;
    bool valid;
    FlyCapture2::Format7PacketInfo pInfo;
    if (mCamera->ValidateFormat7Settings (&settings, &valid, &pInfo) != FlyCapture2::PGRERROR_OK) {
          std::cout << "CapturePtgrey: Invalid settings" << std::endl;
          return;
    }
    if(!valid) {
       std::cout << "CapturePtgrey: Invalid settings" << std::endl;
       return;
    }
    packetSize = pInfo.maxBytesPerPacket; 
    if (mCamera->SetFormat7Configuration (&settings, packetSize) != FlyCapture2::PGRERROR_OK) {
          std::cout << "CapturePtgrey: Configuration failed packetSize " << packetSize << std::endl;
          return;
    }
    mXResolution = xResolution;
    mYResolution = yResolution;
    if(wasCapturing) {
      stop();
      start();
    } else {
      stop();
    }
}

bool CapturePtgrey::start()
{
    if (isCapturing()) {
        return isCapturing();
    }
    
    stringstream id(captureDevice().id());
    id.setf(ios_base::hex, ios_base::basefield);
    id >> mGUID.value[0]; id.get();
    id >> mGUID.value[1]; id.get();
    id >> mGUID.value[2]; id.get();
    id >> mGUID.value[3];

    if (mCamera->Connect(&mGUID) != FlyCapture2::PGRERROR_OK) {
         std::cout << "PGRERROR" << std::endl;
        return false;
    }

    FlyCapture2::VideoMode videoMode;
    FlyCapture2::FrameRate frameRate;
    if (mCamera->GetVideoModeAndFrameRate (&videoMode, &frameRate) != FlyCapture2::PGRERROR_OK) {
        return false;
    }
    
    std::cout << "Video Mode: " << DecoderRing(videoMode) << std::endl;
    if (videoMode == FlyCapture2::VIDEOMODE_640x480RGB) {
      mChannels = 3;
      mXResolution = 640;
      mYResolution = 480;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_640x480Y8) {
      mChannels = 1;
      mXResolution = 640;
      mYResolution = 480;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_800x600RGB) {
      mChannels = 3;
      mXResolution = 800;
      mYResolution = 600;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_800x600Y8) {
      mChannels = 1;
      mXResolution = 800;
      mYResolution = 600;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1024x768RGB) {
      mChannels = 3;
      mXResolution = 1024;
      mYResolution = 768;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1024x768Y8) {
      mChannels = 1;
      mXResolution = 1024;
      mYResolution = 768;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1280x960RGB) {
      mChannels = 3;
      mXResolution = 1280;
      mYResolution = 960;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1280x960Y8) {
      mChannels = 1;
      mXResolution = 1280;
      mYResolution = 960;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1600x1200RGB) {
      mChannels = 3;
      mXResolution = 1600;
      mYResolution = 1200;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_1600x1200Y8) {
      mChannels = 1;
      mXResolution = 1600;
      mYResolution = 1200;
    
    } else if (videoMode == FlyCapture2::VIDEOMODE_FORMAT7) {
        FlyCapture2::Format7ImageSettings settings;
        unsigned int packetSize;
        float percentage;

        if (mCamera->GetFormat7Configuration (&settings, &packetSize, &percentage) != FlyCapture2::PGRERROR_OK) {
          return false;
        }

        switch(settings.pixelFormat) {
          case FlyCapture2::PIXEL_FORMAT_MONO8:
            mChannels = 1;
            break;
          case FlyCapture2::PIXEL_FORMAT_411YUV8:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_422YUV8:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_444YUV8:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_RGB8:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_MONO16:
            mChannels = 1;
            break;
          case FlyCapture2::PIXEL_FORMAT_RAW8:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_RAW16:
            mChannels = 3;
            break;
          case FlyCapture2::PIXEL_FORMAT_MONO12:
            mChannels = 1;
            break;
          case FlyCapture2::PIXEL_FORMAT_RAW12:
            mChannels = 3;
            break;
          default:
            mChannels = 1;
        }
        mXResolution = settings.width;
        mYResolution = settings.height;
    } else {
        return false;
    }

    mReturnFrame = cvCreateImage(cvSize(mXResolution, mYResolution), IPL_DEPTH_8U, mChannels);
    if (mCamera->StartCapture() != FlyCapture2::PGRERROR_OK) {
        std::cout << "Could not start capture" << std::endl;
        return false;
    }
    std::cout << "Started capture" << std::endl;
    mIsCapturing = true;
    return isCapturing();
}

void CapturePtgrey::stop()
{
    if (isCapturing()) {
        mCamera->StopCapture();
        cvReleaseImage(&mReturnFrame);
        mIsCapturing = false;
    }
}

IplImage *CapturePtgrey::captureImage()
{
    if (!isCapturing()) {
        return NULL;
    }

    if (mCamera->RetrieveBuffer(mImage) == FlyCapture2::PGRERROR_OK) {
        unsigned long length = mReturnFrame->widthStep * mYResolution;
        memcpy(mReturnFrame->imageData, mImage->GetData(), length);
    }
    return mReturnFrame;
}

IplImage *CapturePtgrey::captureImage(struct timeval* timestamp)
{
    if (!isCapturing()) {
        return NULL;
    }

    if (mCamera->RetrieveBuffer(mImage) == FlyCapture2::PGRERROR_OK) {
        gettimeofday(timestamp,NULL);
        unsigned long length = mReturnFrame->widthStep * mYResolution;
        memcpy(mReturnFrame->imageData, mImage->GetData(), length);
    }
    return mReturnFrame;
}

bool CapturePtgrey::showSettingsDialog()
{
    return false;
}

string CapturePtgrey::SerializeId()
{
    return "CapturePtgrey";
}

bool CapturePtgrey::Serialize(Serialization *serialization)
{
    return false;
}

CapturePluginPtgrey::CapturePluginPtgrey(const string &captureType)
    : CapturePlugin(captureType)
{
}

CapturePluginPtgrey::~CapturePluginPtgrey()
{
}

CapturePlugin::CaptureDeviceVector CapturePluginPtgrey::enumerateDevices()
{
    CaptureDeviceVector devices;

    FlyCapture2::BusManager bus;
    if (bus.RescanBus() != FlyCapture2::PGRERROR_OK) {
        return devices;
    }

    unsigned int numberCameras = 0;
    bus.GetNumOfCameras(&numberCameras);
    
    for (unsigned int i = 0; i < numberCameras; i++) {
        FlyCapture2::PGRGuid guid;
        bus.GetCameraFromIndex(i, &guid);
        stringstream convert;
        convert << hex << guid.value[0];
        convert << "_" << hex << guid.value[1];
        convert << "_" << hex << guid.value[2];
        convert << "_" << hex << guid.value[3];
        stringstream description;
        FlyCapture2::Camera camera;
        if (camera.Connect(&guid) != FlyCapture2::PGRERROR_OK) continue;
        FlyCapture2::CameraInfo info;
        if (camera.GetCameraInfo (&info) != FlyCapture2::PGRERROR_OK) continue;
        description << info.vendorName << " ";
        description << info.modelName;
        CaptureDevice captureDevice(mCaptureType, convert.str(), description.str());
        devices.push_back(captureDevice);
    }

    std::cout << "Enumerated devices" << std::endl;
    return devices;
}

Capture *CapturePluginPtgrey::createCapture(const CaptureDevice captureDevice)
{
    std::cout << "Create Capture" << std::endl;
    return new CapturePtgrey(captureDevice);
    std::cout << "Created Capture" << std::endl;
}

void registerPlugin(const string &captureType, alvar::CapturePlugin *&capturePlugin)
{
    std::cout << "Register Capture Plugin" << std::endl;
    capturePlugin = new CapturePluginPtgrey(captureType);
    std::cout << "Registered Capture Plugin" << std::endl;
}

} // namespace plugins
} // namespace alvar
