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
    
    void pitchDetect(int i, float pitchL, float pitchH);
    void chgClr(int idx, string palette);
    

    
    // scene stuff
    ofxPostProcessing post;
    ofEasyCam cam;
    ofLight light;
    
    ofxBulletWorldSoft			world;    
    vector<ofxBulletRope*>  ropes;
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    float amp, freq;
    
    std::array<ofColor, 9> activeClrs;
    
    int zs[10];
    int ys[10];
    
    
    std::vector< std::vector <ofSpherePrimitive> > spheres;
    
    std::vector< std::vector <ofMesh> > sMesh;
    std::vector< std::vector <ofPoint> > ctrs;

    std::vector< std::vector < std::vector <ofVec3f> > > sVerts;
        
    int fn = 5;
    
    int numRopes = 9;
    int numNodes = 30;
    
    ofPoint ctr;
    
    int aniTs[9];
    
    int fRangeL = 30;
    int fRangeH = 120;
    
    int vSizeL = 5;
    int vSizeH = 50;
    
    int nyRange = 50;
    
    vector<int> pNodes;
    
    map< string, std::array<ofColor, 9> > palettes;
    
    int ropeMode = 0;
    
    ofVec3f pPsA, pPsZ;
    
    
    ofxBulletBox*				ground;
    ofxBulletBox*				ceiling;
    ofxBulletBox*				lWall;
    ofxBulletBox*				rWall;
    ofxBulletBox*				back;
    ofxBulletBox*				front;
    

};









