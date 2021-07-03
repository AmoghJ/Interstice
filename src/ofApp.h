#pragma once

#include "ofMain.h"
#include "ofxCurve.h"
#include "ofxGui/src/ofxGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofEasyCam cam;

		ofxPanel gui;
		ofxVec3Slider arbPoint;

		ofxFloatSlider sinX;
		ofxFloatSlider sinPeriod;
		ofxFloatSlider sinAmp;
		ofxFloatSlider curveZ;

		ofxFloatSlider transparency;

		ofxFloatSlider redPhase;
		ofxFloatSlider greenPhase;
		ofxFloatSlider bluePhase;

		ofxFloatSlider redAmp;
		ofxFloatSlider greenAmp;
		ofxFloatSlider blueAmp;

		ofxFloatSlider colorPeriod;

		ofxFloatSlider lineWidth;

		ofParameter<int> rotationCountGui;
		ofParameter<int> baseCountGui;

		ofParameterGroup group;

		ofxFloatSlider cameraDistance;

		void listenerFunction();
};
