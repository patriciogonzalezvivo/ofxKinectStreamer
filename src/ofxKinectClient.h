/**
	Defines a thread that receives a frame over TCP.
	@author: Jan Christian Hjorth Hansen - rocknrolldk@gmail.com
 */

#ifndef OFXKINECTCLIENT
#define OFXKINECTCLIENT

#include <string>
using std::string;

#include "ofMain.h"

//  Kinect stuff
#include "ofxBase3DVideo.h"

//  Streamming stuff
#include "Poco/Activity.h"
#include "Poco/RWLock.h"
#include "ofxNetwork.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 11999
/**
	This setting defines the length of the wait in each thread loop.
	Optimize for performance.
 */
#define RECEIVER_ACTIVITY_SLEEP 50
#define RECEIVER_FRAME_WIDTH 640
#define RECEIVER_FRAME_HEIGHT 480

class ofxKinectClient {
public:
	ofxKinectClient();
	ofxKinectClient(string ip, int port, int frameWidth, int frameHeight);
	~ofxKinectClient();
	
	void start();
	void stop();
	bool isConnected();
	
	/**
		Writes into the received frame the pixels received over TCP.
	 */
    void readFrame(ofTexture & texture);
	
private:
	Poco::Activity<ofxKinectClient> activity;
	Poco::RWLock rwlock;
	
	ofxTCPClient client;
	string ip;
	int port;
	
	char* pixels;
	int frameWidth;
	int frameHeight;
	int frameSize;
	
	void runActivity(); //the main thread function
};

#endif
