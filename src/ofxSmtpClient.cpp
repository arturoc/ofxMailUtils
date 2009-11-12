/*
    ofxMailUtils
    author: arturo
*/

#include "ofxMailUtils.h"

// ----------------------------------------------------------------------
ofxSmtpClientUtils::ofxSmtpClientUtils(){
	connected = false;
	try{
		session=new Poco::Net::SMTPClientSession(OFX_SMTP_HOST,OFX_SMTP_PORT);
		session->login();
		connected = true;
	}catch(Poco::Exception e){
		ofLog(OF_LOG_ERROR,"cannot connect to the server");
	}
}
// ----------------------------------------------------------------------
ofxSmtpClientUtils::~ofxSmtpClientUtils(){
    stop();
}
//-------------------------------
// non blocking functions
void ofxSmtpClientUtils::addMessage(ofxMailMessage message){
	messages.push(message);
	if(!isThreadRunning())
		start();
}

//-------------------------------
// blocking functions
void ofxSmtpClientUtils::sendMessage(ofxMailMessage & message){
	if(connected)
		session->sendMessage(message.getPocoMessage());
	else
		ofLog(OF_LOG_ERROR,"cannot send, not connected to the smtp server");
}


// ----------------------------------------------------------------------
void ofxSmtpClientUtils::start() {
     if (isThreadRunning() == false){
        printf("thread started\n");
        startThread(true,true);
    }
}
// ----------------------------------------------------------------------
void ofxSmtpClientUtils::stop() {
    stopThread();
}
// ----------------------------------------------------------------------
void ofxSmtpClientUtils::threadedFunction(){

    // loop through this process whilst thread running
    while( isThreadRunning() == true ){
    	if(messages.size()){
    		sendMessage(messages.front());
    		messages.pop();
    	}else{
    		stop();
    	}

    }

}



