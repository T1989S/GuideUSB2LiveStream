#ifndef	 _GUIDEUSB2LIVESTREAM_H_
#define  _GUIDEUSB2LIVESTREAM_H_

#define GUIDEUSBLIVESTREAM_API __declspec(dllexport)

enum guide_usb_video_mode_e
{
	X16 = 0,                           //X16 
	X16_PARAM = 1,              //X16+参数行
	Y16 = 2,                           //Y16
	Y16_PARAM = 3,              //Y16+参数行
	YUV = 4,                          //YUV 
	YUV_PARAM = 5,            //YUV+参数行
	Y16_YUV = 6,                  //Y16+YUV
	Y16_PARAM_YUV = 7     //Y16+参数行+YUV
};

enum guide_usb_device_status_e
{
	DEVICE_CONNECT_OK = 1,                 //连接正常
	DEVICE_DISCONNECT_OK = -1,          //断开连接
};

struct guide_usb_device_info_t
{
	int width;                                                   //图像宽度
	int height;                                                  //图像高度
	guide_usb_video_mode_e video_mode;      //视频模式
};

struct guide_usb_frame_data_t
{
	int frame_width;                              //图像宽度
	int frame_height;                             //图像高度
	BYTE* frame_rgb_data;                    //rgb数据
	int frame_rgb_data_length;              //rgb数据长度
	short* frame_src_data;                     //原始数据
	int frame_src_data_length;               //原始数据长度
	short* frame_yuv_data;                    //yuv数据
	int frame_yuv_data_length;              //yuv数据长度
	short* paramLine;                            //参数行
	int paramLine_length;                      //参数行长度
};

struct guide_usb_serial_data_t
{
	BYTE* serial_recv_data;					 //接收数据
	int serial_recv_data_length;			 //接收的数据长度
};

struct guide_usb_measure_external_param_t
{
	unsigned short emiss;					//发射率
	unsigned short relHum;				//湿度       
	unsigned short distance;				//距离
	short reflectedTemper;					//反射温度
	short atmosphericTemper;			//环境温度
	unsigned short modifyK;				//曲率修正参数
	short modifyB;								//绝对温度修正参数   
};

typedef int(_stdcall *OnDeviceConnectStatusCB)(guide_usb_device_status_e deviceStatus);
typedef int(_stdcall *OnFrameDataReceivedCB)(guide_usb_frame_data_t *pVideoData);
typedef int(_stdcall *OnSerialDataReceivedCB)(guide_usb_serial_data_t *pSerialData);

extern "C"
{
	//初始化
	GUIDEUSBLIVESTREAM_API int guide_usb_initial();
	//获取控制命令
	GUIDEUSBLIVESTREAM_API int guide_usb_getserialdata(int deviceID, OnSerialDataReceivedCB serialRecvCB);
	//连接设备
	GUIDEUSBLIVESTREAM_API int guide_usb_openstream(guide_usb_device_info_t* deviceInfo, OnFrameDataReceivedCB frameRecvCB, OnDeviceConnectStatusCB connectStatusCB);
	//断开设备                                                        
	GUIDEUSBLIVESTREAM_API int guide_usb_closestream();
	//发送命令
	GUIDEUSBLIVESTREAM_API int guide_usb_sendcommand(char* cmd, int length);
	//升级
	GUIDEUSBLIVESTREAM_API int guide_usb_upgrade(const char* file);
	//设置伪彩
	GUIDEUSBLIVESTREAM_API int guide_usb_setpalette(int index);
	//退出
	GUIDEUSBLIVESTREAM_API int guide_usb_exit();
};
#endif