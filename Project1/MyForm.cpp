#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "MyForm.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtppacket.h"
#include "Audioclient.h"
#include "Audiopolicy.h"
#include "Mmdeviceapi.h"
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


// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
				if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
				if ((punk) != NULL)  \
								{ (punk)->Release(); (punk) = NULL; }

static RTPSession sess;
char devices_str[10][30];
char client_name[30] = "John's PC\0";
char ackd = 0;
uint8_t p_control = 0;
bool stop_stream = true;
bool rec_init = false;
uint8_t my_client_idx;
RTPIPv4Address my_address;
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

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



void ask_devices(){
	printf(" Asking for Devices \n");
	//RTPIPv4Address addr;
	//getMyIP(&addr);
	uint32_t tmp = my_address.GetIP();
	//printf(" %u \n", my_address.GetIP());
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

HRESULT send_data(uint8_t * data, uint32_t numFrames, BOOL * done){

	//printf("got to send data");

	if (data != NULL){
		pandaPacketData pd;
		pd.cl_array_idx = my_client_idx;
		uint32_t idx, mx_pkt;
		mx_pkt = 0;

		for (idx = 0; idx < numFrames; idx++){
			if (mx_pkt == MAX_BUFF_SZ){
				pd.data_len = MAX_BUFF_SZ;
				sess.SendPacketEx(&pd, sizeof(pandaPacketData), AUDIO, 0, 0);
				mx_pkt = 0;
			}
			pd.data[idx] = data[idx];
			mx_pkt++;
		}
		*done = TRUE;
		//send any trailing data
		pd.data_len = mx_pkt;
		//printf("sent data \n");
		return sess.SendPacketEx(&pd, sizeof(pandaPacketData), AUDIO, 0, 0);
	}
	else{
		//printf("No cap \n");
		return sess.SendPacketEx(0, 0, AUDIO, 0, 0);;
	}	
}
HRESULT RecordAudioStream()
{
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioCaptureClient *pCaptureClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 packetLength = 0;
	BOOL bDone = FALSE;
	BYTE *pData;
	DWORD flags;

	stop_stream = false;

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	EXIT_ON_ERROR(hr)

		hr = pEnumerator->GetDefaultAudioEndpoint(
		eCapture, eConsole, &pDevice);
	EXIT_ON_ERROR(hr)

		hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		hnsRequestedDuration,
		0,
		pwfx,
		NULL);
	EXIT_ON_ERROR(hr)

		// Get the size of the allocated buffer.
		hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

		hr = pAudioClient->GetService(
		IID_IAudioCaptureClient,
		(void**)&pCaptureClient);
	EXIT_ON_ERROR(hr)

		// Notify the audio sink which format to use.
		//hr = pMySink->SetFormat(pwfx);
	//EXIT_ON_ERROR(hr)

		// Calculate the actual duration of the allocated buffer.
		hnsActualDuration = (double)REFTIMES_PER_SEC *
		bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start recording.
	EXIT_ON_ERROR(hr)

		// Each loop fills about half of the shared buffer.
		while (bDone == FALSE)
		{
		// Sleep for half the buffer duration.
		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		EXIT_ON_ERROR(hr)

			while (packetLength != 0)
			{
			// Get the available data in the shared buffer.
			hr = pCaptureClient->GetBuffer(
				&pData,
				&numFramesAvailable,
				&flags, NULL, NULL);
			EXIT_ON_ERROR(hr)

				if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
				{
				pData = NULL;  // Tell CopyData to write silence.
				}

			// Copy the available capture data to the audio sink.
			hr = send_data(pData, numFramesAvailable, &bDone);//pMySink->CopyData(
				//pData, numFramesAvailable, &bDone);
			EXIT_ON_ERROR(hr)

				hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			EXIT_ON_ERROR(hr)

				hr = pCaptureClient->GetNextPacketSize(&packetLength);
			EXIT_ON_ERROR(hr)
			}
		}

	hr = pAudioClient->Stop();  // Stop recording.
	EXIT_ON_ERROR(hr)

	Exit:
		//printf(" errored out \n ");
		CoTaskMemFree(pwfx);
		SAFE_RELEASE(pEnumerator)
		SAFE_RELEASE(pDevice)
		SAFE_RELEASE(pAudioClient)
		SAFE_RELEASE(pCaptureClient)

		return hr;
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

			sess.SendPacketEx(&sp, sizeof(sp), SET_NAME, 0, 0);
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
				default: { printf("can't ID packet \n"); break; }
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
	//printf(" d1: %u, d2 %u \n", sp.selected[0], sp.selected[1]);
	sess.SendPacketEx(&sp, sizeof(selectPacket), SET_OUT, 0, 0);
	
}
void end_stream(){
	if (stop_stream = false){
		stop_stream = true;
		CoUninitialize();
		rec_init = false;
	}
}
//send audio packets to stream
void MyForm::stream(){
	if (ackd == 1){
		if (!rec_init){
			CoInitialize(NULL);
		}
		//printf("called record stream");
		RecordAudioStream();
	}
	else{
		this->timer2->Stop(); 
		end_stream();
		MessageBox::Show("Please set your IP correctly First");
		
	}
}

void MyForm::stop_stream(){
	end_stream();
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
	//MessageBox::Show("Oppening a Port to Listen...");
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//establish rtp protocols
	inet_pton(AF_INET, "192.168.1.5", &destip);
	destip = ntohl(destip);

	portbase = 6002;
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