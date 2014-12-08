#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "MyForm.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtppacket.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include "..\..\CapstoneHeaders\pandaheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace Project1;
using namespace jrtplib;

static RTPSession sess;
char devices_str[10][30];
char client_name[30] = "John's PC :)\0";
char ackd = 0;
uint8_t my_client_idx;
RTPIPv4Address my_address;

//
// This function checks if there was a RTP error. If so, it displays an error
// message and exists.
//

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		Sleep(4000);
		exit(-1);
	}
}


bool getMyIP(RTPIPv4Address * myIP)
{
	char szBuffer[1024];

#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return false;
#endif


	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return false;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return false;
	}

	//Obtain the computer's IP
	RTPIPv4Address addr(((struct in_addr *)(host->h_addr))->S_un.S_addr, 6000);
	memcpy(myIP, &addr, sizeof(RTPIPv4Address));

#ifdef WIN32
	WSACleanup();
#endif
	return true;
}

void ask_devices(){
	printf("\n Asking for Devices \n");
	//RTPIPv4Address addr;
	//getMyIP(&addr);
	uint32_t tmp = my_address.GetIP();
	printf(" %u \n", my_address.GetIP());
	checkerror( sess.SendPacketEx(&tmp, 4, GET_DEV, 0, 0) );
}

void MyForm::send_upd_packet(){
	/*
	printf("Sending packet \n");
	signupPacket sp;
	sp.address = my_address->GetIP();
	strcpy_s( &sp.name[0], STR_BUFF_SZ, &client_name[0]);

	sess.SendPacketEx(&sp, sizeof(signupPacket), SET_NAME, 0, 0);
	*/
	ask_devices();
}


/********************************************************************
*						GET Packets to ID new Sources
*
*
*
********************************************************************/
void MyForm::pollPackets(){
	
	if (ackd == 0){
		if (my_client_idx == NUM_MAX_CLIENTS){
			signupPacket sp;
			uint32_t addr = my_address.GetIP();
			memcpy(&sp.address, &addr, 4);
			strcpy_s(&sp.name[0], STR_BUFF_SZ, &client_name[0]);

			sess.SendPacketEx(&sp, sizeof(signupPacket), SET_NAME, 0, 0);
		}
		else{
			ask_devices();
		}
	}
	sess.BeginDataAccess();
	// check incoming packets
	if (sess.GotoFirstSourceWithData())
	{
		do
		{
			RTPPacket *pack;

			while ((pack = sess.GetNextPacket()) != NULL)
			{
				uint16_t header;
				header = pack->GetExtensionID();

				uint16_t idx = 0;
				uint8_t placed = FALSE;
				// You can examine the data here
				//printf("Got packet !\n");
				switch (header){
				case PANDA_ACK: { 
					memcpy_s(&my_client_idx, 1, pack->GetPayloadData(), 1); 
					printf("My idx is %u \n", my_client_idx);
					break; 
				}

				case PANDA_NACK: { printf(" NACK RECIEVED \n"); break; }
				case SET_ID:{
					memcpy(&my_client_idx, pack->GetPayloadData(), 1);
					ask_devices();
					break;
				}

				case SET_DEV: {
					printf(" SET DEV RECIEVED \n");
					ackd = 1;
					devPacket * devp;
					devp = (devPacket *)pack->GetPayloadData();
					for (idx = 0; idx < NUM_MAX_DEVICES; idx++){
						if (strlen( devp->devices[idx]) > 2){
							//printf("good");
							sprintf_s( devices_str[idx], 30, devp->devices[idx]);
						}
						else{
							//printf("nope");
							strcpy_s(&(devices_str[idx][0]), 2, "\0");
						}
					}
					//printf("parsed");
					this->Devices->BeginUpdate();
					this->Devices->DataSource = (gcnew cli::array< System::Object^  >(10) {
						gcnew String(&devices_str[0][0]),
						gcnew String(&devices_str[1][0]),
						gcnew String(&devices_str[2][0]),
						gcnew String(&devices_str[3][0]),
						gcnew String(&devices_str[4][0]),
						gcnew String(&devices_str[5][0]),
						gcnew String(&devices_str[6][0]),
						gcnew String(&devices_str[7][0]),
						gcnew String(&devices_str[8][0]),
						gcnew String(&devices_str[9][0]),
					});
					this->Devices->EndUpdate();
					break;
				}
				default: { printf("can't ID packet"); break; }
				}

				// we don't longer need the packet, so
				// we'll delete it				
				sess.DeletePacket(pack);
			}
		} while (sess.GotoNextSourceWithData());
	}

	sess.EndDataAccess();
}

void MyForm::send_select(bool * selected){
	selectPacket sp;
	int i;

	sp.cl_array_idx = my_client_idx;
	for (i = 0; i < NUM_MAX_DEVICES; i++){
		if (selected[i]){
			sp.selected[i] = 1;
		}
		else{
			sp.selected[i] = 0;
		}
	}
	printf(" d1: %u, d2 %u", sp.selected[0], sp.selected[1]);
	sess.SendPacketEx(&sp, sizeof(selectPacket), SET_OUT, 0, 0);
	
}
void MyForm::set_address(uint32_t addr){
	//printf("setting IP to %i \n", addr);
	my_address.SetIP(addr);
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif // WIN32

	uint16_t portbase, destport;
	uint32_t destip;
	uint32_t idx;
	int status;

	my_client_idx = NUM_MAX_CLIENTS; 
	for (idx = 0; idx < NUM_MAX_DEVICES; idx++){
		strcpy_s( devices_str[idx], 2,  "\0");
	}

	// Enabling Windows XP visual effects before any controls are created
	MessageBox::Show("Oppening a Port to Listen...");
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//establish rtp protocols
	inet_pton(AF_INET, "192.168.1.5", &destip);
	destip = ntohl(destip);

	portbase = 6000;
	destport = 6000;

	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	sessparams.SetOwnTimestampUnit(1.0 / 10.0);
	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams, &transparams);
	checkerror(status);
	sess.SetDefaultPayloadType(0);
	checkerror(status);
	sess.SetDefaultMark(0);
	checkerror(status);
	sess.SetDefaultTimestampIncrement(1);
	checkerror(status);

	RTPIPv4Address addr(destip, destport);

	status = sess.AddDestination(addr);
	checkerror(status);

	//status = send_upd_packet();
	//checkerror(status);



	//find all decives
	//status = ask_devices();
	//checkerror(status);

	// Create the main window and run it
	Application::Run(gcnew MyForm());

	sess.BYEDestroy(RTPTime(10, 0), 0, 0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 0;
}