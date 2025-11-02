#pragma once

namespace Pad//クラスだといろいろな場所で使えない理由は何か？
{
	void Update();

	bool IsPress(int key);
	//押された瞬間を取得
	bool IsTrigger(int key);
	//離した瞬間を取得
	bool IsRelease(int key);
}


