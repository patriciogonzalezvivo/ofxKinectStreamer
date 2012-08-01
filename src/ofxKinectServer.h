
/**
	Defines a thread that sends frames over TCP. A frame can be an image or a
	frame from a video which results in streaming simulation.
	@author: Jan Christian Hjorth Hansen - rocknrolldk@gmail.com
 */
#ifndef FRAMESENDER_H
#define FRAMESENDER_H

#include <stdexcept>
using std::length_error;

#include "ofMain.h"
#include "Poco/Activity.h"
#include "Poco/RWLock.h"
#include "ofxNetwork.h"

#define DEFAULT_PORT 11999
/**
	This setting defines the length of the wait in each thread loop.
	Optimize for performance.
 */
#define SENDER_ACTIVITY_SLEEP 100
#define SENDER_FRAME_WIDTH 640
#define SENDER_FRAME_HEIGHT 480

class FrameSender {
public:
	FrameSender();
	FrameSender(int port, int frameWidth, int frameHeight);
	~FrameSender();
    
    void setPixelsSrc(ofPixels & pixels) throw (length_error); 
	void start();
	void stop();
    
	int getNumClients();
	

private:	
    void runActivity(); //the main thread function
	Poco::Activity<FrameSender> activity;
	Poco::RWLock rwlock;
	
	ofxTCPServer server;
    ofPixels * buffer;
    
    int port;
	int frameWidth, frameHeight, frameSize;
	
	
};

#endif