/**
	Implements a thread that sends frames over TCP. A frame can be an image or a
	frame from a video which results in streaming simulation.
	@author: Jan Christian Hjorth Hansen - rocknrolldk@gmail.com
 */

#include "ofxKinectServer.h"

#include <cstdio>
using std::cout;
using std::endl;

ofxKinectServer::ofxKinectServer() : activity(this, &ofxKinectServer::runActivity) {
	ofxKinectServer(DEFAULT_PORT, SENDER_FRAME_WIDTH, SENDER_FRAME_HEIGHT);
}

ofxKinectServer::ofxKinectServer(int port, int frameWidth, int frameHeight) : activity(this, &ofxKinectServer::runActivity) {
	this->port = port;
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	frameSize = frameWidth * frameHeight * 3;
	
	server.setup(port, true); //this causes OS X to ask for permission to receive from network
	server.setVerbose(true); //NOTE: Remove when done testing
}

ofxKinectServer::~ofxKinectServer() {
    
}

void ofxKinectServer::start() {
	activity.start();
}

void ofxKinectServer::stop() {
	activity.stop();
}

int ofxKinectServer::getNumClients(){
	return server.getNumClients();
}

/**
	Writes a new frame into the buffer frame that gets sent via TCP.
 */
void ofxKinectServer::setPixelsSrc(ofPixels &pixels) throw (length_error) {
	if (pixels.getWidth() > frameWidth || pixels.getHeight() > frameHeight) {
		throw length_error("ofxKinectServer::updateFrame: Frame size is bigger than the size of the buffer.");
	}
	
	if (rwlock.tryWriteLock()) {
        buffer = &pixels; 
		rwlock.unlock();
	}
}

void ofxKinectServer::runActivity() {
	while (!activity.isStopped()) {
		
		//only the first connected client will be considered: id=0
		if (server.isClientConnected(0)) {
            
			if (rwlock.tryReadLock()) {
				
				//Send frame by throttling, ie splitting frame up into chunks (rows) and sending them sequentially
                
				const char* index = (const char*)buffer->getPixels(); //start at beginning of pixel array
				int length = buffer->getWidth() * 3;//length of one row of pixels in the image
                int size = buffer->getHeight() * buffer->getWidth() * 3;
                
                int pixelCount = 0;
                
				while (pixelCount < size ) {
					server.sendRawBytes(0, index, length); //send the first row of the image
					index += length; //increase pointer so that it points to the next image row
					pixelCount += length; //increase pixel count by one row
				}
				
				rwlock.unlock();
			}
		} else {
			//cout << "NOT" << endl;
		}
		
		ofSleepMillis(SENDER_ACTIVITY_SLEEP);
	}
}
