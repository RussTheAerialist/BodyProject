#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;

    settings.setPosition({0.0f, 0.0f});
		settings.setSize(768, 1024);
    settings.windowMode = OF_WINDOW;

    auto projectorWin = ofCreateWindow(settings);

    ofRunApp(projectorWin, std::make_shared<ofDisplayApp>());

    ofRunMainLoop();

}
