#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxNetwork.h"
#include "FrameReceiver.h"

//Globally known constants
#define CAM1_WIDTH 640
#define CAM1_HEIGHT 480
#define CAM2_WIDTH 640
#define CAM2_HEIGHT 480
#define SERVER1_PORT 11999
#define SERVER1_IP "127.0.0.1"
#define SERVER2_PORT 11998
#define SERVER2_IP "127.0.0.1"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void exit();
	
private:
    ofxCvColorImage frame;
	
    int cam1ByteSize;
    int cam2ByteSize;
	
    ofxCvColorImage receivedFrame;
	
    FrameReceiver* frameReceiver;	
    
    ofTexture   texture;
    ofFbo       fbo;
    ofShader    shader;
    
    int width, height;
};
