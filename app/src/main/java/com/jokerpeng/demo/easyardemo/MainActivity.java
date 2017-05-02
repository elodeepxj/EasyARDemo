/**
* Copyright (c) 2015-2016 VisionStar Information Technology (Shanghai) Co., Ltd. All Rights Reserved.
* EasyAR is the registered trademark or trademark of VisionStar Information Technology (Shanghai) Co., Ltd in China
* and other countries for the augmented reality technology developed by VisionStar Information Technology (Shanghai) Co., Ltd.
*/

package com.jokerpeng.demo.easyardemo;

import android.content.res.Configuration;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.ViewGroup;
import android.view.WindowManager;

import cn.easyar.engine.EasyAR;


public class MainActivity extends ActionBarActivity {

    /*
    * Steps to create the key for this sample:
    *  1. login www.easyar.com
    *  2. create app with
    *      Name: HelloAR
    *      Package Name: cn.easyar.samples.helloar
    *  3. find the created item in the list and show key
    *  4. set key string bellow
    */
    static String key = "XVEYabcS68cNNDvJ8asy7e4Bt0CZ3X2JVa2yaIWmY8PtXFbBVLh22DCXgap5kY2nxLCdlEvyUt7FtdE2WmiwqdNFU1FKJ7aT6yYe2d61bf32dcc90b058eb994309939b9969ItYEz3HzgAI8o6Mkb5aPO9h8WKLlpiAMQxUlhR2rGzhatpYruHXw263Z8NcL4F1xXme";

    static {
        System.loadLibrary("EasyAR");
        System.loadLibrary("myAR");
    }

    public static native void nativeInitGL();//B-helloar.cc
    public static native void nativeResizeGL(int w, int h);//C-helloar.cc
    public static native void nativeRender();//D-helloar.cc
    private native boolean nativeInit();//A-helloar.cc
    private native void nativeDestory();//F-helloar.cc
    private native void nativeRotationChange(boolean portrait);//E-helloar.cc

    public static native boolean nativeCall();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        EasyAR.initialize(this, key);
        nativeInit();//A

        GLView glView = new GLView(this);
        glView.setRenderer(new Renderer());
        glView.setZOrderMediaOverlay(true);

        ((ViewGroup) findViewById(R.id.preview)).addView(glView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        nativeRotationChange(getWindowManager().getDefaultDisplay().getRotation() == android.view.Surface.ROTATION_0);//E
    }

    @Override
    public void onConfigurationChanged(Configuration config) {
        super.onConfigurationChanged(config);
        nativeRotationChange(getWindowManager().getDefaultDisplay().getRotation() == android.view.Surface.ROTATION_0);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeDestory();//F
    }
    @Override
    protected void onResume() {
        super.onResume();
        EasyAR.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        EasyAR.onPause();
    }


}
