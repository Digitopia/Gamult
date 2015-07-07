#include "ofMain.h"
#include "ofApp.h"

int main() {
    
#ifdef TARGET_OF_IOS
    ofAppiOSWindow * window = new ofAppiOSWindow();
    window->enableRetina();
    window->enableHardwareOrientation();
    window->enableOrientationAnimation();
    ofSetupOpenGL(window, 1024, 768, OF_WINDOW);
#else
    ofSetupOpenGL(1024, 768, OF_WINDOW);
#endif
    
    ofRunApp(new ofApp());
    return 0;
}

#ifdef TARGET_OF_IOS
extern "C"{
    size_t fwrite$UNIX2003( const void *a, size_t b, size_t c, FILE *d )
    {
        return fwrite(a, b, c, d);
    }
    char* strerror$UNIX2003( int errnum )
    {
        return strerror(errnum);
    }
    time_t mktime$UNIX2003(struct tm * a)
    {
        return mktime(a);
    }
    double strtod$UNIX2003(const char * a, char ** b) {
        return strtod(a, b);
    }
}
#endif
