#include <PControl.h>

PControl::PControl(unsigned char packet[]) 
{
	printf("Parsing control packet.....\n");
	length = (packet[0] << 8) | packet[1];
	printf("Packet length: %u\n", length);
	if(length >= 14)
	{
		opcode = (packet[2] >> 3);
		printf("opcode: %u\n", opcode);
		
		keyID = (packet[2] & 0x07);
		printf("keyID: %u\n", keyID);
		
		localSessionID = 0;
		for(int i = 3; i <= 10; i++)
			localSessionID = (localSessionID << 8) | packet[i];
		printf("localSessionID: %llu\n", localSessionID);
		
		ackLength = packet[11];
		int position = 12;
		remoteSessionID = 0;
		if(ackLength > 0)
		{
			printf("Parsing ack array of length: %u\n", ackLength);
			for(int i = 0; i < ackLength; i++)
			{
				for(int j = 0; j < 4; j++)
					ackIDs[(i*4)+j] = packet[position+j];
				position = position+4;
			}
			for(int i = position; i < position+8; i++)
				remoteSessionID = (remoteSessionID << 8) | packet[i];
			printf("remoteSessionID: %llu\n", remoteSessionID);
			position = position+8;
		}
		
		messagePacketID = 0;
		for(int i = position; i < position+4; i++)
			messagePacketID = (messagePacketID << 8) | packet[i];
		printf("messagePacketID: %lu\n", messagePacketID);
		position = position+4;
		
		if(position < length+2)
		{
			tlsLength = length+2-position;
			printf("TLS payload length: %u\n", tlsLength);
			printf("TLS payload: \n");
			for(int i = position; i < length+2; i++)
			{
				tlsPayload[i-position] = packet[i];
				printf("0x%02x ", packet[i]);
			}
			printf("\n");
		}
		
		printf("Parsing finished.\n\n");
	}
	else
		printf("Parsing failed, packet length too short.\n\n");
}

void PControl::toPacket(unsigned char* buff)
{
	//write length
	buff[0] = (length >> 8) & 0xff;
	buff[1] = length & 0xff;
	
	//write opcode and keyID
	buff[2] = ((opcode << 3) | keyID) & 0xff;
	
	//write local session ID
	for(int i = 10; i >= 3; i--)
		buff[3+10-i] = (localSessionID >> 8*(i-3)) & 0xff;
	
	//write ack ID array length
	buff[11] = ackLength & 0xff;
	
	int position = 12;
	if(ackLength > 0)
	{
		//write ack ID's
		for(int i = 0; i < ackLength; i++)
		{
			for(int j = 0; j < 4; j++)
				buff[position+j] = ackIDs[(i*4)+j];
			position = position+4;
		}
		
		//write remote session ID
		int j = position+7;
		for(int i = position; i <= position+7; i++)
		{
			buff[j--] = (remoteSessionID >> 8*(i-position)) & 0xff;
		}
		position = position+8;
	}
	
	//write message packet ID
	int j = position+3;
	for(int i = position; i <= position+3; i++)
	{
		buff[j--] = (messagePacketID >> 8*(i-position)) & 0xff;
	}
	position = position+4;
	
	//write tls payload if present
	if(tlsLength > 0)
		for(int i = position; i < length+2; i++)
			buff[i] = tlsPayload[i-position];
}

int PControl::packetSize()
{
	return length+2;
}

void PControl::setLength(unsigned short newLength)
{
	length = newLength;
}

unsigned short PControl::getLength()
{
	return length;
}

void PControl::setOPcode(unsigned short newOPcode)
{
	opcode = newOPcode;
}

unsigned short PControl::getOPcode()
{
	return opcode;
}

void PControl::setKeyID(unsigned short newKeyID)
{
	keyID = newKeyID;
}

unsigned short PControl::getKeyID()
{
	return keyID;
}

void PControl::setLocalSessionID(unsigned long long newLocalSessionID)
{
	localSessionID = newLocalSessionID;
}

unsigned long long PControl::getLocalSessionID()
{
	return localSessionID;
}

void PControl::setRemoteSessionID(unsigned long long newRemoteSessionID)
{
	remoteSessionID = newRemoteSessionID;
}

unsigned long long PControl::getRemoteSessionID()
{
	return remoteSessionID;
}

void PControl::setMessagePacketID(unsigned long newMessagePacketID)
{
	messagePacketID = newMessagePacketID;
}

unsigned long PControl::getMessagePacketID()
{
	return messagePacketID;
}

void PControl::setAckLength(unsigned short newAckLength)
{
	ackLength = newAckLength;
}

unsigned short PControl::getAckLength()
{
	return ackLength;
}

void PControl::setAckIDs(unsigned char newIDs[], int length)
{
	if(length <= 20)
		for(int i = 0; i < length; i++)
			ackIDs[i] = newIDs[i];
}

unsigned char* PControl::getAckIDs()
{
	return ackIDs;
}

void PControl::setTLSPayload(unsigned char newPayload[], int length)
{
	if(length <= 1200)
	{
		tlsLength = length;
		for(int i = 0; i < length; i++)
			tlsPayload[i] = newPayload[i];
	}
}

unsigned char* PControl::getTLSPayload()
{
	return tlsPayload;
}