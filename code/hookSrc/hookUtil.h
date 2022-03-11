#ifndef UTIL_H
#define UTIL_H
#include<windows.h>
#include<stdio.h>
#include <QStandardItemModel>
#include <QDebug>
#include<QString>
#include <QTableWidgetItem>
#include <TlHelp32.h>
#include<QVariant>
#include <psapi.h>
#include <QStringListModel>
#include <CONIO.H>
#include "../util.h"

#define MAXPATH 260
#define SHELL_SIZE 2288


BOOL getPrivilege();
BOOL enumProcess(QStandardItemModel * theModel);
BOOL setMemory(QStandardItemModel * theModel);
BOOL hook(DWORD pid, QString proName,QString moduleName, QString funcName);
BOOL enumModuleAndFunc(DWORD pid, QStringListModel * model,
                       QHash<QString, QStringList> *modAndFunc,
                       QString proName);
extern BYTE code[];

//inlineHook
//定义如下结构，保存一次InlineHook所需要的信息
typedef struct _HOOK_DATA {
    HANDLE handle;          //待Hook的进程的句柄
    QString szApiName;	//待Hook的API名字
    QString szModuleName;	//待Hook的API所属模块的名字
    int  HookCodeLen;		//Hook长度
    BYTE oldEntry[8];		//保存Hook位置的原始指令
    BYTE newEntry[8];		//保存要写入Hook位置的新指令
    BYTE HotPatchCode[8];	//用于HotPatch式Hook
    DWORD HookPoint;		//待HOOK的位置
    DWORD JmpBackAddr;		//回跳到原函数中的位置
    DWORD pfnTrampolineFun;	//调用原始函数的通道
    DWORD pfnDetourFun;		//HOOK过滤函数
    DWORD shellcode;         //shellcode的首地址
}HOOK_DATA, * PHOOK_DATA;


extern HOOK_DATA hookdata;
extern BYTE DetourFun[];
extern BYTE TrampolineFun[];

BOOL GetCodeLength(PHOOK_DATA pHookData);
BOOL IfHooked(PHOOK_DATA pHookData);
BOOL SkipJmpAddress(PHOOK_DATA pHookData);
void InitHookEntry(PHOOK_DATA pHookData);
BOOL InstallCodeHook(PHOOK_DATA pHookData);
BOOL inlinehook(DWORD pid, QString proName,QString moduleName, QString funcName);


#endif // UTIL_H

