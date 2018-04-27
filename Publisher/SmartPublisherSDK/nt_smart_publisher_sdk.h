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
	 Image处理提供单独的结构体, 降低存储空间，方便拷贝
	*/
	typedef struct _NT_SmartPublisherImageSDKAPI
	{
		/*
		分配Image, 分配后，SDK内部会初始化这个结构体, 失败的话返回NULL
		*/
		NT_PB_Image* (NT_API *AllocateImage)(NT_VOID);

		/*
		释放Image, 注意一定要调用这个接口释放内存，如果在你自己的模块中释放，
		Windows会出问题的,
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *FreeImage)(NT_PB_Image** ppImage);

		/*
		克隆一个Image， 失败返回NULL
		*/
		NT_PB_Image* (NT_API* CloneImage)(const NT_PB_Image* src);

		/*
		拷贝Image, 会先释放dst的资源，然后再拷贝
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* CopyImage)(NT_PB_Image* dst, const NT_PB_Image* src);

		/*
		给图像一个面设置数据，如果这个面已经有数据，将会释放掉再设置
		这个设置行为依赖于NT_PB_Image::format_
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetImagePlane)(NT_PB_Image* image, NT_INT32 plane,
			NT_INT32 planeStride, const NT_UINT8* planeData, NT_INT32 planeDataSize);

		/*
		加载PNG图片
		file_name_utf8: 必须是utf8编码
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* LoadImage)
			(
			NT_PCSTR file_name_utf8,
			NT_PB_Image** ppImage
			);

	} NT_SmartPublisherImageSDKAPI;

	/*
	绘制相关接口, 绘制后续可能会提供更多接口
	*/
	typedef struct _NT_SmartPublisherDrawImageSDKAPI
	{
		/*
		成功返回NT_ERC_OK
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
		flag目前传0，后面扩展用， pReserve传NULL,扩展用,
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *Init)(NT_UINT32 flag, NT_PVOID pReserve);

		/*
		这个是最后一个调用的接口
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *UnInit)();

		/*
		video_option 请参考 NT_PB_E_VIDEO_OPTION,
		auido_option 请参考 NT_PB_E_AUDIO_OPTION
		flag目前传0，后面扩展用， pReserve传NULL,扩展用,
		获取Handle
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *Open)(NT_PHANDLE pHandle, NT_UINT32 video_option, NT_UINT32 auido_option, NT_UINT32 flag, NT_PVOID pReserve);

		/*
		调用这个接口之后handle失效，
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *Close)(NT_HANDLE handle);


		/*
		设置事件回调，如果想监听事件的话，建议调用Open成功后，就调用这个接口
		*/
		NT_UINT32(NT_API *SetEventCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_PB_SDKEventCallBack call_back);


		/*参数相关设置*/
		/*
		设置屏幕裁剪
		left: 屏幕左上角x位置
		top:  屏幕左上角y位置
		width: 宽度, 必须是16的倍数
		height: 高度, 必须是16的倍数
		*/
		NT_UINT32(NT_API *SetScreenClip)(NT_HANDLE handle, NT_UINT32 left, NT_UINT32 top, NT_UINT32 width, NT_UINT32 height);


		/*
		移动屏幕剪切区域，这个接口只能推送或者录像中调用
		left: 屏幕左上角x位置
		top:  屏幕左上角y位置
		*/
		NT_UINT32(NT_API *MoveScreenClipRegion)(NT_HANDLE handle, NT_UINT32 left, NT_UINT32 top);


		/*
		允许使用DXGI屏幕采集方式, 这种方式需要win8及以上系统才支持
		is_enable: 1表示启用DXGI采集，0表示不启用, 其他值直接返回错误
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *EnableDXGIScreenCapturer)(NT_HANDLE handle, NT_INT32 is_enable);


		/*
		*采集屏幕时停用Aero, 这个只对win7有影响，win8及以上系统, 微软已经抛弃了Aero Glass效果
		*is_disable: 1:表示停用,如果设置为1的话，在win7系统上开始捕屏时可能黑屏闪烁一下, 0:表示不停用
		* sdk内部默认值是1
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *DisableAeroScreenCapturer)(NT_HANDLE handle, NT_INT32 is_disable);


		/*
		这个接口主要判断顶层窗口能否能被捕获, 如果不能被捕获的话返回NT_ERC_FAILED
		如果返回NT_ERC_OK,表示可能能捕获
		*/
		NT_UINT32(NT_API *CheckCapturerWindow)(NT_HWND hwnd);

		/*
		设置要捕获的窗口的句柄
		*/
		NT_UINT32(NT_API *SetCaptureWindow)(NT_HANDLE handle, NT_HWND hwnd);


		/*
		设置帧率
		*/
		NT_UINT32(NT_API *SetFrameRate)(NT_HANDLE handle, NT_UINT32 frame_rate);

		/*+++摄像头相关接口+++*/
		
		/*
		获取摄像头数量
		pNumer: 返回设备数量
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceNumber)(NT_INT32* pNumer);

		/*
		 返回摄像头设备信息
		 device_index: 设备索引
		 device_name_utf8:  设备名称，传NULL将不返回名称，如果不是NULL的话, device_name_length必须大于等于256, 返回utf8编码的设备名称
		 device_name_length: 设备名称缓冲大小，如果device_name_utf8是NULL，则传入0, 否则必须大于等于256
		 device_unique_id_utf8: 设备唯一ID, 传NULL将不返回ID,如果不传NULL的话，device_unique_id_length必须大于等于1024,返回utf8编码的设备ID
		 device_unique_id_length: 设备唯一ID缓冲代销, 如果是device_unique_id_utf8NULL, 则传入0，否则必须大于等于1024
		 成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceInfo)(
			NT_INT32	device_index,
			NT_PSTR		device_name_utf8,
			NT_UINT32	device_name_length,
			NT_PSTR		device_unique_id_utf8,
			NT_UINT32	device_unique_id_length
			);

		/*
		返回摄像头能力数
		device_unique_id_utf8: 设备唯一id
		capability_number: 返回的设备能力数
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceCapabilityNumber)(
			NT_PCSTR device_unique_id_utf8,
			NT_INT32* capability_number
			);

		/*
		返回摄像头能力
		device_unique_id_utf8: 设备唯一id,
		device_capability_index: 设备能力索引
		capability: 设备能力
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* GetVideoCaptureDeviceCapability)(
			NT_PCSTR device_unique_id_utf8,
			NT_INT32 device_capability_index,
			NT_PB_VideoCaptureCapability* capability);


		/*
		* 在多个实例推送多路时，对于一个摄像头来说,所有实例只能共享摄像头，那么只有一个实例可以改变摄像头分辨率,
		  其他实例使用这个缩放后的图像.
		  在使用多实例时，调用这个接口禁止掉实例的分辨率设置能力.只留一个实例能改变分辨，如果不设置，行为
		  未定义.
		  这个接口必须在 SetLayersConfig, AddLayerConfig 之前调用
		  device_unique_id_utf8: 设备唯一id
		  成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* DisableVideoCaptureResolutionSetting)(
			NT_HANDLE handle,
			NT_PCSTR device_unique_id_utf8
			);


		/*
		启动摄像头预览
		device_unique_id_utf8: 设备唯一id
		hwnd: 绘制的窗口句柄
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* StartVideoCaptureDevicePreview)(
			NT_PCSTR device_unique_id_utf8,
			NT_HWND hwnd
			);

		/*
		*上下反转摄像头预览图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipVerticalCameraPreview)(NT_HWND hwnd, NT_INT32 is_flip);


		/*
		*水平反转摄像头预览图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipHorizontalCameraPreview)(NT_HWND hwnd, NT_INT32 is_flip);


		/*
		*旋转摄像头预览图像, 顺时针旋转
		degress： 设置0， 90， 180， 270度有效，其他值无效
		注意：除了0度，其他角度播放会耗费更多CPU
		*/
		NT_UINT32(NT_API *RotateCameraPreview)(NT_HWND hwnd, NT_INT32 degress);


		/*
		告诉SDK预览窗口大小改变
		hwnd: 绘制的窗口句柄
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* VideoCaptureDevicePreviewWindowSizeChanged)
			(
			NT_HWND hwnd
			);

		/*
		停止摄像头预览
		hwnd: 绘制的窗口句柄
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* StopVideoCaptureDevicePreview)(
			NT_HWND hwnd
			);


		/***************************/
		/*****获取摄像头RBG32图像接口++/
		/***************************/
		/*
		调用流程:
		1. StartGetVideoCaptureDeviceImage 获取句柄,切保存句柄
		2. GetVideoCaptureDeviceImage 获取图像
		3. StopGetVideoCaptureDeviceImage 停止， 之后句柄将无效
		*/

		/*
		调用此接口前置条件: 必须调用过Init接口, 否则会奔溃或失败

		pVideoCaptrueImageHandle: 要返回的句柄的指针，请不要和其他句柄搞混
		device_unique_id_utf8: 设备唯一id
		成功返回 NT_ERC_OK 并返回VideoCaptrueImageHandle句柄
		*/
		
		NT_UINT32(NT_API* StartGetVideoCaptureDeviceImage)
			(
			NT_PHANDLE pVideoCaptrueImageHandle,
			NT_PCSTR   device_unique_id_utf8
			);


		/*
		*上下反转设备图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipVerticalVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 is_flip);


		/*
		*水平反转设备图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipHorizontalVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 is_flip);


		/*
		*旋转设备图像, 顺时针旋转
		degress： 设置0， 90， 180， 270度有效，其他值无效
		注意：除了0度，其他角度播放会耗费更多CPU
		*/
		NT_UINT32(NT_API *RotateVideoCaptureDeviceImage)(NT_HANDLE videoCaptrueImageHandle, NT_INT32 degress);

		/*
		调用这个接口可以获取摄像头图像

		videoCaptrueImageHandle： 句柄, 通过StartGetVideoCaptureDeviceImage得到的
		
		isDiscardImage: 1：表示取到图像后，就把SDK内部保存的图像删除掉, 0:表示取到图像后, SDK内部的图像不删除，
		那么下一次再调用这个接口的时候，如果摄像头没有产生新的图像，就会返回上次返回过的图像
		
		ppImage: 指向图像指针的指针，如果有图像的话, 会填充 *ppImage

		返回值：
		NT_ERC_OK： 表示取到图像, *ppImage必然有值
		NT_ERC_PB_NO_IMAGE: 表示SDK内部目前没有图像，需要等待摄像头产生图像
		其他值: 可能是其他错误，比如参数错误等等

		注意:
		1. 这个接口返回的图像 宽和高可能会变， 就是说每一次调用返回的图像宽高可能不一样
		2. 取到的图像是从上到下的
		*/

		NT_UINT32(NT_API* GetVideoCaptureDeviceImage)
			(
			NT_HANDLE     videoCaptrueImageHandle,
			NT_INT32      isDiscardImage,
			NT_PB_Image** ppImage
			);


		/*
		停止获取摄像头图像
		成功返回 NT_ERC_OK

		注意:必须在调用UnInit之前调用
		*/
		NT_UINT32(NT_API* StopGetVideoCaptureDeviceImage)
			(
			NT_HANDLE  videoCaptrueImageHandle
			);


		/***************************/
		/*****获取摄像头RBA32图像接口--/
		/***************************/

		/*
		设置摄像头信息
		*/
		NT_UINT32(NT_API *SetVideoCaptureDeviceBaseParameter)(NT_HANDLE handle, NT_PCSTR device_unique_id_utf8, NT_UINT32 width, NT_UINT32 height);

		/*
		*上下反转摄像头图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipVerticalCamera)(NT_HANDLE handle, NT_INT32 is_flip);

		/*
		*水平反转摄像头图像
		*is_flip: 1:表示反转, 0:表示不反转
		*/
		NT_UINT32(NT_API *FlipHorizontalCamera)(NT_HANDLE handle, NT_INT32 is_flip);

		/*
		*旋转摄像头图像, 顺时针旋转
		degress： 设置0， 90， 180， 270度有效，其他值无效
		注意：除了0度，其他角度播放会耗费更多CPU
		*/
		NT_UINT32(NT_API *RotateCamera)(NT_HANDLE handle, NT_INT32 degress);


		/*---摄像头相关接口---*/


		/*+++视频合成相关设置+++*/
		

		/*
		设置视频合成层， 传入的是一个数组， 请正确填充每一层
		reserve: 这个参数保留， 当前请传0
		confs: 层配置数组
		count: 数组大小
		flag: 目前传0
		pReserve: 保留将来用
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetLayersConfig)(NT_HANDLE handle, NT_INT32 reserve, 
			const NT_PB_LayerBaseConfig** confs, NT_INT32 count,
			NT_UINT32 flag, NT_PVOID pReserve);


		/*
		清除所有层配置，注意这个接口只能在推送或者录像之前调用，否则结果未定义
		这个接口主要是给C#使用,C++请直接使用SetLayersConfig
		reserve: 这个参数保留， 当前请传0
		flag: 目前传0
		pReserve: 保留将来用
		成功返回 NT_ERC_OK
		*/
		NT_UINT32 (NT_API *ClearLayersConfig)(NT_HANDLE handle, NT_INT32 reserve,
			NT_UINT32 flag, NT_PVOID pReserve);


		/*
		增加层配置，注意这个接口只能在推送或者录像之前调用，否则结果未定义
		这个接口主要是给C#使用,C++请直接使用SetLayersConfig
		reserve: 这个参数保留， 当前请传0
		conf: 配置层
		layer_type: 层类型
		flag: 目前传0
		pReserve: 保留将来用
		成功返回 NT_ERC_OK
		*/
		NT_UINT32 (NT_API *AddLayerConfig)(NT_HANDLE handle, NT_INT32 reserve,
			NT_PVOID conf, NT_INT32 layer_type,
			NT_UINT32 flag, NT_PVOID pReserve);

		/*
		动态禁止或者启用层
		index: 层索引， 不能是第0层，如果传0的话，会失败
		reserve: 保留字段，请传0
		is_enable: 1表示能用，0表示禁止, 其他值直接返回错误
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *EnableLayer)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, NT_INT32 is_enable);

		/*
		更新层相关配置, 注意不是层的所有字段都可以更新，只是部分可以更新，并且有些层没有字段可以更新,
		传入的参数，SDK只选择能更新的字段更新，不能更新的字段会被忽略
		reserve: 保留字段，请传0
		conf: 配置
		flag: 请传0
		pReserve: 保留字段，请传0
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API * UpdateLayerConfig)(NT_HANDLE handle, NT_INT32 reserve,
			const NT_PB_LayerBaseConfig* conf, NT_UINT32 flag, NT_PVOID pReserve);


		/*
		这个接口是给C#使用的, C++请使用上面的UpdateLayerConfig接口
		更新层相关配置, 注意不是层的所有字段都可以更新，只是部分可以更新，并且有些层没有字段可以更新,
		传入的参数，SDK只选择能更新的字段更新，不能更新的字段会被忽略
		reserve: 保留字段，请传0
		conf: 配置
		flag: 请传0
		pReserve: 保留字段，请传0
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API * UpdateLayerConfigV2)(NT_HANDLE handle, NT_INT32 reserve,
			NT_PVOID conf, NT_INT32 layer_type, NT_UINT32 flag, NT_PVOID pReserve);

		/*
		修改层区域
		pReserve: 保留字段，请传0
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *UpdateLayerRegion)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, const NT_PB_RectRegion* region);

		/*
		给index层投递Image数据，目前主要是用来把rgb和yuv视频数据传给相关层
		reserve: 保留字段，请传0
		index: 层索引
		image: 图像
		flag: 请传0
		pReserve: 保留字段，请传0
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API * PostLayerImage)(NT_HANDLE handle, NT_INT32 reserve,
			NT_INT32 index, const NT_PB_Image* image, NT_UINT32 flag, NT_PVOID pReserve);

		/*---视频合成相关设置---*/

		
		/*+++视频截屏相关接口+++*/

		/*
		捕获图片
		file_name_utf8: 文件名称，utf8编码
		call_back_data: 回调时用户自定义数据
		call_back: 回调函数,用来通知用户截图已经完成或者失败
		成功返回 NT_ERC_OK
		只有在推送或者录像时调用才可能成功，其他情况下调用，返回错误.
		因为生成PNG文件比较耗时，一般需要几百毫秒,为防止CPU过高，SDK会限制截图请求数量,当超过一定数量时，
		调用这个接口会返回NT_ERC_PB_TOO_MANY_CAPTURE_IMAGE_REQUESTS. 这种情况下, 请延时一段时间，等SDK处理掉一些请求后，再尝试.
		*/
		NT_UINT32(NT_API* CaptureImage)(NT_HANDLE handle, NT_PCSTR file_name_utf8,
			NT_PVOID call_back_data, NT_PB_SDKCaptureImageCallBack call_back);


		/*---视频截屏相关接口---*/



		/*+++视频编码相关接口+++*/

		/*
		注意，码率控制两种方式，要么是 SetVideoQuality + SetVideoMaxBitRate
		要么是 SetVideoMaxBitRate  + SetVideoBitRate
		*/
		/*
		设置视频质量, 范围[0-20], 默认是10, 值越小质量越好，但码率会越大
		*/
		NT_UINT32(NT_API *SetVideoQuality)(NT_HANDLE handle, NT_INT32 quality);

		/*
		设置最大视频码率, 单位kbps
		*/
		NT_UINT32(NT_API *SetVideoMaxBitRate)(NT_HANDLE handle, NT_INT32 kbit_rate);

		/*
		设置视频码率, 单位kbps, 默认是0，不使用平均码率方式
		*/
		NT_UINT32(NT_API *SetVideoBitRate)(NT_HANDLE handle, NT_INT32 kbit_rate);


		/*
		* 在一些特殊场景下, 视频分辨率会改变, 如果设置一个固定码率的的话,当视频分辨率变大的时候会变的模糊,变小的话又会浪费码率
		* 所以提供可以设置一组码率的接口,满足不同分辨率切换的需求
		* 规则: 比如设置两组分辨率 640*360, 640*480, 那么当分辨率小于等于640*360时都使用640*360的码率,
		* 当分辨率大于640*360且小于等于640*480时，就使用640*480的码率,如果分辨率大于640*480 那就使用640*480的分辨率
		* 为了设置的更准确, 建议多划分几组, 让区间变小
		* 调用这个接口每次设置一组,设置多组就调用多次
		*
		*/
		NT_UINT32(NT_API* AddVideoEncoderBitrateGroupItem)(NT_HANDLE handle, const NT_PB_VideoEncoderBitrateGroupItem* item);

		
		/*
		清除视频码率组
		*/
		NT_UINT32(NT_API* ClearVideoEncoderBitrateGroup)(NT_HANDLE handle);


		/*
		设置关键帧间隔, 比如1表示所有帧都是关键帧，10表示每10帧里面一个关键帧，25表示每25帧一个关键帧
		*/
		NT_UINT32(NT_API *SetVideoKeyFrameInterval)(NT_HANDLE handle, NT_INT32 interval);


		/*
		设置H264 profile.
		profile: 1: H264 baseline(默认值). 2: H264 main. 3. H264 high
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetVideoEncoderProfile)(NT_HANDLE handle, NT_INT32 profile);

		/*
		设置H264编码速度
		speed: 范围是 1 到 6,  值越小，速度越快，质量也越差
		*/
		NT_UINT32(NT_API *SetVideoEncoderSpeed)(NT_HANDLE handle, NT_INT32 speed);

		/*
		设置是否对图像进行相同比较,相同图像比较一般在采集桌面时有一定好处，可能能降低码率
		is_compare_image: 1:表示比较, 0:表示不比较, 其他值无效
		max_frame_interval: 
		*/
		NT_UINT32(NT_API *SetVideoCompareSameImage)(NT_HANDLE handle, NT_INT32 is_compare_image,
			NT_UINT32 max_frame_interval);

		/*
		设置视频最大关键帧间隔, 这个接口一般不使用，这里是用来配合SetVideoCompareSameImage接口的.
		比如开启图像比较后，SDK发现连续20s图像都是相同的，但播放端需要收到关键帧才能解码播放，所以需要一个限制.
		interval:单位是毫秒， 请和SetVideoKeyFrameInterval接口区分开，他们的参数设置方式是不同的
		*/
		NT_UINT32(NT_API *SetVideoMaxKeyFrameInterval)(NT_HANDLE handle, NT_INT32 interval);

		/*---视频编码相关接口---*/


		/*+++音频相关接口+++*/

		/*
		获取系统音频输入设备数
		*/
		NT_UINT32(NT_API* GetAuidoInputDeviceNumber)(NT_INT32* pNumer);

		/*
		获取音频输入设备名称
		device_id: 设备ID，需要输入, 从0开始，最大值不能超过设备数
		device_name_buff: 设备名称，返回的字符串以0结尾
		device_name_buff_size: 设备名称buffer的大小,建议512
		*/
		NT_UINT32(NT_API* GetAuidoInputDeviceName)(NT_UINT32 device_id, NT_PSTR device_name_buff, NT_UINT32 device_name_buff_size);

		/*
		*设置推送音频编码类型
		*type: 1:使用AAC编码, 2:使用speex编码, 其他值返回错误
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetPublisherAudioCodecType)(NT_HANDLE handle, NT_INT32 type);


		/*
		*设置推送Speex编码质量
		*quality: 范围是0-10, 默认是8:大约28kbps, 值越大，质量越好，码率也越大
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetPublisherSpeexEncoderQuality)(NT_HANDLE handle, NT_INT32 quality);


		/*
		设置推送静音
		is_mute: 0表示不静音, 1表示静音
		*/
		NT_UINT32(NT_API* SetMute)(NT_HANDLE handle, NT_INT32 is_mute);

		/*
		设置音频输入设备ID
		device_id: 设备id, 一般从0开始 
		*/
		NT_UINT32(NT_API* SetAuidoInputDeviceId)(NT_HANDLE handle, NT_UINT32 device_id);

		/*
		检查是否能捕获扬声器音频
		out_flag: 1表示可以捕获扬声器，0:表示不可以捕获扬声器
		*/
		NT_UINT32(NT_API* IsCanCaptureSpeaker)(NT_INT32* out_flag);

		/*
		*设置回音消除
		*isCancel： 1表示消除回音, 0表示不消除回音
		*delay: 回音时延，单位是毫秒,目前推荐设置100ms, 如果设置为0的话，将使用100ms
		*注意这个只对麦克风捕获有作用，扬声器捕获无效
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetEchoCancellation)(NT_HANDLE handle, NT_INT32 isCancel, NT_INT32 delay);
		
		/*
		*设置音频噪音抑制
		*isNS: 1表示噪音抑制, 0表示不抑制
		*注意，这个一般用在采集麦克风上，采集系统播放声音时必须要性不大
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetNoiseSuppression)(NT_HANDLE handle, NT_INT32 isNS);

		/*
		*设置音频自动增益控制
		*isAGC: 1表示增益控制, 0表示不控制
		*注意，这个一般用在采集麦克风上，采集系统播放声音时必须要性不大
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetAGC)(NT_HANDLE handle, NT_INT32 isAGC);


		/*
		*设置端点检测(Voice Activity Detection (VAD))
		*isVAD: 1表示端点检测, 0表示不检测
		*注意，这个一般用在采集麦克风上，采集系统播放音乐声音时效果不好
		*成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetVAD)(NT_HANDLE handle, NT_INT32 isVAD);

		/*---音频相关接口---*/



		/*+++推送相关接口+++*/

		/*
		设置推送的URL
		支持同时推送到多个RTMP服务器上， 最多可以同时支持推到三个服务器上
		为设置多个URL，请调用多次
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetURL)(NT_HANDLE handle, NT_PCSTR url, NT_PVOID pReserve);


		/*
		*设置视频包时间戳回调
		url:推送url
		min_call_interval:最小回调时间间隔（单位是毫秒), 如果是0的话，发送每个视频包时都会回调
		callbackdata:回调时用户自定义数据
		call_back:回调函数指针
		*/
		NT_UINT32(NT_API *SetVideoPacketTimestampCallBack)(NT_HANDLE handle,
			NT_PCSTR url, NT_UINT32 min_call_interval,
			NT_PVOID call_back_data, NT_PB_SDKVideoPacketTimestampCallBack call_back);


		/*
		设置推送状态回调
		call_back_data: 回调时用户自定义数据
		call_back
		*/
		NT_UINT32(NT_API *SetPublisherStatusCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_PB_SDKPublisherStatusCallBack call_back);

		/*
		启动推送
		*/
		NT_UINT32(NT_API *StartPublisher)(NT_HANDLE handle, NT_PVOID pReserve);

		/*
		停止推送
		*/
		NT_UINT32(NT_API *StopPublisher)(NT_HANDLE handle);

		/*---推送相关接口---*/


		/*+++录像相关接口+++*/

		/*
		设置本地录像目录, 必须是英文目录，否则会失败
		*/
		NT_UINT32(NT_API *SetRecorderDirectory)(NT_HANDLE handle, NT_PCSTR dir, NT_PVOID pReserve);

		/*
		设置单个录像文件最大大小, 当超过这个值的时候，将切割成第二个文件
		size: 单位是KB(1024Byte), 当前范围是 [5MB-800MB], 超出将被设置到范围内
		*/
		NT_UINT32(NT_API *SetRecorderFileMaxSize)(NT_HANDLE handle, NT_UINT32 size);

		/*
		设置录像文件名生成规则
		*/
		NT_UINT32(NT_API *SetRecorderFileNameRuler)(NT_HANDLE handle, NT_PB_RecorderFileNameRuler* ruler);

		/*
		启动录像
		*/
		NT_UINT32(NT_API *StartRecorder)(NT_HANDLE handle, NT_PVOID pReserve);

		/*
		停止录像
		*/
		NT_UINT32(NT_API *StopRecorder)(NT_HANDLE handle);

		/*---录像相关接口---*/


		/*
		* 投递编码过的视频数据给SDK
		* codec_id:请参考NT_MEDIA_CODEC_ID, 注意不要传递auido codec id，否则结果没有定义
		* data: 编码数据
		* size: 编码数据大小
		* is_key_frame: 1：表示是关键帧， 0:表示不是
		* timestamp: 时间戳
		* 成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostVideoEncodedData)(NT_HANDLE handle, NT_UINT32 codec_id, 
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp);


		/*
		* 投递编码过的视频数据给SDK V2版 
		* codec_id:请参考NT_MEDIA_CODEC_ID, 注意不要传递auido codec id，否则结果没有定义
		* data: 编码数据
		* size: 编码数据大小
		* is_key_frame: 1：表示是关键帧， 0:表示不是
		* timestamp: 解码时间戳
		* presentation_timestamp: 显示时间戳
		* 注意：请确保 presentation_timestamp >= timestamp, 否则结果未定义
		* 成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostVideoEncodedDataV2)(NT_HANDLE handle, NT_UINT32 codec_id,
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp, NT_UINT64 presentation_timestamp);


		/*
		* 投递编码过的音频数据给SDK
		*codec_id:请参考NT_MEDIA_CODEC_ID, 注意不要传递video codec id，否则结果没有定义
		* data: 编码数据
		* size: 编码数据大小
		* is_key_frame: 1：表示是关键帧， 0:表示不是
		* timestamp: 时间戳
		* parameter_info: 当编码是aac的时候，这个是AudioSpecificConfig, 其他编码请传NULL
		* parameter_info_size: parameter_info 长度
		* 成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *PostAudioEncodedData)(NT_HANDLE handle, NT_UINT32 codec_id,
			const NT_BYTE* data, NT_UINT32 size,
			NT_INT32 is_key_frame, NT_UINT64 timestamp,
			const NT_BYTE*  parameter_info,
			NT_UINT32 parameter_info_size);


		/*
		万能接口, 设置参数， 大多数问题， 这些接口都能解决
		*/
		NT_UINT32(NT_API *SetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*
		万能接口, 得到参数， 大多数问题，这些接口都能解决
		*/
		NT_UINT32(NT_API *GetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*+++屏幕选取工具+++*/
		/*
		 注意这个接口返回的句柄，一定要和上面的推送时用的Open接口返回的句柄区分开,
		 完全是不同的句柄，用错结果未定义
		*/

		/*
		 mode: 请参考NT_PB_E_SCREEN_REGION_CHOOSE_MODE
		 mode_data: mode == NT_PB_E_SCREEN_REGION_CHOOSE_MODE_CHOOSE, mode_data 是NULL.
		            mode == NT_PB_E_SCREEN_REGION_CHOOSE_MODE_MOVE, mode_data 是NT_PB_RectRegion*
		 callback: 这个必须要有，并且必须处理, 否则资源泄漏
		 callback_data: callback data
		 flag目前传0，后面扩展用， pReserve传NULL,扩展用,
		 打开一个屏幕选取工具的toolHandle
		 成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *OpenScreenRegionChooseTool)(NT_PHANDLE pToolHandle,
			NT_UINT32 mode, 
			NT_PVOID  mode_data,
			NT_PB_SDKScreenRegionChooseCallBack callback,
			NT_PVOID callback_data,
			NT_UINT32 flag, NT_PVOID pReserve);

		/*
		调用这个接口之后toolHandle失效，
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *CloseScreenRegionChooseTool)(NT_HANDLE toolHandle);

		/*---屏幕选取工具---*/


		/*+++ NT_PB_Image 操作函数+++*/
		NT_SmartPublisherImageSDKAPI ImageAPI_;
		/*--- NT_PB_Image 操作函数---*/


		/*+++绘制操作+++*/
		NT_SmartPublisherDrawImageSDKAPI drawImageAPI_;
		/*---绘制操作---*/

	} NT_SmartPublisherSDKAPI;


	NT_UINT32 NT_API NT_GetSmartPublisherSDKAPI(NT_SmartPublisherSDKAPI* pAPI);

#ifdef __cplusplus
}
#endif

#endif
