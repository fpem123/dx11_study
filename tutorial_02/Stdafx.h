#pragma once
/*
 * stdafx.h : 자주 사용하지만 자주 병경되지 않는 
 * 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
 * 들어있는 파일
 * 작성일 : 09.25.21
 * 마지막 업데이트 : 09.25.21
 */

//#include "targetver.h"

// 잘 사용하지 않는 내용은 헤더에서 제외
#define WIN32_LEAN_AND_MEAN

// 윈도우즈 헤더
#include <windows.h>

// C 런타임 헤더
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TO-DO : 프로그램에 필요한 추가 헤더는 여기서 참조
#include "DxDefine.h"