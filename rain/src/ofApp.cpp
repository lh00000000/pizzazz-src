#include "ofApp.h"
#include "MSAPhysics3D.h"
#include <stdlib.h>

#define MIN_MASS 1
#define	GRAVITY 1
#define LIFESPAN 200

msa::physics::World3D physics;
//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    physics.clear();
    // set world dimensions, not essential, but speeds up collision
    physics.setWorldSize(ofVec3f(-ofGetWidth()/2, -ofGetHeight(), -ofGetWidth()/2), ofVec3f(ofGetWidth()/2, ofGetHeight(), ofGetWidth()/2));
    physics.setSectorCount(1);
    physics.setDrag(0.97f);
    physics.setDrag(1);		// FIXTHIS
    physics.enableCollision();
    
    /*
    // or tell the system to create and add particles
    physics.makeParticle(ofVec3f(-ofGetWidth()/4, 0, -ofGetWidth()/4), MIN_MASS);		// create a node in top left back and fix
    physics.makeParticle(ofVec3f( ofGetWidth()/4, 0, -ofGetWidth()/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
    physics.makeParticle(ofVec3f(-ofGetWidth()/4, 0,  ofGetWidth()/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
    physics.makeParticle(ofVec3f( ofGetWidth()/4, 0,  ofGetWidth()/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix
    */
    
    physics.setGravity(ofVec3f(0, GRAVITY, 0));
    
    // setup lighting
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0, ofGetHeight()/2, 0.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    
    // enable back-face culling (so we can see through the walls)
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    addDrop();
}

void ofApp::addDrop() {
    
    float posX		= ofRandom(-ofGetWidth()/2, ofGetWidth()/2);
    float posY		= 0;
    float posZ		= ofRandom(-ofGetWidth()/2, ofGetWidth()/2);
    float mass		= ofRandom(1, 3);
    float bounce	= 1/mass;
    float radius	= mass/2;
    
    msa::physics::Particle3D *p = physics.makeParticle(ofVec3f(posX, posY, posZ));
    p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
    
}

void ofApp::cleanDrops() {
    
    msa::physics::Particle3D *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
    //cout << p->getVelocity().lengthSquared() << endl;
    if(p && p->getPosition().y > ofGetHeight()-5 && p->getVelocity().lengthSquared() < 1 && p->getAge() > LIFESPAN) p->kill();

}

//--------------------------------------------------------------
void ofApp::update(){
    if (ofRandom(0, 1) > ofGetMouseY()/ofGetHeight()) {
        addDrop();
    }
    cleanDrops();
    physics.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    
    glPushMatrix();
    glTranslatef(ofGetWidth()/2, 0, -ofGetWidth() / 3);		// center scene
    static float rot = 0;
    glRotatef(rot, 0, 1, 0);			// rotate scene
    rot += 0.1;
    
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    
    // draw right wall
    glColor3f(0.1, 0.1, 0.1);		glVertex3f(ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    glColor3f(0, 0, 0);				glVertex3f(ofGetWidth()/2, -ofGetHeight(), ofGetWidth()/2);
    glColor3f(0.05, 0.05, 0.05);	glVertex3f(ofGetWidth()/2, -ofGetHeight(), -ofGetWidth()/2);
    glColor3f(0.15, 0.15, 0.15);		glVertex3f(ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    
    // back wall
    glColor3f(0.1, 0.1, 0.1);		glVertex3f(ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    glColor3f(0, 0, 0);				glVertex3f(ofGetWidth()/2, -ofGetHeight(), -ofGetWidth()/2);
    glColor3f(0.05, 0.05, 0.05);	glVertex3f(-ofGetWidth()/2, -ofGetHeight(), -ofGetWidth()/2);
    glColor3f(.15, 0.15, 0.15);		glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    
    // left wall
    glColor3f(.1, 0.1, 0.1);		glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    glColor3f(0, 0, 0);				glVertex3f(-ofGetWidth()/2, -ofGetHeight(), -ofGetWidth()/2);
    glColor3f(0.05, 0.05, 0.05);	glVertex3f(-ofGetWidth()/2, -ofGetHeight(), ofGetWidth()/2);
    glColor3f(.15, 0.15, 0.15);		glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    
    // front wall
    glColor3f(0.05, 0.05, 0.05);	glVertex3f(ofGetWidth()/2, -ofGetHeight(), ofGetWidth()/2);
    glColor3f(.15, 0.15, 0.15);		glVertex3f(ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    glColor3f(.1, 0.1, 0.1);		glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    glColor3f(0, 0, 0);				glVertex3f(-ofGetWidth()/2, -ofGetHeight(), ofGetWidth()/2);
    
    // floor
    glColor3f(.2, 0.2, 0.2);
    glVertex3f(ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    glVertex3f(ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, -ofGetWidth()/2);
    glVertex3f(-ofGetWidth()/2, ofGetHeight()+1, ofGetWidth()/2);
    glEnd();
    
    
    
    // draw particles
    //glAlphaFunc(GL_GREATER, 0.5);
    
    //ofEnableNormalizedTexCoords();
    //ballImage.getTextureReference().bind();
    for(int i=0; i<physics.numberOfParticles(); i++) {
        msa::physics::Particle3D *p = physics.getParticle(i);
        if(p->isFixed()) glColor4f(1, 1, 1, 0.9);
        else glColor4f(1, 1, 1, ofMap(p->getAge(), 0, LIFESPAN, 1, 0));
        
        glEnable(GL_ALPHA_TEST);
        
        // draw ball
        glPushMatrix();
        glTranslatef(p->getPosition().x, p->getPosition().y, p->getPosition().z);
        glRotatef(180-rot, 0, 1, 0);
        
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
        glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
        glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
        glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());
        glEnd();
        glPopMatrix();
        
        //glDisable(GL_ALPHA_TEST);
        
        /*
        //shadows
        float alpha = ofMap(p->getPosition().y, -ofGetHeight() * 1.5, ofGetHeight(), 0, 1);
        if(alpha>0) {
            glPushMatrix();
            glTranslatef(p->getPosition().x, ofGetHeight(), p->getPosition().z);
            glRotatef(-90, 1, 0, 0);
            glColor4f(0, 0, 0, alpha * alpha * alpha * alpha);
            				//ofCircle(0, 0, p->getRadius());
            float r = p->getRadius() * alpha;
            
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(-r, -r);
            glTexCoord2f(1, 0); glVertex2f(r, -r);
            glTexCoord2f(1, 1); glVertex2f(r, r);
            glTexCoord2f(0, 1); glVertex2f(-r, r);
            glEnd();
            glPopMatrix();
        } */
        
    }
    //ballImage.getTextureReference().unbind();
    //ofDisableNormalizedTexCoords();
    
    glPopMatrix();
    
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor4f(0, 0, 0, 1);
    ofColor(0,0,0,255);
    ofDrawBitmapString(" FPS: " + ofToString(ofGetFrameRate(), 2)
                       + " | Number of particles: " + ofToString(physics.numberOfParticles(), 2)
                       + " | Number of springs: " + ofToString(physics.numberOfSprings(), 2)
                       + "\nLook at source code keyPressed to see keyboard shortcuts"
                       , 0, 0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
