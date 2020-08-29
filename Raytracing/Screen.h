#pragma once
// �������
#include <windows.h>

class Screen
{
public:
	int init(int width, int height, const char* title); // ��ʼ������
	void update(); // ����
	void dispatch(); // �ɷ���Ϣ
	void close(); // �ر�
	int isKeyPressed(int key); // ���Ƿ񱻰���
	int getKeyUpEvent(int key);
	int isExit(); // �Ƿ�ر�
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
