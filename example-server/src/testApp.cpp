#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	
    width = 640*0.5;
    height = 480*0.5;
    
    gui.setup("Panel");
    gui.add(minDist.setup("min", 500, 500, 6000));
    gui.add(maxDist.setup("max", 4000, 500, 6000));
    gui.loadFromFile("settings.xml");
    
    gui.setPosition(15, height + 15);
    
    oscReceiver.setup( 3435 );
    
#ifdef FAKEKINECT
    grabber.initGrabber(width,height,true);
    
#else
    kinect.setRegistration(false);
	kinect.enableDepthNearValueWhite(true);
    kinect.setDepthClipping(minDist, maxDist);
    
	// ir:false, rgb:false, texture:true
	kinect.init(false, false, true);
	kinect.open();
#endif
    
    pixels.allocate(width, height, 3);
    
    string shaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex;\
    \
    void main (void){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    float channels = 3.0;\
    float clampFactor = 1.0/channels;\
    float doubleClampFactor = clampFactor*2.0;\
    \
    float depth = texture2DRect(tex, pos).r;\
    vec3 color = vec3(0.0);\
    \
    if ( depth < clampFactor ){\
    color.r = depth * channels;\
    } else if ( depth < doubleClampFactor){\
    color.r = 1.0;\
    color.g = (depth - clampFactor)*3.0;\
    } else {\
    color.r = 1.0;\
    color.g = 1.0;\
    color.b = (depth - doubleClampFactor)*3.0;\
    }\
    \
    gl_FragColor = vec4( color , 1.0);\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram(); 
    
    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(0);
    fbo.end();
    
	//setup server
	frameSender = new FrameSender(11999, width, height);
    frameSender->setPixelsSrc( pixels );
	frameSender->start();
    

}

//--------------------------------------------------------------
void testApp::update(){
    
#ifdef FAKEKINECT
	grabber.update();
	if(grabber.isFrameNew()){
        fbo.begin();
        ofClear(0);
        ofSetColor(255);
        
        shader.begin();
        shader.setUniformTexture("tex", grabber.getTextureReference(), 0);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
        glTexCoord2f(width, height); glVertex3f(width, height, 0);
        glTexCoord2f(0,height);  glVertex3f(0,height, 0);
        glEnd();
        shader.end();
        
        fbo.end();
        
        fbo.getTextureReference().readToPixels( pixels );
        //frameSender->updateFrame( pixels );
    }
#else
    kinect.update();
    if(kinect.isFrameNew()){
        fbo.begin();
        ofClear(0);
        ofSetColor(255);
        
        shader.begin();
        shader.setUniformTexture("tex", kinect.getDepthTextureReference(), 0);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
        glTexCoord2f(width, height); glVertex3f(width, height, 0);
        glTexCoord2f(0,height);  glVertex3f(0,height, 0);
        glEnd();
        shader.end();
        
        fbo.end();
        
        fbo.getTextureReference().readToPixels( pixels );
        frameSender->updateFrame( pixels );
    }
    
    while(oscReceiver.hasWaitingMessages()){
        
		//  Get the next message
        //
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        //  Check & do from OSC messages
        //
		if(m.getAddress() == "/remoteKinect/min"){
			minDist = m.getArgAsFloat(0);
            kinect.setDepthClipping(minDist, maxDist);
		} else if(m.getAddress() == "/remoteKinect/max"){
			maxDist = m.getArgAsFloat(0);
            kinect.setDepthClipping(minDist, maxDist);
		} 
	}
#endif
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(20,20,20);
	fbo.draw(0,0);
    
    ofSetWindowTitle( "SERVER ( "+ofToString(ofGetFrameRate()) + " fps )" );
    
    gui.draw();
}

void testApp::exit(){
#ifdef FAKEKINECT
	grabber.close();
#else
    kinect.close();
#endif
	frameSender->stop();
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
    kinect.setDepthClipping(minDist, maxDist);
#endif
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

