#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 


	//grabscreen
	counter = 0;
	ofSetCircleResolution(100);
	texScreen.allocate(1024, 1024, GL_RGBA);
	ofBackground(230, 230, 240);
	myCamGrabber.initGrabber(1024, 768);


	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
	
	soundStream.printDeviceList();
	
	int bufferSize = 256;

	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;


	ofSoundStreamSettings settings;

	// if you want to set the device id to be different than the default
	// auto devices = soundStream.getDeviceList();
	// settings.device = devices[4];

	// you can also get devices for an specific api
	// auto devices = soundStream.getDevicesByApi(ofSoundDevice::Api::PULSE);
	// settings.device = devices[0];

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setInDevice(devices[0]);
	}

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

}

//--------------------------------------------------------------
void ofApp::update(){

	//grapscreen
	myCamGrabber.update();
	counter += 1;

	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//grabscreen
	// 1st, draw on screen:
	ofSetHexColor(0x66CC33);
	//ofDrawRectangle(100,100,300,300);

	ofSetHexColor(0xffffff);
	//	ofPushMatrix();
	//		ofTranslate(200,200,0);
	//		ofRotateDeg(counter,0,0,1);
	//		ofDrawCircle(0,0,80);
	//		ofDrawCircle(100,0,10);	// a small one
	//	ofPopMatrix();
	//	ofSetHexColor(0x333333);
	//	ofDrawBitmapString("(a) on screen", 150,200);
	//
	//	ofSetHexColor(0xFFCC33);
	//	ofDrawCircle(mouseX, mouseY,20);

	myCamGrabber.draw(0, 0);

	// 2nd, grab a portion of the screen into a texture
	// this is quicker then grabbing into an ofImage
	// because the transfer is done in the graphics card
	// as opposed to bringing pixels back to memory
	// note: you need to allocate the texture to the right size

	//texScreen.loadScreenData(myCamGrabber.getWidth()/2, myCamGrabber.getHeight()/2, 200, 200);



	// finally, draw that texture on screen, how ever you want
	// (note: you can even draw the texture before you call loadScreenData,
	// in order to make some trails or feedback type effects)
	ofPushMatrix();
	ofSetHexColor(0xffffff);
	ofTranslate(550, 300, 0);
	//glRotatef(counter, 0.1f, 0.03f, 0);
	float width = 200 + 100 * sin(counter / 200.0f);
	float height = 200 + 100 * sin(counter / 200.0f);;
	//texScreen.draw(-width / 2, -height / 2, width, height);
	ofPopMatrix();

	ofPushMatrix();
	ofSetHexColor(0xffffff);
	ofTranslate(700, 210, 0);
	ofRotateDeg(counter, 0.1f, 0.03f, 0);
	//texScreen.draw(-50, -50, 100, 100);
	ofPopMatrix();

	ofPushMatrix();
	/*for (int i = 0; i < 20; i++) {
		ofScale(0.9, 0.9);
		texScreen.draw(i, 10, 100, 100);
	}*/
	/* ofScale(0.9,0.9);
	 texScreen.draw(10,10,100,100);
	 ofScale(0.8,0.8);
	 texScreen.draw(20,20,100,100);
	 ofScale(0.7,0.7);
	 texScreen.draw(30,30,100,100);*/
	ofPopMatrix();

	ofSetHexColor(0x333333);
	//ofDrawBitmapString("(b) in a texture, very meta!", 500, 200);
	//-------------------------grabscreen
	//-------------------------audio
	ofSetColor(225);
	//ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
	//ofDrawBitmapString("press 's' to unpause the audio\n'e' to pause the audio", 31, 92);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 170, 0);
			
		ofSetColor(225);
		//ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		//ofDrawRectangle(0, 0, 512, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < left.size(); i++){
				//ofVertex(i*2, 100 -left[i]*180.0f);
				
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 370, 0);
			
		ofSetColor(225);
		//ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		//ofDrawRectangle(0, 0, 512, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < right.size(); i++){
				//ofVertex(i*2, 100 -right[i]*180.0f);
				
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
	// draw the average volume:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(565, 170, 0);
			
		ofSetColor(225);
		//ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
		//ofDrawRectangle(0, 0, 400, 400);
		
		ofSetColor(245, 58, 135);
		ofFill();	
		
		//ofDrawCircle(200, 200, scaledVol * 190.0f);
		//for (int p = 0; p < right.size(); p++) {
		//
		//	ofDrawCircle(p, ofRandom(right.size()), right[0] * 190.0f);
		//}
		
		ofDrawCircle(200, 200, right[0] * 190.0f); ///right is just one point 
		//ofDrawCircle(100, 100, smoothedVol);
		
		//lets draw the volume history as a graph
		ofBeginShape();
		for (unsigned int i = 0; i < volHistory.size(); i++){
			if( i == 0 ) ofVertex(i, 400);

			//ofVertex(i, 400 - volHistory[i] * 70);
			
			if( i == volHistory.size() -1 ) ofVertex(i, 400);
		}
		ofEndShape(false);		
			
		ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	//ofDrawBitmapString(reportString, 32, 589);

	/*for (int x = 0; x < right.size(); x++) {
		texScreen.draw(left[0], right[x]*5, right[x] * 190.0f);
		
		}*/

	

	/*for (int i = 0; i < right.size(); i++) {
			ofScale(0.9, 0.9);
			texScreen.draw(1000+(right[i] * 190.0f+i), 10, 100, 100);*/
		
			ofPushMatrix();
			ofSetHexColor(0xffffff);
			ofTranslate(550, 300, 0);
			//glRotatef(counter, 0.1f, 0.03f, 0);
		
			for (int i = 0; i < right.size(); i=i+1){
				
				texScreen.loadScreenData(i+mouseX, i + mouseY, 100, 100);
				float width = right[i] * 290.0f;
				float height = left[i] * 290.0f;
				ofScale(0.99,0.99);
				//texScreen.draw((right[i] * 190.0f + i*10), -height/2 , 600, 600);
				//texScreen.draw(-800 + (width / 2 + (-right[i] * 190.0f * i + 100)), -800 + (-height / 2), -400, -400);
				texScreen.draw(800 + (width / 2 + (right[i] * 190.0f * i + 100)), 800 + (height / 2), 100, 100);
				
				//texScreen.draw(500+(width / 2 + (right[i] * 190.0f * i + 100)), height / 2, 500, 500);
				//texScreen.draw(500 + (width / 2 + (right[i] * 190.0f * i + 100)), 500+(height / 2), 100, 100);
			    texScreen.draw(500 + (width / 2 + (right[i] * 190.0f * i + 100)), 500 + (height / 2), -100, -100);
				//texScreen.draw(-width / 2 + (right[i] * 190.0f  * i+100), -height / 2, 500, 500); /// sound and picture better sound reaction
			texScreen.draw(-width / 2 + (right[i] * 190.0f * i + 100), -height / 2, 500, 500); /// sound and picture better sound reaction
				//texScreen.draw((right[i] * 190.0f + i*10), -height/2 , 600, 600);
			//texScreen.draw((right[i] * 190.0f + i * 10)+800, (-height / 2)+400, -600, -600);
				
				//texScreen.draw(-width / 2 + (right[i] * 190.0f + i * 100), -height / 2+ (left[i] * 190.0f + i * 100),500, 500);
				
			/*texScreen.draw(-width / 2, -height / 2, width, height);*/
			ofPopMatrix();
			
	}
			//texScreen.draw(ofGetWidth(),ofGetHeight(),1000,1000);
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input){
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (size_t i = 0; i < input.getNumFrames(); i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

