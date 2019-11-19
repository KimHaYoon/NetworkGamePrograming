#pragma once
#pragma comment (lib, "msimg32.lib")
#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)
#include <winsock2.h>
#include <windows.h>
#include "resource.h"
#include "Define.h"
#include "Struct.h"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include <conio.h>
#include "Protocol.h"

using namespace std;

// 사용자 정의 데이터 수신 함수
static int recvn( SOCKET s, char *buf, int len, int flags )
{
	int received;
	char *ptr = buf;
	int left = len;

	while ( left > 0 ) {
		received = recv( s, ptr, left, flags );
		if ( received == SOCKET_ERROR )
			return SOCKET_ERROR;
		else if ( received == 0 )
			break;
		left -= received;
		ptr += received;
	}

	return ( len - left );
}