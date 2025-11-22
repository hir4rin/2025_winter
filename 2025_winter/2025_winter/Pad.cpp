#include "Pad.h"
#include "DxLib.h"

namespace 
{
	//前のフレームのパッド押し下げ状態
	int lastPad = 0;
	//このフレームのパッド押し下げ状態
	int nowPad = 0;

}


namespace Pad
{
	void Update()
	{
		//前のフレームの状態を保存
		lastPad = nowPad;

		//現在のパッドの状態を取得
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool Pad::IsPress(int key)//keyがキーボードってこと
	{
		return (nowPad & key);
	}

	bool Pad::IsTrigger(int key)
	{
		bool isNow = (nowPad & key);  //現在フレーム
		bool isLast = (lastPad & key);//前のフレーム

		//現在のフレームに押されていて前回のフレームに押されていない
		if (isNow && !isLast)
		{
			return true;
		}
		return false;
	}

	bool Pad::IsRelease(int key)
	{
		bool isNow = (nowPad & key);  //現在フレーム

		bool isLast = (lastPad & key);//前のフレーム

		//現在のフレームに押されていなくて前回のフレームに押されている
		if (!isNow && isLast)
		{
			return true;
		}
		return false;
	}
}