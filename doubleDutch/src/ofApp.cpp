#include "ofApp.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

void ofApp::setup()
{
    ofBackground(0);
    sender.setup(HOST, PORT);
    receiver.setup(PORTR);
    
//    ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ) );
    
    

    
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
//    post.createPass<DofPass>()->setEnabled(true);
    
    // Setup box positions
    for (unsigned i = 0; i < NUM_BOXES; ++i){
        posns.push_back(ofVec3f(ofRandom(-ofGetWidth()/2, ofGetWidth()/2), ofRandom(-ofGetHeight()/2, ofGetHeight()/2), ofRandom(-ofGetWidth()/2, ofGetWidth()/2)));
        cols.push_back(ofColor::fromHsb(255 * i / (float)NUM_BOXES, 255, 255, 255));
    }
    
    
    
    
    
    world.setup();
    world.setCamera(&cam);
    
    
    for(int i=0; i<10; i++){
        zs[i] = ofRandom(-200, 200);
        ys[i] = ofRandom(-150, 150);
        ofVec3f nodeA = ofVec3f( -ofGetWidth()/2, ys[i], zs[i]);
        ofVec3f nodeZ = ofVec3f( ofGetWidth()/2,ys[i],zs[i]);
        ropes.push_back( new ofxBulletRope() );
        ((ofxBulletRope*)ropes[i])->create( &world, nodeA, nodeZ, 100 );
        ropes[i]->add();
        ropes[i]->setMass(10.1f);
        ropes[i]->setStiffness(1, 1, 1);
        ropes[i]->setFixedAt(0);
        ropes[i]->setFixedAt(101);

        
        
//        if(i!=2){
//            ropes[i]->setFixedAt(0);
//            ropes[i]->setFixedAt(101);
//        }
//        if(i==2){
//            ropes[i]->setFixedAt(50);
//        }
    }
    
//  Fill Spheres Array
    for(int i=0;i<ropes.size();i++){
        spheres.push_back(std::vector<ofSpherePrimitive>());
        for(int j=0;j<102;j++){
            spheres[i].push_back( ofSpherePrimitive(30,12) );
        }
    }
    
//    planet = spheres[0][50].getMesh();
//
//     vector<ofVec3f>& verts = planet.getVertices();
    
    
//    tm = spheres[0][50].getMesh();
//    tv = tm.getVertices();
    
////  Fill Vertices Array
//    for(int i=0;i<ropes.size();i++){
//        sMesh.push_back(std::vector<ofMesh>());
//        for(int j=0;j<101;j++){
//            sMesh[i].push_back( spheres[i][j].getMesh() );
//            iVerts[i][j] =  sMesh[i][j].getVertices() ;
//        }
//    }

    



    // Setup light
    //    light.setPosition(1000, 1000, 2000);
//    light.setPosition(ofGetWidth()/2, 2000, 0);
    light.setPosition(0, 2000, -750);
    
//    create our own box mesh as there is a bug with
//    normal scaling and ofDrawBox() at the moment
    boxMesh = ofMesh::box(20, 20, 20);
//    boxMesh = ofMesh::sphere(20);
    
//    colors[0] = ofColor::lime;
//    colors[1] = ofColor::hotPink;
//    colors[2] = ofColor::yellow;
//    colors[3] = ofColor::orange;
//    colors[4] = ofColor::aquamarine;
//    colors[5] = ofColor::fireBrick;
//    colors[6] = ofColor::seaGreen;
//    colors[7] = ofColor::purple;
//    colors[8] = ofColor::silver;
//    colors[9] = ofColor::salmon;
    
    
//    colors[0] = ofColor(106,106,96);
//    colors[1] = ofColor(67,81,85);
//    colors[2] = ofColor(98, 113, 116);
//    colors[3] = ofColor(29, 46, 54);
//    colors[4] = ofColor(81, 67, 47);
//    colors[5] = ofColor(111, 82, 72);
//    colors[6] = ofColor(93, 79, 61);
//    colors[7] = ofColor(148, 150, 145);
//    colors[8] = ofColor(136, 120, 94);
//    colors[9] = ofColor(39, 58, 60);
//
    
    // de Kooning Seated Woman
    colors[0] = ofColor(176, 30, 7);
    colors[1] = ofColor(196, 0, 0);
    colors[2] = ofColor(185, 103, 10);
    colors[3] = ofColor(235, 225, 136);
    colors[4] = ofColor(21, 149, 102);
    colors[5] = ofColor(111, 166, 21);
    colors[6] = ofColor(203, 133, 28);
    colors[7] = ofColor(224, 178, 0);
    colors[8] = ofColor(106, 150, 36);
    colors[9] = ofColor(60, 133, 72);
    
    // Kandinsky yellow-red-blue
//    colors[0] = ofColor(49, 47, 165);
//    colors[1] = ofColor(124, 145, 252);
//    colors[2] = ofColor(134, 96, 200);
//    colors[3] = ofColor(119, 115, 206);
//    colors[4] = ofColor(143, 108, 53);
//    colors[5] = ofColor(176, 158, 92);
//    colors[6] = ofColor(29, 21, 50);
//    colors[7] = ofColor(38, 11, 22);
//    colors[8] = ofColor(165, 152, 208);
//    colors[9] = ofColor(224, 194, 252);
    
    // Matisse La Danse
//    colors[0] = ofColor(6, 130, 96);
//    colors[1] = ofColor(17, 105, 174);
//    colors[2] = ofColor(209, 98, 45);
//    colors[3] = ofColor(93, 50, 37);
//    colors[4] = ofColor(88, 113, 162);
//    colors[5] = ofColor(0, 130, 88);
//    colors[6] = ofColor(0, 102, 173);
//    colors[7] = ofColor(224, 99, 30);
//    colors[8] = ofColor(0, 130, 94);
    //    colors[9] = ofColor(0, 105, 176);
    
    
    
    // Jasper Johns Target
//        colors[0] = ofColor(195, 14, 22);
//        colors[1] = ofColor(10, 99, 225);
//        colors[2] = ofColor(244, 197, 9);
//        colors[3] = ofColor(21, 133, 78);
//        colors[4] = ofColor(245, 81, 0);
//        colors[5] = ofColor(120, 51, 255);
//        colors[6] = ofColor(203, 50, 39);
//        colors[7] = ofColor(0, 64, 190);
//        colors[8] = ofColor(251, 206, 0);
//        colors[9] = ofColor(14, 67, 177);
    
    // Klee Southern Tunisian Gardens
//        colors[0] = ofColor(174, 42, 6);
//        colors[1] = ofColor(224, 183, 54);
//        colors[2] = ofColor(44, 150, 115);
//        colors[3] = ofColor(157, 122, 43);
//        colors[4] = ofColor(207, 222, 195);
//        colors[5] = ofColor(204, 43, 0);
//        colors[6] = ofColor(215, 157, 20);
//        colors[7] = ofColor(0, 158, 95);
//        colors[8] = ofColor(198, 125, 0);
//        colors[9] = ofColor(101, 165, 48);

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
    }
    
    world.update();
    
    //move in circle
    
//    xPos = xOrig + radius * cos(angle);
//    yPos = yOrig + radius * sin(angle);
    
    if(freq > 72 && freq < 73){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[0]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[0]) );
    }
    
    if(freq > 74 && freq < 75){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[1]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, nypos, zs[1]) );
    }
    
    if(freq > 70 && freq < 71){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[2]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[2]) );
        ropes[2]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, -nypos, zs[2]) );
    }
    
    if(freq > 57 && freq < 58){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[3]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[3]) );
    }
    
    if(freq > 59.5 && freq < 60.5){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[4]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, nypos, zs[4]) );
    }
    
    if(freq > 62 && freq < 63){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[5]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[5]) );
        ropes[5]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, -nypos, zs[5]) );
    }
    
    if(freq > 77.5 && freq < 78.5){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[6]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[6]) );
    }
    
    if(freq > 79.7 && freq < 81){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[7]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, nypos, zs[7]) );
    }
    
    if(freq > 82 && freq < 83){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[8]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[8]) );
        ropes[8]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, -nypos, zs[8]) );
    }
    
    if(freq > 67 && freq < 68.3){
        float nypos = ofMap(amp, -30, -10, -200, 200);
        nypos = ofClamp(nypos, -200, 200);
        if(amp < -50){nypos = 0;}
        ropes[9]->setNodePositionAt(0,  ofVec3f(-ofGetWidth()/2, nypos, zs[9]) );
        ropes[9]->setNodePositionAt(101,  ofVec3f(ofGetWidth()/2, -nypos, zs[9]) );
    }
    
   
    
    
    
    for(int i=0;i<ropes.size();++i){
        for(int j=0; j<102; j = j+3){
            ofPoint n = ropes[i]->getNodePos(j);
            spheres[i][j].setPosition(n);
        }
    }
//    planet.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
//       planet = spheres[0][50].getMesh();
   
    
 if(ofGetFrameNum()%fn == 0){
     fn = round(ofRandom(7, 30));
     planet = spheres[2][50].getMesh();
     
     ofPoint ctr = planet.getCentroid();

   vector<ofVec3f>& verts = planet.getVertices();
     sw1 = (sw1+1)%2;

     int rad = ofRandom(5, 50);
    for(unsigned int i = 0; i < verts.size(); i++){
//       
//            verts[i].x = verts[i].x + ofRandom(-10, 10);
//            verts[i].y = verts[i].y + ofRandom(-10, 10);
//            verts[i].z = verts[i].z + ofRandom(-10, 10);
        
        
       
        
        verts[i].x = ctr.x + ofRandom(-rad, rad);
        verts[i].y = ctr.y + ofRandom(-rad, rad);
        verts[i].z = ctr.z + ofRandom(-rad, rad);
        
        
        
        }
    }

    
//    tm = spheres[0][50].getMesh();
//    vector<ofVec3f>& tvv = tm.getVertices();
//    for(int i=0;i<tvv.size();i++){
//        if(ofGetFrameNum()%5 ==0){
//            tvv[i].x = tvv[i].x + ofRandom(-20, 20);
//            tvv[i].y = tvv[i].y + ofRandom(-20, 20);
//            tvv[i].z = tvv[i].z + ofRandom(-20, 20);
//        }
//    }
//


    
    
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
    
    // draw boxes
//    for (unsigned i = 0; i < posns.size(); ++i){
//        ofSetColor(cols[i]);
//        ofPushMatrix();
//        ofTranslate(posns[i]);
//        boxMesh.draw();
//        ofPopMatrix();
//    }
    
//    ofRotateY(120);
//    ofRotateX(20);
    
//    ofRotateX(180);
    

    
    for(int i=0;i<ropes.size();++i){
        ofSetColor(colors[i]);
//      ropes[i]->draw();
        for(int j=0; j<102; j = j+3){
            
            
            
//            ofMesh tiMesh = spheres[i][j].getMesh();
//            vector<ofVec3f>& tiVerts = tiMesh.getVertices();
//            
//            
//            
//            for(int k=0; k<tiVerts.size(); ++k){
//                if(ofGetFrameNum()%5 ==0){
//                    tiVerts[k].x = tiVerts[k].x + ofRandom(-20, 20);
//                    tiVerts[k].y = tiVerts[k].y + ofRandom(-20, 20);
//                    tiVerts[k].z = tiVerts[k].z + ofRandom(-20, 20);
//                }
//            }
//
//            
//            
//            tiMesh.draw();
//            spheres[i][j].draw();
//            tm.draw();
        }
    }
    
    ofPoint n = ropes[7]->getNodePos(51);
    glPushMatrix();
    ofTranslate(n);

    //planet.draw();
    planet.drawWireframe();
    glPopMatrix();
   
    
    
    // end scene and draw
    post.end();
    
    // set gl state back to original
    glPopAttrib();
    
    
}

void ofApp::keyPressed(int key){
   
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    


}


