#include "ofApp.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

void ofApp::setup(){
    
    ofBackground(0);
    sender.setup(HOST, PORT);
    receiver.setup(PORTR);
    
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    
    nodeinc = ofGetWidth()/numNodes;
    stringMid = floor(numStringNodes/2);
    
    
    world.setup();
    world.setGravity(ofVec3f(0,-50, 0));
    world.setCamera(&cam);
    
    ceiling = new ofxBulletBox();
    ceiling->create( world.world, ofVec3f(0., (ofGetHeight()/2)-200, 0.), 0., 2000., 50.f, 2000.f );
    ceiling->setProperties(.25, .95);
    ceiling->add();

    lWall = new ofxBulletBox();
    lWall->create( world.world, ofVec3f((-ofGetWidth()/2)-5, 0., 0.), 0., 50.f, 2000.f, 2000.f );
    lWall->setProperties(.25, .95);
    lWall->add();
    
    rWall = new ofxBulletBox();
    rWall->create( world.world, ofVec3f((ofGetWidth()/2)+5, 0., 0.), 0., 50.f, 2000.f, 2000.f );
    rWall->setProperties(.25, .95);
    rWall->add();

    back = new ofxBulletBox();
    back->create( world.world, ofVec3f(0., 0., -ofGetHeight()/2), 0., 2000.f, 2000.f, 50.f );
    back->setProperties(.25, .95);
    back->add();
    
    front = new ofxBulletBox();
    front->create( world.world, ofVec3f(0., 0., ofGetHeight()/2), 0., 2000.f, 2000.f, 50.f );
    front->setProperties(.25, .95);
    front->add();

    ground = new ofxBulletBox();
    ground->create( world.world, ofVec3f(0., (-ofGetHeight()/2)+200, 0.), 0., 2000., 50.f, 2000.f );
    ground->setProperties(.25, .95);
    ground->add();
    
      

    for(int i=0; i<numRopes; i++){
        zs[i] = ofRandom(-150, 150);
        ys[i] = ofRandom(-170, 170);
        ofVec3f nodeA = ofVec3f( -ofGetWidth()/2, ys[i], zs[i]);
        ofVec3f nodeZ = ofVec3f( ofGetWidth()/2,ys[i],zs[i]);
        ropes.push_back( new ofxBulletRope() );
        ((ofxBulletRope*)ropes[i])->create( &world, nodeA, nodeZ, numNodes );
        ropes[i]->add();
        ropes[i]->setMass(10.4f);
        ropes[i]->setStiffness(1, 1, 1);
        ropes[i]->setFixedAt(0);
        ropes[i]->setFixedAt(numNodes+1);
    }
    
    for(int i=0;i<ropes.size();i++){
        strings.push_back(std::vector<ofxBulletRope*>());
        
        for(int j=0;j<numNodes;j++){
            strings[i].push_back( new ofxBulletRope() );
            ((ofxBulletRope*)strings[i][j])->create( &world, ofVec3f( -(ofGetWidth()/2) + (nodeinc*j), ys[i]-100, zs[i]), ofVec3f(-(ofGetWidth()/2) + (nodeinc*j), ys[i]+100, zs[i]), numStringNodes );
            strings[i][j]->add();
            strings[i][j]->setMass(10.4f);
            strings[i][j]->setStiffness(10,10,10);

        }
    }
    

    
//  Fill Spheres Array
    for(int i=0;i<ropes.size();i++){
        spheres.push_back(std::vector<ofSpherePrimitive>());
        sMesh.push_back(std::vector<ofMesh>());
        ctrs.push_back(std::vector<ofPoint>());

        for(int j=0;j<numNodes;j++){
            spheres[i].push_back( ofSpherePrimitive(30,8) );
            
            sMesh[i].push_back( spheres[i][j].getMesh() );
            ctrs[i].push_back( sMesh[i][j].getCentroid() );
        }
    }
    
    for(int i=0;i<ropes.size();i++){
        for(int j=0;j<numNodes;j++){
            vector<ofVec3f>& verts = sMesh[i][j].getVertices();
            int rad = ofRandom(vSizeL, vSizeH);
        
                for(unsigned int k = 0; k < verts.size(); k++){
                    verts[k].x = ctr.x + ofRandom(-rad, rad);
                    verts[k].y = ctr.y + ofRandom(-rad, rad);
                    verts[k].z = ctr.z + ofRandom(-rad, rad);
                }
        }
    }
    
    
    
    // Setup light
//    light.setPosition(0, 2000, -750);
    light.setPosition(0, 0, 750);
    
    for(int i=0;i<numRopes;i++){ aniTs[i] = 20; }
    
    palettes["dekooning"] = //de Kooning Seated Woman
    {   ofColor(176, 30, 7),
        ofColor(196, 0, 0),
        ofColor(185, 103, 10),
        ofColor(235, 225, 136),
        ofColor(21, 149, 102),
        ofColor(111, 166, 21),
        ofColor(203, 133, 28),
        ofColor(224, 178, 0),
        ofColor(106, 150, 36)    };
    
    palettes["kandinsky"] = // Kandinsky yellow-red-blue
    {   ofColor(49, 47, 165),
        ofColor(124, 145, 252),
        ofColor(134, 96, 200),
        ofColor(119, 115, 206),
        ofColor(143, 108, 53),
        ofColor(176, 158, 92),
        ofColor(29, 21, 50),
        ofColor(38, 11, 22),
        ofColor(165, 152, 208) };
    
    palettes["johns"] = // Jasper Johns Target
    {   ofColor(195, 14, 22),
        ofColor(10, 99, 225),
        ofColor(244, 197, 9),
        ofColor(21, 133, 78),
        ofColor(245, 81, 0),
        ofColor(120, 51, 255),
        ofColor(203, 50, 39),
        ofColor(0, 64, 190),
        ofColor(251, 206, 0) };
    
    palettes["klee"] = // Klee Southern Tunisian Gardens
    {   ofColor(174, 42, 6),
        ofColor(224, 183, 54),
        ofColor(44, 150, 115),
        ofColor(157, 122, 43),
        ofColor(207, 222, 195),
        ofColor(204, 43, 0),
        ofColor(215, 157, 20),
        ofColor(0, 158, 95),
        ofColor(198, 125, 0) };
    
    palettes["white"] =
    {   ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white,
        ofColor::white };
    
    activeClrs = palettes["dekooning"];
    

    ofHideCursor();
}

void ofApp::update(){
    
    ofxOscMessage m;
    m.setAddress("/getFreqAmp");
    sender.sendMessage(m, false);
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if(m.getAddress() == "/freqAmp"){
            freq = m.getArgAsFloat(0);
            amp = m.getArgAsFloat(1);
        }
        
        if(m.getAddress() == "/chgClr"){
            chgClr(m.getArgAsInt(0), m.getArgAsString(1));
        }
        
        if(m.getAddress() == "/chgRopeMode"){
            chgRopeMode(m.getArgAsInt(0));
        }
        
        if(m.getAddress() == "/catPushDir"){
            catPushDir = m.getArgAsInt(0);
        }
        
    }
    
    world.update();
    
    //move in circle
    
//    xPos = xOrig + radius * cos(angle);
//    yPos = yOrig + radius * sin(angle);
    
    
    pitchDetect(0, 69.5, 71.);
    pitchDetect(1, 71.3,72.7);
    pitchDetect(2, 73.2,75.);
    pitchDetect(3, 57.4,58.6);
    pitchDetect(4, 59.1,60.7);
    pitchDetect(5, 61,62.7);
    pitchDetect(6, 66.3,68.);
    pitchDetect(7, 89.,90.);
    pitchDetect(8, 80.,82.);
    
    if(drawTumbleweeds){
        
        for(int i=0;i<ropes.size();++i){
            for(int j=0; j<numNodes; j++){
                ofPoint n = ropes[i]->getNodePos(j);
                spheres[i][j].setPosition(n);
            }
        }
   
        for(int i=0;i<numRopes;i++){
            if(ofGetFrameNum()%aniTs[i] == 0){
                aniTs[i] = round( ofRandom(fRangeL, fRangeH) );
                int nNds = round( ofRandom( 1, (numNodes-1) ) );
                pNodes.clear();
              
                for(int j=0;j<nNds;j++){
                    int ndch = round( ofRandom( 1, (numNodes-1) ) );
                    vector<ofVec3f>& verts = sMesh[i][ndch].getVertices();
                    int rad = ofRandom(vSizeL, vSizeH);
                  
                    for(unsigned int k = 0; k < verts.size(); k++){
                        verts[k].x = ctr.x + ofRandom(-rad, rad);
                        verts[k].y = ctr.y + ofRandom(-rad, rad);
                        verts[k].z = ctr.z + ofRandom(-rad, rad);
                    }
                }
            }
        }
    }
    
    else{
        for(int i=0;i<ropes.size();i++){
            for(int j=0;j<numNodes; j++){
                strings[i][j]->setNodePositionAt(0, ropes[i]->getNodePos(j));
            }
        }
    }
    
    
}


void ofApp::draw(){
    
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    light.enable();
    
    // begin scene to post process
    post.begin(cam);
    ofSetLineWidth(1);

    for(int i=0;i<ropes.size();++i){
        ofSetColor(activeClrs[i]);
        
        if(drawTumbleweeds){
            for(int j=0; j<numNodes; j++){
                ofPoint nd = ropes[i]->getNodePos(j);
                glPushMatrix();
//              ofRotate((ofGetFrameNum()*7)%360, 1,0,0);
            
                ofTranslate(nd);
            
                sMesh[i][j].drawWireframe();
                glPopMatrix();
            }
        }
        
        else{
            for(int j=0;j<numNodes;j++){
                strings[i][j]->draw();
            }
        }
    }
    
    
    // end scene and draw
    post.end();
    
    // set gl state back to original
    glPopAttrib();
    
    
}

void ofApp::pitchDetect(int i, float pitchL, float pitchH){

    switch(ropeMode){

        case 0:
            if(freq > pitchL && freq < pitchH){
                ropes[i]->addForce( btVector3(0., 15000., 7500), 15);
            }
            break;
            
        case 1:
            if(freq > pitchL && freq < pitchH){
                int tnd = round(ofRandom(0, (numNodes+1)));
                int tForce = ofRandom(6000, 8000);
                float tForceY = ofRandom(13000., 21000.);
                ropes[i]->addForce( btVector3(0., tForceY, 0), 15);
                
                
                switch(catPushDir){
                    case 0:
                        ropes[i]->addForce( btVector3(0., tForce, 0), tnd);
                        break;
                        
                    case 1:
                        ropes[i]->addForce( btVector3(tForce, 0, 0), tnd);
                        break;
                        
                    case 2:
                        ropes[i]->addForce( btVector3(-tForce, 0, 0), tnd);
                        break;
                        
                    case 3:
                        ropes[i]->addForce( btVector3(0, 0, tForce), tnd);
                        break;
                        
                    case 4:
                        ropes[i]->addForce( btVector3(0, 0, -tForce), tnd);
                        break;
                        
                    case 5:
                        ropes[i]->addForce( btVector3(0, -tForce, 0), tnd);
                        break;
                        
                }

            }
            break;
            
        case 2:
            if(freq > pitchL && freq < pitchH){
                int tnd = round(ofRandom(0, (numNodes+1)));
                int tForce = ofRandom(2800, 2000);

                switch(catPushDir){
                    case 0:
                        ropes[i]->addForce( btVector3(0., tForce, 0), tnd);
                        break;
                        
                    case 1:
                        ropes[i]->addForce( btVector3(tForce, 0, 0), tnd);
                        break;
                        
                    case 2:
                        ropes[i]->addForce( btVector3(-tForce, 0, 0), tnd);
                        break;
                        
                    case 3:
                        ropes[i]->addForce( btVector3(0, 0, tForce), tnd);
                        break;
                        
                    case 4:
                        ropes[i]->addForce( btVector3(0, 0, -tForce), tnd);
                        break;
                        
                    case 5:
                        ropes[i]->addForce( btVector3(0, -tForce, 0), tnd);
                        break;
                }
                
            }
            break;
            
        case 3:
            if(freq > pitchL && freq < pitchH){
                int tnd = round(ofRandom(0, (numNodes+1)));
                int tForce = ofRandom(7000, 9000);
                
                switch(catPushDir){
                    case 0:
                        ropes[i]->addForce( btVector3(0., tForce, 0), tnd);
                        break;
                        
                    case 1:
                        ropes[i]->addForce( btVector3(tForce, 0, 0), tnd);
                        break;
                        
                    case 2:
                        ropes[i]->addForce( btVector3(-tForce, 0, 0), tnd);
                        break;
                        
                    case 3:
                        ropes[i]->addForce( btVector3(0, 0, tForce), tnd);
                        break;
                        
                    case 4:
                        ropes[i]->addForce( btVector3(0, 0, -tForce), tnd);
                        break;
                        
                    case 5:
                        ropes[i]->addForce( btVector3(0, -tForce, 0), tnd);
                        break;
                }
                
            }
            break;
            
        case 4:
            if(freq > pitchL && freq < pitchH){
                int tnd = round(ofRandom(0, (numNodes+1)));
                int tForce = ofRandom(10000, 15000);
                
                switch(catPushDir){
                    case 0:
                        ropes[i]->addForce( btVector3(0., tForce, 0), tnd);
                        break;
                        
                    case 1:
                        ropes[i]->addForce( btVector3(tForce, 0, 0), tnd);
                        break;
                        
                    case 2:
                        ropes[i]->addForce( btVector3(-tForce, 0, 0), tnd);
                        break;
                        
                    case 3:
                        ropes[i]->addForce( btVector3(0, 0, tForce), tnd);
                        break;
                        
                    case 4:
                        ropes[i]->addForce( btVector3(0, 0, -tForce), tnd);
                        break;
                        
                    case 5:
                        ropes[i]->addForce( btVector3(0, -tForce, 0), tnd);
                        break;
                }
                
            }
            break;
    }
}

void ofApp::chgClr(int ropeNum, string palette){
    activeClrs[ropeNum] = palettes[palette][ropeNum];
}

void ofApp::chgRopeMode(int modeNum){
    
    switch(modeNum){
            
        case 0:
            for(int i=0;i<ropes.size();i++){
                ropes[i]->setNodePositionAt(0, ofVec3f(-ofGetWidth()/2, ys[i], zs[i]));
                ropes[i]->setNodePositionAt( (numNodes+1),  ofVec3f(ofGetWidth()/2, ys[i], zs[i]) );
                
                ropes[i]->setFixedAt(0);
                ropes[i]->setFixedAt(numNodes+1);
            }

            world.setGravity(ofVec3f(0,-50, 0));
            ropeMode = 0;
            break;
            
        case 1:
            for(int i=0;i<ropes.size();i++){
                ropes[i]->setNodeMass(0, 10.4f);
                ropes[i]->setNodeMass(numNodes+1, 10.4f);
            }
            world.setGravity(ofVec3f(0,-50, 0));
            ropeMode = 1;
            break;
            
        case 2:
            for(int i=0;i<ropes.size();i++){
                ropes[i]->setNodeMass(0, 10.4f);
                ropes[i]->setNodeMass(numNodes+1, 10.4f);
            }
            world.setGravity(ofVec3f(0,0, 0));
            ropeMode = 2;
            break;
            
        case 3:
            for(int i=0;i<ropes.size();i++){
                ropes[i]->setNodeMass(0, 10.4f);
                ropes[i]->setNodeMass(numNodes+1, 10.4f);
            }
            world.setGravity(ofVec3f(0,0, 0));
            ropeMode = 3;
            drawTumbleweeds = false;
            break;
            
        case 4:
            for(int i=0;i<ropes.size();i++){
                ropes[i]->setNodeMass(0, 10.4f);
                ropes[i]->setNodeMass(numNodes+1, 10.4f);
            }
            world.setGravity(ofVec3f(0,0, 0));
            ropeMode = 4;
            drawTumbleweeds = false;
            activeClrs = palettes["white"];
            break;
    }
   
    
}

void ofApp::keyPressed(int key){

   
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    drawTumbleweeds = false;
    
 
}


