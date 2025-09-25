#include <Windows.h>
#include <fstream>
#include <string>
#include "EditWidgetDefinitions.h"

HWND hEditWidget;



void CreateEditWidget(HWND hWnd) {
	RECT clientRectangle;
	GetClientRect(hWnd, &clientRectangle);
	hEditWidget = CreateWindow(L"edit", L"Put any text here!", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, clientRectangle.top, clientRectangle.left, clientRectangle.right, clientRectangle.bottom, hWnd, NULL, NULL, NULL);
}

void SaveData(LPCWSTR filePath) {
	HANDLE fileToSave = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int saveFileLength = GetWindowTextLengthW(hEditWidget) + 1;
	WCHAR* data = new WCHAR[saveFileLength];

	saveFileLength = GetWindowTextW(hEditWidget, data, saveFileLength) ;

	DWORD bytesIterated;
	WriteFile(fileToSave, data, saveFileLength*sizeof(WCHAR), &bytesIterated, NULL);
	CloseHandle(fileToSave);
	delete[] data;
}

using namespace std;
void LoadData(LPCWSTR filePath) {
	ifstream file(filePath, ios::binary | ios::ate);
	if (!file.is_open()) {
		MessageBox(NULL, L"Не удалось открыть файл", L"Ошибка", MB_ICONERROR);
		return;
	}
	streamsize size = file.tellg();
	file.seekg(0, ios::beg);
	WCHAR* buffer = new WCHAR[size / sizeof(WCHAR) + 1];
	ZeroMemory(buffer, (size / sizeof(WCHAR) + 1) * sizeof(WCHAR));
	file.read(reinterpret_cast<char*>(buffer), size);
	file.close();
	SetWindowTextW(hEditWidget, buffer);

	delete[] buffer;
}

