#include "stdafx.h"
#include "Publisher.h"


int CalBitRate(int frame_rate, int w, int h) {
	auto kbit_rate = 2000;

	int area = w * h;

	if (area <= (320 * 300)) {
		kbit_rate = 280;
	} else if (area <= (360 * 320)) {
		kbit_rate = 360;
	} else if (area <= (640 * 480)) {
		kbit_rate = 580;
	} else if (area <= (800 * 600)) {
		kbit_rate = 620;
	} else if (area <= (900 * 700)) {
		kbit_rate = 820;
	} else if (area <= (1280 * 720)) {
		kbit_rate = 1600;
	} else if (area <= (1366 * 768)) {
		kbit_rate = 2000;
	} else if (area <= (1600 * 900)) {
		kbit_rate = 2300;
	} else if (area <= (1600 * 1050)) {
		kbit_rate = 2500;
	} else {
		kbit_rate = 2800;
	}

	kbit_rate = kbit_rate * frame_rate / 25;

	if (kbit_rate < 80)
		kbit_rate = 80;

	return kbit_rate;
}
int CalMaxKBitRate(int frame_rate, int w, int h) {
	auto max_kbit_rate = 2000;

	int area = w * h;

	if (area <= (320 * 300)) {
		max_kbit_rate = 600;
	} else if (area <= (360 * 320)) {
		max_kbit_rate = 800;
	} else if (area <= (640 * 480)) {
		max_kbit_rate = 1300;
	} else if (area <= (800 * 600)) {
		max_kbit_rate = 1500;
	} else if (area <= (900 * 700)) {
		max_kbit_rate = 2200;
	} else if (area <= (1280 * 720)) {
		max_kbit_rate = 3000;
	} else if (area <= (1366 * 768)) {
		max_kbit_rate = 4000;
	} else if (area <= (1600 * 900)) {
		max_kbit_rate = 4500;
	} else if (area <= (1600 * 1050)) {
		max_kbit_rate = 4800;
	} else {
		max_kbit_rate = 5500;
	}

	max_kbit_rate = max_kbit_rate * frame_rate / 25;

	if (max_kbit_rate < 130)
		max_kbit_rate = 140;

	return max_kbit_rate;
}

BOOL CALLBACK MonitorEnumProcSize(HMONITOR hm, HDC, LPRECT prect, LPARAM user_data) {
	if (hm == NULL) {
		return TRUE;
	}

	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);

	if (GetMonitorInfo(hm, &mi)) {
		if (mi.dwFlags & MONITORINFOF_PRIMARY) {
			if (user_data != NULL) {
				std::pair<int, int>* size = (std::pair<int, int>*)(user_data);
				size->first = mi.rcMonitor.right - mi.rcMonitor.left;
				size->second = mi.rcMonitor.bottom - mi.rcMonitor.top;

				return FALSE;
			}
		}
	}

	return TRUE;
}

void PublisherRoutines::_setOptionsToPublisherSDK() {
	int w = 0, h = 0;

	std::pair<int, int> size(0, 0);

	auto ret = EnumDisplayMonitors(NULL, NULL, &MonitorEnumProcSize, (LPARAM)&size);
	if (size.first == 0 || size.second == 0) {
		w = ::GetSystemMetrics(SM_CXSCREEN);
		h = ::GetSystemMetrics(SM_CYSCREEN);
	} else {
		w = size.first;
		h = size.second;
	}

	_publisherAPI.SetScreenClip(_publisherHandle, 0, 0, 0, 0);
	_publisherAPI.SetFrameRate(_publisherHandle, _frameRate);
	_publisherAPI.EnableDXGIScreenCapturer(_publisherHandle, 1);
	_publisherAPI.SetVideoMaxBitRate(_publisherHandle, CalMaxKBitRate(_frameRate, w, h));
	_publisherAPI.SetVideoBitRate(_publisherHandle, CalBitRate(_frameRate, w, h));
	_publisherAPI.SetVideoKeyFrameInterval(_publisherHandle, 10);
	_publisherAPI.SetVideoEncoderProfile(_publisherHandle, 1);
	_publisherAPI.SetVideoEncoderSpeed(_publisherHandle, 3);

	_setAudioOptionsToPublisherSDK();
}

void PublisherRoutines::setInfo(const RoleInfo &info) {
	_id = info.id;
	_frameRate = info.frameRate;
	_port = info.port;
}

void PublisherRoutines::run() {
	_openPublisherHandle(true);
	_setOptionsToPublisherSDK();
	auto wURL = L"rtmp://localhost:" + std::to_wstring(_port) + L"/live/" + _id + L"_publisher";
	std::wcout << L"publishing at: " + wURL << std::endl;
	_publisherAPI.SetURL(_publisherHandle, std::string(wURL.begin(), wURL.end()).c_str(), NULL);

	if (NT_ERC_OK != _publisherAPI.StartPublisher(_publisherHandle, NULL)) {
		std::wcout << L"Call publisher_api.StartPublisher failed" << std::endl;
		return;
	}
}
