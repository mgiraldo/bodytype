#ifndef _TEST_APP
#define _TEST_APP

// uncomment this for OF 007 compatibility
//#define OFX_OPENNI_BETA

#include "ofMain.h"
#include "ofxOpenNI.h"
#include <deque>
#include <time.h>

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void drawUsers();
		void userDotsDraw(ofxTrackedUser *skeleton);
        void userLinesDraw(ofxTrackedUser *skeleton);
		void drawLine(ofxLimb &limb);
		void lineToRect(int color, float x1, float y1, float x2, float y2, float radius);
        void drawDot(ofxLimb &limb);
		void showTextBox(int type);
        void createFont();
		bool controlIsToggled(string c);
		void toggleControl(int i);
        void captureLetterform();
		void captureBlank();
        void drawLetters();
		void setupRemote();
		void updateRemote();
		void setupTrails();
		void drawInterface();
		void updateInterface();
		void setupControls();
		bool inButton(float x, float y, float sx, float sy, float sw, float sh);
		void updateTrails(deque <ofPoint> &trail, float x, float y, float z);
		void drawTrails(int color, int outlineOffset);
		std::string getAuxiliaryExecutablePath(const std::string& executableName);
		string deviceId;
		ofxOpenNIContext context;
		ofxDepthGenerator depth;
		ofxUserGenerator user;
		ofxImageGenerator rgbimage;
};

#endif
