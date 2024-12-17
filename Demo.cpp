#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "guideusb2livestream.h"

#define WIDTH 640
#define HEIGHT 512

//定义函数指针
typedef int(*_guide_usb_initial)(void);
typedef int(*_guide_usb_exit)(void);
typedef int(*_guide_usb_openstream)(guide_usb_device_info_t* , OnFrameDataReceivedCB , OnDeviceConnectStatusCB );
typedef int(*_guide_usb_closestream)(void);
typedef int(*_guide_usb_sendcommand)(unsigned char* , int);
typedef int(*_guide_usb_getserialdata)(int,OnSerialDataReceivedCB);

//定义帧数据回调
int __stdcall OnFrameDataCallBack(guide_usb_frame_data_t* pVideoData)
{
	printf("OnFrameDataCallBack\n");
	printf("frame_src_data_length = %d\n", pVideoData->frame_src_data_length);
	printf("frame_rgb_data_length = %d\n", pVideoData->frame_rgb_data_length);
	// pVideoData->frame_yuv_data YUV数据
	// pVideoData->frame_src_data Y16数据     pGray
	// pVideoData->paramLine 参数行数据        pParamline

	// 将 Y16 数据转换为 OpenCV 格式
	cv::Mat grayFrame(HEIGHT, WIDTH, CV_16U, pVideoData->frame_src_data);
	cv::Mat displayFrame;

	// 转换为可视化的 8 位灰度图像
	grayFrame.convertTo(displayFrame, CV_8U, 255.0 / 65535.0);

	// 显示图像
	cv::imshow("Infrared Video Stream", displayFrame);
	if (cv::waitKey(1) == 27) {  // 按 ESC 键退出
		return -1;
	}

	return 0;
}

//定义视频流状态回调
int __stdcall OnConnectCallBack(guide_usb_device_status_e deviceStatus)
{
	switch (deviceStatus)
	{
	case DEVICE_CONNECT_OK:
		printf("VideoStream Capture start...\n");
		break;
	case DEVICE_DISCONNECT_OK:
		printf("VideoStream Capture end...\n");
		break;
	}
	return 0;
}

//定义端点通信回调
int __stdcall OnSerialDataReceivedCallBack(guide_usb_serial_data_t* pSerialData)
{
	for (int i = 0; i < pSerialData->serial_recv_data_length; i++)
	{
		if (i == (pSerialData->serial_recv_data_length - 1))
		{
			printf("%x\n", pSerialData->serial_recv_data[i]);
		}
		else
		{
			printf("%x ", pSerialData->serial_recv_data[i]);
		}
	}
	return 0;
}

HMODULE GuideUSB2LiveStream = NULL;
int main()
{
	GuideUSB2LiveStream = LoadLibrary("D:/气体成像/usb2.0_win_sdk_v1.3_20230824/demo/C++/src/Demo/Demo/Debug/GuideUSB2LiveStream.dll");
	if (GuideUSB2LiveStream == NULL) {
		DWORD errorCode = GetLastError();
		printf("加载GuideUSB2LiveStream.dll动态库失败，错误码: %lu\n", errorCode);
		//printf("加载GuideUSB2LiveStream.dll动态库失败\n");
	}

	else
	{
		printf("加载GuideUSB2LiveStream.dll动态库成功\n");
	}

	//获取dll函数接口地址
	_guide_usb_initial initialize = (_guide_usb_initial)GetProcAddress(GuideUSB2LiveStream,"guide_usb_initial");
	_guide_usb_openstream openStream = (_guide_usb_openstream)GetProcAddress(GuideUSB2LiveStream,"guide_usb_openstream");
	_guide_usb_closestream closeStream = (_guide_usb_closestream)GetProcAddress(GuideUSB2LiveStream,"guide_usb_closestream");
	_guide_usb_sendcommand sendcommand = (_guide_usb_sendcommand)GetProcAddress(GuideUSB2LiveStream,"guide_usb_sendcommand");
	_guide_usb_getserialdata getserialdata = (_guide_usb_getserialdata)GetProcAddress(GuideUSB2LiveStream, "guide_usb_getserialdata");

	int ret;
	ret = initialize();//初始化
	printf("guide_usb_initial ret = %d\n",ret);
	getserialdata(1,OnSerialDataReceivedCallBack);//使能端点通信

	unsigned char cmd[12] = { 0x55, 0xAA, 0x07, 0x02, 0x01, 0x08, 0x00, 0x00, 0x00, 0x01, 0x0D, 0xF0 };
	ret = sendcommand(cmd,12);//发送命令
	printf("guide_usb_sendcommand ret = %d\n", ret);

	guide_usb_device_info_t* deviceInfo  = (guide_usb_device_info_t*)malloc(sizeof(guide_usb_device_info_t));
	deviceInfo->width = WIDTH;
	deviceInfo->height = HEIGHT;
	deviceInfo->video_mode = Y16_PARAM_YUV;

	getchar();

	//开启视频流
	openStream(deviceInfo, OnFrameDataCallBack, OnConnectCallBack);

	printf("--------------------\n");

	// 保持窗口活动
	while (true) {
		if (cv::waitKey(1) == 27) {  // 按 ESC 键退出
			break;
		}
	}

	// 关闭视频流和释放资源
	closeStream();
	cv::destroyAllWindows();
	return 0;
}