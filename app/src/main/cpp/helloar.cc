/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

#include "ar.hpp"
#include "renderer.hpp"
#include <jni.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "EasyAR", __VA_ARGS__)

#define JNIFUNCTION_NATIVE(sig) Java_com_jokerpeng_demo_easyardemo_MainActivity_##sig

extern "C" {
    JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeInit(JNIEnv* env, jobject object));
    JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeDestory(JNIEnv* env, jobject object));
    JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeInitGL(JNIEnv* env, jobject object));
    JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeResizeGL(JNIEnv* env, jobject object, jint w, jint h));
    JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRender(JNIEnv* env, jobject obj));
    JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRotationChange(JNIEnv* env, jobject obj, jboolean portrait));

    JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeCall(JNIEnv* env, jobject object));
};

namespace EasyAR {
namespace samples {

class HelloAR : public AR
{
public:
    HelloAR();
    virtual void initGL();
    virtual void resizeGL(int width, int height);
    virtual void render();
    virtual bool getXXX();
private:
    Vec2I view_size;
    Renderer renderer;
    bool xxx_;
};

HelloAR::HelloAR()
{
    view_size[0] = -1;
}
    //新增
bool HelloAR::getXXX(){
        return xxx_;
}


void HelloAR::initGL()
{
    LOGI("11111111","1111111");
    renderer.init();
    augmenter_ = Augmenter();
    augmenter_.attachCamera(camera_);
}
//C1
void HelloAR::resizeGL(int width, int height)
{
    view_size = Vec2I(width, height);
}
//D1
void HelloAR::render()
{
    xxx_ = false;
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Frame frame = augmenter_.newFrame();
    if(view_size[0] > 0){
        AR::resizeGL(view_size[0], view_size[1]);
        if(camera_ && camera_.isOpened())
            view_size[0] = -1;
    }
    augmenter_.setViewPort(viewport_);
    augmenter_.drawVideoBackground();
    glViewport(viewport_[0], viewport_[1], viewport_[2], viewport_[3]);

    for (int i = 0; i < frame.targets().size(); ++i) {
        AugmentedTarget::Status status = frame.targets()[i].status();
        if (status == AugmentedTarget::kTargetStatusTracked) {//跟踪到
//            LOGI("//////////////////////////","1111111");
            Matrix44F projectionMatrix = getProjectionGL(camera_.cameraCalibration(), 0.2f, 500.f);
            Matrix44F cameraview = getPoseGL(frame.targets()[i].pose());
            ImageTarget target = frame.targets()[i].target().cast_dynamic<ImageTarget>();
            xxx_ = true;
            renderer.render(projectionMatrix, cameraview, target.size());//绘制图形
        }
    }
}

}
}

EasyAR::samples::HelloAR ar;
//A
JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeInit(JNIEnv*, jobject))
{
    bool status = ar.initCamera();//A1-ar.cc初始化相机
    ar.loadFromJsonFile("targets.json", "argame");//A2-ar.cc
    ar.loadFromJsonFile("targets.json", "idback");//A2-ar.cc
    ar.loadAllFromJsonFile("targets2.json");//A3-ar.cc
    ar.loadFromImage("namecard.jpg");//A4-ar.cc
//    ar.loadFromAbsolutePath("ta","sl");
    status &= ar.start();//A5-ar.cc
    return status;
}
//F
JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeDestory(JNIEnv*, jobject))
{
    ar.clear();//F1-ar.cc
}
//B
JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeInitGL(JNIEnv*, jobject))
{
    ar.initGL();//B1
}
//C
JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeResizeGL(JNIEnv*, jobject, jint w, jint h))
{
    ar.resizeGL(w, h);//C1
}
//D
JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRender(JNIEnv*, jobject))
{
    ar.render();//D1
}
//E
JNIEXPORT void JNICALL JNIFUNCTION_NATIVE(nativeRotationChange(JNIEnv*, jobject, jboolean portrait))
{
    ar.setPortrait(portrait);//E1-ar.cc
}

JNIEXPORT jboolean JNICALL JNIFUNCTION_NATIVE(nativeCall(JNIEnv*, jobject)){
    return ar.getXXX();
}