#pragma once
#include "zpack/zpack.h"

#define zPackFClose(rfile) rfile->deleteMe()

namespace PxcUtil
{

enum EzPOpen_Result
{
	EzPOpen_Ok = 0,
	EzPOpen_SimplePath,
	EzPOpen_NoPack,
	EzPOpen_NoFile,
};

EzPOpen_Result zPackFOpen(const char* szComboPath, zp::IReadFile** ppReadFile);
void zPackRelease();
void zPackPathSwitch(bool bFlag);
void zPackAddPathAim(const char* szPack, const char* szPath);
void zPackCombinePath(std::string& strPath);
void zPackClearPathAims();

}