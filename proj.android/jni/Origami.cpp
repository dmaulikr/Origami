#include <jni.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "OGame.hpp"
#include <Core/ODirector.h>
#include <Core/Utils/OResourceManager.h>

extern "C" {
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv *env, jclass type, jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resize(JNIEnv *env, jclass type, jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv *env, jclass type);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_cleanUp(JNIEnv *env, jclass type);
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setAssetManager(JNIEnv *env, jobject obj, jobject assetManager);
AAssetManager *mgr;
};

OGame *s_game = nullptr;
uint g_width;
uint g_height;
const float 	MS_IN_SECOND	   = 1000.0f;
double          tickCounter        = 0;
float     		DESIRED_FRAMETIME  = 0;
const int       MAX_PHYSICS_STEPS  = 10;
unsigned int    frames             = 0;
unsigned int    updates            = 0;
double 			previousTicks	   = 0;


double accumulator  = 0;

OGame *game()
{
	if (s_game == nullptr) {
		s_game = new OGame("", g_width, g_height);
	}
	return s_game;
}


JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_init(JNIEnv *env, jclass type, jint width, jint height) {
	g_width = width;
	g_height = height;

	if (!s_game) {
		game()->Start();
	}
	OResourceManager::checkResources();

	DESIRED_FRAMETIME = MS_IN_SECOND / game()->GetPreferredFPS();//
	previousTicks = ODirector::director()->getTimer()->getTime();
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_resize(JNIEnv *env, jclass type, jint width, jint height) {
	g_width = width;
	g_height = height;

    if (ODirector::director()->getCurrentScene()) {
        ODirector::director()->setFrameSize(g_width, g_height);

        OSize designResolutionSize = ODirector::director()->getDesignResolutionSize();

        if (g_width > g_height) { //Landscape
            ODirector::director()->setDesignResolutionSize(MAX(designResolutionSize.width, designResolutionSize.height), MIN(designResolutionSize.width, designResolutionSize.height), ResolutionPolicy::FIXED_HEIGHT);
        }
        else
        {
            ODirector::director()->setDesignResolutionSize(MIN(designResolutionSize.width, designResolutionSize.height), MAX(designResolutionSize.width, designResolutionSize.height), ResolutionPolicy::FIXED_WIDTH);
        }


        OSize fS = ODirector::director()->getFrameSize();
        glViewport(0, 0, fS.width, fS.height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gl2jni_GL2JNILib_step(JNIEnv *env, jclass type) {



	 double startTicks =  ODirector::director()->getTimer()->getTime();
	 double passedTime = startTicks - previousTicks;
	 previousTicks = startTicks;

	 accumulator += passedTime;


	 int   loops = 0;

	 while(accumulator >= DESIRED_FRAMETIME && loops < MAX_PHYSICS_STEPS)
	 {
		 game()->Update(DESIRED_FRAMETIME / MS_IN_SECOND);
		 accumulator -= DESIRED_FRAMETIME;
		 updates ++;
		 loops++;
	 }

	 game()->Refresh();

	 float interpolation = float(ODirector::director()->getTimer()->getTime() + DESIRED_FRAMETIME - previousTicks )/ float( DESIRED_FRAMETIME );
	 game()->Render(interpolation);
	 frames++;

#ifdef O_MODE_DEBUG
	    tickCounter += passedTime;
	    if (tickCounter >= MS_IN_SECOND) {

			game()->setFPS(frames);
			game()->setUPS(updates);

			frames = 0;
			updates = 0;
			tickCounter -= MS_IN_SECOND;

			game()->Tick();
	    }
#endif
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setAssetManager(JNIEnv *env, jobject obj, jobject assetManager)
{
	mgr = AAssetManager_fromJava(env, assetManager);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_cleanUp(JNIEnv *env, jclass type)
{
	if (s_game) {
		SAFE_DELETE(s_game);
	}
}