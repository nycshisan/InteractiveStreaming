/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#ifndef NT_SMART_PUBLISHER_SDK_H_
#define NT_SMART_PUBLISHER_SDK_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SMARTPUBLISHERSDK_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SMARTPUBLISHERSDK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SMARTPUBLISHERSDK_EXPORTS
#define SMARTPUBLISHERSDK_API __declspec(dllexport)
#else
#define SMARTPUBLISHERSDK_API __declspec(dllimport)
#endif

#include "nt_type_define.h"

#include "nt_smart_publisher_define.h"

#ifdef __cplusplus
extern "C"{
#endif

	/*
	 Image�����ṩ�����Ľṹ��, ���ʹ洢�ռ䣬���㿽��
	*/
	typedef struct _NT_SmartPublisherImageSDKAPI
	{
		/*
		����Image, �����SDK�ڲ����ʼ������ṹ��, ʧ�ܵĻ�����NULL
		*/
		NT_PB_Image* (NT_API *AllocateImage)(NT_VOID);

		/*
		�ͷ�Image, ע��һ��Ҫ��������ӿ��ͷ��ڴ棬��������Լ���ģ�����ͷţ�
		Windows��������,
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *FreeImage)(NT_PB_Image** ppImage);

		/*
		��¡һ��Image�� ʧ�ܷ���NULL
		*/
		NT_PB_Image* (NT_API* CloneImage)(const NT_PB_Image* src);

		/*
		����Image, �����ͷ�dst����Դ��Ȼ���ٿ���
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* CopyImage)(NT_PB_Image* dst, const NT_PB_Image* src);

		/*
		��ͼ��һ�����������ݣ����������Ѿ������ݣ������ͷŵ�������
		���������Ϊ������NT_PB_Image::format_
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetImagePlane)(NT_PB_Image* image, NT_INT32 plane,
			NT_INT32 planeStride, const NT_UINT8* planeData, NT_INT32 planeDataSize);

		/*
		����PNGͼƬ
		file_name_utf8: ������utf8����
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* LoadImage)
			(
			NT_PCSTR file_name_utf8,
			NT_PB_Image** ppImage
			);

	} NT_SmartPublisherImageSDKAPI;

	/*
	������ؽӿ�, ���ƺ������ܻ��ṩ����ӿ�
	*/
	typedef struct _NT_SmartPublisherDrawImageSDKAPI
	{
		/*
		�ɹ�����NT_ERC_OK
		*/
		NT_UINT32(NT_API *Draw)(NT_HDC hdc, NT_INT32 xDst, NT_INT32 yDst,
			NT_INT32 dstWidth, NT_INT32 dstHeight,
			NT_INT32 xSrc, NT_INT32 ySrc,
			NT_INT32 srcWidth, NT_INT32 srcHeight,
			const NT_PB_Image* image);

	} NT_SmartPublisherDrawImageSDKAPI;


	typedef struct _NT_SmartPublisherSDKAPI
	{
		/*
		flagĿǰ��0��������չ�ã� pReserve��NULL,��չ��,
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *Init)(NT_UINT32 flag, NT_PVOID pReserve);

		/*
		��������һ�����õĽӿ�
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *UnInit)();

		/*
		video_option ��ο� NT_PB_E_VIDEO_OPTION,
		auido_option ��ο� NT_PB_E_AUDIO_OPTION
		flagĿǰ��0��������չ�ã� pReserve��NULL,��չ��,
		��ȡHandle
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *Open)(NT_PHANDLE pHandle, NT_UINT32 video_option, NT_UINT32 auido_option, NT_UINT32 flag, NT_PVOID pReserve);

		/*
		��������ӿ�֮��handleʧЧ��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *Close)(NT_HANDLE handle);


		/*
		�����¼��ص������������¼��Ļ����������Open�ɹ��󣬾͵�������ӿ�
		*/
		NT_UINT32(NT_API *SetEventCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_PB_SDKEventCallBack call_back);


		/*�����������*/
		/*
		������Ļ�ü�
		left: ��Ļ���Ͻ�xλ��
		top:  ��Ļ���Ͻ�yλ��
		width: ���, ������16�ı���
		height: �߶�, ������16�ı���
		*/
		NT_UINT32(NT_API *SetScreenClip)(NT_HANDLE handle, NT_UINT32 left, NT_UINT32 top, NT_UINT32 width, NT_UINT32 height);


		/*
		�ƶ���Ļ������������ӿ�ֻ�����ͻ���¼���е���
		left: ��Ļ���Ͻ�xλ��
		top:  ��Ļ���Ͻ�yλ��
		*/
		NT_UINT32(NT_API *MoveScreenClipRegion)(NT_HANDLE handle, NT_UINT32 left, NT_UINT32 top);


		/*
		����ʹ��DXGI��Ļ�ɼ���ʽ, ���ַ�ʽ��Ҫwin8������ϵͳ��֧��
		is_enable: 1��ʾ����DXGI�ɼ���0��ʾ������, ����ֱֵ�ӷ��ش���
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *EnableDXGIScreenCapturer)(NT_HANDLE handle, NT_INT32 is_enable);


		/*
		*�ɼ���Ļʱͣ��Aero, ���ֻ��win7��Ӱ�죬win8������ϵͳ, ΢���Ѿ�������Aero GlassЧ��
		*is_disable: 1:��ʾͣ��,�������Ϊ1�Ļ�����win7ϵͳ�Ͽ�ʼ����ʱ���ܺ�����˸һ��, 0:��ʾ��ͣ��
		* sdk�ڲ�Ĭ��ֵ��1
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *DisableAeroScreenCapturer)(NT_HANDLE handle, NT_INT32 is_disable);


		/*
		����ӿ���Ҫ�ж϶��㴰���ܷ��ܱ�����, ������ܱ�����Ļ�����NT_ERC_FAILED
		�������NT_ERC_OK,��ʾ�����ܲ���
		*/
		NT_UINT32(NT_API *CheckCapturerWindow)(NT_HWND hwnd);

		/*
		����Ҫ����Ĵ��ڵľ��
		*/
		NT_UINT32(NT_API *SetCaptureWindow)(NT_HANDLE handle, NT_HWND hwnd);


		/*
		����֡��
		*/
		NT_UINT32(NT_API *SetFrameRate)(NT_HANDLE handle, NT_UINT32 frame_rate);

		/*+++����ͷ��ؽӿ�+++*/
		
		/*
		��ȡ����ͷ����
		pNumer: �����豸����
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceNumber)(NT_INT32* pNumer);

		/*
		 ��������ͷ�豸��Ϣ
		 device_index: �豸����
		 device_name_utf8:  �豸���ƣ���NULL�����������ƣ��������NULL�Ļ�, device_name_length������ڵ���256, ����utf8������豸����
		 device_name_length: �豸���ƻ����С�����device_name_utf8��NULL������0, ���������ڵ���256
		 device_unique_id_utf8: �豸ΨһID, ��NULL��������ID,�������NULL�Ļ���device_unique_id_length������ڵ���1024,����utf8������豸ID
		 device_unique_id_length: �豸ΨһID�������, �����device_unique_id_utf8NULL, ����0�����������ڵ���1024
		 �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceInfo)(
			NT_INT32	device_index,
			NT_PSTR		device_name_utf8,
			NT_UINT32	device_name_length,
			NT_PSTR		device_unique_id_utf8,
			NT_UINT32	device_unique_id_length
			);

		/*
		��������ͷ������
		device_unique_id_utf8: �豸Ψһid
		capability_number: ���ص��豸������
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceCapabilityNumber)(
			NT_PCSTR device_unique_id_utf8,
			NT_INT32* capability_number
			);

		/*
		��������ͷ����
		device_unique_id_utf8: �豸Ψһid,
		device_capability_index: �豸��������
		capability: �豸����
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceCapability)(
			NT_PCSTR device_unique_id_utf8,
			NT_INT32 device_capability_index,
			NT_PB_VideoCaptureCapability* capability);


		/*
		* �ڶ��ʵ�����Ͷ�·ʱ������һ������ͷ��˵,����ʵ��ֻ�ܹ�������ͷ����ôֻ��һ��ʵ�����Ըı�����ͷ�ֱ���,
		  ����ʵ��ʹ��������ź��ͼ��.
		  ��ʹ�ö�ʵ��ʱ����������ӿڽ�ֹ��ʵ���ķֱ�����������.ֻ��һ��ʵ���ܸı�ֱ棬��������ã���Ϊ
		  δ����.
		  ����ӿڱ����� SetLayersConfig, AddLayerConfig ֮ǰ����
		  device_unique_id_utf8: �豸Ψһid
		  �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* DisableVideoCaptureResolutionSetting)(
			NT_HANDLE handle,
			NT_PCSTR device_unique_id_utf8
			);


		/*
		��������ͷԤ��
		device_unique_id_utf8: �豸Ψһid
		hwnd: ���ƵĴ��ھ��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* StartVideoCaptureDevicePreview)(
			NT_PCSTR device_unique_id_utf8,
			NT_HWND hwnd
			);

		/*
		*���·�ת����ͷԤ��ͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipVerticalCameraPreview)(NT_HWND hwnd, NT_INT32 is_flip);


		/*
		*ˮƽ��ת����ͷԤ��ͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipHorizontalCameraPreview)(NT_HWND hwnd, NT_INT32 is_flip);


		/*
		*��ת����ͷԤ��ͼ��, ˳ʱ����ת
		degress�� ����0�� 90�� 180�� 270����Ч������ֵ��Ч
		ע�⣺����0�ȣ������ǶȲ��Ż�ķѸ���CPU
		*/
		NT_UINT32(NT_API *RotateCameraPreview)(NT_HWND hwnd, NT_INT32 degress);


		/*
		����SDKԤ�����ڴ�С�ı�
		hwnd: ���ƵĴ��ھ��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* VideoCaptureDevicePreviewWindowSizeChanged)
			(
			NT_HWND hwnd
			);

		/*
		ֹͣ����ͷԤ��
		hwnd: ���ƵĴ��ھ��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* StopVideoCaptureDevicePreview)(
			NT_HWND hwnd
			);


		/***************************/
		/*****��ȡ����ͷRBG32ͼ��ӿ�++/
		/***************************/
		/*
		��������:
		1. StartGetVideoCaptureDeviceImage ��ȡ���,�б�����
		2. GetVideoCaptureDeviceImage ��ȡͼ��
		3. StopGetVideoCaptureDeviceImage ֹͣ�� ֮��������Ч
		*/

		/*
		���ô˽ӿ�ǰ������: ������ù�Init�ӿ�, ����ᱼ����ʧ��

		pVideoCaptrueImageHandle: Ҫ���صľ����ָ�룬�벻Ҫ������������
		device_unique_id_utf8: �豸Ψһid
		�ɹ����� NT_ERC_OK ������VideoCaptrueImageHandle���
		*/
		
		NT_UINT32(NT_API* StartGetVideoCaptureDeviceImage)
			(
			NT_PHANDLE pVideoCaptrueImageHandle,
			NT_PCSTR   device_unique_id_utf8
			);


		/*
		*���·�ת�豸ͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipVerticalVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 is_flip);


		/*
		*ˮƽ��ת�豸ͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipHorizontalVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 is_flip);


		/*
		*��ת�豸ͼ��, ˳ʱ����ת
		degress�� ����0�� 90�� 180�� 270����Ч������ֵ��Ч
		ע�⣺����0�ȣ������ǶȲ��Ż�ķѸ���CPU
		*/
		NT_UINT32(NT_API *RotateVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 degress);

		/*
		��������ӿڿ��Ի�ȡ����ͷͼ��

		videoCaptrueImageHandle�� ���, ͨ��StartGetVideoCaptureDeviceImage�õ���
		
		isDiscardImage: 1����ʾȡ��ͼ��󣬾Ͱ�SDK�ڲ������ͼ��ɾ����, 0:��ʾȡ��ͼ���, SDK�ڲ���ͼ��ɾ����
		��ô��һ���ٵ�������ӿڵ�ʱ���������ͷû�в����µ�ͼ�񣬾ͻ᷵���ϴη��ع���ͼ��
		
		ppImage: ָ��ͼ��ָ���ָ�룬�����ͼ��Ļ�, ����� *ppImage

		����ֵ��
		NT_ERC_OK�� ��ʾȡ��ͼ��, *ppImage��Ȼ��ֵ
		NT_ERC_PB_NO_IMAGE: ��ʾSDK�ڲ�Ŀǰû��ͼ����Ҫ�ȴ�����ͷ����ͼ��
		����ֵ: �������������󣬱����������ȵ�

		ע��:
		1. ����ӿڷ��ص�ͼ�� ��͸߿��ܻ�䣬 ����˵ÿһ�ε��÷��ص�ͼ���߿��ܲ�һ��
		2. ȡ����ͼ���Ǵ��ϵ��µ�
		*/

		NT_UINT32(NT_API* GetVideoCaptureDeviceImage)
			(
			NT_HANDLE     videoCaptrueImageHandle,
			NT_INT32      isDiscardImage,
			NT_PB_Image** ppImage
			);


		/*
		ֹͣ��ȡ����ͷͼ��
		�ɹ����� NT_ERC_OK

		ע��:�����ڵ���UnInit֮ǰ����
		*/
		NT_UINT32(NT_API* StopGetVideoCaptureDeviceImage)
			(
			NT_HANDLE  videoCaptrueImageHandle
			);


		/***************************/
		/*****��ȡ����ͷRBA32ͼ��ӿ�--/
		/***************************/

		/*
		��������ͷ��Ϣ
		*/
		NT_UINT32(NT_API *SetVideoCaptureDeviceBaseParameter)(NT_HANDLE handle, NT_PCSTR device_unique_id_utf8, NT_UINT32 width, NT_UINT32 height);

		/*
		*���·�ת����ͷͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipVerticalCamera)(NT_HANDLE handle, NT_INT32 is_flip);

		/*
		*ˮƽ��ת����ͷͼ��
		*is_flip: 1:��ʾ��ת, 0:��ʾ����ת
		*/
		NT_UINT32(NT_API *FlipHorizontalCamera)(NT_HANDLE handle, NT_INT32 is_flip);

		/*
		*��ת����ͷͼ��, ˳ʱ����ת
		degress�� ����0�� 90�� 180�� 270����Ч������ֵ��Ч
		ע�⣺����0�ȣ������ǶȲ��Ż�ķѸ���CPU
		*/
		NT_UINT32(NT_API *RotateCamera)(NT_HANDLE handle, NT_INT32 degress);


		/*---����ͷ��ؽӿ�---*/


		/*+++��Ƶ�ϳ��������+++*/
		

		/*
		������Ƶ�ϳɲ㣬 �������һ�����飬 ����ȷ���ÿһ��
		reserve: ������������� ��ǰ�봫0
		confs: ����������
		count: �����С
		flag: Ŀǰ��0
		pReserve: ����������
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetLayersConfig)(NT_HANDLE handle, NT_INT32 reserve, 
			const NT_PB_LayerBaseConfig** confs, NT_INT32 count,
			NT_UINT32 flag, NT_PVOID pReserve);


		/*
		������в����ã�ע������ӿ�ֻ�������ͻ���¼��֮ǰ���ã�������δ����
		����ӿ���Ҫ�Ǹ�C#ʹ��,C++��ֱ��ʹ��SetLayersConfig
		reserve: ������������� ��ǰ�봫0
		flag: Ŀǰ��0
		pReserve: ����������
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32 (NT_API *ClearLayersConfig)(NT_HANDLE handle, NT_INT32 reserve,
			NT_UINT32 flag, NT_PVOID pReserve);


		/*
		���Ӳ����ã�ע������ӿ�ֻ�������ͻ���¼��֮ǰ���ã�������δ����
		����ӿ���Ҫ�Ǹ�C#ʹ��,C++��ֱ��ʹ��SetLayersConfig
		reserve: ������������� ��ǰ�봫0
		conf: ���ò�
		layer_type: ������
		flag: Ŀǰ��0
		pReserve: ����������
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32 (NT_API *AddLayerConfig)(NT_HANDLE handle, NT_INT32 reserve,
			NT_PVOID conf, NT_INT32 layer_type,
			NT_UINT32 flag, NT_PVOID pReserve);

		/*
		��̬��ֹ�������ò�
		index: �������� �����ǵ�0�㣬�����0�Ļ�����ʧ��
		reserve: �����ֶΣ��봫0
		is_enable: 1��ʾ���ã�0��ʾ��ֹ, ����ֱֵ�ӷ��ش���
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *EnableLayer)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, NT_INT32 is_enable);

		/*
		���²��������, ע�ⲻ�ǲ�������ֶζ����Ը��£�ֻ�ǲ��ֿ��Ը��£�������Щ��û���ֶο��Ը���,
		����Ĳ�����SDKֻѡ���ܸ��µ��ֶθ��£����ܸ��µ��ֶλᱻ����
		reserve: �����ֶΣ��봫0
		conf: ����
		flag: �봫0
		pReserve: �����ֶΣ��봫0
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API * UpdateLayerConfig)(NT_HANDLE handle, NT_INT32 reserve,
			const NT_PB_LayerBaseConfig* conf, NT_UINT32 flag, NT_PVOID pReserve);


		/*
		����ӿ��Ǹ�C#ʹ�õ�, C++��ʹ�������UpdateLayerConfig�ӿ�
		���²��������, ע�ⲻ�ǲ�������ֶζ����Ը��£�ֻ�ǲ��ֿ��Ը��£�������Щ��û���ֶο��Ը���,
		����Ĳ�����SDKֻѡ���ܸ��µ��ֶθ��£����ܸ��µ��ֶλᱻ����
		reserve: �����ֶΣ��봫0
		conf: ����
		flag: �봫0
		pReserve: �����ֶΣ��봫0
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API * UpdateLayerConfigV2)(NT_HANDLE handle, NT_INT32 reserve,
			NT_PVOID conf, NT_INT32 layer_type, NT_UINT32 flag, NT_PVOID pReserve);

		/*
		�޸Ĳ�����
		pReserve: �����ֶΣ��봫0
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *UpdateLayerRegion)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, const NT_PB_RectRegion* region);

		/*
		��index��Ͷ��Image���ݣ�Ŀǰ��Ҫ��������rgb��yuv��Ƶ���ݴ�����ز�
		reserve: �����ֶΣ��봫0
		index: ������
		image: ͼ��
		flag: �봫0
		pReserve: �����ֶΣ��봫0
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API * PostLayerImage)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, const NT_PB_Image* image, NT_UINT32 flag, NT_PVOID pReserve);

		/*---��Ƶ�ϳ��������---*/

		
		/*+++��Ƶ������ؽӿ�+++*/

		/*
		����ͼƬ
		file_name_utf8: �ļ����ƣ�utf8����
		call_back_data: �ص�ʱ�û��Զ�������
		call_back: �ص�����,����֪ͨ�û���ͼ�Ѿ���ɻ���ʧ��
		�ɹ����� NT_ERC_OK
		ֻ�������ͻ���¼��ʱ���òſ��ܳɹ�����������µ��ã����ش���.
		��Ϊ����PNG�ļ��ȽϺ�ʱ��һ����Ҫ���ٺ���,Ϊ��ֹCPU���ߣ�SDK�����ƽ�ͼ��������,������һ������ʱ��
		��������ӿڻ᷵��NT_ERC_PB_TOO_MANY_CAPTURE_IMAGE_REQUESTS. ���������, ����ʱһ��ʱ�䣬��SDK�����һЩ������ٳ���.
		*/
		NT_UINT32(NT_API* CaptureImage)(NT_HANDLE handle, NT_PCSTR file_name_utf8,
			NT_PVOID call_back_data, NT_PB_SDKCaptureImageCallBack call_back);


		/*---��Ƶ������ؽӿ�---*/



		/*+++��Ƶ������ؽӿ�+++*/

		/*
		ע�⣬���ʿ������ַ�ʽ��Ҫô�� SetVideoQuality + SetVideoMaxBitRate
		Ҫô�� SetVideoMaxBitRate  + SetVideoBitRate
		*/
		/*
		������Ƶ����, ��Χ[0-20], Ĭ����10, ֵԽС����Խ�ã������ʻ�Խ��
		*/
		NT_UINT32(NT_API *SetVideoQuality)(NT_HANDLE handle, NT_INT32 quality);

		/*
		���������Ƶ����, ��λkbps
		*/
		NT_UINT32(NT_API *SetVideoMaxBitRate)(NT_HANDLE handle, NT_INT32 kbit_rate);

		/*
		������Ƶ����, ��λkbps, Ĭ����0����ʹ��ƽ�����ʷ�ʽ
		*/
		NT_UINT32(NT_API *SetVideoBitRate)(NT_HANDLE handle, NT_INT32 kbit_rate);


		/*
		* ��һЩ���ⳡ����, ��Ƶ�ֱ��ʻ�ı�, �������һ���̶����ʵĵĻ�,����Ƶ�ֱ��ʱ���ʱ�����ģ��,��С�Ļ��ֻ��˷�����
		* �����ṩ��������һ�����ʵĽӿ�,���㲻ͬ�ֱ����л�������
		* ����: ������������ֱ��� 640*360, 640*480, ��ô���ֱ���С�ڵ���640*360ʱ��ʹ��640*360������,
		* ���ֱ��ʴ���640*360��С�ڵ���640*480ʱ����ʹ��640*480������,����ֱ��ʴ���640*480 �Ǿ�ʹ��640*480�ķֱ���
		* Ϊ�����õĸ�׼ȷ, ����໮�ּ���, �������С
		* ��������ӿ�ÿ������һ��,���ö���͵��ö��
		*
		*/
		NT_UINT32(NT_API* AddVideoEncoderBitrateGroupItem)(NT_HANDLE handle, const NT_PB_VideoEncoderBitrateGroupItem* item);

		
		/*
		�����Ƶ������
		*/
		NT_UINT32(NT_API* ClearVideoEncoderBitrateGroup)(NT_HANDLE handle);


		/*
		���ùؼ�֡���, ����1��ʾ����֡���ǹؼ�֡��10��ʾÿ10֡����һ���ؼ�֡��25��ʾÿ25֡һ���ؼ�֡
		*/
		NT_UINT32(NT_API *SetVideoKeyFrameInterval)(NT_HANDLE handle, NT_INT32 interval);


		/*
		����H264 profile.
		profile: 1: H264 baseline(Ĭ��ֵ). 2: H264 main. 3. H264 high
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetVideoEncoderProfile)(NT_HANDLE handle, NT_INT32 profile);

		/*
		����H264�����ٶ�
		speed: ��Χ�� 1 �� 6,  ֵԽС���ٶ�Խ�죬����ҲԽ��
		*/
		NT_UINT32(NT_API *SetVideoEncoderSpeed)(NT_HANDLE handle, NT_INT32 speed);

		/*
		�����Ƿ��ͼ�������ͬ�Ƚ�,��ͬͼ��Ƚ�һ���ڲɼ�����ʱ��һ���ô��������ܽ�������
		is_compare_image: 1:��ʾ�Ƚ�, 0:��ʾ���Ƚ�, ����ֵ��Ч
		max_frame_interval: 
		*/
		NT_UINT32(NT_API *SetVideoCompareSameImage)(NT_HANDLE handle, NT_INT32 is_compare_image,
			NT_UINT32 max_frame_interval);

		/*
		������Ƶ���ؼ�֡���, ����ӿ�һ�㲻ʹ�ã��������������SetVideoCompareSameImage�ӿڵ�.
		���翪��ͼ��ȽϺ�SDK��������20sͼ������ͬ�ģ������Ŷ���Ҫ�յ��ؼ�֡���ܽ��벥�ţ�������Ҫһ������.
		interval:��λ�Ǻ��룬 ���SetVideoKeyFrameInterval�ӿ����ֿ������ǵĲ������÷�ʽ�ǲ�ͬ��
		*/
		NT_UINT32(NT_API *SetVideoMaxKeyFrameInterval)(NT_HANDLE handle, NT_INT32 interval);

		/*---��Ƶ������ؽӿ�---*/


		/*+++��Ƶ��ؽӿ�+++*/

		/*
		��ȡϵͳ��Ƶ�����豸��
		*/
		NT_UINT32(NT_API* GetAuidoInputDeviceNumber)(NT_INT32* pNumer);

		/*
		��ȡ��Ƶ�����豸����
		device_id: �豸ID����Ҫ����, ��0��ʼ�����ֵ���ܳ����豸��
		device_name_buff: �豸���ƣ����ص��ַ�����0��β
		device_name_buff_size: �豸����buffer�Ĵ�С,����512
		*/
		NT_UINT32(NT_API* GetAuidoInputDeviceName)(NT_UINT32 device_id, NT_PSTR device_name_buff, NT_UINT32 device_name_buff_size);

		/*
		*����������Ƶ��������
		*type: 1:ʹ��AAC����, 2:ʹ��speex����, ����ֵ���ش���
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetPublisherAudioCodecType)(NT_HANDLE handle, NT_INT32 type);


		/*
		*��������Speex��������
		*quality: ��Χ��0-10, Ĭ����8:��Լ28kbps, ֵԽ������Խ�ã�����ҲԽ��
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetPublisherSpeexEncoderQuality)(NT_HANDLE handle, NT_INT32 quality);


		/*
		�������;���
		is_mute: 0��ʾ������, 1��ʾ����
		*/
		NT_UINT32(NT_API* SetMute)(NT_HANDLE handle, NT_INT32 is_mute);

		/*
		������Ƶ�����豸ID
		device_id: �豸id, һ���0��ʼ 
		*/
		NT_UINT32(NT_API* SetAuidoInputDeviceId)(NT_HANDLE handle, NT_UINT32 device_id);

		/*
		����Ƿ��ܲ�����������Ƶ
		out_flag: 1��ʾ���Բ�����������0:��ʾ�����Բ���������
		*/
		NT_UINT32(NT_API* IsCanCaptureSpeaker)(NT_INT32* out_flag);

		/*
		*���û�������
		*isCancel�� 1��ʾ��������, 0��ʾ����������
		*delay: ����ʱ�ӣ���λ�Ǻ���,Ŀǰ�Ƽ�����100ms, �������Ϊ0�Ļ�����ʹ��100ms
		*ע�����ֻ����˷粶�������ã�������������Ч
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetEchoCancellation)(NT_HANDLE handle, NT_INT32 isCancel, NT_INT32 delay);
		
		/*
		*������Ƶ��������
		*isNS: 1��ʾ��������, 0��ʾ������
		*ע�⣬���һ�����ڲɼ���˷��ϣ��ɼ�ϵͳ��������ʱ����Ҫ�Բ���
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetNoiseSuppression)(NT_HANDLE handle, NT_INT32 isNS);

		/*
		*������Ƶ�Զ��������
		*isAGC: 1��ʾ�������, 0��ʾ������
		*ע�⣬���һ�����ڲɼ���˷��ϣ��ɼ�ϵͳ��������ʱ����Ҫ�Բ���
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetAGC)(NT_HANDLE handle, NT_INT32 isAGC);


		/*
		*���ö˵���(Voice Activity Detection (VAD))
		*isVAD: 1��ʾ�˵���, 0��ʾ�����
		*ע�⣬���һ�����ڲɼ���˷��ϣ��ɼ�ϵͳ������������ʱЧ������
		*�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetVAD)(NT_HANDLE handle, NT_INT32 isVAD);

		/*---��Ƶ��ؽӿ�---*/



		/*+++������ؽӿ�+++*/

		/*
		�������͵�URL
		֧��ͬʱ���͵����RTMP�������ϣ� ������ͬʱ֧���Ƶ�������������
		Ϊ���ö��URL������ö��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetURL)(NT_HANDLE handle, NT_PCSTR url, NT_PVOID pReserve);


		/*
		*������Ƶ��ʱ����ص�
		url:����url
		min_call_interval:��С�ص�ʱ��������λ�Ǻ���), �����0�Ļ�������ÿ����Ƶ��ʱ����ص�
		callbackdata:�ص�ʱ�û��Զ�������
		call_back:�ص�����ָ��
		*/
		NT_UINT32(NT_API *SetVideoPacketTimestampCallBack)(NT_HANDLE handle,
			NT_PCSTR url, NT_UINT32 min_call_interval,
			NT_PVOID call_back_data, NT_PB_SDKVideoPacketTimestampCallBack call_back);


		/*
		��������״̬�ص�
		call_back_data: �ص�ʱ�û��Զ�������
		call_back
		*/
		NT_UINT32(NT_API *SetPublisherStatusCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_PB_SDKPublisherStatusCallBack call_back);

		/*
		��������
		*/
		NT_UINT32(NT_API *StartPublisher)(NT_HANDLE handle, NT_PVOID pReserve);

		/*
		ֹͣ����
		*/
		NT_UINT32(NT_API *StopPublisher)(NT_HANDLE handle);

		/*---������ؽӿ�---*/


		/*+++¼����ؽӿ�+++*/

		/*
		���ñ���¼��Ŀ¼, ������Ӣ��Ŀ¼�������ʧ��
		*/
		NT_UINT32(NT_API *SetRecorderDirectory)(NT_HANDLE handle, NT_PCSTR dir, NT_PVOID pReserve);

		/*
		���õ���¼���ļ�����С, ���������ֵ��ʱ�򣬽��и�ɵڶ����ļ�
		size: ��λ��KB(1024Byte), ��ǰ��Χ�� [5MB-800MB], �����������õ���Χ��
		*/
		NT_UINT32(NT_API *SetRecorderFileMaxSize)(NT_HANDLE handle, NT_UINT32 size);

		/*
		����¼���ļ������ɹ���
		*/
		NT_UINT32(NT_API *SetRecorderFileNameRuler)(NT_HANDLE handle, NT_PB_RecorderFileNameRuler* ruler);

		/*
		����¼��
		*/
		NT_UINT32(NT_API *StartRecorder)(NT_HANDLE handle, NT_PVOID pReserve);

		/*
		ֹͣ¼��
		*/
		NT_UINT32(NT_API *StopRecorder)(NT_HANDLE handle);

		/*---¼����ؽӿ�---*/


		/*
		* Ͷ�ݱ��������Ƶ���ݸ�SDK
		* codec_id:��ο�NT_MEDIA_CODEC_ID, ע�ⲻҪ����auido codec id��������û�ж���
		* data: ��������
		* size: �������ݴ�С
		* is_key_frame: 1����ʾ�ǹؼ�֡�� 0:��ʾ����
		* timestamp: ʱ���
		* �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostVideoEncodedData)(NT_HANDLE handle, NT_UINT32 codec_id, 
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp);


		/*
		* Ͷ�ݱ��������Ƶ���ݸ�SDK V2�� 
		* codec_id:��ο�NT_MEDIA_CODEC_ID, ע�ⲻҪ����auido codec id��������û�ж���
		* data: ��������
		* size: �������ݴ�С
		* is_key_frame: 1����ʾ�ǹؼ�֡�� 0:��ʾ����
		* timestamp: ����ʱ���
		* presentation_timestamp: ��ʾʱ���
		* ע�⣺��ȷ�� presentation_timestamp >= timestamp, ������δ����
		* �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostVideoEncodedDataV2)(NT_HANDLE handle, NT_UINT32 codec_id,
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp, NT_UINT64 presentation_timestamp);


		/*
		* Ͷ�ݱ��������Ƶ���ݸ�SDK
		*codec_id:��ο�NT_MEDIA_CODEC_ID, ע�ⲻҪ����video codec id��������û�ж���
		* data: ��������
		* size: �������ݴ�С
		* is_key_frame: 1����ʾ�ǹؼ�֡�� 0:��ʾ����
		* timestamp: ʱ���
		* parameter_info: ��������aac��ʱ�������AudioSpecificConfig, ���������봫NULL
		* parameter_info_size: parameter_info ����
		* �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostAudioEncodedData)(NT_HANDLE handle, NT_UINT32 codec_id,
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp,
			const NT_BYTE*  parameter_info,
			NT_UINT32 parameter_info_size);


		/*
		���ܽӿ�, ���ò����� ��������⣬ ��Щ�ӿڶ��ܽ��
		*/
		NT_UINT32(NT_API *SetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*
		���ܽӿ�, �õ������� ��������⣬��Щ�ӿڶ��ܽ��
		*/
		NT_UINT32(NT_API *GetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*+++��Ļѡȡ����+++*/
		/*
		 ע������ӿڷ��صľ����һ��Ҫ�����������ʱ�õ�Open�ӿڷ��صľ�����ֿ�,
		 ��ȫ�ǲ�ͬ�ľ�����ô���δ����
		*/

		/*
		 mode: ��ο�NT_PB_E_SCREEN_REGION_CHOOSE_MODE
		 mode_data: mode == NT_PB_E_SCREEN_REGION_CHOOSE_MODE_CHOOSE, mode_data ��NULL.
		            mode == NT_PB_E_SCREEN_REGION_CHOOSE_MODE_MOVE, mode_data ��NT_PB_RectRegion*
		 callback: �������Ҫ�У����ұ��봦��, ������Դй©
		 callback_data: callback data
		 flagĿǰ��0��������չ�ã� pReserve��NULL,��չ��,
		 ��һ����Ļѡȡ���ߵ�toolHandle
		 �ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *OpenScreenRegionChooseTool)(NT_PHANDLE pToolHandle,
			NT_UINT32 mode, 
			NT_PVOID  mode_data,
			NT_PB_SDKScreenRegionChooseCallBack callback,
			NT_PVOID callback_data,
			NT_UINT32 flag, NT_PVOID pReserve);

		/*
		��������ӿ�֮��toolHandleʧЧ��
		�ɹ����� NT_ERC_OK
		*/
		NT_UINT32(NT_API *CloseScreenRegionChooseTool)(NT_HANDLE toolHandle);

		/*---��Ļѡȡ����---*/


		/*+++ NT_PB_Image ��������+++*/
		NT_SmartPublisherImageSDKAPI ImageAPI_;
		/*--- NT_PB_Image ��������---*/


		/*+++���Ʋ���+++*/
		NT_SmartPublisherDrawImageSDKAPI drawImageAPI_;
		/*---���Ʋ���---*/

	} NT_SmartPublisherSDKAPI;


	NT_UINT32 NT_API NT_GetSmartPublisherSDKAPI(NT_SmartPublisherSDKAPI* pAPI);

#ifdef __cplusplus
}
#endif

#endif
