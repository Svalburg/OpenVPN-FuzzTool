#ifndef PCONTROL_H
#define PCONTROL_H

#include <stdio.h>

using namespace std;

class PControl 
{
	public:
		PControl(unsigned char packet[]);
		void toPacket(unsigned char* buff);
		int packetSize();
		void setLength(unsigned short newLength);
		unsigned short getLength();
		void setOPcode(unsigned short newOPcode);
		unsigned short getOPcode();
		void setKeyID(unsigned short newKeyID);
		unsigned short getKeyID();
		void setLocalSessionID(unsigned long long newLocalSessionID);
		unsigned long long getLocalSessionID();
		void setRemoteSessionID(unsigned long long newRemoteSessionID);
		unsigned long long getRemoteSessionID();
		void setMessagePacketID(unsigned long newMessagePacketID);
		unsigned long getMessagePacketID();
		void setAckLength(unsigned short newAckLength);
		unsigned short getAckLength();
		void setAckIDs(unsigned char newIDs[], int length);
		unsigned char* getAckIDs();
		void setTLSPayload(unsigned char newPayload[], int length);
		unsigned char* getTLSPayload();
		
	private:
		unsigned short length;
		unsigned short opcode;
		unsigned short keyID;
		unsigned long long localSessionID;
		unsigned short ackLength;
		unsigned char ackIDs[20];
		unsigned long long remoteSessionID;
		unsigned long messagePacketID;
		unsigned int tlsLength; //NOTE: this field is not included in the network packet
		unsigned char tlsPayload[1200];
};

#endif //PCONTROL_H