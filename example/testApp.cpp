#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofAddListener(pop3.newMessageEvent,this,&testApp::newMessage);

	ofxMailAccount account;
	account.accountId=0;
	account.accountName="account_name"; // this is just to show the name of the account where the message arrive
	account.user="user@mail.com";		// login to your mail account
	account.password="passwd";			// password of your mail account
	account.host="mail.server.com";		// pop3 server
	// account.port=110; // by default

	pop3.addAccount(account);
	pop3.setCheckInterval(20);
	pop3.start();
}

//--------------------------------------------------------------
void testApp::update(){
	sprintf(message,"fps: %i",(int)ofGetFrameRate());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(message,20,20);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	ofxMailMessage message;
	message.recipients.push_back("mail@server.com");
	message.sender="me@mail.com";
	message.subject="subject of the message";
	message.date=Poco::Timestamp();
	smtp.addMessage(message);
	cout << "testApp::keyPressed: new message sent\n";

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
void testApp::mouseReleased(){
}



void testApp::newMessage(ofxMailMessage & message){
	cout << "\n----------------------\n";
	cout << "testApp::newMessage: \n";
	cout << "from: "<< message.sender << "\n";
	cout << "subject: "<<message.subject + "\n\n";
	cout << message.content + "\n----------------------\n\n";
}
