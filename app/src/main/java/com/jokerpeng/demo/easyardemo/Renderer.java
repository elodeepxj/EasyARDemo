/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

package com.jokerpeng.demo.easyardemo;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Renderer implements GLSurfaceView.Renderer {
    private final String TAG = "Renderer";
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.e(TAG,"onSurfaceCreated");
        MainActivity.nativeInitGL();//B
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        Log.e(TAG,"onSurfaceChanged");
        MainActivity.nativeResizeGL(w, h);//C
    }

    public void onDrawFrame(GL10 gl) {
        boolean a = MainActivity.nativeCall();
        Log.e("------------","a=     "+a+"");
        MainActivity.nativeRender();//D
    }

}
