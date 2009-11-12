#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxMailUtils.h"


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();


		char message[256];
		char formName[128];
		int  formNum;

		ofxPop3ClientUtils pop3;
		ofxSmtpClientUtils smtp;


		//------------------------------
		// http events
		void newMessage( ofxMailMessage & message );

};

#endif

