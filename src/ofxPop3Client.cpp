/*
    ofxMailUtils
    author: arturo
*/

#include "ofxMailUtils.h"

// ----------------------------------------------------------------------
ofxPop3ClientUtils::ofxPop3ClientUtils(){
    removeFromServer=false;
}
// ----------------------------------------------------------------------
ofxPop3ClientUtils::~ofxPop3ClientUtils(){
    stop();
}
// ----------------------------------------------------------------------
void ofxPop3ClientUtils::setCheckInterval(int seconds){
	checkInterval=seconds;
}
// ----------------------------------------------------------------------
void ofxPop3ClientUtils::addAccount(ofxMailAccount account){
    accounts.push_back(account);
}

// ----------------------------------------------------------------------
void ofxPop3ClientUtils::start() {
     if (isThreadRunning() == false){
        printf("thread started\n");
        startThread(true,true);
    }
}
// ----------------------------------------------------------------------
void ofxPop3ClientUtils::stop() {
    stopThread();
}
// ----------------------------------------------------------------------
void ofxPop3ClientUtils::threadedFunction(){

    // loop through this process whilst thread running
    while( isThreadRunning() == true ){
    	if(accounts.size()>0){
    		int totalNumMessages=0;
			ofxMailAccount account = accounts.front();
			for(unsigned int i=0;i<accounts.size();i++){

				totalNumMessages = checkAccount(accounts[i]);
				printf("ofxMailUtils: (thread running) account %s checked, new messages: %i\n", accounts[i].accountName.c_str(), totalNumMessages);
			}

			printf("ofxMailUtils: (thread running) accounts checked, new messages: %i\n", totalNumMessages);

    	}
		ofSleepMillis(checkInterval*1000);
    }

}

int ofxPop3ClientUtils::checkAccount(ofxMailAccount & account){

    Poco::Net::POP3ClientSession session= Poco::Net::POP3ClientSession (account.host,(Poco::UInt16) account.port);
	session.login(account.user,account.password);

	int numMessages = session.messageCount();
	if(numMessages){
		POP3ClientSession::MessageInfoVec messageList;
		session.listMessages(messageList);
		for(unsigned int i =0;i< messageList.size();i++){
			Poco::Net::MailMessage message;
			session.retrieveMessage(messageList[i].id,message);

			ofxMailMessage ofMessage= ofxMailMessage(messageList[i].id,message);
			if(!removeFromServer){
				bool alreadyReaded = checkAlreadyReaded(account,messageList[i].id);
				if(!alreadyReaded){
					ofNotifyEvent(newMessageEvent,ofMessage);
					account.alreadyReaded.push_back(messageList[i].id);

				}else{
					numMessages--;
				}
			}else{
				//TODO: Remove message from server
			}

		}
	}
	return numMessages;
}


bool ofxPop3ClientUtils::checkAlreadyReaded(ofxMailAccount account,int id){
	for(unsigned i=0; i<account.alreadyReaded.size();i++)
		if (account.alreadyReaded[i]==id)
			return true;

	return false;

};


