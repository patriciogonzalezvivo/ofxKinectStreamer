#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//setup cam
	ofSetVerticalSync(true); 

	
	//setup client
	receiver = new ofxKinectClient("127.0.0.1", 11999, 320,240);
	receiver->start();
    
    ofSetWindowShape(320, 240);
}

//--------------------------------------------------------------
void testApp::update(){
	//get frame
    receiver->update();
}
void testApp::exit(){
	receiver->stop();
}
//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(20,20,20,255);
    receiver->draw(0, 0);
	ofSetWindowTitle("fps:: "+ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'x'){
		receiver->stop();
	}else if(key == 'c'){
		receiver->start();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

