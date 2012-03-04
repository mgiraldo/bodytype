#include "testApp.h"
#include "ofxOsc.h"

#define PORTOUT 12346
#define PORTIN	12345
#define HOST "192.168.1.2"

ofxOscReceiver	receiver;
ofxOscSender	sender;

ofSoundPlayer shutterSound;

int fontColor = 0x000000;

bool showExtraFeeds = false;
bool showDepth = false;
bool fontHasTop = true;
bool fontHasBottom = true;
bool fontHasDots = true;
bool fontHasLines = true;
bool fontIsOutline = false;
bool fontHasTrails = true;
bool hasStarted = false;
bool isSending = false;
bool nameIsSet = false;
bool emailIsSet = false;
bool mouseMode = false;
bool rightHand = true;
bool isFullScreen = false;
bool settingsChanged = true;
bool debugMode = false;

struct letterform {
	string filename;
	float xpos;
    float ypos;
};

float minDot = 5;
float maxDot = 80;
float xini = 0;
float yini = 10;
float letterWidth = 500;
float letterHeight = 666;
float letterSpace = 0;
float lineWidth = 2.0;
int outlinePass = 0;

int row = 0;
int col = 0;
int maxCols = 10;

ofTrueTypeFont inputFont;
ofTrueTypeFont timerFont;
ofTrueTypeFont letterFont;
ofTrueTypeFont hudFont;

ofImage poseImage;

int timePerLetter = 500;
string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//vector <string> lettersUnicode;
int currentLetter = 0;
int lastLetterTime = 0;
vector <letterform> userFont;

// preview
int previewWidth = 1024;
int previewHeight = 768;
int previewX = 0;
int previewY = 0;

// light paint
const int trailSize = 200; // 800
deque <ofPoint> leftTrail (trailSize);
deque <ofPoint> rightTrail (trailSize);
int current = 0;
int maxRadius = 20;
int minRadius = 0.5;
int trailDecay = 2;

// interface
struct button {
	float x;
	float y;
	float w;
	float h;
};

int controlHighlight = 0xcccccc;
int controlSelected = 0xeeeeee;
int controlColor = 0x000000;
int controlUpdate = 0;
int controlWait = 1000;
int controlDistance = 200;
int controlX = 50;
int controlY = 200;
int controlSize = 50;
bool controlIsActive = false;
string controlCurrent = "";
string controlLast = "";
map <string, button> controlButtons;
string controlNames[] = {"start", "restart", "send", "lines", "dots", "trails", "top", "bottom", "outline", "width1", "width2", "width3", "width4", "width5", "width6", "width7", "width8", "width9", "width10"};

string bodyFontName = "mgaFont";
string bodyFontFile = "mgaFont";
string userName = "";
string userEmail = "";

float shutter = 0.0;
float shutterDecay = 5;

//--------------------------------------------------------------
void testApp::setup(){
	setupRemote();
	shutterSound.loadSound("shutter.wav");
    srand(time(NULL));
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofEnableSmoothing();
	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	poseImage.loadImage("pose.png");
	context.setupUsingXMLFile();//ofToDataPath("config/SamplesConfig.xml"));
	depth.setup(&context);
	user.setup(&context, &depth);
	rgbimage.setup(&context);
	timerFont.loadFont("Inconsolata.otf",100);
	letterFont.loadFont("Inconsolata.otf",200);
	inputFont.loadFont("Inconsolata.otf",24);
	hudFont.loadFont("Inconsolata.otf",10);
	setupTrails();
	setupControls();
}

void testApp::setupRemote() {
	// listen on the given port
	receiver.setup( PORTIN );
	sender.setup( HOST, PORTOUT );
}

void testApp::setupControls() {
	// start button
	controlButtons["start"].x = 0;
	controlButtons["start"].y = -controlSize * 2;
	controlButtons["start"].w = controlSize;
	controlButtons["start"].h = controlSize;
	// dots
	controlButtons["send"].x = controlSize * 4;
	controlButtons["send"].y = -controlSize * 2;
	controlButtons["send"].w = controlSize;
	controlButtons["send"].h = controlSize;
	// restart
	controlButtons["restart"].x = controlSize * 2;
	controlButtons["restart"].y = -controlSize * 2;
	controlButtons["restart"].w = controlSize;
	controlButtons["restart"].h = controlSize;
	// lines
	controlButtons["lines"].x = 0;
	controlButtons["lines"].y = 0;
	controlButtons["lines"].w = controlSize;
	controlButtons["lines"].h = controlSize;
	// dots
	controlButtons["dots"].x = controlSize;
	controlButtons["dots"].y = 0;
	controlButtons["dots"].w = controlSize;
	controlButtons["dots"].h = controlSize;
	// trails
	controlButtons["trails"].x = controlSize * 2;
	controlButtons["trails"].y = 0;
	controlButtons["trails"].w = controlSize;
	controlButtons["trails"].h = controlSize;
	// top
	controlButtons["top"].x = controlSize * 3;
	controlButtons["top"].y = 0;
	controlButtons["top"].w = controlSize;
	controlButtons["top"].h = controlSize;
	// bottom
	controlButtons["bottom"].x = controlSize * 4;
	controlButtons["bottom"].y = 0;
	controlButtons["bottom"].w = controlSize;
	controlButtons["bottom"].h = controlSize;
	// outline
	controlButtons["outline"].x = controlSize * 5;
	controlButtons["outline"].y = 0;
	controlButtons["outline"].w = controlSize;
	controlButtons["outline"].h = controlSize;
	// width1
	controlButtons["width1"].x = 0;
	controlButtons["width1"].y = controlSize;
	controlButtons["width1"].w = controlSize;
	controlButtons["width1"].h = controlSize * .2;
	// width2
	controlButtons["width2"].x = 0;
	controlButtons["width2"].y = controlSize + (controlSize*.2*2);
	controlButtons["width2"].w = controlSize;
	controlButtons["width2"].h = controlSize * .2;
	// width3
	controlButtons["width3"].x = 0;
	controlButtons["width3"].y = controlSize + (controlSize*.2*3);
	controlButtons["width3"].w = controlSize;
	controlButtons["width3"].h = controlSize * .2;
	// width4
	controlButtons["width4"].x = 0;
	controlButtons["width4"].y = controlSize + (controlSize*.2*4);
	controlButtons["width4"].w = controlSize;
	controlButtons["width4"].h = controlSize * .2;
	// width5
	controlButtons["width5"].x = 0;
	controlButtons["width5"].y = controlSize + (controlSize*.2*5);
	controlButtons["width5"].w = controlSize;
	controlButtons["width5"].h = controlSize * .2;
	// width6
	controlButtons["width6"].x = 0;
	controlButtons["width6"].y = controlSize + (controlSize*.2*6);
	controlButtons["width6"].w = controlSize;
	controlButtons["width6"].h = controlSize * .2;
	// width7
	controlButtons["width7"].x = 0;
	controlButtons["width7"].y = controlSize + (controlSize*.2*7);
	controlButtons["width7"].w = controlSize;
	controlButtons["width7"].h = controlSize * .2;
	// width8
	controlButtons["width8"].x = 0;
	controlButtons["width8"].y = controlSize + (controlSize*.2*8);
	controlButtons["width8"].w = controlSize;
	controlButtons["width8"].h = controlSize * .2;
	// width9
	controlButtons["width9"].x = 0;
	controlButtons["width9"].y = controlSize + (controlSize*.2*9);
	controlButtons["width9"].w = controlSize;
	controlButtons["width9"].h = controlSize * .2;
	// width10
	controlButtons["width10"].x = 0;
	controlButtons["width10"].y = controlSize + (controlSize*.2*10);
	controlButtons["width10"].w = controlSize;
	controlButtons["width10"].h = controlSize * .2;
}

/**/
void testApp::drawInterface() {
	ofSetRectMode(OF_RECTMODE_CORNER);
	
	ofSetColor(controlSelected);
	int nTrackedUsers = user.getTrackedUsers().size();
	ofxTrackedUser *tracked = NULL;
	if (nTrackedUsers>0) {
		tracked = user.getTrackedUser(0);
	}
	if(tracked != NULL) {
		if (inButton(ofMap(tracked->left_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->left_lower_arm.end.y, 0, 480, 0, 768, true), controlX, controlY, controlSize, controlSize)
			|| inButton(ofMap(tracked->right_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->right_lower_arm.end.y, 0, 480, 0, 768, true), controlX, controlY, controlSize, controlSize)) {
			ofSetColor(controlHighlight);
			controlIsActive = true;
		} else {
			controlIsActive = false;
		}
	} else if (mouseMode && inButton(mouseX, mouseY, controlX, controlY, controlSize, controlSize)) {
		controlIsActive = true;
	} else {
		controlIsActive = false;
	}
	ofRect(controlX, controlY, controlSize, controlSize);
	ofSetColor(fontColor);
	hudFont.drawString("menu",controlX + 2, controlY + 10);
	
	if (controlIsActive) {
		ofSetColor(255, 255, 255, 170);
		ofRect(0, 0, 1024, 768);
		ofSetColor(fontColor);
		if (mouseMode || user.getTrackedUsers().size()>0) {
			// highlight area
			if (hasStarted) {
				// highlight area
				ofSetColor(0xff0000);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["start"].x, controlY + controlButtons["start"].y, controlButtons["start"].w, controlButtons["start"].h);
			
			ofSetColor(0xffffff);
			ofRect(controlX + controlDistance + controlButtons["send"].x, controlY + controlButtons["send"].y, controlButtons["send"].w, controlButtons["send"].h);
			
			ofSetColor(0xffffff);
			ofRect(controlX + controlDistance + controlButtons["restart"].x, controlY + controlButtons["restart"].y, controlButtons["restart"].w, controlButtons["restart"].h);
			
			if (fontHasLines) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["lines"].x, controlY + controlButtons["lines"].y, controlButtons["lines"].w, controlButtons["lines"].h);
			
			if (fontHasDots) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["dots"].x, controlY + controlButtons["dots"].y, controlButtons["dots"].w, controlButtons["dots"].h);
			
			if (fontHasTrails) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["trails"].x, controlY + controlButtons["trails"].y, controlButtons["trails"].w, controlButtons["trails"].h);
			
			if (fontHasTop) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["top"].x, controlY + controlButtons["top"].y, controlButtons["top"].w, controlButtons["top"].h);
			
			if (fontHasBottom) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["bottom"].x, controlY + controlButtons["bottom"].y, controlButtons["bottom"].w, controlButtons["bottom"].h);
			
			if (fontIsOutline) {
				// highlight area
				ofSetColor(controlSelected);
			} else {
				ofSetColor(0xffffff);
			}
			ofRect(controlX + controlDistance + controlButtons["outline"].x, controlY + controlButtons["outline"].y, controlButtons["outline"].w, controlButtons["outline"].h);
			
			ofSetColor(0xffffff);
			ofRect(controlX + controlDistance + controlButtons["width1"].x, controlY + controlButtons["width1"].y, controlButtons["width1"].w, controlSize*2);
			ofSetColor(controlSelected);
			int multiplier = round(lineWidth/2);
			ofRect(controlX + controlDistance + controlButtons["width1"].x, controlY + controlButtons["width1"].y, controlButtons["width1"].w, (controlSize*.2)*multiplier);
			
			// tool tip and highlights
			int i;
			string c;
			string s;
			float baseX = mouseX;
			float baseY = mouseY;
			if(tracked != NULL) {
				baseX = ofMap(tracked->right_lower_arm.end.x, 0, 640, 0, 1024, true);
				baseY = ofMap(tracked->right_lower_arm.end.y, 0, 480, 0, 768, true);
			}
			for (i=0; i<19; ++i) {
				c = controlNames[i];
				// exclue width buttons
				if (inButton(baseX, baseY, controlX + controlDistance + controlButtons[c].x, controlY + controlButtons[c].y, controlButtons[c].w, controlButtons[c].h)) {
					s = (controlIsToggled(c)) ? ":ON" : ":OFF";
					ofSetColor(controlHighlight);
					if (c.find("width")==string::npos) {
						// not a width button
						if ((c.compare("start")==0 && !hasStarted) || c.compare("restart")==0 || c.compare("send")==0) {
							// one of the top buttons
							s = "";
							ofRect(controlX + controlDistance + controlButtons[c].x, controlY + controlButtons[c].y, controlButtons[c].w, controlButtons[c].h);
						} else if (c.compare("start")==0 && hasStarted) {
							// start becomes stop
							s = "";
							c = "stop";
							ofRect(controlX + controlDistance + controlButtons[c].x, controlY + controlButtons[c].y, controlButtons[c].w, controlButtons[c].h);
						} else {
							// regular button
							ofRect(controlX + controlDistance + controlButtons[c].x, controlY + controlButtons[c].y, controlButtons[c].w, controlButtons[c].h);
						}
					} else {
						// width highlights
						// draw base rect
						s = "";
						multiplier = ofToInt(c.substr(5));
						ofRect(controlX + controlDistance + controlButtons["width1"].x, controlY + controlButtons["width1"].y, controlButtons["width1"].w, (controlSize*.2)*multiplier);
					}
					ofSetColor(fontColor);
					timerFont.drawString(c + s,controlX + controlSize * 5, controlY + controlSize * 3);
					break;
				}
			}
			// end tool tip and highlights
			
			// record
			if (!hasStarted) {
				// highlight area
				ofSetColor(0xff0000);
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofCircle(controlX + controlDistance + controlButtons["start"].x + controlSize * .5, controlY + controlButtons["start"].y + controlSize * .6, controlSize * .3);
				ofSetRectMode(OF_RECTMODE_CORNER);
			} else {
				ofSetColor(fontColor);
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofRect(controlX + controlDistance + controlButtons["start"].x + controlSize * .5, controlY + controlButtons["start"].y + controlSize * .6, controlSize * .5, controlSize * .5);
				ofSetRectMode(OF_RECTMODE_CORNER);
			}
			
			// restart
			ofSetColor(fontColor);
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofRect(controlX + controlDistance + controlButtons["restart"].x + controlSize * .5, controlY + controlButtons["restart"].y + controlSize * .6, controlButtons["restart"].w * .5, controlButtons["restart"].h * .5);
			ofSetRectMode(OF_RECTMODE_CORNER);
			
			// send
			ofSetColor(fontColor);
			ofBeginShape();
			ofVertex(controlX + controlDistance + controlButtons["send"].x + 17, controlY + controlButtons["send"].y + 12);
			ofVertex(controlX + controlDistance + controlButtons["send"].x + 37, controlY + controlButtons["send"].y + 27);
			ofVertex(controlX + controlDistance + controlButtons["send"].x + 17, controlY + controlButtons["send"].y + 42);
			ofEndShape();
			
			// texts & separators
			ofSetColor(controlColor);
			
			if (!hasStarted) {
				hudFont.drawString("start",controlX + controlDistance + controlButtons["start"].x + 2, controlY + controlButtons["start"].y + 10);
			} else {
				hudFont.drawString("stop",controlX + controlDistance + controlButtons["start"].x + 2, controlY + controlButtons["start"].y + 10);
			}
			hudFont.drawString("send",controlX + controlDistance + controlButtons["send"].x + 2, controlY + controlButtons["send"].y + 10);
			hudFont.drawString("restart",controlX + controlDistance + controlButtons["restart"].x + 2, controlY + controlButtons["restart"].y + 10);
			hudFont.drawString("lines",controlX + controlDistance + controlButtons["lines"].x + 2, controlY + controlButtons["lines"].y + 10);
			hudFont.drawString("dots",controlX + controlDistance + controlButtons["dots"].x + 2, controlY + controlButtons["dots"].y + 10);
			hudFont.drawString("trails",controlX + controlDistance + controlButtons["trails"].x + 2, controlY + controlButtons["trails"].y + 10);
			hudFont.drawString("top",controlX + controlDistance + controlButtons["top"].x + 2, controlY + controlButtons["top"].y + 10);
			hudFont.drawString("bottom",controlX + controlDistance + controlButtons["bottom"].x + 2, controlY + controlButtons["bottom"].y + 10);
			hudFont.drawString("outline",controlX + controlDistance + controlButtons["outline"].x + 2, controlY + controlButtons["outline"].y + 10);
			hudFont.drawString("width",controlX + controlDistance + controlButtons["width1"].x + 2, controlY + controlButtons["width1"].y + 10);
			
			ofSetColor(fontColor);
			ofRect(controlX + controlDistance + controlButtons["start"].x, controlY + controlButtons["start"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["send"].x, controlY + controlButtons["send"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["send"].x + controlButtons["send"].w, controlY + controlButtons["send"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["restart"].x, controlY + controlButtons["restart"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["restart"].x + controlButtons["restart"].w, controlY + controlButtons["restart"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["start"].x + controlButtons["start"].w, controlY + controlButtons["start"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["lines"].x, controlY + controlButtons["lines"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["lines"].x + controlButtons["lines"].w, controlY + controlButtons["lines"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["dots"].x + controlButtons["dots"].w, controlY + controlButtons["dots"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["trails"].x + controlButtons["trails"].w, controlY + controlButtons["trails"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["top"].x + controlButtons["top"].w, controlY + controlButtons["top"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["bottom"].x + controlButtons["bottom"].w, controlY + controlButtons["bottom"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["outline"].x + controlButtons["outline"].w, controlY + controlButtons["outline"].y, 1, controlSize);
			ofRect(controlX + controlDistance + controlButtons["width1"].x, controlY + controlButtons["width1"].y, 1, controlSize*2);
			ofRect(controlX + controlDistance + controlButtons["width1"].x + controlButtons["width1"].w, controlY + controlButtons["width1"].y, 1, controlSize*2);
		}
	}
		
	// reset rect mode
	ofSetRectMode(OF_RECTMODE_CENTER);
	// red circles in hands
	if(tracked != NULL) {
		ofSetColor(0xff0000);
		ofCircle(ofMap(tracked->left_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->left_lower_arm.end.y, 0, 480, 0, 768, true),5);
		ofCircle(ofMap(tracked->right_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->right_lower_arm.end.y, 0, 480, 0, 768, true),5);
	}
}
/**/

void testApp::setupTrails() {
	int i;
	for (i=0;i<leftTrail.size();++i) {
		leftTrail[i] = ofPoint(-1000,-1000,2048);
		rightTrail[i] = ofPoint(-1000,-1000,2048);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	context.update();
	user.update();
	//updateInterface();
	updateRemote();
}

void testApp::updateRemote() {
	// check for waiting messages
	while( receiver.hasWaitingMessages() ){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		string cmd = m.getAddress();
		// check for mouse moved message
		if (cmd.find("/remote/")!=string::npos) {
			string btn = m.getAddress().substr(8);
			int val = floor(m.getArgAsFloat( 0 ));
			if (btn == "depth") {
				keyReleased('q');
			} else if (btn == "start") {
				keyReleased('s');
			} else if (btn == "restart") {
				keyReleased('r');
			} else if (btn == "trails") {
				keyReleased('w');
			} else if (btn == "top") {
				keyReleased('t');
			} else if (btn == "bottom") {
				keyReleased('b');
			} else if (btn == "dots") {
				keyReleased('d');
			} else if (btn == "lines") {
				keyReleased('l');
			} else if (btn == "outline") {
				keyReleased('o');
			} else if (btn == "fullscreen") {
				keyReleased('f');
			} else if (btn == "width") {
				lineWidth = val * 2;
			} else if (btn == "send" && val == 0) {
				isSending = !isSending;
			} else if (btn == "depth") {
				showDepth = !showDepth;
			} else if (btn == "debug") {
				debugMode = !debugMode;
			} else if (btn == "prev" && val == 0) {
				keyReleased(OF_KEY_LEFT);
			} else if (btn == "next" && val == 0) {
				keyReleased(OF_KEY_RIGHT);
			}
		}
	}
	if (settingsChanged) {
		settingsChanged = false;
		ofxOscBundle b;
		ofxOscMessage m1;
		m1.setAddress( "/remote/trails" );
		m1.addFloatArg( fontHasTrails ? 1 : 0 );
		b.addMessage(m1);
		ofxOscMessage m2;
		m2.setAddress( "/remote/top" );
		m2.addFloatArg( fontHasTop ? 1 : 0 );
		b.addMessage(m2);
		ofxOscMessage m3;
		m3.setAddress( "/remote/bottom" );
		m3.addFloatArg( fontHasBottom ? 1 : 0 );
		b.addMessage(m3);
		ofxOscMessage m4;
		m4.setAddress( "/remote/lines" );
		m4.addFloatArg( fontHasLines ? 1 : 0 );
		b.addMessage(m4);
		ofxOscMessage m5;
		m5.setAddress( "/remote/dots" );
		m5.addFloatArg( fontHasDots ? 1 : 0 );
		b.addMessage(m5);
		ofxOscMessage m6;
		m6.setAddress( "/remote/letter" );
		m6.addStringArg(letters.substr(currentLetter,1));
		b.addMessage(m6);
		ofxOscMessage m7;
		m7.setAddress( "/remote/width" );
		m7.addFloatArg(lineWidth/2);
		b.addMessage(m7);
		ofxOscMessage m8;
		m8.setAddress( "/remote/depth" );
		m8.addFloatArg( showDepth ? 1 : 0 );
		b.addMessage(m8);
		ofxOscMessage m9;
		m9.setAddress( "/remote/start" );
		m9.addFloatArg( hasStarted ? 1 : 0 );
		b.addMessage(m9);
		ofxOscMessage m10;
		m10.setAddress( "/remote/outline" );
		m10.addFloatArg( fontIsOutline ? 1 : 0 );
		b.addMessage(m10);
		ofxOscMessage m11;
		m11.setAddress( "/remote/fullscreen" );
		m11.addFloatArg( isFullScreen ? 1 : 0 );
		b.addMessage(m11);
		ofxOscMessage m12;
		m12.setAddress( "/remote/debug" );
		m12.addFloatArg( debugMode ? 1 : 0 );
		b.addMessage(m12);
		sender.sendBundle( b );
	}
}

void testApp::updateInterface() {
	if (controlIsActive) {
		float baseX = mouseX;
		float baseY = mouseY;
		int nTrackedUsers = user.getTrackedUsers().size();
		ofxTrackedUser *tracked = NULL;
		if (nTrackedUsers>0) {
			tracked = user.getTrackedUser(0);
		}
		if(tracked != NULL) {
			baseX = ofMap(tracked->right_lower_arm.end.x, 0, 640, 0, 1024, true);
			baseY = ofMap(tracked->right_lower_arm.end.y, 0, 480, 0, 768, true);
		}
		
		int i;
		string c;
		
		for (i=0; i<19; ++i) {
			c = controlNames[i];
			if (inButton(baseX, baseY, controlX + controlDistance + controlButtons[c].x, controlY + controlButtons[c].y, controlButtons[c].w, controlButtons[c].h)) {
				if (controlCurrent.compare(c)==0 && controlLast.compare(c)!=0 && controlWait < ofGetElapsedTimeMillis()-controlUpdate) {
					// same button...
					// over enough time
					controlUpdate = ofGetElapsedTimeMillis();
					toggleControl(i);
					controlLast = c;
				} else if (controlCurrent.compare(c)!=0) {
					// different button
					controlCurrent = c;
					controlUpdate = ofGetElapsedTimeMillis();
				}
				return;
			}
		}
		controlUpdate = ofGetElapsedTimeMillis();
		controlLast = "";
	}
}

void testApp::toggleControl(int i) {
	switch (i) {
		case 2:
			isSending = !isSending;
			break;
		case 4:
			fontHasDots = !fontHasDots;
			break;
		case 6:
			fontHasTop = !fontHasTop;
			break;
		case 7:
			fontHasBottom = !fontHasBottom;
			break;
		case 3:
			fontHasLines = !fontHasLines;
			break;
		case 5:
			fontHasTrails = !fontHasTrails;
			break;
		case 'q':
			showDepth = !showDepth;
			break;
		case 0:
			hasStarted = !hasStarted;
			if (hasStarted) lastLetterTime = ofGetElapsedTimeMillis();
			break;
		case 8:
			fontIsOutline = !fontIsOutline;
			break;
		case 1:
			currentLetter = 0;
			lastLetterTime = ofGetElapsedTimeMillis();
			userFont.clear();
			col = 0;
			row = 0;
			break;
		case 9:
			lineWidth = 2.0;
			break;
		case 10:
			lineWidth = 4.0;
			break;
		case 11:
			lineWidth = 6.0;
			break;
		case 12:
			lineWidth = 8.0;
			break;
		case 13:
			lineWidth = 10.0;
			break;
		case 14:
			lineWidth = 12.0;
			break;
		case 15:
			lineWidth = 14.0;
			break;
		case 16:
			lineWidth = 16.0;
			break;
		case 17:
			lineWidth = 18.0;
			break;
		case 18:
			lineWidth = 20.0;
			break;
		default:
			break;
	}
}

bool testApp::controlIsToggled(string c) {
	if (c.compare("dots")==0) {
		return fontHasDots;
	} else if (c.compare("top")==0) {
		return fontHasTop;
	} else if (c.compare("bottom")==0) {
		return fontHasBottom;
	} else if (c.compare("lines")==0) {
		return fontHasLines;
	} else if (c.compare("trails")==0) {
		return fontHasTrails;
	} else if (c.compare("start")==0) {
		return hasStarted;
	} else if (c.compare("outline")==0) {
		return fontIsOutline;
	}
	return false;
}

bool testApp::inButton(float x, float y, float sx, float sy, float sw, float sh) {
	return (x > sx && x < sx + sw && y > sy && y < sy + sh);
}

void testApp::updateTrails(deque <ofPoint> &trail, float x, float y, float z){
	// put the mouse x,y in the top slot
	trail.push_front(ofPoint(x,y,z));
	trail.pop_back();
}

//--------------------------------------------------------------
void testApp::draw(){
	if (!isSending) {
		if (showDepth) {
			ofSetRectMode(OF_RECTMODE_CORNER);
			depth.draw(previewX,previewY,previewWidth,previewHeight);
			ofSetRectMode(OF_RECTMODE_CENTER);
		}
		
		// draw the user
		drawUsers();
		
		// HELP
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofSetColor(fontColor);
		hudFont.drawString("S     : capture shape",10, 15);
		hudFont.drawString("L     : show/hide lines",10, 30);
		hudFont.drawString("D     : show/hide dots",10, 45);
		hudFont.drawString("<- -> : change letter",10, 60);
		hudFont.drawString("F     : fullscreen on/off",10, 75);
		hudFont.drawString("G     : generate font",10, 90);
		hudFont.drawString("Esc   : quit",10, 105);
		string debugging = "NO";
		if (debugMode) debugging = "YES";
		hudFont.drawString("Debug : " + debugging,10, 120);
		
		time_t rawtime;
		rawtime = time (NULL);
		stringstream st;
		st << rawtime;
		//hudFont.drawString("debug: " + st.str(),10, 120);
		
		inputFont.drawString("draw:", 810, 465);
		ofSetColor(0xcccccc);
		ofRect(800, 700, 224, 20);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(0x000000);
		letterFont.drawString(letters.substr(currentLetter,1), 800, 650);
		// /HELP
		
		// the game/app itself
		if (hasStarted) {
			// check time passed
			int elapsed = ofGetElapsedTimeMillis()-lastLetterTime;
			if (elapsed>timePerLetter) {
				// update timer
				lastLetterTime = ofGetElapsedTimeMillis();
				// save the frame
				captureLetterform();
				// go to the next letter
				hasStarted = false;
				settingsChanged = true;
				// old stuff, when letters skipped automatically
				if (false) {
					if (currentLetter<letters.size()-1) {
						currentLetter++;
					} else {
						hasStarted = false;
						currentLetter = 0;
					}
				}
				shutter = 255;
			} else {
				// show the time
				ofSetRectMode(OF_RECTMODE_CORNER);
				ofSetColor(255,255,255,170);
				ofRect(800, 468, 224, 300);
				ofSetColor(fontColor);
				timerFont.drawString(ofToString(round((timePerLetter-elapsed)/1000),0), 810, 440);
				//inputFont.drawString("draw:", 810, 465);
				ofSetColor(0xcccccc);
				if (elapsed>timePerLetter-500) ofSetColor(0xff0000);
				ofRect(800, 700, 224, 20);
				ofSetColor(0x000000);
				ofRect(800, 700, ofMap(timePerLetter-elapsed, 0, timePerLetter, 0, 224), 20);
				ofSetRectMode(OF_RECTMODE_CENTER);
				// show the letter
				//letterFont.drawString(letters.substr(currentLetter,1), 800, 650);
			}
		}
		
		// draw rec circle
		if (hasStarted) {
			ofSetColor(0xff0000);
			ofCircle(5, 5, 3);
		} else {
			ofSetColor(0xcccccc);
			ofCircle(5, 5, 3);
		}
		
		// draw all letters
		//drawLetters();
		
		// draw background rectangle
		ofSetColor(255, 0, 0);
		ofNoFill();
		ofSetLineWidth(1.0);
		ofRect(512, 384, letterWidth, letterHeight);
		ofFill();
		
		// draw the interface buttons
		//drawInterface();
		
		int nTrackedUsers = user.getTrackedUsers().size();
		ofxTrackedUser *tracked = NULL;
		if (nTrackedUsers>0) {
			tracked = user.getTrackedUser(0);
		}
		if(tracked == NULL) {
			ofSetColor(0xFFFFFF);
			ofSetRectMode(OF_RECTMODE_CORNER);
			depth.draw(previewX,previewY,previewWidth,previewHeight);
			poseImage.draw(0,0,1024,768);
			ofSetRectMode(OF_RECTMODE_CENTER);
		}
	} else {
		if (!nameIsSet) {
			showTextBox(1);
		} else if (!emailIsSet) {
			showTextBox(2);
		} else {
			ofSetColor(0xff0000);
			captureBlank();
			timerFont.drawString("saving", 300, 360);
			createFont();
			userEmail = "";
			userName = "";
			emailIsSet = false;
			nameIsSet = false;
			isSending = false;
		}
	}
	// snap photo effect
	if (shutter>0) {
		ofSetRectMode(OF_RECTMODE_CORNER);
		shutter = shutter-shutterDecay;
		ofSetColor(255, 255, 255, shutter);
		ofRect(0, 0, 1024, 768);
		ofSetRectMode(OF_RECTMODE_CENTER);
		if (shutter<=0) shutter = 0;
	}
	if (showExtraFeeds) {
		depth.draw(604, 638, 240, 180);
		rgbimage.draw(854,638,-240,180);
	}
}

void testApp::drawTrails(int color, int outlineOffset){
	int s = trailSize;
	int i;
	ofSetColor(color);
	for (i=0;i<s;++i) {
		leftTrail[i].z += trailDecay;
		lineToRect(color, leftTrail[i].x, 
				   leftTrail[i].y, 
				   leftTrail[i+1].x, 
				   leftTrail[i+1].y, (maxDot+minDot)-ABS(ofMap(leftTrail[i].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
		ofCircle(leftTrail[i].x, leftTrail[i].y, (maxDot+minDot)-ABS(ofMap(leftTrail[i].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
		if (rightHand) {
			rightTrail[i].z += trailDecay;
			lineToRect(color, rightTrail[i].x, 
					   rightTrail[i].y, 
					   rightTrail[i+1].x, 
					   rightTrail[i+1].y, (maxDot+minDot)-ABS(ofMap(rightTrail[i].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
			ofCircle(rightTrail[i].x, rightTrail[i].y, (maxDot+minDot)-ABS(ofMap(rightTrail[i].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
		}
	}
	ofCircle(leftTrail[0].x, leftTrail[0].y, (maxDot+minDot)-ABS(ofMap(leftTrail[0].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
	if (rightHand) {
		ofCircle(rightTrail[0].x, rightTrail[0].y, (maxDot+minDot)-ABS(ofMap(rightTrail[0].z, 0, 2048, minDot, maxDot, true))+outlineOffset);
	}
}

void testApp::drawLetters() {
    int i;
    ofImage img;
    for (i=0;i<userFont.size();++i) {
        img.loadImage("output/" + userFont[i].filename);
        ofSetColor(0xffffff);
        img.draw(userFont[i].xpos+letterWidth/maxCols,userFont[i].ypos+letterHeight/maxCols,letterWidth,letterHeight);
    }
}

void testApp::drawUsers() {
	int i;
	int nTrackedUsers = user.getTrackedUsers().size();
	ofxTrackedUser *tracked = NULL;
	for (i=0; i<nTrackedUsers; i++) {
		// get the user
		tracked = user.getTrackedUser(i);
		if(tracked != NULL) {
            if (fontIsOutline) {
                // thick pass
                outlinePass = 1;
                if (fontHasLines) {
                    userLinesDraw(tracked);
                }
                if (fontHasDots) {
                    userDotsDraw(tracked);
                }
                // thin pass
                outlinePass = 0;
                if (fontHasLines) {
                    userLinesDraw(tracked);
                }
                if (fontHasDots) {
                    userDotsDraw(tracked);
                }
            } else {
                if (fontHasLines) {
                    userLinesDraw(tracked);
                }
                if (fontHasDots) {
                    userDotsDraw(tracked);
                }
            }
		}
	}
	// draw light paint for tracked #1
	if (nTrackedUsers>0) {
		tracked = user.getTrackedUser(0);
	}
	if(tracked != NULL) {
		if (fontHasTrails) {
			updateTrails(leftTrail, ofMap(tracked->left_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->left_lower_arm.end.y, 0, 480, 0, 768, true), tracked->left_lower_arm.end.z);
			updateTrails(rightTrail, ofMap(tracked->right_lower_arm.end.x, 0, 640, 0, 1024, true), ofMap(tracked->right_lower_arm.end.y, 0, 480, 0, 768, true), tracked->right_lower_arm.end.z);
			if (fontIsOutline) {
				drawTrails(fontColor,3);
				drawTrails(0xffffff,0);
			} else {
				drawTrails(fontColor,0);
			}
		}
	}
	if (mouseMode && tracked == NULL) {
		updateTrails(leftTrail, mouseX, mouseY, 100);
		if (fontIsOutline) {
			drawTrails(fontColor,3);
			drawTrails(0xffffff,0);
		} else {
			drawTrails(fontColor,0);
		}
	}
}

void testApp::userDotsDraw(ofxTrackedUser *skeleton) {
	//drawDot(skeleton->neck);
	
    if (fontHasTop) {
        // left arm + shoulder
        drawDot(skeleton->left_shoulder);
        drawDot(skeleton->left_upper_arm);
        drawDot(skeleton->left_lower_arm);
        
        // right arm + shoulder
        drawDot(skeleton->right_shoulder);
        drawDot(skeleton->right_upper_arm);
        drawDot(skeleton->right_lower_arm);
        
        // upper torso
        drawDot(skeleton->left_upper_torso);
        drawDot(skeleton->right_upper_torso);
    }
	
    if (fontHasBottom) {
        // left lower torso + leg
        drawDot(skeleton->left_lower_torso);
        drawDot(skeleton->left_upper_leg);
        drawDot(skeleton->left_lower_leg);
        
        // right lower torso + leg
        drawDot(skeleton->right_lower_torso);
        drawDot(skeleton->right_upper_leg);
        drawDot(skeleton->right_lower_leg);
        
        drawDot(skeleton->hip);
    }
}

void testApp::userLinesDraw(ofxTrackedUser *skeleton) {
	//drawLine(skeleton->neck);
	
    if (fontHasTop) {
        // left arm + shoulder
        drawLine(skeleton->left_shoulder);
        drawLine(skeleton->left_upper_arm);
        drawLine(skeleton->left_lower_arm);
        
        // right arm + shoulder
        drawLine(skeleton->right_shoulder);
        drawLine(skeleton->right_upper_arm);
        drawLine(skeleton->right_lower_arm);
        
        // upper torso
        drawLine(skeleton->left_upper_torso);
        drawLine(skeleton->right_upper_torso);
    }
	
    if (fontHasBottom) {
        // left lower torso + leg
        drawLine(skeleton->left_lower_torso);
        drawLine(skeleton->left_upper_leg);
        drawLine(skeleton->left_lower_leg);
        
        // right lower torso + leg
        drawLine(skeleton->right_lower_torso);
        drawLine(skeleton->right_upper_leg);
        drawLine(skeleton->right_lower_leg);
        
        drawLine(skeleton->hip);
    }
}

void testApp::drawLine(ofxLimb &limb) {
	/**/
    float myWidth = lineWidth;
    if (!fontIsOutline) {
		lineToRect(fontColor, ofMap(limb.begin.x, 0, 640, 0, 1024, true), ofMap(limb.begin.y, 0, 480, 0, 768, true), ofMap(limb.end.x, 0, 640, 0, 1024, true), ofMap(limb.end.y, 0, 480, 0, 768, true), myWidth);	
    } else if (fontIsOutline && outlinePass==1) {
        myWidth += 2.0;
		lineToRect(fontColor, ofMap(limb.begin.x, 0, 640, 0, 1024, true), ofMap(limb.begin.y, 0, 480, 0, 768, true), ofMap(limb.end.x, 0, 640, 0, 1024, true), ofMap(limb.end.y, 0, 480, 0, 768, true), myWidth);	
    } else {
		lineToRect(0xffffff, ofMap(limb.begin.x, 0, 640, 0, 1024, true), ofMap(limb.begin.y, 0, 480, 0, 768, true), ofMap(limb.end.x, 0, 640, 0, 1024, true), ofMap(limb.end.y, 0, 480, 0, 768, true), myWidth);	
    }
}

void testApp::lineToRect(int color, float x1, float y1, float x2, float y2, float radius) {
	ofSetColor(color);
	float theta = atan2((y2-y1),(x2-x1));
	float x11 = cos((.5*PI)+theta) * radius + x1;
	float y11 = sin((.5*PI)+theta) * radius + y1;
	float x12 = -cos((.5*PI)+theta) * radius + x1;
	float y12 = -sin((.5*PI)+theta) * radius + y1;
	float x21 = cos((.5*PI)+theta) * radius + x2;
	float y21 = sin((.5*PI)+theta) * radius + y2;
	float x22 = -cos((.5*PI)+theta) * radius + x2;
	float y22 = -sin((.5*PI)+theta) * radius + y2;
	ofBeginShape();
	ofVertex(x11, y11);
	ofVertex(x12, y12);
	ofVertex(x22, y22);
	ofVertex(x21, y21);
	ofEndShape();
	if (false) {
		ofSetColor(0xff0000);
		ofCircle(x11, y11, 4);
		ofSetColor(0x00ff00);
		ofCircle(x12, y12, 4);
		ofSetColor(0x0000ff);
		ofCircle(x21, y21, 4);
		ofSetColor(0xff00ff);
		ofCircle(x22, y22, 4);
		ofSetColor(0xcccccc);
		ofCircle(x1, y1, 4);
		ofSetColor(0x333333);
		ofCircle(x2, y2, 4);
	}
}

void testApp::drawDot(ofxLimb &limb) {
	/**/
    float radius;
	if (fontHasLines) {
		radius = lineWidth;
	} else {
		radius = 0;
	}
    if (!fontIsOutline) {
        ofSetColor(fontColor);
    } else if (fontIsOutline && outlinePass==1) {
        radius += 3;
        ofSetColor(fontColor);
    } else {
        ofSetColor(0xffffff);
    }
	/**
	ofCircle(ofMap(limb.begin.x,0,1024,previewX,previewX+previewWidth), ofMap(limb.begin.y,0,768,previewY,previewY+previewHeight), radius+(maxDot+minDot)-ABS(ofMap(limb.begin.z, 0, 2048, minDot, maxDot, true)));
	ofCircle(ofMap(limb.end.x,0,1024,previewX,previewX+previewWidth), ofMap(limb.end.y,0,768,previewY,previewY+previewHeight), radius+(maxDot+minDot)-ABS(ofMap(limb.end.z, 0, 2048, minDot, maxDot, true)));
	**/
	
	ofCircle(ofMap(limb.begin.x, 0, 640, 0, 1024, true), ofMap(limb.begin.y, 0, 480, 0, 768, true), radius+(maxDot+minDot)-ABS(ofMap(limb.begin.z, 0, 2048, minDot, maxDot, true)));
	ofCircle(ofMap(limb.end.x, 0, 640, 0, 1024, true), ofMap(limb.end.y, 0, 480, 0, 768, true), radius+(maxDot+minDot)-ABS(ofMap(limb.end.z, 0, 2048, minDot, maxDot, true)));
}

void testApp::showTextBox(int type) {
	ofSetColor(fontColor);
	int startCursor = 180;
	inputFont.drawString("generate your font!", xini, yini+20);
	if (type == 1) {
		// name
		inputFont.drawString("font name:", xini, yini+60);
		ofSetColor(0xff0000);
		inputFont.drawString(userName, xini + startCursor, yini+60);
	}  else if (type == 2) {
		// email
		inputFont.drawString("your email:", xini, yini+60);
		ofSetColor(0xff0000);
		inputFont.drawString(userEmail, xini + startCursor, yini+60);
	}
}

void testApp::createFont() {
	if (isSending && nameIsSet && emailIsSet) {
		string basedir = ofToDataPath("output/", true);
		string commanddir = basedir + "fontforgecommand.pe";
		string cmd = "";
		ofstream outfile (commanddir.c_str());
		outfile << "#!/opt/local/bin/fontforge\n";
		outfile << "i=1\nglyph = \"\"\nNew()\nSetFontNames(\"" + userName + "\",\"" + userName + "\",\"" + userName + "\",\"\",\"2011 " + userName + " " + userEmail + " and mauricio giraldo www.mauriciogiraldo.com Creative Commons Attribution-ShareAlike 3.0 Unported License. http://creativecommons.org/licenses/by-sa/3.0/\",\"1.0\")\nwhile ( i < $argc )\nbasepath = $argv[i]:r\nglyph = Strsub(basepath,Strrstr(basepath,\"/\")+1)\nSelect(glyph)\nImport($argv[i])\nAutoHint()\nAutoTrace()\ni = i + 1\nendloop\nGenerate(\"" + basedir + userName + "font.ttf" + "\")";
		outfile.close();
		// make executable
		cmd = "chmod +x " + commanddir;
		system(cmd.c_str());
		// execute command
		cmd = commanddir + " " + basedir + "*.bmp";
		system(cmd.c_str());
		if (!debugMode) {
			// erase command file and bmps
			cmd = "rm " + commanddir;
			system(cmd.c_str());
		}
		// send email
		// cmd = "/usr/bin/php /Applications/MAMP/htdocs/mga/lab/bodytype/sendfont.php " + userName + " " + userEmail;
		// create ZIP
		cmd = "zip -j " + basedir + "font_" + userEmail + ".zip " + basedir + "*.bmp " + basedir + userName + "font.ttf";
		system(cmd.c_str());
		if (!debugMode) {
			// clean up the mess
			cmd = "rm " + basedir + "*.bmp";
			system(cmd.c_str());
			cmd = "rm " + basedir + "*.ttf";
			system(cmd.c_str());
		}
	}
	// zip
//	time_t t;
//	t = time(NULL);
//	stringstream st;
//	st << t;
//	cmd = "zip -1 -q -r -j " + basedir +  "packet" + st.str() + ".zip " + basedir;
//	printf(cmd.c_str());
//	system(cmd.c_str());
}

void testApp::captureBlank() {
    ofImage img;
    img.grabScreen(512-letterWidth*.5, 384-letterHeight*.5, letterWidth, letterHeight);
    string filename;
    filename = " .bmp";
	img.saveImage("output/" + filename);
}

void testApp::captureLetterform() {
	shutterSound.play();
    ofImage img;
    //img.grabScreen(previewX+(previewWidth*.5)-(letterWidth), previewY+(previewHeight*.5)-(letterHeight), letterWidth*2, letterHeight*2);
    img.grabScreen(512-letterWidth*.5, 384-letterHeight*.5, letterWidth, letterHeight);
    // put saved frame in place
	string prefix = letters.substr(currentLetter,1);
    string filename;
    filename = prefix+".bmp";
	img.saveImage("output/" + filename);
	string cmd;
    string basedir = ofToDataPath("output/", true);
	//string convertPath = getAuxiliaryExecutablePath("convert");
	string convertPath = "/opt/local/bin/convert";
    cmd = convertPath + " " + basedir + prefix + ".bmp -colorspace Gray -colors 2 " + basedir + prefix + ".png";
    system(cmd.c_str());
	/**/
    cmd = convertPath + " " + basedir + prefix + ".png " + basedir + prefix + ".bmp";
    system(cmd.c_str());
    cmd = "rm " + basedir + prefix + ".png";
    system(cmd.c_str());
	/**/
	img.clear();
	// vintage stuff to show all the letters a user has created
    letterform theLetter;
    theLetter.xpos = xini + (col*(letterWidth/maxCols + letterSpace));
    theLetter.ypos = yini + (row*(letterHeight/maxCols + letterSpace));
    theLetter.filename = filename;
    userFont.push_back(theLetter);
    if (col+1>=maxCols) {
        col=0;
        row++;
    } else {
        col++;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

int spc_email_isvalid(const char *address) {
	int        count = 0;
	const char *c, *domain;
	const char *rfc822_specials = "()<>@,;:\\\"[]";
	
	/* first we validate the name portion (name@domain) */
	for (c = address;  *c;  c++) {
		if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == 
						   '\"')) {
			while (*++c) {
				if (*c == '\"') break;
				if (*c == '\\' && (*++c == ' ')) continue;
				if (*c < ' ' || *c >= 127) return 0;
			}
			if (!*c++) return 0;
			if (*c == '@') break;
			if (*c != '.') return 0;
			continue;
		}
		if (*c == '@') break;
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
	}
	if (c == address || *(c - 1) == '.') return 0;
	
	/* next we validate the domain portion (name@domain) */
	if (!*(domain = ++c)) return 0;
	do {
		if (*c == '.') {
			if (c == domain || *(c - 1) == '.') return 0;
			count++;
		}
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
	} while (*++c);
	
	return (count >= 1);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if (!isSending) {
		if (key == 'z' || key == 'f' || key == 'p' || key == 'd' || key == 't' || key == 'b' || key == 'l' || key == 'w'
			 || key == 'q' || key == 's' || key == 'o' || key == 'r' || key == '1' || key == '2' || key == '3' || key == '4'
			 || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0' || key == OF_KEY_LEFT 
			 || key == OF_KEY_RIGHT) {
			settingsChanged = true;
		}
		switch (key) {
			case 'f':
				if (!isSending) ofToggleFullscreen();
				isFullScreen = !isFullScreen;
				break;
			case 'z':
				debugMode = !debugMode;
				break;
			case 'g':
				isSending = !isSending;
				break;
			case 'd':
				fontHasDots = !fontHasDots;
				break;
			case 't':
				fontHasTop = !fontHasTop;
				break;
			case 'b':
				fontHasBottom = !fontHasBottom;
				break;
			case 'l':
				fontHasLines = !fontHasLines;
				break;
			case 'w':
				fontHasTrails = !fontHasTrails;
				break;
			case 'q':
				showDepth = !showDepth;
				break;
			case 's':
				hasStarted = !hasStarted;
				if (hasStarted) lastLetterTime = ofGetElapsedTimeMillis();
				break;
			case 'o':
				fontIsOutline = !fontIsOutline;
				break;
			case 'r':
				currentLetter = 0;
				lastLetterTime = ofGetElapsedTimeMillis();
				userFont.clear();
				col = 0;
				row = 0;
				break;
			case '1':
				lineWidth = 2.0;
				break;
			case '2':
				lineWidth = 4.0;
				break;
			case '3':
				lineWidth = 6.0;
				break;
			case '4':
				lineWidth = 8.0;
				break;
			case '5':
				lineWidth = 10.0;
				break;
			case '6':
				lineWidth = 12.0;
				break;
			case '7':
				lineWidth = 14.0;
				break;
			case '8':
				lineWidth = 16.0;
				break;
			case '9':
				lineWidth = 18.0;
				break;
			case '0':
				lineWidth = 20.0;
				break;
			case OF_KEY_LEFT:
				if (currentLetter>0) {
					currentLetter--;
				}
				lastLetterTime = ofGetElapsedTimeMillis();
				break;
			case OF_KEY_RIGHT:
				if (currentLetter<letters.size()-1) {
					currentLetter++;
				}
				lastLetterTime = ofGetElapsedTimeMillis();
				break;
				
			default:
				break;
		}
	} else {
		if (!nameIsSet) {
			if (key != OF_KEY_RETURN) {
				if (key != OF_KEY_BACKSPACE) {
					if (key != ' ') userName += key;
				} else {
					if (userName.size()>0) {
						userName.erase(userName.size()-1,1);
					}
				}
			} else {
				if (userName.size()>0) nameIsSet = true;
			}
		} else if (!emailIsSet) {
			if (key != OF_KEY_RETURN && key != ' ') {
				if (key != OF_KEY_BACKSPACE) {
					if (key != ' ') userEmail += key;
				} else {
					if (userEmail.size()>0) {
						userEmail.erase(userEmail.size()-1,1);
					}
				}
			} else {
				if (userEmail.size()>0 && spc_email_isvalid(userEmail.c_str())==1) emailIsSet = true;
			}
		}
	}

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

////////

std::string testApp::getAuxiliaryExecutablePath(const std::string& executableName) {
	CFStringRef executableNameString = CFStringCreateWithCString(kCFAllocatorDefault, executableName.c_str(), kCFStringEncodingUTF8);
	CFURLRef convertURL = CFBundleCopyAuxiliaryExecutableURL(CFBundleGetMainBundle(), executableNameString);
	assert(convertURL != NULL);
	std::vector<char> stringBuffer(1024 + 1, ' ');	// Include space for zero terminator
	Boolean success = CFURLGetFileSystemRepresentation(convertURL, true, reinterpret_cast<UInt8*>(&stringBuffer[0]), stringBuffer.size());
	assert(success);
	CFRelease(executableNameString);
	CFRelease(convertURL);
	return std::string(&stringBuffer[0]);
}