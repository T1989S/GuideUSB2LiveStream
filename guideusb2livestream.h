#ifndef	 _GUIDEUSB2LIVESTREAM_H_
#define  _GUIDEUSB2LIVESTREAM_H_

#define GUIDEUSBLIVESTREAM_API __declspec(dllexport)

enum guide_usb_video_mode_e
{
	X16 = 0,                           //X16 
	X16_PARAM = 1,              //X16+������
	Y16 = 2,                           //Y16
	Y16_PARAM = 3,              //Y16+������
	YUV = 4,                          //YUV 
	YUV_PARAM = 5,            //YUV+������
	Y16_YUV = 6,                  //Y16+YUV
	Y16_PARAM_YUV = 7     //Y16+������+YUV
};

enum guide_usb_device_status_e
{
	DEVICE_CONNECT_OK = 1,                 //��������
	DEVICE_DISCONNECT_OK = -1,          //�Ͽ�����
};

struct guide_usb_device_info_t
{
	int width;                                                   //ͼ����
	int height;                                                  //ͼ��߶�
	guide_usb_video_mode_e video_mode;      //��Ƶģʽ
};

struct guide_usb_frame_data_t
{
	int frame_width;                              //ͼ����
	int frame_height;                             //ͼ��߶�
	BYTE* frame_rgb_data;                    //rgb����
	int frame_rgb_data_length;              //rgb���ݳ���
	short* frame_src_data;                     //ԭʼ����
	int frame_src_data_length;               //ԭʼ���ݳ���
	short* frame_yuv_data;                    //yuv����
	int frame_yuv_data_length;              //yuv���ݳ���
	short* paramLine;                            //������
	int paramLine_length;                      //�����г���
};

struct guide_usb_serial_data_t
{
	BYTE* serial_recv_data;					 //��������
	int serial_recv_data_length;			 //���յ����ݳ���
};

struct guide_usb_measure_external_param_t
{
	unsigned short emiss;					//������
	unsigned short relHum;				//ʪ��       
	unsigned short distance;				//����
	short reflectedTemper;					//�����¶�
	short atmosphericTemper;			//�����¶�
	unsigned short modifyK;				//������������
	short modifyB;								//�����¶���������   
};

typedef int(_stdcall *OnDeviceConnectStatusCB)(guide_usb_device_status_e deviceStatus);
typedef int(_stdcall *OnFrameDataReceivedCB)(guide_usb_frame_data_t *pVideoData);
typedef int(_stdcall *OnSerialDataReceivedCB)(guide_usb_serial_data_t *pSerialData);

extern "C"
{
	//��ʼ��
	GUIDEUSBLIVESTREAM_API int guide_usb_initial();
	//��ȡ��������
	GUIDEUSBLIVESTREAM_API int guide_usb_getserialdata(int deviceID, OnSerialDataReceivedCB serialRecvCB);
	//�����豸
	GUIDEUSBLIVESTREAM_API int guide_usb_openstream(guide_usb_device_info_t* deviceInfo, OnFrameDataReceivedCB frameRecvCB, OnDeviceConnectStatusCB connectStatusCB);
	//�Ͽ��豸                                                        
	GUIDEUSBLIVESTREAM_API int guide_usb_closestream();
	//��������
	GUIDEUSBLIVESTREAM_API int guide_usb_sendcommand(char* cmd, int length);
	//����
	GUIDEUSBLIVESTREAM_API int guide_usb_upgrade(const char* file);
	//����α��
	GUIDEUSBLIVESTREAM_API int guide_usb_setpalette(int index);
	//�˳�
	GUIDEUSBLIVESTREAM_API int guide_usb_exit();
};
#endif