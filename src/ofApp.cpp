#include "ofApp.h"

vector<ofPolyline> lines;

vector<glm::vec3> baseVertices;

glm::vec3 arbPoint;

int res = 25;

float rotation = 3*2+1;
float baseCount = 3 * 2 + 1;

glm::vec3 speedVector;

ofImage img;
bool startRec = false;

ofFbo fbo;

float stopSinCurve = 0;

class CCurve {
public:
	vector<glm::vec3> vertices;

	CCurve() {

	}

	CCurve(vector<glm::vec3>* _vertices) {
		vertices = *_vertices;
	}
};

vector<CCurve> curves;

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);

	fbo.allocate(2816, 1720, GL_RGB, 4);
	fbo.begin();
	ofClear(ofColor(26.0f));
	fbo.end();

	gui.setup();
	gui.add(arbPoint.setup("Base Point", ofVec3f(125.0f, 50.0f, -50.0f), ofVec3f(-100, -100, -100), ofVec3f(100, 100, 100)));
	gui.add(sinX.setup("X Period", 5, -20, 20));
	gui.add(sinPeriod.setup("Sin Period", 4, -20, 20));
	gui.add(sinAmp.setup("Sin Amp", 20, 0, 100));
	gui.add(curveZ.setup("Curve Z", 52, -100, 100));

	group.add(rotationCountGui.set("Rotation Count", 51, 0, 51));
	group.add(baseCountGui.set("Base Count", 4, 0, 20));

	gui.add(group);
	gui.add(transparency.setup("Transparancy", 232, 0, 255));

	gui.add(redPhase.setup("Red Phase", 0, 0, 2 * PI));
	gui.add(greenPhase.setup("Green Phase", 0, 0, 2 * PI));
	gui.add(bluePhase.setup("Blue Phase", 0, 0, 2 * PI));

	gui.add(redAmp.setup("Red Amp", 255, 0, 255));
	gui.add(greenAmp.setup("Green Amp", 255, 0, 255));
	gui.add(blueAmp.setup("Blue Amp", 255, 0, 255));

	gui.add(colorPeriod.setup("Color Period", 17.6, 0, 20));

	gui.add(lineWidth.setup("Line Width", 2.8, 0, 20));

	gui.add(cameraDistance.setup("Camera Dist", 200, 100, 250));

	rotation = 3 * rotationCountGui + 1;
	baseCount = 3 * baseCountGui + 1;
	//arbPoint = glm::vec3(125.0f, 50.0f, -50.0f);

	//Generate base curve
	for (int i = 0; i < baseCount; i++) {

		glm::vec3 v = glm::vec3(i * sinX, sin(i * sinPeriod) * sinAmp, (float)curveZ);

		baseVertices.push_back(v);

	}


	for (float i = 0; i < rotation; i++) {

		CCurve curve;

			for (int j = 0; j < baseVertices.size(); j++) {
		
			glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), (float)glm::radians(i/rotation * 360), baseVertices.at(j));
			glm::vec3 n_point = glm::vec3(glm::vec4((glm::vec3)arbPoint, 1.0f) * rot_mat);

			curve.vertices.push_back(n_point);
		}

		curves.push_back(curve);
	}

	for (int i = 0; i < curves.size(); i++) {
		ofPolyline line;
		
		line.addVertices(evalBspline(curves.at(i).vertices, res));
		line.close();

		lines.push_back(line);
	}

		ofPolyline line;
		line.addVertices(evalBspline(baseVertices, res));
		lines.push_back(line);
}

//--------------------------------------------------------------
void ofApp::update(){

	rotation = 3 * rotationCountGui + 1;
	baseCount = 3 * baseCountGui + 1;

	float t = ofGetFrameNum()/60.0f;

	speedVector = glm::vec3(ofNoise(t / 4)*200-50, ofNoise(t / 4 + 50)*200-50, ofNoise(t / 4 + 500)*200-50);
	arbPoint = speedVector;

	sinPeriod = t / 30;

	sinX = sin(t / 10) * 20;

	stopSinCurve = sin(sinPeriod);

	for (int i = 0; i < baseCount; i++) {
		glm::vec3 v = glm::vec3(i * sinX, sin(i * sinPeriod) * sinAmp, (float)curveZ);

		baseVertices.at(i) = v;

	}


	for (float i = 0; i < curves.size(); i++) {

		for (int j = 0; j < curves.at(i).vertices.size(); j++) {

			glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), (float)glm::radians(i / rotation * 360), baseVertices.at(j));
			glm::vec3 n_point = glm::vec3(glm::vec4((glm::vec3)arbPoint, 1.0f) * rot_mat);

			curves.at(i).vertices.at(j) = n_point;
		}
	}

	lines.clear();

	for (int i = 0; i < curves.size(); i++) {
		ofPolyline line;

		line.addVertices(evalBspline(curves.at(i).vertices, res));
		line.close();

		lines.push_back(line);
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	
	fbo.begin();

	ofBackground(26.0f);

	cam.setDistance(cameraDistance);

	cam.begin();

	ofEnableDepthTest();

	float t = ofGetFrameNum()/60.0f;

	ofSetLineWidth(lineWidth);

	for (float i = 0; i < lines.size()-1; i++) {

		float phase = i / colorPeriod + t;

		float brightness = (sin(phase) * 0.5 + 0.5) * (255.0f - 26.0f) + 26.0f;
		ofSetColor(ofColor::fromHsb(24.0f/360.0f*255.0f,255.0f,brightness,brightness * transparency/255.0f));
		
		lines.at(i).draw();
	}

	/*for (int i = 0; i < curves.size(); i++) {
		for (int j = 0; j < curves.at(i).vertices.size(); j++) {
			ofDrawSphere(curves.at(i).vertices.at(j), 0.5);
		}
	}*/

	/*for (int i = 0; i < baseVertices.size(); i++) {
		ofDrawSphere(baseVertices.at(i), 1);
	}*/

	/*ofSetColor(255, 0, 0);
	ofDrawSphere(arbPoint, 1);

	ofSetColor(0, 255, 0);
	ofDrawSphere(0, 0, 0, 1);*/

	ofDisableDepthTest();

	cam.end();

	fbo.end();

	ofBackground(0.0f);

	fbo.draw(0.0f, 0.0f, ofGetWidth(), ofGetHeight());

	if (startRec) {
		ofPixels pixels;
		fbo.readToPixels(pixels);
		img.setFromPixels(pixels);

		img.save("output/render" + to_string(ofGetFrameNum()) + ".jpg");
	}

	gui.draw();
}

void ofApp::listenerFunction() {

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'h') {
		startRec = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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