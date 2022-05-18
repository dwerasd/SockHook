#pragma once


#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define IS_SET(flag, bit)        ((flag) & (bit))



ULONG FindPattern(ULONG dwEntry , ULONG dwMemSize , LPBYTE Pattern, long dwpSize);
