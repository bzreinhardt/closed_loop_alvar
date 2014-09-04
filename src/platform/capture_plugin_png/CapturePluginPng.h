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

#ifndef CAPTUREPLUGINPNG_H
#define CAPTUREPLUGINPNG_H

/**
 * \file CapturePluginPng.h
 *
 * \brief This file implements a capture plugin based on Png.
 */

#ifdef WIN32
    #ifdef ALVAR_Capture_Plugin_Png_BUILD
        #define ALVAR_CAPTURE_PLUGIN_PNG_EXPORT __declspec(dllexport)
    #else
        #define ALVAR_CAPTURE_PLUGIN_PNG_EXPORT __declspec(dllimport)
    #endif
#else
    #define ALVAR_CAPTURE_PLUGIN_PNG_EXPORT
#endif

#include "Capture.h"
#include "CapturePlugin.h"

#include "highgui.h"
#include <fstream>

namespace alvar {
  
/**
 * \brief Dynamically loaded plugins namespace.
 */
namespace plugins {

/**
 * \brief Implementation of Capture interface for Png plugin.
 */
class ALVAR_CAPTURE_PLUGIN_PNG_EXPORT CapturePng
    : public alvar::Capture
{
public:
    /**
     * \brief Constructor.
     *
     * \param captureDevice Information of which camera to create.
     */
    CapturePng(const CaptureDevice captureDevice);
    /**
     * \brief Destructor.
     */
    ~CapturePng();
    bool start();
    void stop();
    IplImage *captureImage();
    IplImage *captureImage(struct timeval *timestamp);
    bool showSettingsDialog();
	std::string SerializeId();
	bool Serialize(Serialization *serialization);
private:
    IplImage mImage;
    bool mHasTimestamps;
    std::ifstream mTimestamps;
    unsigned int mPicIdx;
    struct timeval mLastTS;
    struct timeval mLastTime;
};

/**
 * \brief Implementation of CapturePlugin interface for Png plugin.
 */
class ALVAR_CAPTURE_PLUGIN_PNG_EXPORT CapturePluginPng
    : public alvar::CapturePlugin
{
public:
    /**
     * \brief Constructor.
     *
     * \param captureType A unique identifier for the capture plugin.
     */
    CapturePluginPng(const std::string &captureType);
    /**
     * \brief Destructor.
     */
    ~CapturePluginPng();
    CaptureDeviceVector enumerateDevices();
    Capture *createCapture(const CaptureDevice captureDevice);
};

extern "C" ALVAR_CAPTURE_PLUGIN_PNG_EXPORT void registerPlugin(const std::string &captureType, alvar::CapturePlugin *&capturePlugin);

} // namespace plugins
} // namespace alvar

#endif
