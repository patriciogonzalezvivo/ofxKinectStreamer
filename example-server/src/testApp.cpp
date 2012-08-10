#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	
    gui.setup("Panel");
    gui.add(minDist.setup("min", 500, 500, 6000));
    gui.add(maxDist.setup("max", 4000, 500, 6000));
    gui.loadFromFile("settings.xml");
    
    gui.setPosition(15, 480*0.5 + 15);
    
	//  Setup the server
	sender = new ofxKinectServer(11999, 320, 240);
	sender->start();
}

//--------------------------------------------------------------
void testApp::update(){
    sender->update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(20,20,20);
    
	sender->draw(0,0);
    
    ofSetWindowTitle( "SERVER ( "+ofToString(ofGetFrameRate()) + " fps )" );
    
    gui.draw();
}

void testApp::exit(){
    sender->close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
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
#ifndef FAKEKINECT
    sender->setDepthClipping(minDist, maxDist);
#endif
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

