#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//setup cam
	ofSetVerticalSync(true); 

	
	//setup client
	receiver = new ofxKinectClient("127.0.0.1", 11999, 320,240);
	receiver->start();
    
    //ofSetWindowShape(320, 240);
}

//--------------------------------------------------------------
void testApp::update(){
	//get frame
    //if( receiver->isFrameNew() ){
        receiver->update();
    //}
    
}
void testApp::exit(){
	receiver->stop();
}
//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(20,20,20,255);
    receiver->draw(0, 0);
    
    easyCam.begin();
    drawPointCloud();
    easyCam.end();
    
	ofSetWindowTitle("fps:: "+ofToString(ofGetFrameRate()));
}

void testApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(receiver->getDistanceAt(x, y) > 0) {
				mesh.addColor( ofColor::white );
				mesh.addVertex( receiver->getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	glEnable(GL_DEPTH_TEST);
	mesh.drawVertices();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
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

