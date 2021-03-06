#include "ofMain.h"
#include "ofApp.h"

int main() {

    #if defined TARGET_OF_IOS
    ofiOSWindowSettings window;
    window.enableRetina = true;
    window.enableHardwareOrientation = true;
    window.enableAntiAliasing = true;
    window.numOfAntiAliasingSamples = 4;
    ofCreateWindow(window);
    #else
    ofSetupOpenGL(1024, 768, OF_FULLSCREEN);
    #endif

    ofRunApp(new ofApp());
    return 0;
}


#ifdef TARGET_ANDROID
#include <jni.h>

extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif
