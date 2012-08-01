#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//setup cam
	ofSetVerticalSync(true); 

    width = 320;
    height = 240;
    
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
	
	//setup client
	frameReceiver = new FrameReceiver("127.0.0.1", 11999, 320,240);
	frameReceiver->start();
    
    ofSetWindowShape(width, height);
}

//--------------------------------------------------------------
void testApp::update(){
	//get frame
	if(frameReceiver->estaConectado()){
		frameReceiver->readFrame( texture );
        
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
void testApp::exit(){
	frameReceiver->stop();
}
//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(20,20,20,255);
    fbo.draw(0, 0, width, height);
	ofSetWindowTitle("fps:: "+ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'x'){
		frameReceiver->stop();
	}else if(key == 'c'){
		frameReceiver->start();
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

