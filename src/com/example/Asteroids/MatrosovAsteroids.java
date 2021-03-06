/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This is a small port of the "San Angeles Observation" demo
 * program for OpenGL ES 1.x. For more details, see:
 *
 *    http://jet.ro/visuals/san-angeles-observation/
 *
 * This program demonstrates how to use a GLSurfaceView from Java
 * along with native OpenGL calls to perform frame rendering.
 *
 * Touching the screen will start/stop the animation.
 *
 * Note that the demo runs much faster on the emulator than on
 * real devices, this is mainly due to the following facts:
 *
 * - the demo sends bazillions of polygons to OpenGL without
 *   even trying to do culling. Most of them are clearly out
 *   of view.
 *
 * - on a real device, the GPU bus is the real bottleneck
 *   that prevent the demo from getting acceptable performance.
 *
 * - the software OpenGL engine used in the emulator uses
 *   the system bus instead, and its code rocks :-)
 *
 * Fixing the program to send less polygons to the GPU is left
 * as an exercise to the reader. As always, patches welcomed :-)
 */
package com.example.Asteroids;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

public class MatrosovAsteroids extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        //Remove title bar
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        //Remove notification bar
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        mGLView = new DemoGLSurfaceView(this);
        setContentView(mGLView);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }

    private GLSurfaceView mGLView;

    static {
        System.loadLibrary("asteroids");
    }
}

class DemoGLSurfaceView extends GLSurfaceView {
    public DemoGLSurfaceView(Context context) {
        super(context);
        mRenderer = new DemoRenderer();
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {

//        	android.util.Log.i("Asteroids", "action = " + event.getActionMasked());
      
    	switch (event.getActionMasked())
    	{
    	case MotionEvent.ACTION_DOWN:
    	{
    		int id = event.getPointerId(0);
    		nativeOnTouchDown(id, event.getX(0), event.getY(0));
    		break;
    	}
    	case MotionEvent.ACTION_POINTER_DOWN:
    	{
    		int i = event.getActionIndex();
    		int id = event.getPointerId(i);
    		nativeOnTouchDown(id, event.getX(i), event.getY(i));
    		break;
    	}
    	case MotionEvent.ACTION_MOVE:
    	{
    		for (int i = 0; i < event.getPointerCount(); i++)
    		{
        		nativeOnTouchMove(event.getPointerId(i), event.getX(i), event.getY(i));
    		}
    		break;
    	}
    	case MotionEvent.ACTION_POINTER_UP:
    	{
    		int i = event.getActionIndex();
    		int id = event.getPointerId(i);
    		nativeOnTouchUp(id);       		
    		break;
    	}
    	case MotionEvent.ACTION_UP:
    	{
    		int id = event.getPointerId(0);
    		nativeOnTouchUp(id);
    		break;
    	}
    	case MotionEvent.ACTION_CANCEL:
    	{
    		nativeOnTouchCancel();
    		break;
    	}
    	}

        return true;
    }

   @Override
    public void onPause() {
        super.onPause();
        nativePause();
    }

   @Override
    public void onResume() {
        super.onResume();
        nativeResume();
    }


    DemoRenderer mRenderer;

    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeOnTouchDown(int id, float x, float y);
    private static native void nativeOnTouchMove(int id, float x, float y);
    private static native void nativeOnTouchUp(int id);
    private static native void nativeOnTouchCancel();
}

class DemoRenderer implements GLSurfaceView.Renderer {
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInit();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
        nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        nativeRender();
    }

    private static native void nativeInit();
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
    private static native void nativeDone();
}
