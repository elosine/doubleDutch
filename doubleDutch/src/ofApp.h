#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxBullet.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 57120
#define PORTR 12321

class ofApp : public ofBaseApp
{
public:
    static const unsigned NUM_BOXES = 100;
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    // scene stuff
    ofxPostProcessing post;
    ofEasyCam cam;
    ofLight light;
    
    // boxes
    vector<ofVec3f> posns;
    vector<ofColor> cols;
    ofVboMesh boxMesh;
    
    ofxBulletWorldSoft			world;    
//  ofxBulletRope*              rope;
    vector<ofxBulletRope*>  ropes;
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    float amp, freq;
    
    ofColor colors[10];
    
    int zs[10];
    int ys[10];
    
//    vector< vector<ofSpherePrimitive> > spheres;
    
    std::vector< std::vector <ofSpherePrimitive> > spheres;
    
//    std::vector< std::vector <ofMesh> > sMesh;
//    
//    vector<ofVec3f> iVerts[10][102];
//    
//    vector<ofVec3f> tv;
//    
//    ofMesh tm;
    
    ofMesh planet;
    
    vector<ofVec3f> iVerts;
    
    int sw1;
    
    int fn = 5;
    

    

};









