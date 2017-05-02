/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#ifndef __EASYAR_SAMPLE_UTILITY_AR_H__
#define __EASYAR_SAMPLE_UTILITY_AR_H__

#include "camera.hpp"
#include "imagetracker.hpp"
#include "augmenter.hpp"
#include "imagetarget.hpp"
#include "frame.hpp"
#include "utility.hpp"
#include <string>

namespace EasyAR{
namespace samples{

class AR
{
public:
    AR();
    virtual ~AR();
    virtual bool initCamera();
    virtual void loadFromImage(const std::string& path);
    virtual void loadFromJsonFile(const std::string& path, const std::string& targetname);
    virtual void loadAllFromJsonFile(const std::string& path);
    virtual void loadFromAbsolutePath(const std::string&path, const std::string& targetname);
    virtual bool start();
    virtual bool stop();
    virtual bool clear();

    virtual void initGL();
    virtual void resizeGL(int width, int height);
    virtual void render();
    void setPortrait(bool portrait);
protected:
    CameraDevice camera_;
    ImageTracker tracker_;
    Augmenter augmenter_;
    bool portrait_;
    Vec4I viewport_;
};

}
}
#endif
