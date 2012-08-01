/**
	Defines a thread that receives a frame over TCP.
	@author: Jan Christian Hjorth Hansen - rocknrolldk@gmail.com
 */

#include "FrameReceiver.h"

#include <cstdio>
using std::cout;
using std::endl;

FrameReceiver::FrameReceiver() : activity(this, &FrameReceiver::runActivity) {
	FrameReceiver(DEFAULT_IP, DEFAULT_PORT, RECEIVER_FRAME_WIDTH, RECEIVER_FRAME_HEIGHT);
}

FrameReceiver::FrameReceiver(string ip, int port, int frameWidth, int frameHeight) : activity(this, &FrameReceiver::runActivity) {
	this->ip = ip;
	this->port = port;
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	this->frameSize = frameWidth * frameHeight * 3;
	
	pixels = new char[frameSize];
	
	cout << "try to initialize receiver" << endl;
	
	client.setup(ip, port, true);
	client.setVerbose(true); // remove when done testing
    
	cout << "Receiver initialized" << endl;
}

FrameReceiver::~FrameReceiver() {
	delete [] pixels;
}

void FrameReceiver::start() {
	activity.start();
}

void FrameReceiver::stop() {
	activity.stop();
}

void FrameReceiver::readFrame(ofTexture & texture) {
	if (texture.getWidth() < frameWidth || texture.getHeight() < frameHeight) {
		throw length_error("FrameReceiver::readFrame: Frame size is bigger than the size of the buffer.");
	}
	
	if (rwlock.tryReadLock()) {
        texture.loadData((unsigned char*)pixels, frameWidth, frameHeight, GL_RGB);
		rwlock.unlock();
	}
}

bool FrameReceiver::estaConectado(){
	return client.isConnected();
}
void FrameReceiver::runActivity() {
	while (!activity.isStopped()) {
		if (client.isConnected()) {
            
			if (rwlock.tryWriteLock()) {
				//Receiving loop that must ensure a frame is received as a whole
				char* receivePos = pixels;
				int length = frameWidth * 3;
				int totalReceivedBytes = 0;
				while (totalReceivedBytes < frameSize) {
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
