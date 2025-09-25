#pragma once
#define BUFFER_SIZE 256
extern HWND hEditWidget;

void CreateEditWidget(HWND hWnd);
void SaveData(LPCWSTR filePath);
void LoadData(LPCWSTR filePath);