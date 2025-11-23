#include "Player.h"
#include "DxLib.h"
#include "Character.h"
#include "Pad.h"
#include "Vec2.h"
#include <cassert> // ←assert用
#include "Input.h"
#include "Camera.h"
#include "Arrow.h"
#include "Bg.h"
#include <cmath>
#include <memory>

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置
	constexpr float kSpeed = 10.0f;//移動速度
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ//当たり判定の幅高さに使われている
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr float  player_scale = 3.0f;

	constexpr float kJumpPower = 20.0f;//ジャンプ力

	constexpr float kSmallJumpFrame = 5;//小ジャンプのフレーム


	constexpr float kSmallJumpHeight = 1.0f;//小ジャンプの高さ

	constexpr float kBigJumpHeight = 1.5f;//大ジャンプの高さ

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置

	constexpr float kBurningSpeed = 30.0f;//バーニングのスピード
	

}



Player::Player() :
	m_frame(0),
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isNomove(false),
	arrowFrame(-1),
	isArrowAttack(false)
	//m_pBg(nullptr)
{
	m_pos = kInitPos;
	m_handle = LoadGraph("data/player.png");
	_anim = Anim::Idle;
	_state = PlayerState::Normal;
	_type = PlayerType::Normal;
}
Player::~Player()
{
	DeleteGraph(m_handle);
}

void Player::Init()
{
}

void Player::Update()
{
}

void Player::Update(Input& input)
{

	m_frame++;
	m_animframe++;
	if (arrowFrame >= 0)
	{
		arrowFrame--;
	}
	DrawFormatString(10, 40, GetColor(255, 0, 0), "arrowFrameは%dです", arrowFrame);

	Rect chipRect;




	//当たり判定更新
	Character::SetRect();

	AnimSelect(_anim);

	InputUpdate(input);//特殊行動の入力検知

	switch (_state)
	{
	case PlayerState::Normal://アップデートの遷移
		NormalUpdate(input);
		break;
	case PlayerState::Attack://アップデートの遷移
		AttackUpdate();
		break;
	case PlayerState::Copy://アップデートの遷移
		CopyUpdate();
		break;
	}
	CheckHitMapPlayer(chipRect);

	if(m_pos.x <0)//画面外に出ないようにする
	{
		m_pos.x = 0;
	}

}

void Player::Draw()//使わない
{
//	float drawX = m_pos.x - m_pBg->GetScrollX();
//	float drawY = m_pos.y - m_pBg->GetScrollY();
//
//
//
//
//#ifdef _DEBUG
//	//当たり判定の描画
//	Character::Draw();
//	m_attackRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 0), false);
//	m_burningRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 0), false);
//	m_frozenRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 0), false);
//	m_archerRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 0), false);
//	m_copyRect.DrawScroll(m_pBg->GetScrollX(), m_pBg->GetScrollY(), GetColor(0, 255, 0), false);
//#endif
//	switch (_type)
//	{
//	case PlayerType::Normal:;//アニメーションの遷移
//		NormalAnim();
//		break;
//	case PlayerType::Burning:;//アニメーションの遷移
//		BurningAnim();
//		break;
//	case PlayerType::Frozen:;//アニメーションの遷移
//		FrozenAnim();
//		break;
//	case PlayerType::Archer:;//アニメーションの遷移
//		ArcherAnim();
//		break;
//	}
//
//	if (m_isRight)
//	{
//		DrawRectRotaGraph(drawX, drawY,
//		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
//		player_cut_w, player_cut_h,//切り取りの幅
//		player_scale, 0.0f, m_handle, true);
//	}
//	else
//	{
//		DrawRectRotaGraph(drawX, drawY,
//		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
//		player_cut_w, player_cut_h,//切り取りの幅
//		player_scale, 0.0f, m_handle, true, true);
//	}
//
//#ifdef _DEBUG
//	DrawFormatString(1000, 10, GetColor(255, 0, 0), "_animは%dです", _anim);
//	DrawFormatString(1000, 20, GetColor(255, 0, 0), "_stateは%dです", _state);
//	DrawFormatString(1000, 30, GetColor(255, 0, 0), "_typeは%dです", _type);
//	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです", charaIdx);
//	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです", charaIdy);
//#endif
}

void Player::Draw(Camera& camera)
{
	
#ifdef _DEBUG
	//当たり判定の描画
	//当たり判定の描画(変身攻撃など)
	switch (_state)
	{
	case PlayerState::Normal://アップデートの遷移
	
		break;
	case PlayerState::Attack://アップデートの遷移
		switch (_type)
		{
		case PlayerType::Normal://Normalの攻撃判定
			m_attackRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
			break;
		case PlayerType::Burning://バーニングの攻撃判定
			m_burningRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
			break;
		case PlayerType::Frozen://フローズンの攻撃判定
			m_frozenRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
			break;
		case PlayerType::Archer://アーチャーの攻撃判定
			m_archerRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
			break;

		}
		
		break;
	case PlayerState::Copy://アップデートの遷移
		m_copyRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 255, 0), false);
		break;
	}
	m_colRect.DrawCamera(camera.drawOffset.x,camera.drawOffset.y,GetColor(0,0,255),false);
	
	
	
	
	
#endif
	switch (_type)
	{
	case PlayerType::Normal://アニメーションの遷移
		NormalAnim();
		break;
	case PlayerType::Burning://アニメーションの遷移
		BurningAnim();
		break;
	case PlayerType::Frozen://アニメーションの遷移
		FrozenAnim();
		break;
	case PlayerType::Archer://アニメーションの遷移
		ArcherAnim();
		break;

	}

	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y,
		player_cut_w * charaIdx, player_cut_h * charaIdy,//切り取り左上
		player_cut_w, player_cut_h,//切り取りの幅
		player_scale, 0.0f, m_handle, true, true);
	}

#ifdef _DEBUG
	DrawFormatString(1000, 10, GetColor(255, 0, 0), "_animは%dです", _anim);
	DrawFormatString(1000, 20, GetColor(255, 0, 0), "_stateは%dです", _state);
	DrawFormatString(1000, 30, GetColor(255, 0, 0), "_typeは%dです", _type);
	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです", charaIdx);
	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです", charaIdy);
#endif
}

void Player::InputUpdate(Input& input)
{
	if (isNomove)return;
	//特殊行動の入力検知
	if (input.IsTriggered("Attack"))
	{
		_state = PlayerState::Attack;
		m_vel.x = 0;//攻撃中は動けないようにするため
		m_vel.y = 0;

		if (_type == PlayerType::Archer)
		{
			if (arrowFrame >= 0)return;
			//矢を回す
			arrowFrame = arrowtime;

		}
		
		if (_type == PlayerType::Burning)
		{
			if (coolTimer <= 0)
			{
			burningTimer = burningTime;
			isBurningAttack = true;
			}
		}
	}
	if (input.IsTriggered("Copy"))
	{
		_state = PlayerState::Copy;
	}

	if (input.IsTriggered("CopyOut"))
	{
		if (!(_type == PlayerType::Normal))
		{
			ChangeNormal();


		}
	}
}

void Player::NormalUpdate(Input& input)
{
	Move(input);
	Jump(input);
	Character::Gravity();

	//着地時にアニメーションを帰るところ
	if (m_isGround)
	{

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		if (_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}
	//m_pos += m_vel;
}

void Player::JumpUpdate(Input& input)
{
	Jump(input);
	Character::Gravity();


}

void Player::AttackUpdate()
{
	burningTimer--;
	coolTimer--;
	if (burningTimer >= 0.0f)
	{
		if (_type == PlayerType::Burning)//バーニングの攻撃処理
		{
			float dist = (m_isRight) ? kBurningSpeed : -kBurningSpeed;
			bool hit = MoveWithCollisionX(dist);//衝突判定付きで少しずつ移動(少しずつの間当たり判定)
			if (hit)
			{
				//衝突したので追加の処理
				//攻撃終了

				burningTimer = 0;
				isBurningAttack = false;
				_state = PlayerState::Normal;
				//isNomove = false;
			}
			/*if (m_isRight)m_pos.x += kBurningSpeed;
			else m_pos.x -= kBurningSpeed;*/
			
		}
	}
	else
	{
		if (_type == PlayerType::Burning)//減速時のbarningの攻撃処理
		{
			/*if (m_isRight)m_pos.x += kBurningSpeed / 3;
			else m_pos.x -= kBurningSpeed / 3;*/
			float dist = (m_isRight) ? kBurningSpeed/3 : -kBurningSpeed/3;
			bool hit = MoveWithCollisionX(dist);//衝突判定付きで少しずつ移動(少しずつの間当たり判定)
			if (hit)
			{
				//衝突したので追加の処理
				//攻撃終了

				burningTimer = 0;
				isBurningAttack = false;
				_state = PlayerState::Normal;
				//isNomove = false;
			}
		}
	}
	//着地時にアニメーションを帰るところ
	if (m_pos.y > kGround)
	{
		m_pos.y = kGround;
		m_isGround = true;

		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		if (_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}
	//バーニングだけ攻撃中は重力を受けない
	//微妙だったため一旦なし
	/*if (!(_type == PlayerType::Burning))
	{
		Character::Gravity();
	}*/
	//m_pos += m_vel;

	Attack();
}

void Player::CopyUpdate()
{
	Character::Gravity();
	Copy();
	//m_pos += m_vel;


}

void Player::Move(Input& input)
{
	AnimSelect(_anim);
	if (isNomove)return;
	//ここに処理を追加していく
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_vel.x = -kSpeed;
		m_isRight = false;


	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_vel.x = kSpeed;
		m_isRight = true;

	}
	else
	{
		m_vel.x = 0.0f;

	}
	if (_anim != Anim::Jump)
	{
		if (m_vel.x > 0.1f || m_vel.x < -0.1f)
		{
			AnimSelect(Anim::Walk);
		}
		else
		{
			AnimSelect(Anim::Idle);
		}
	}
}
void Player::Jump(Input& input)
{
	//ジャンプ中はスキップ
	if (!m_isGround) return;

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isJumpPreparing = true;

	}

	if (!m_isJumpPreparing) return;
	m_jumpFrame++;

	float jumpHeight = kJumpPower;//ジャンプの高さを決める

	//ボタンを離した瞬間にジャンプする
	if (!Pad::IsRelease(PAD_INPUT_1)) return;

	if (m_jumpFrame < kSmallJumpFrame)
	{
		jumpHeight = kSmallJumpHeight;
	}
	else
	{
		jumpHeight = kBigJumpHeight;
	}

	m_vel.y = -kJumpPower * jumpHeight;
	m_isGround = false;
	m_isJumpPreparing = false;
	AnimSelect(Anim::Jump);

}

void Player::Attack()
{

	isNomove = true;

	//アニメーション
	AnimSelect(Anim::Attack);
	//判定をつける

	switch (_type)
	{
	case PlayerType::Normal:
		//Normalの攻撃アニメーション
		m_attackRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -70.0f), m_pos.y - kCharaSize, 60.0f, 80.0f);//50
		break;
	case PlayerType::Burning:
		m_burningRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -100.0f), m_pos.y - kCharaSize, 90.0f, 80.0f);//80
		break;
	case PlayerType::Frozen:
		m_frozenRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -120.0f), m_pos.y - kCharaSize, 110.0f, 80.0f);//100
		break;
	case PlayerType::Archer:
		m_archerRect.SetLT(0, 0, 0, 0);
		break;
	default:
		break;
	}



}

void Player::Copy()
{
	isNomove = true;
	m_vel = zero;
	//アニメーション
	AnimSelect(Anim::Copy);
	//判定をつける
	m_copyRect.SetCenter(m_pos.x, m_pos.y - kCharaSize / 2, 80.0f, 100.0f);
}

void Player::AnimSelect(const Anim& anim)
{
	switch (_type)
	{
	case PlayerType::Normal://アニメーションの切り替わりの遷移
		AnimSelectNormal(anim);
		break;
	case PlayerType::Burning://アニメーションの切り替わりの遷移
		AnimSelectBurning(anim);
		break;
	case PlayerType::Frozen://アニメーションの切り替わりの遷移
		AnimSelectFrozen(anim);
		break;
	case PlayerType::Archer://アニメーションの切り替わりの遷移
		AnimSelectArcher(anim);
		break;
	default:
		assert(false && "存在しないタイプです");
		break;
	}

	return;
}

void Player::AnimSelectNormal(const Anim& anim)
{
	if (_anim == Anim::Attack && charaIdx == 6)//攻撃アニメーション終了
	{
		if (_type == PlayerType::Burning)
		{
			coolTimer = coolTime;
		}
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;
		

	}
	if (_anim == Anim::Copy && charaIdx == 5)//コピーアニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;
	if (_anim == Anim::Copy)return;


	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectBurning(const Anim& anim)
{
	if (_anim == Anim::Attack && charaIdx == 7)//攻撃アニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}
	if (_anim == Anim::Copy && charaIdx == 7)//コピーアニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;
	if (_anim == Anim::Copy)return;


	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectFrozen(const Anim& anim)
{
	if (_anim == Anim::Attack && charaIdx == 14)//攻撃アニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}
	if (_anim == Anim::Copy && charaIdx == 14)//コピーアニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;
	if (_anim == Anim::Copy)return;


	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectArcher(const Anim& anim)
{
	if (_anim == Anim::Attack && charaIdx == 11)//攻撃アニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}
	if (_anim == Anim::Copy && charaIdx == 8)//コピーアニメーション終了
	{
		_anim = Anim::Idle;
		isNomove = false;
		_state = PlayerState::Normal;

	}

	if (_anim == Anim::Attack)return;
	if (_anim == Anim::Copy)return;


	if (_anim != anim)
	{
		_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}



void Player::NormalAnim()
{
	switch (_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / 6) % 6;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / 8) % 8;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 5)
		{
			charaIdx = 5;
			charaIdy = 3;
		}
		else
		{
			charaIdx = (m_animframe / 10) % 2 + 4;
			charaIdy = 3;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / 7) % 7;
		charaIdy = 2;
		break;
	case Anim::Copy:
		charaIdx = (m_animframe / 10) % 6;
		charaIdy = 4;
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;
	}
}
void Player::BurningAnim()
{
	switch (_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / 4) % 6;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / 4) % 8;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 3)
		{
			charaIdx = 3;
			charaIdy = 1;
		}
		else
		{
			charaIdx = (m_animframe / 10) % 3 + 0;
			charaIdy = 1;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / 5) % 8;
		charaIdy = 5;
		break;
	case Anim::Copy:
		charaIdx = (m_animframe / 10) % 8;//【注意】切り替わった直後のアニメーションはここが流れてる
		charaIdy = 5;
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
}

void Player::FrozenAnim()
{
	switch (_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / 6) % 6;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / 8) % 8;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 3)
		{
			charaIdx = 3;
			charaIdy = 6;
		}
		else
		{
			charaIdx = (m_animframe / 2) % 4 + 0;
			charaIdy = 6;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / 4) % 15;
		charaIdy = 2;
		break;
	case Anim::Copy://【注意】切り替わった直後のアニメーションはここが流れてる
		charaIdx = (m_animframe / 10) % 15 + 4;
		charaIdy = 4;
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
}

void Player::ArcherAnim()
{
	switch (_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / 6) % 6;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / 8) % 8;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 2)
		{
			charaIdx = 2; [[]]
				charaIdy = 1;
		}
		else
		{
			charaIdx = (m_animframe / 10) % 3 + 0;
			charaIdy = 1;
		}
		break;
	case Anim::Attack:
		if (m_animframe == 27) isArrowAttack = true;//chraIdxが9になった瞬間に矢を発射する//9 * 3;
		charaIdx = (m_animframe / 3) % 12;
		charaIdy = 3;
		break;
	case Anim::Copy://【注意】切り替わった直後のアニメーションはここが流れてる
		charaIdx = (m_animframe / 3) % 9;
		charaIdy = 2;
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
}

bool Player::MoveWithCollisionX(float distance)
{
	if (distance == 0.0f) return false;

	//1ステップ当たりの移動量
	const float stepSize = 4.0f;
	int steps = static_cast<int>(std::ceil(std::abs((distance)) / stepSize));
	if (steps <= 0) steps = 1;

	//1ステップの実際の移動量
	float stepDelta = distance / static_cast<float>(steps);

	Rect chipRect;
	for (int i = 0; i < steps; ++i)
	{
		m_pos.x += stepDelta;
		//衝突チェック
		bool collided = Character::CheckHitMapPlayer_(chipRect);
		if (collided)
		{
			if (m_isRight)
			{
				//m_pos.x = m_colRect.Getleft() -  kCharaSize / 2;
			
			}
			else
			{
				//m_pos.x = m_colRect.GetRight() + kCharaSize / 2;
			}
			//衝突したのでこれ以上進まない
			m_pos.x -= stepDelta;
			return true;
		}

	}


	//衝突なし
	return false;
}

void Player::ChangeNormal()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/player.png");
	_type = PlayerType::Normal;
	_state = PlayerState::Normal;
}

void Player::ChangeBurning()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Burning.png");
	_type = PlayerType::Burning;
	_state = PlayerState::Normal;
}

void Player::ChangeFrozen()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Frozen.png");
	_type = PlayerType::Frozen;
	_state = PlayerState::Normal;
}

void Player::ChangeArcher()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Archer.png");
	_type = PlayerType::Archer;
	_state = PlayerState::Normal;
}

std::shared_ptr<Arrow> Player::ShotArrow()
{
	std::shared_ptr<Arrow> arrow;
	arrow = std::make_shared<Arrow>();
	
	arrow->ChangePos() = m_pos;

	arrow->m_playerdir = m_isRight;
		

		return arrow;
	
}
