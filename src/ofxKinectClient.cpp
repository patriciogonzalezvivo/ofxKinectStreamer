/**
	Defines a thread that receives a frame over TCP.
	@author: Jan Christian Hjorth Hansen - rocknrolldk@gmail.com
 */

#include "ofxKinectClient.h"

#include <cstdio>
using std::cout;
using std::endl;

ofxKinectClient::ofxKinectClient() : activity(this, &ofxKinectClient::runActivity) {
	ofxKinectClient(DEFAULT_IP, DEFAULT_PORT, RECEIVER_FRAME_WIDTH, RECEIVER_FRAME_HEIGHT);
}

ofxKinectClient::ofxKinectClient(string _ip, int _port, int _width, int _height) : activity(this, &ofxKinectClient::runActivity) {
	this->ip = _ip;
	this->port = _port;
	this->width = _width;
	this->height = _height;
	this->size = _width * _height * 3;
	
	pixels = new char[size];
    string shaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex;\
    \
    void main (void){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    vec3 src = texture2DRect(tex, pos).rgb;\
    \
    float clampFactor = 1.0/3.0;\
    \
    float depth = 0.0;\
    \
    depth = src.r*clampFactor + src.g*clampFactor + src.b*clampFactor;\
    \
    gl_FragColor = vec4( depth, depth, depth , 1.0);\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram(); 
    
    texture.allocate(width, height, GL_RGB);
    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(0);
    fbo.end();
	
	cout << "try to initialize receiver" << endl;
	
	client.setup(ip, port, true);
	client.setVerbose(true); // remove when done testing
    
	cout << "Receiver initialized" << endl;
}

ofxKinectClient::~ofxKinectClient() {
	delete [] pixels;
}

void ofxKinectClient::update() {
    if (isConnected()){
        
        if (rwlock.tryReadLock()) {
            texture.loadData((unsigned char*)pixels, width, height, GL_RGB);
            rwlock.unlock();
        }
        
        fbo.begin();
        ofClear(0);
        ofSetColor(255);
        
        shader.begin();
        shader.setUniformTexture("tex", texture, 0);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
        glTexCoord2f(width, height); glVertex3f(width, height, 0);
        glTexCoord2f(0,height);  glVertex3f(0,height, 0);
        glEnd();
        shader.end();
        
        fbo.end();
	}
}

void ofxKinectClient::runActivity() {
	while (!activity.isStopped()) {
		if (client.isConnected()) {
            
			if (rwlock.tryWriteLock()) {
				//Receiving loop that must ensure a frame is received as a whole
				char* receivePos = pixels;
				int length = width * 3;
				int totalReceivedBytes = 0;
				while (totalReceivedBytes < size) {
					int receivedBytes = client.receiveRawBytes(receivePos, length); //returns received bytes
					totalReceivedBytes += receivedBytes;
					receivePos += receivedBytes;
				}
				
				rwlock.unlock();
			}
		}
		else {
			cout << "NOT connected to server" << endl;
		}
        
		ofSleepMillis(RECEIVER_ACTIVITY_SLEEP);
	}
}

void ofxKinectClient::draw(int _x, int _y){
    fbo.draw(_x,_y);
}