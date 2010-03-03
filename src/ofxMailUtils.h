/*
    ofxPop3Client
    arturo

*/

#ifndef _POCO_MAIL_TOOLS
#define _POCO_MAIL_TOOLS

#include "ofMain.h"

#include "ofxThread.h"






#include "Poco/Net/MailMessage.h"
#include "Poco/Net/POP3ClientSession.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/FilePartSource.h"

#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include <queue>

#define OFX_SMTP_HOST "localhost"
#define OFX_SMTP_PORT 25

using Poco::Net::SMTPClientSession;
using Poco::Net::POP3ClientSession;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

#define OFX_HTTP_GET  0
#define OFX_HTTP_POST 1

class ofxPop3Listener;
class ofxPop3EventManager;

struct ofxMailMessage{
	ofxMailMessage(int id, Poco::Net::MailMessage& mailMessage){
		this->id=id;
		try{
			date=mailMessage.getDate();
		}catch(Poco::Exception){

		}

		sender = mailMessage.getSender();
		subject = mailMessage.getSubject();
		contentType = mailMessage.getContentType();
		content = mailMessage.getContent();
		//TODO: get attachments from poco message
	}

	ofxMailMessage(){
		date=Poco::Timestamp();
	}

	Poco::Net::MailMessage & getPocoMessage(){
		Poco::Net::MailMessage & message = *(new Poco::Net::MailMessage());
		message.setDate(date);
		message.setSender(sender);
		message.setSubject(subject);
		message.setContentType(contentType);
		message.setContent(content);
		for(unsigned int i=0;i<recipients.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,recipients[i]));
		}
		for(unsigned int i=0;i<cc.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,cc[i]));
		}
		for(unsigned int i=0;i<bcc.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,bcc[i]));
		}
		for(unsigned int i=0; i < attachmentPaths.size(); i++){
			message.addAttachment(attachmentPaths[i], new Poco::Net::FilePartSource(ofToDataPath(attachmentPaths[i], true)));
		}
		return message;
	}

	int id;
	Poco::Timestamp date;
	string sender;
	string subject;
	string contentType;
	string content;
	vector<string> recipients;
	vector<string> cc;
	vector<string> bcc;
	vector<string> attachmentPaths;
};


struct ofxMailAccount{
	ofxMailAccount(){
		port=110;
	}
	int accountId;
	string accountName;
	string user;
	string password;
	string host;
	unsigned int port;
	vector<int> alreadyReaded;
};

class ofxPop3ClientUtils : public ofxThread{

	public:

		ofxPop3ClientUtils();
		~ofxPop3ClientUtils();

		bool removeFromServer;

		void setCheckInterval(int seconds);

		//-------------------------------
		// non blocking functions
		void addAccount(ofxMailAccount account);

		//-------------------------------
		// blocking functions
        int checkAccount(ofxMailAccount & account);


		//-------------------------------
		// threading stuff
		void threadedFunction();
		void start();
        void stop();

        ofEvent<ofxMailMessage> newMessageEvent;

    protected:
    	bool checkAlreadyReaded(ofxMailAccount account,int id);

        vector<ofxMailAccount> accounts;
		bool verbose;
		int checkInterval;


};


class ofxSmtpClientUtils:public ofxThread{
public:

	ofxSmtpClientUtils();
	~ofxSmtpClientUtils();


	//-------------------------------
	// non blocking functions
	void addMessage(ofxMailMessage message);

	//-------------------------------
	// blocking functions
	void sendMessage(ofxMailMessage & message);


	//-------------------------------
	// threading stuff
	void threadedFunction();
	void start();
	void stop();
protected:
	queue <ofxMailMessage> messages;
	bool verbose;
	bool connected;
	SMTPClientSession * session;
};

#endif
