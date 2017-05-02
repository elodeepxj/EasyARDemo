/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#include "ar.hpp"
#include <algorithm>
#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "EasyAR", __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

namespace EasyAR{
namespace samples{

class HelloCallBack : public TargetLoadCallBack
{
public:
    virtual ~HelloCallBack() {};
    virtual void operator() (const Target target, const bool status)
    {
        LOGI("load target: %s (%d) %s\n", target.name(), target.id(), status ? "success" : "fail");

        delete this;
    }
};

AR::AR()
{
    portrait_ = false;
}

AR::~AR()
{
    clear();
}
//A1初始化相机
bool AR::initCamera()
{
    bool status = true;
    status &= camera_.open();//调用camera.hpp--打开类型为camera的camera设备。在桌面系统中，kDeviceDefault表示默认的camera，而大于0的数值表示设备号码（从1开始编号）这里是0默认
    camera_.setSize(Vec2I(1280, 720));//调用camera.hpp--设置当前图像大小。最接近设置值的可选值将被使用。可以使用size来获取实际的大小。在成功的open之后使用。
    status &= tracker_.attachCamera(camera_);//调用imagetracker.hpp--将CameraDevice连接到ImageTracker。在有CameraDevice连接之前，ImageTracker不会开始运行跟踪。
    status &= augmenter_.attachCamera(camera_);//调用augmenter.hhp--将CameraDevice连接到Augmenter。如果没有CameraDevice的连接，通过newFrame得到的Frame将会是空的。
    return status;
}
//A4加载图片
void AR::loadFromImage(const std::string& path)
{
    ImageTarget target;
    std::string jstr = "{\n"//把图片文件名拼成json字符串
                       "  \"images\" :\n"
                       "  [\n"
                       "    {\n"
                       "      \"image\" : \"" + path + "\",\n"
                       "      \"name\" : \"" + path.substr(0, path.find_first_of(".")) + "\"\n"
                       "    }\n"
                       "  ]\n"
                       "}";
    target.load(jstr.c_str(), EasyAR::kStorageAssets | EasyAR::kStorageJson);
    tracker_.loadTarget(target, new HelloCallBack());
}
//A2取某个json字符串中某个对象
void AR::loadFromJsonFile(const std::string& path, const std::string& targetname)
{
    ImageTarget target;
    target.load(path.c_str(), EasyAR::kStorageAssets, targetname.c_str());/*加载一个json文件或json字符串。这个方法只会解析json文件或字符串。
如果name非空则加载名字为name的target，否则加载第一个target。
如果path是json文件的路径，storageType应该设成kStorageApp或kStorageAssets或kStorageAbsolute，表示路径的类型。json文件内部的路径应该是绝对路径或相对于json文件的相对路径。
如果path是json字符串, storageType应该设成(kStorageApp | kStorageJson)或(kStorageAssets | kStorageJson)或( kStorageAbsolute | kStorageJson)。
 json字符串里的路径应该是绝对路径或相对于storageType所指向的根目录的相对路径。*/

    tracker_.loadTarget(target, new HelloCallBack());/*加载一个Target进入tracker。Target只有在成功加载进入ImageTracker之后才能被识别和跟踪。
这个方法是异步方法。加载过程可能会需要一些时间来完成，这段时间内新的和丢失的target的检测可能会花比平时更多的时间，但是检测到之后的跟踪不受影响。
 如果你希望知道加载的结果，需要提供一个非null的callback。callback将会在其它线程上被调用。跟踪线程和除了其它加载/卸载之外的操作都不会被阻塞。*/
}
//新增，从绝对路径获取图片
void AR::loadFromAbsolutePath(const std::string& path, const std:: string& targetname)
{
    ImageTarget target;
    target.load(path.c_str(),EasyAR::kStorageAbsolute,targetname.c_str());
    tracker_.loadTarget(target,new HelloCallBack());
}


//A3取json字符串中所有对象
void AR::loadAllFromJsonFile(const std::string& path)
{
    TargetList targets = ImageTarget::loadAll(path.c_str(), EasyAR::kStorageAssets);/*从类型为storageType的path中加载json文件或json字符串中列出的所有target。*/
    for (int i = 0; i < targets.size(); ++i) {
        tracker_.loadTarget(targets[i], new HelloCallBack());
    }
}
//A5
bool AR::start()
{
    bool status = true;
    status &= camera_.start();//开始捕获图像。
    camera_.setFocusMode(CameraDevice::kFocusModeContinousauto);//设置对焦模式为focusMode。在start之后使用。kFocusModeContinousauto=连续自动对焦模式。
    status &= tracker_.start();//开始跟踪算法。在有CameraDevice连接之前跟踪不会真正开始。
    return status;
}

bool AR::stop()
{
    bool status = true;
    status &= tracker_.stop();
    status &= camera_.stop();
    return status;
}
//F1
bool AR::clear()
{
    bool status = true;
    status &= stop();
    status &= camera_.close();/*关闭camera。如果CameraDevice被关闭，
 * 所有算法将丢失与该CameraDevice的连接，并且这些连接即使在重新打开设备之后也是无法恢复的，需要重新调用attach方法进行连接。*/
    camera_.clear();
    tracker_.clear();
    augmenter_.clear();
    return status;
}
//C1重新获取GL的尺寸
void AR::resizeGL(int width, int height)
{
    Vec2I size = Vec2I(1, 1);
    if(camera_.isOpened())
        size = camera_.size();
    if (size[0] == 0 || size[1] == 0)
        return;
    if(portrait_)
        std::swap(size[0], size[1]);
    float scaleRatio = std::max((float)width / (float)size[0], (float)height / (float)size[1]);//取宽，高比例大的
    Vec2I viewport_size = Vec2I((int)(size[0] * scaleRatio), (int)(size[1] * scaleRatio));
    viewport_ = Vec4I(0, height - viewport_size[1], viewport_size[0], viewport_size[1]);
}

void AR::initGL()
{

}

void AR::render()
{

}
//E1
void AR::setPortrait(bool portrait)
{
    portrait_ = portrait;
}

}
}
