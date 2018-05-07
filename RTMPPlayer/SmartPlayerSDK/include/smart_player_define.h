/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/


#ifndef SMART_PLAYER_DEFINE_H_
#define SMART_PLAYER_DEFINE_H_

#ifdef WIN32
#include <windows.h>
#endif

#ifdef SMART_HAS_COMMON_DIC
#include "../../topcommon/nt_type_define.h"
#include "../../topcommon/nt_base_code_define.h"
#else
#include "nt_type_define.h"
#include "nt_base_code_define.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifndef NT_HWND_
#define NT_HWND_
#ifdef WIN32
typedef HWND NT_HWND;
#else
typedef void* NT_HWND;
#endif
#endif


#ifndef NT_HDC_
#define NT_HDC_

#ifdef _WIN32
typedef HDC NT_HDC;
#else
typedef void* NT_HDC;
#endif

#endif


/*错误码*/
typedef enum _SP_E_ERROR_CODE
{
	NT_ERC_SP_HWND_IS_NULL = (NT_ERC_SMART_PLAYER_SDK | 0x1), // 窗口句柄是空
	NT_ERC_SP_HWND_INVALID = (NT_ERC_SMART_PLAYER_SDK | 0x2), // 窗口句柄无效
	NT_ERC_SP_TOO_MANY_CAPTURE_IMAGE_REQUESTS = (NT_ERC_SMART_PLAYER_SDK | 0x3), // 太多的截图请求
	NT_ERC_SP_WINDOW_REGION_INVALID = (NT_ERC_SMART_PLAYER_SDK | 0x4), // 窗口区域无效，可能窗口宽或者高小于1
	NT_ERC_SP_DIR_NOT_EXIST = (NT_ERC_SMART_PLAYER_SDK | 0x5), // 目录不存在
	NT_ERC_SP_NEED_RETRY = (NT_ERC_SMART_PLAYER_SDK | 0x6), // 需要重试
} SP_E_ERROR_CODE;


/*设置参数ID, 这个目前这么写，SmartPlayerSDK 已经划分范围*/
typedef enum _SP_E_PARAM_ID
{
	SP_PARAM_ID_BASE = NT_PARAM_ID_SMART_PLAYER_SDK,
	
} SP_E_PARAM_ID;


/*事件ID*/
typedef enum _NT_SP_E_EVENT_ID
{
	NT_SP_E_EVENT_ID_BASE = NT_EVENT_ID_SMART_PLAYER_SDK,

	NT_SP_E_EVENT_ID_CONNECTING				= NT_SP_E_EVENT_ID_BASE | 0x2,	/*连接中*/
	NT_SP_E_EVENT_ID_CONNECTION_FAILED		= NT_SP_E_EVENT_ID_BASE | 0x3,	/*连接失败*/
	NT_SP_E_EVENT_ID_CONNECTED				= NT_SP_E_EVENT_ID_BASE | 0x4,	/*已连接*/
	NT_SP_E_EVENT_ID_DISCONNECTED			= NT_SP_E_EVENT_ID_BASE | 0x5,	/*断开连接*/
	NT_SP_E_EVENT_ID_NO_MEDIADATA_RECEIVED	= NT_SP_E_EVENT_ID_BASE | 0x8,	/*收不到RTMP数据*/

	/* 接下来请从0x81开始*/
	NT_SP_E_EVENT_ID_START_BUFFERING = NT_SP_E_EVENT_ID_BASE | 0x81, /*开始缓冲*/
	NT_SP_E_EVENT_ID_BUFFERING		 = NT_SP_E_EVENT_ID_BASE | 0x82, /*缓冲中, param1 表示百分比进度*/
	NT_SP_E_EVENT_ID_STOP_BUFFERING  = NT_SP_E_EVENT_ID_BASE | 0x83, /*停止缓冲*/

	NT_SP_E_EVENT_ID_DOWNLOAD_SPEED  = NT_SP_E_EVENT_ID_BASE | 0x91, /*下载速度， param1表示下载速度，单位是(Byte/s)*/

	NT_SP_E_EVENT_ID_PLAYBACK_REACH_EOS		= NT_SP_E_EVENT_ID_BASE | 0xa1, /*播放结束, 直播流没有这个事件，点播流才有*/
	NT_SP_E_EVENT_ID_RECORDER_REACH_EOS		= NT_SP_E_EVENT_ID_BASE | 0xa2, /*录像结束, 直播流没有这个事件, 点播流才有*/
	NT_SP_E_EVENT_ID_PULLSTREAM_REACH_EOS   = NT_SP_E_EVENT_ID_BASE | 0xa3, /*拉流结束, 直播流没有这个事件，点播流才有*/

	NT_SP_E_EVENT_ID_DURATION				= NT_SP_E_EVENT_ID_BASE | 0xa8, /*视频时长，如果是直播，则不上报，如果是点播的话, 若能从视频源获取视频时长的话，则上报， param1表示视频时长，单位是毫秒(ms)*/

} NT_SP_E_EVENT_ID;


//定义视频帧图像格式
typedef enum _NT_SP_E_VIDEO_FRAME_FORMAT
{
	NT_SP_E_VIDEO_FRAME_FORMAT_RGB32 = 1, // 32位的rgb格式, r, g, b各占8, 另外一个字节保留, 内存字节格式为: bb gg rr xx, 主要是和windows位图匹配, 在小端模式下，按DWORD类型操作，最高位是xx, 依次是rr, gg, bb
	NT_SP_E_VIDEO_FRAME_FORMAT_ARGB  = 2, // 32位的argb格式，内存字节格式是: bb gg rr aa 这种类型，和windows位图匹配
	NT_SP_E_VIDEO_FRAME_FROMAT_I420  = 3, // YUV420格式, 三个分量保存在三个面上
} NT_SP_E_VIDEO_FRAME_FORMAT;


// 定义视频帧结构.
typedef struct _NT_SP_VideoFrame
{
	NT_INT32  format_;  // 图像格式, 请参考NT_SP_E_VIDEO_FRAME_FORMAT
	NT_INT32  width_;   // 图像宽
	NT_INT32  height_;  // 图像高

	NT_UINT64 timestamp_; // 时间戳, 一般是0,不使用, 以ms为单位的

	// 具体的图像数据, argb和rgb32只用第一个, I420用前三个
	NT_UINT8* plane0_;
	NT_UINT8* plane1_;
	NT_UINT8* plane2_;
	NT_UINT8* plane3_;

	// 每一个平面的每一行的字节数，对于argb和rgb32，为了保持和windows位图兼容，必须是width_*4
	// 对于I420, stride0_ 是y的步长, stride1_ 是u的步长, stride2_ 是v的步长,
	NT_INT32  stride0_;
	NT_INT32  stride1_;
	NT_INT32  stride2_;
	NT_INT32  stride3_;

} NT_SP_VideoFrame;


// 如果三项都是0的话，将不能启动录像
typedef struct _NT_SP_RecorderFileNameRuler
{
	NT_UINT32	type_; // 这个值目前默认是0，将来扩展用
	NT_PCSTR	file_name_prefix_; // 设置一个录像文件名前缀, 例如:daniulive
	NT_INT32	append_date_; // 如果是1的话，将在文件名上加日期, 例如:daniulive-2017-01-17
	NT_INT32	append_time_; //  如果是1的话，将增加时间，例如:daniulive-2017-01-17-17-10-36
} NT_SP_RecorderFileNameRuler;


/*
*拉流吐视频数据时，一些相关的数据
*/
typedef struct _NT_SP_PullStreamVideoDataInfo
{
	NT_INT32  is_key_frame_; /* 1:表示关键帧, 0：表示非关键帧 */
	NT_UINT64 timestamp_;	/* 解码时间戳, 单位是毫秒 */
	NT_INT32  width_;	/* 一般是0 */
	NT_INT32  height_; /* 一般也是0 */
	NT_BYTE*  parameter_info_; /* 一般是NULL */
	NT_UINT32 parameter_info_size_; /* 一般是0 */
	NT_UINT64 presentation_timestamp_; /*显示时间戳, 这个值要大于或等于timestamp_， 单位是毫秒*/

} NT_SP_PullStreamVideoDataInfo;


/*
*拉流吐音频数据时，一些相关的数据
*/
typedef struct _NT_SP_PullStreamAuidoDataInfo
{
	NT_INT32  is_key_frame_; /* 1:表示关键帧, 0：表示非关键帧 */
	NT_UINT64 timestamp_;	/* 单位是毫秒 */
	NT_INT32  sample_rate_;	/* 一般是0 */
	NT_INT32  channel_; /* 一般是0 */
	NT_BYTE*  parameter_info_; /* 如果是AAC的话，这个是有值的, 其他编码一般忽略 */
	NT_UINT32 parameter_info_size_; /*如果是AAC的话，这个是有值的, 其他编码一般忽略 */
	NT_UINT64 reserve_; /* 保留  */

} NT_SP_PullStreamAuidoDataInfo;


/*
当播放器得到时候视频大小后，会回调
*/
typedef NT_VOID(NT_CALLBACK *SP_SDKVideoSizeCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_INT32 width, NT_INT32 height);

/*
调用Start时传入, 回调接口
*/
typedef NT_VOID(NT_CALLBACK *SP_SDKStartPlayCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT32 result);


/*
视频图像回调
status:目前不用，默认是0，将来可能会用
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKVideoFrameCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT32 status,
	const NT_SP_VideoFrame* frame);


/*
音频PCM数据回调, 目前每帧长度是10ms
status:目前不用，默认是0，将来可能会用
data: PCM 数据
size: 数据大小
sample_rate: 采样率
channel: 通道数
per_channel_sample_number: 每个通道的采样数
*/
typedef NT_VOID(NT_CALLBACK* NT_SP_SDKAudioPCMFrameCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT32 status,
	NT_BYTE* data, NT_UINT32 size,
	NT_INT32 sample_rate, NT_INT32 channel, NT_INT32 per_channel_sample_number);

/*
截屏回调
result: 如果截屏成功的话，result是NT_ERC_OK,其他错误
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKCaptureImageCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT32 result,
	NT_PCSTR file_name);


/*
绘制视频时，视频帧时间戳回调, 这个用在一些特殊场景下，没有特殊需求的用户不需要关注
timestamp: 单位是毫秒
reserve1: 保留参数
reserve2: 保留参数
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKRenderVideoFrameTimestampCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT64 timestamp,
	NT_UINT64 reserve1, NT_PVOID reserve2);


/*
录像回调
status: 1:表示开始写一个新录像文件. 2:表示已经写好一个录像文件
file_name: 实际录像文件名
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKRecorderCallBack)(NT_HANDLE handle, NT_PVOID user_data, NT_UINT32 status,
	NT_PCSTR file_name);


/*
*拉流时，视频数据回调
video_codec_id: 请参考NT_MEDIA_CODEC_ID
data: 视频数据
size: 视频数据大小
info: 视频数据相关信息
reserve: 保留参数
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKPullStreamVideoDataCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 video_codec_id, NT_BYTE* data, NT_UINT32 size, 
	NT_SP_PullStreamVideoDataInfo* info,
	NT_PVOID reserve);


/*
*拉流时，音频数据回调
auido_codec_id: 请参考NT_MEDIA_CODEC_ID
data: 音频数据
size: 音频数据大小
info: 音频数据相关信息
reserve: 保留参数
*/
typedef NT_VOID(NT_CALLBACK* SP_SDKPullStreamAudioDataCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 auido_codec_id, NT_BYTE* data, NT_UINT32 size, 
	NT_SP_PullStreamAuidoDataInfo* info,
	NT_PVOID reserve);


/*
*播放器事件回调
event_id: 事件ID，请参考NT_SP_E_EVENT_ID
param1 到 param6, 值的意义和具体事件ID相关, 注意如果具体事件ID没有说明param1-param6的含义，那说明这个事件不带参数
*/
typedef NT_VOID(NT_CALLBACK* NT_SP_SDKEventCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 event_id,
	NT_INT64  param1,
	NT_INT64  param2,
	NT_UINT64 param3,
	NT_PCSTR  param4,
	NT_PCSTR  param5,
	NT_PVOID  param6
	);


#ifdef __cplusplus
}
#endif

#endif