/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#ifndef SMART_PLAYER_SDK_H_
#define SMART_PLAYER_SDK_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SMARTPLAYERSDK_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SMARTPLAYERSDK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SMARTPLAYERSDK_EXPORTS
#define SMARTPLAYERSDK_API __declspec(dllexport)
#else
#define SMARTPLAYERSDK_API __declspec(dllimport)
#endif

#ifdef SMART_HAS_COMMON_DIC
#include "../../topcommon/nt_type_define.h"
#else
#include "nt_type_define.h"
#endif

#include "smart_player_define.h"

#ifdef __cplusplus
extern "C"{
#endif

	typedef struct _SmartPlayerSDKAPI
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
		flag目前传0，后面扩展用， pReserve传NULL,扩展用,
		NT_HWND hwnd, 绘制画面用的窗口, 可以设置为NULL
		获取Handle
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *Open)(NT_PHANDLE pHandle, NT_HWND hwnd, NT_UINT32 flag, NT_PVOID pReserve);

		/*
		调用这个接口之后handle失效，
		成功返回 NT_ERC_OK
		*/
		NT_UINT32(NT_API *Close)(NT_HANDLE handle);


		/*
		设置事件回调，如果想监听事件的话，建议调用Open成功后，就调用这个接口
		*/
		NT_UINT32(NT_API *SetEventCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_SP_SDKEventCallBack call_back);

		/*
		设置视频大小回调接口
		*/
		NT_UINT32(NT_API *SetVideoSizeCallBack)(NT_HANDLE handle, 
			NT_PVOID call_back_data, SP_SDKVideoSizeCallBack call_back);


		/*
		设置视频回调, 吐视频数据出来
		frame_format: 只能是NT_SP_E_VIDEO_FRAME_FORMAT_RGB32, NT_SP_E_VIDEO_FRAME_FROMAT_I420
		*/
		NT_UINT32(NT_API *SetVideoFrameCallBack)(NT_HANDLE handle,
			NT_INT32 frame_format,
			NT_PVOID call_back_data, SP_SDKVideoFrameCallBack call_back);

		/*
		*设置绘制视频帧时，视频帧时间戳回调
		*注意如果当前播放流是纯音频，那么将不会回调，这个仅在有视频的情况下才有效
		*/
		NT_UINT32(NT_API *SetRenderVideoFrameTimestampCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, SP_SDKRenderVideoFrameTimestampCallBack call_back);


		/*
		设置音频PCM帧回调, 吐PCM数据出来，目前每帧大小是10ms.
		*/
		NT_UINT32(NT_API *SetAudioPCMFrameCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, NT_SP_SDKAudioPCMFrameCallBack call_back);
			
		/*
		开始播放,传URL进去
		注意：这个接口目前不再推荐使用，请使用StartPlay. 为方便老客户升级，暂时保留. 
		*/
		NT_UINT32(NT_API *Start)(NT_HANDLE handle, NT_PCSTR url, 
			NT_PVOID call_back_data, SP_SDKStartPlayCallBack call_back);
		
		/*
		停止播放
		注意: 这个接口目前不再推荐使用，请使用StopPlay. 为方便老客户升级，暂时保留. 
		*/
		NT_UINT32(NT_API *Stop)(NT_HANDLE handle);

		/*
		*提供一组新接口++
		*/
		
		/*
		*设置URL
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetURL)(NT_HANDLE handle, NT_PCSTR url);


		/*
		handle: 播放句柄
		hwnd: 这个要传入真正用来绘制的窗口句柄
		is_support: 如果支持的话 *is_support 为1， 不支持的话为0
		接口调用成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *IsSupportD3DRender)(NT_HANDLE handle, NT_HWND hwnd, NT_INT32* is_support);


		/*
		设置绘制窗口句柄，如果在调用Open时设置过，那这个接口可以不调用
		如果在调用Open时设置为NULL，那么这里可以设置一个绘制窗口句柄给播放器
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetRenderWindow)(NT_HANDLE handle, NT_HWND hwnd);

		/*
		* 设置是否播放出声音，这个和静音接口是有区别的
		* 这个接口的主要目的是为了用户设置了外部PCM回调接口后，又不想让SDK播放出声音时使用
		* is_output_auido_device: 1: 表示允许输出到音频设备，默认是1， 0：表示不允许输出. 其他值接口返回失败
		* 成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetIsOutputAudioDevice)(NT_HANDLE handle, NT_INT32 is_output_auido_device);


		/*
		*开始播放, 注意StartPlay和Start不能混用，要么使用StartPlay, 要么使用Start.
		* Start和Stop是老接口，不推荐用。请使用StartPlay和StopPlay新接口
		*/
		NT_UINT32(NT_API *StartPlay)(NT_HANDLE handle);

		/*
		*停止播放
		*/
		NT_UINT32(NT_API *StopPlay)(NT_HANDLE handle);

		/*
		设置本地录像目录, 必须是英文目录，否则会失败
		*/
		NT_UINT32(NT_API *SetRecorderDirectory)(NT_HANDLE handle, NT_PCSTR dir);

		/*
		设置单个录像文件最大大小, 当超过这个值的时候，将切割成第二个文件
		size: 单位是KB(1024Byte), 当前范围是 [5MB-800MB], 超出将被设置到范围内
		*/
		NT_UINT32(NT_API *SetRecorderFileMaxSize)(NT_HANDLE handle, NT_UINT32 size);

		/*
		设置录像文件名生成规则
		*/
		NT_UINT32(NT_API *SetRecorderFileNameRuler)(NT_HANDLE handle, NT_SP_RecorderFileNameRuler* ruler);


		/*
		设置录像回调接口
		*/
		NT_UINT32(NT_API *SetRecorderCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, SP_SDKRecorderCallBack call_back);

		/*
		启动录像
		*/
		NT_UINT32(NT_API *StartRecorder)(NT_HANDLE handle);

		/*
		停止录像
		*/
		NT_UINT32(NT_API *StopRecorder)(NT_HANDLE handle);

		/*
		* 设置拉流时，吐视频数据的回调
		*/
		NT_UINT32(NT_API *SetPullStreamVideoDataCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, SP_SDKPullStreamVideoDataCallBack call_back);

		/*
		* 设置拉流时，吐音频数据的回调
		*/
		NT_UINT32(NT_API *SetPullStreamAudioDataCallBack)(NT_HANDLE handle,
			NT_PVOID call_back_data, SP_SDKPullStreamAudioDataCallBack call_back);

		/*
		启动拉流
		*/
		NT_UINT32(NT_API *StartPullStream)(NT_HANDLE handle);

		/*
		停止拉流
		*/
		NT_UINT32(NT_API *StopPullStream)(NT_HANDLE handle);


		/*
		*提供一组新接口--
		*/
		 
		/*
		绘制窗口大小改变时，必须调用
		*/
		NT_UINT32(NT_API* OnWindowSize)(NT_HANDLE handle, NT_INT32 cx, NT_INT32 cy);

		/*
		万能接口, 设置参数， 大多数问题， 这些接口都能解决
		*/
		NT_UINT32(NT_API *SetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*
		万能接口, 得到参数， 大多数问题，这些接口都能解决
		*/
		NT_UINT32(NT_API *GetParam)(NT_HANDLE handle, NT_UINT32 id, NT_PVOID pData);

		/*
		设置buffer,最小0ms
		*/
		NT_UINT32(NT_API *SetBuffer)(NT_HANDLE handle, NT_INT32 buffer);

		/*
		静音接口，1为静音，0为不静音
		*/
		NT_UINT32(NT_API *SetMute)(NT_HANDLE handle, NT_INT32 is_mute);

		/*
		设置RTSP TCP 模式, 1为TCP, 0为UDP, 仅RTSP有效
		*/
		NT_UINT32(NT_API* SetRTSPTcpMode)(NT_HANDLE handle, NT_INT32 isUsingTCP);

		/*
		设置秒开, 1为秒开, 0为不秒开
		*/
		NT_UINT32(NT_API* SetFastStartup)(NT_HANDLE handle, NT_INT32 isFastStartup);

		/*
		设置低延时播放模式，默认是正常播放模式
		mode: 1为低延时模式， 0为正常模式，其他只无效
		接口调用成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetLowLatencyMode)(NT_HANDLE handle, NT_INT32 mode);

		/*
		设置旋转，顺时针旋转
		degress： 设置0， 90， 180， 270度有效，其他值无效
		注意：除了0度，其他角度播放会耗费更多CPU
		接口调用成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API* SetRotation)(NT_HANDLE handle, NT_INT32 degress);

		/*
		设置下载速度上报, 默认不上报下载速度
		is_report: 上报开关, 1: 表上报. 0: 表示不上报. 其他值无效.
		report_interval： 上报时间间隔（上报频率），单位是秒，最小值是1秒1次. 如果小于1且设置了上报，将调用失败
		注意：如果设置上报的话，请设置SetEventCallBack, 然后在回调函数里面处理这个事件.
		上报事件是：NT_SP_E_EVENT_ID_DOWNLOAD_SPEED
		这个接口必须在StartXXX之前调用
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetReportDownloadSpeed)(NT_HANDLE handle,
			NT_INT32 is_report, NT_INT32 report_interval);


		/*
		主动获取下载速度
		speed： 返回下载速度，单位是Byte/s
	   （注意：这个接口必须在startXXX之后调用，否则会失败）
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *GetDownloadSpeed)(NT_HANDLE handle, NT_INT32* speed);


		/*
		获取视频时长
		对于直播的话，没有时长，调用结果未定义
		点播的话，如果获取成功返回NT_ERC_OK， 如果SDK还在解析中，则返回NT_ERC_SP_NEED_RETRY
		*/
		NT_UINT32(NT_API *GetDuration)(NT_HANDLE handle, NT_INT64* duration);


		/*
		获取当前播放时间戳, 单位是毫秒(ms)
		注意:这个时间戳是视频源的时间戳，只支持点播. 直播不支持.
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *GetPlaybackPos)(NT_HANDLE handle, NT_INT64* pos);


		/*
		获取当前拉流时间戳, 单位是毫秒(ms)
		注意:这个时间戳是视频源的时间戳，只支持点播. 直播不支持.
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *GetPullStreamPos)(NT_HANDLE handle, NT_INT64* pos);

		/*
		设置播放位置,单位是毫秒(ms)
		注意:直播不支持,这个接口用于点播
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SetPos)(NT_HANDLE handle, NT_INT64 pos);


		/*
		暂停播放
		isPause: 1表示暂停, 0表示恢复播放, 其他错误
		注意:直播不存在暂停的概念，所以直播不支持,这个接口用于点播
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *Pause)(NT_HANDLE handle, NT_INT32 isPause);


		/*
		切换URL
		url:要切换的url
		switch_pos: 切换到新url以后，设置的播放位置, 默认请填0, 这个只对设置播放位置的点播url有效, 直播url无效
		reserve: 保留参数
		注意: 1. 如果切换的url和正在播放的url相同,sdk则不做任何处理
		调用前置条件: 已经成功调用了 StartPlay, StartRecorder, StartPullStream 三个中的任意一个接口
		成功返回NT_ERC_OK
		*/
		NT_UINT32(NT_API *SwitchURL)(NT_HANDLE handle, NT_PCSTR url, NT_INT64 switch_pos, NT_INT32 reserve);


		/*
		捕获图片
		file_name_utf8: 文件名称，utf8编码
		call_back_data: 回调时用户自定义数据
		call_back: 回调函数,用来通知用户截图已经完成或者失败
		成功返回 NT_ERC_OK
		只有在播放时调用才可能成功，其他情况下调用，返回错误.
		因为生成PNG文件比较耗时，一般需要几百毫秒,为防止CPU过高，SDK会限制截图请求数量,当超过一定数量时，
		调用这个接口会返回NT_ERC_SP_TOO_MANY_CAPTURE_IMAGE_REQUESTS. 这种情况下, 请延时一段时间，等SDK处理掉一些请求后，再尝试.
		*/
		NT_UINT32(NT_API* CaptureImage)(NT_HANDLE handle, NT_PCSTR file_name_utf8,
			NT_PVOID call_back_data, SP_SDKCaptureImageCallBack call_back);


		/*
		* 使用GDI绘制RGB32数据
		* 32位的rgb格式, r, g, b各占8, 另外一个字节保留, 内存字节格式为: bb gg rr xx, 主要是和windows位图匹配, 在小端模式下，按DWORD类型操作，最高位是xx, 依次是rr, gg, bb
		* 为了保持和windows位图兼容，步长(image_stride)必须是width_*4
		* handle: 播放器句柄
		* hdc: 绘制dc
		* x_dst: 绘制面左上角x坐标
		* y_dst: 绘制面左上角y坐标
		* dst_width: 要绘制的宽度
		* dst_height： 要绘制的高度
		* x_src: 源图像x位置
		* y_src: 原图像y位置
		* rgb32_data: rgb32数据，格式参见前面的注释说明
		* rgb32_data_size: 数据大小
		* image_width： 图像实际宽度
		* image_height： 图像实际高度
		* image_stride： 图像步长
		*/
		NT_UINT32(NT_API *GDIDrawRGB32)(NT_HANDLE handle, NT_HDC hdc,
			NT_INT32 x_dst, NT_INT32 y_dst,
			NT_INT32 dst_width, NT_INT32 dst_height,
			NT_INT32 x_src, NT_INT32 y_src,
			NT_INT32 src_width, NT_INT32 src_height,
			const NT_BYTE* rgb32_data, NT_UINT32 rgb32_data_size,
			NT_INT32 image_width, NT_INT32 image_height,
			NT_INT32 image_stride);

	} SmartPlayerSDKAPI;


	NT_UINT32 NT_API GetSmartPlayerSDKAPI(SmartPlayerSDKAPI* pAPI);

#ifdef __cplusplus
}
#endif

#endif