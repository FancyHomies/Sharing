#pragma once
// 窗口相关
#include <windows.h>

class Screen
{
public:
	int init(int width, int height, const char* title); // 初始化窗口
	void update(); // 更新
	void dispatch(); // 派发消息
	void close(); // 关闭
	int isKeyPressed(int key); // 键是否被按下
	int getKeyUpEvent(int key);
	int isExit(); // 是否关闭
	LPVOID getFrameBuffer();

	void setColor(int x, int y, UINT32 c)
	{
		UINT32* fb = (UINT32*) wndFramebuffer;
		fb[x + y * width] = c;
	};

	UINT32 getColor(int x, int y) const
	{
		UINT32* fb = (UINT32*)wndFramebuffer;
		return fb[x + y * width];
	};

private:
	HWND wndHandle;
	HDC wndDc;
	HBITMAP wndHb;
	HBITMAP wndOb;
	LPVOID wndFramebuffer; // framebuffer
	int width;
	int height;
};
