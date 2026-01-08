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
#include <algorithm>

namespace
{
	const Vec2 kInitPos = { 100.0f,700.0f };//初期位置
	constexpr float kSpeed = 10.0f;//移動速度
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ//当たり判定の幅高さに使われている
	constexpr int kPlayerCutW = 100;
	constexpr int kPlayerCutH = 100;

	constexpr float kDamageInterval = 100;

	constexpr float  kPlayerScale = 3.0f;

	constexpr float kJumpPower = -15.0f;	// ジャンプ力
	constexpr int kMaxJumpFrame = 15;	// ジャンプを長押しできる最大時間

	//攻撃のクールタイム
	constexpr int kAttackCool = 8;

	//あまり慣性が乗っている感じはないように見える(笑)
	constexpr float accel = 2.0f;//加速度
	constexpr float decel = 1.0f;//減速度

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置

	constexpr float kBurningSpeed = 30.0f;//バーニングのスピード

	constexpr float kDamageFrame = 40.0f;
	constexpr float kBurningTime = 9.0f;  //バーニングの移動時間

	//Normal用
	//1frameあたりのアニメーションの時間
	constexpr int kNormalIdleDuration = 6;
	constexpr int kNormalWalkDuration = 8;
	constexpr int kNormalJumpDuration = 10;
	constexpr int kNormalAttackDuration = 4;
	constexpr int kNormalCopyDuration = 10;
	constexpr int kNormalDamageDuration = 10;
	constexpr int kNormalDeadDuration = 15;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kNormalIdleNum = 6;
	constexpr int kNormalWalkNum = 6;
	constexpr int kNormalJumpNum = 2;
	constexpr int kNormalAttackNum = 7;
	constexpr int kNormalCopyNum = 6;
	constexpr int kNormalDamageNum = 4;
	constexpr int kNormalDeadNum = 4;
	//Burning用
	//1frameあたりのアニメーションの時間
	constexpr int kBurningIdleDuration = 4;
	constexpr int kBurningWalkDuration = 4;
	constexpr int kBurningJumpDuration = 10;
	constexpr int kBurningAttackDuration = 5;
	constexpr int kBurningCopyDuration = 10;
	constexpr int kBurningDamageDuration = 5;
	constexpr int kBurningDeadDuration = 15;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kBurningIdleNum = 6;
	constexpr int kBurningWalkNum = 8;
	constexpr int kBurningJumpNum = 3;
	constexpr int kBurningAttackNum = 8;
	constexpr int kBurningCopyNum = 8;
	constexpr int kBurningDamageNum = 4;
	constexpr int kBurningDeadNum = 4;
	//frozen用
	//1frameあたりのアニメーションの時間
	constexpr int kBrozenIdleDuration = 6;
	constexpr int kFrozenWalkDuration = 8;
	constexpr int kFrozenJumpDuration = 2;
	constexpr int kFrozenAttackDuration = 2;
	constexpr int kFrozenCopyDuration = 6;
	constexpr int kFrozenDamageDuration = 5;
	constexpr int kFrozenDeadDuration = 15;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kFrozenIdleNum = 6;
	constexpr int kFrozenWalkNum = 8;
	constexpr int kFrozenJumpNum = 4;
	constexpr int kFrozenAttackNum = 15;
	constexpr int kFrozenCopyNum = 10;
	constexpr int kFrozenDamageNum = 4;
	constexpr int kFrozenDeadNum = 4;
	//Archer用
	//1frameあたりのアニメーションの時間
	constexpr int kArcherIdleDuration = 6;
	constexpr int kArcherWalkDuration = 8;
	constexpr int kArcherJumpDuration = 10;
	constexpr int kArcherAttackDuration = 1;
	constexpr int kArcherCopyDuration = 6;
	constexpr int kArcherDamageDuration = 5;
	constexpr int kArcherDeadDuration = 15;

	//そのアニメーションが何このコマ数なのか(X)
	constexpr int kArcherIdleNum = 6;
	constexpr int kArcherWalkNum = 8;
	constexpr int kArcherJumpNum = 3;
	constexpr int kArcherAttackNum = 12;
	constexpr int kArcherCopyNum = 12;
	constexpr int kArcherDamageNum = 4;
	constexpr int kArcherDeadNum = 4;
	//アーチャーの矢をいつ出すか
	constexpr int kArrowAppearCharaIdx = 9;//矢を出すときのcharaIdx
	const int arrowAppearFrame = kArcherAttackDuration * kArrowAppearCharaIdx;//charaIdxが9になる瞬間




	//カメラ
	constexpr int ScreenHeight = 1080;
	//カメラから持ってきた
	const float topPos = ScreenHeight * 0.75;

}


//ゲームプレイ中用
Player::Player(PlayerType type, int hp, Vec2 pos,int Life) :
	m_frame(0),
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isNomove(false),
	arrowFrame(-1),
	isArrowAttack(false),
	isJumping(false),
	BurningPrevPos{ 0,0 },
	BurningAfterPos{ 0,0 },
	isBurningAttack(false),
	damageCount(0),
	m_angle(0.0f),
	m_wasGround(false),
	m_hp(hp),//HPは後で引継ぎできるように変える,
	m_life(Life),//残機数
	m_lastTapTime(0),
	m_lastTapDir(-1),
	m_isDash(false),
	isRotateOne(false),
	m_rotateFrame(0),
	damageTimer(0),
	attackCoolTimer(0)

{
	m_pos = pos;
	m_handle = LoadGraph("data/Game/player.png");
	assert(m_handle >= 0);
	m_anim = Anim::Idle;
	ClearAttackRect();
	/*m_state = PlayerState::Normal;
	m_type = type;*/
	switch (type)
	{
	case PlayerType::Normal:
		ChangeNormal();
		break;
	case PlayerType::Burning:
		ChangeBurning();
		break;
	case PlayerType::Frozen:
		ChangeFrozen();
		break;
	case PlayerType::Archer:
		ChangeArcher();
		break;
	}
}
//ゲームオーバー時用
Player::Player(PlayerType type, Vec2 pos):
	m_frame(0),
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isNomove(false),
	arrowFrame(-1),
	isArrowAttack(false),
	isJumping(false),
	BurningPrevPos{ 0,0 },
	BurningAfterPos{ 0,0 },
	isBurningAttack(false),
	damageCount(0),
	m_angle(0.0f),
	m_wasGround(false),
	m_lastTapTime(0),
	m_lastTapDir(-1),
	m_isDash(false),
	isRotateOne(false),
	m_rotateFrame(0),
	damageTimer(0),
	attackCoolTimer(0)

{
	m_pos = pos;
	m_handle = LoadGraph("data/Game/player.png");
	assert(m_handle >= 0);
	m_anim = Anim::Dead;
	ClearAttackRect();
	/*m_state = PlayerState::Normal;
	m_type = type;*/
	switch (type)
	{
	case PlayerType::Normal:
		ChangeNormal();
		break;
	case PlayerType::Burning:
		ChangeBurning();
		break;
	case PlayerType::Frozen:
		ChangeFrozen();
		break;
	case PlayerType::Archer:
		ChangeArcher();
		break;
	}
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
void Player::GameOverUpdate()
{
	AnimFrameUpdate();
}

void Player::AnimChangeStandUp()
{
	m_anim = Anim::StandUp;
	m_animframe = 0;

}

void Player::GameOverStandUpUpdate(float baseY)
{
	AnimFrameUpdate();
	
	
	if(m_anim == Anim::StandUp)
	{
		if (charaIdx == 0 && m_isTriJump)
		{
			if (m_triJumpFrame <= 20.0f * 2)//20x2回分ジャンプしたら
			{
				m_triJumpFrame++;
				float t = fmod(m_triJumpFrame, 20.0f) / 20.0f;
				float tri = (t < 0.5f) ? t * 2.0f : (1.0f - t) * 2.0f;

				float offsetY = -tri * 40.0f;

				m_pos.y = baseY + offsetY;
			}
			else
			{
				//ジャンプ終了
				m_anim = Anim::Walk;
				m_animframe = 0;
				m_triJumpFrame = 0;
				m_pos.y = baseY;
				//横に移動させる
			}


		}
	}

	if (m_anim == Anim::Walk)
	{
		m_pos.x += 5.0f;
	}

}

void Player::Update(Input& input)
{

	m_frame++;
	attackCoolTimer--;
	coolTimer--;
	damageTimer--;
	//m_animframe++;
	AnimFrameUpdate();
	if (arrowFrame >= 0)
	{
		arrowFrame--;
	}


	Rect chipRect;
	/*if (input.IsPressed("ok"))
	{
		DamageHit(false);
	}*/

	//前フレームの矩形を保存
	m_prevColRect = m_colRect;

	//当たり判定更新
	Character::SetRect();

	AnimSelect(m_anim);

	InputUpdate(input);//特殊行動の入力検知

	//ダッシュ状態検知
	//入力した方向
	int dir = 0;
	if (input.IsTriggered("right"))dir = 1;
	if (input.IsTriggered("left"))dir = -1;

	int nowFrame = m_frame;// 毎フレーム +1 してるカウンタ
	//ダッシュ
	if (dir != 0 && m_isGround)
	{
		//前回と同じ方向を、一定時間内に2回押したらダッシュ
		if (m_lastTapDir == dir && (nowFrame - m_lastTapTime) < doubleTapThreshold)
		{
			m_isDash = true;
			//エフェクトを出す
			for (auto& func : onDashEvents)
			{
				if (func)func();//呼び出し
			}
		}


		//押した情報を記録
		//(もっといい保存がありそう)
		m_lastTapDir = dir;
		m_lastTapTime = nowFrame;

	}
	//ダッシュ状態の解除
	if (!input.IsPressed("right") && !input.IsPressed("left"))
	{
		m_isDash = false;
	}

	//前フレームの攻撃の当たり判定を消す
	ClearAttackRect();

	switch (m_state)
	{
	case PlayerState::Normal://アップデートの遷移
		NormalUpdate(input);
		break;
	case PlayerState::Attack://アップデートの遷移
		AttackUpdate();
		if (isNomove)
		{
			//減速処理(慣性)
			if (m_isGround)//地上
			{
				{
					if (m_vel.x > 0)//右に移動しているとき
					{
						m_vel.x += -decel / 1.5f;
						if (m_vel.x < 0)m_vel.x = 0;
					}
					else if (m_vel.x < 0)//左に移動しているとき
					{
						m_vel.x += decel / 1.5f;
						if (m_vel.x > 0)m_vel.x = 0;
					}
				}
			}
			else//空中
			{
				{
					if (m_vel.x > 0)//右に移動しているとき
					{
						m_vel.x += -decel / 4.0f;
						if (m_vel.x < 0)m_vel.x = 0;
					}
					else if (m_vel.x < 0)//左に移動しているとき
					{
						m_vel.x += decel / 4.0f;
						if (m_vel.x > 0)m_vel.x = 0;
					}
				}
			}
			
		
		
		}
		break;
	case PlayerState::Copy://アップデートの遷移
		CopyUpdate();
		break;
	case PlayerState::Damage:
		//ダメージを食らったときの関数を作る
		DamageUpdate();
		break;
	}

	if (!(m_type == PlayerType::Burning && m_state == PlayerState::Attack))
	{
		Character::Gravity();
	}


	m_wasGround = m_isGround;

	m_hitDir = CheckHitMapPlayer(chipRect);
	//着地effectを出す
	if (!m_wasGround && m_isGround)
	{
		//エフェクトを出す
		for (auto& func : onLandEvents)
		{
			if (func)func();//呼び出し
		}

	}


	//着地時にアニメーションを帰るところ
	if (m_isGround)
	{


		m_jumpFrame = 0;
		if (m_anim != Anim::Damage)m_vel.y = 0.0f;
		if (m_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}



	if (m_pos.x < 0)//画面外に出ないようにする
	{
		m_pos.x = 0;
	}
	if (m_pos.y < 0 + (ScreenHeight - topPos))//画面外に出ないようにする(本当は+drawOffsetCamera)
	{
		m_pos.y = 0 + (ScreenHeight - topPos);
	}

#ifdef _DEBUG

	if (CheckHitKey(KEY_INPUT_Y))
	{
		m_hp = 0;
		m_life = 0;
	}
#endif // _DEBUG


}

void Player::Draw()//使わない
{
	switch (m_type)
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
		DrawRectRotaGraph(m_pos.x, m_pos.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x, m_pos.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true, true);
	}
}

void Player::Draw(Camera& camera)
{

#ifdef _DEBUG
	//当たり判定の描画
	//当たり判定の描画(変身攻撃など)
	switch (m_state)
	{
	case PlayerState::Normal://アップデートの遷移

		break;
	case PlayerState::Attack://アップデートの遷移
		switch (m_type)
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
	m_colRect.DrawCamera(camera.drawOffset.x, camera.drawOffset.y, GetColor(0, 0, 255), false);





#endif
	switch (m_type)
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
	if (damageTimer > 0)
	{
		if (m_frame % 2 == 0)return;
	}


	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true, true);
	}

#ifdef _DEBUG
	DrawFormatString(1000, 10, GetColor(255, 0, 0), "_animは%dです", m_anim);
	DrawFormatString(1000, 20, GetColor(255, 0, 0), "_stateは%dです", m_state);
	DrawFormatString(1000, 30, GetColor(255, 0, 0), "_typeは%dです", m_type);
	DrawFormatString(1000, 50, GetColor(255, 0, 0), "coolTimerは%fです", coolTimer);
	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです", charaIdx);
	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです", charaIdy);
	DrawFormatString(10, 80, GetColor(255, 0, 0), "m_pos.xは%fです", m_pos.x);
	DrawFormatString(10, 100, GetColor(255, 0, 0), "m_pos.yは%fです", m_pos.y);
	DrawFormatString(10, 120, GetColor(255, 0, 0), "m_vel.xは%fです", m_vel.x);
	DrawFormatString(10, 140, GetColor(255, 0, 0), "m_vel.yは%fです", m_vel.y);
#endif
}

void Player::DamageHit(bool ans)
{
	if (damageTimer > 0)return;
	//バーニングの攻撃の初速の時は無敵(案1)
	{
		//if (m_state == PlayerState::Attack && burningTimer > 0)
	//{
	//	return;
	//}
	}

	//バーニングの攻撃しているときは無敵(案2)
	if (m_state == PlayerState::Attack && m_type == PlayerType::Burning)
	{
		return;
	}


	//被弾後
	damageTimer = kDamageInterval;

	m_isRight = ans;
	//動けないようにする
	isNomove = true;
	m_vel.y = 0;
	m_vel.y = -10.0f;
	m_state = PlayerState::Damage;
	AnimSelect(Anim::Damage);
	damageCount = kDamageFrame;
	//ダメージ分HPが減る
	m_hp -= 20;
}
void Player::HealGet(int point)
{
	m_hp += point;
	if (m_hp >= 100)
	{
		m_hp = 100;
	}
}

void Player::LifeDeg()
{
	m_life -= 1;
	if (m_life <= 0)
	{
		m_life = 0;
	}
}

void Player::Death()
{
	m_vel.y = -20;
	//アニメーションを初期に戻す
	{
		m_anim = Anim::Idle;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}

}

void Player::DyingUpdate()
{
	m_angle -= 10.0f;//回転角度
	m_vel.y += 0.8f;
	m_pos.y += m_vel.y;
}

void Player::DyingDraw(Camera& camera)
{

	if (m_isRight)
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true);
	}
	else
	{
		DrawRectRotaGraph(m_pos.x + camera.drawOffset.x, m_pos.y + camera.drawOffset.y + kCharaSize / 4,
		kPlayerCutW * charaIdx, kPlayerCutH * charaIdy,//切り取り左上
		kPlayerCutW, kPlayerCutH,//切り取りの幅
		kPlayerScale, m_angle * DX_PI / 180.0f, m_handle, true, true);
	}
}

void Player::RotateUpdate()
{
	m_rotateFrame++;
	if (m_rotateFrame >= 60)
	{
		if (!isRotateOne)isRotateOne = true;

	}

	if (isRotateOne)
	{
		m_angle += 15.0f;//回転角度
		if (m_angle >= 360)
		{
			m_angle = 0;//きれいに戻す
			isRotateOne = false;

			if (m_rotateNum < 1)//すぐ回転させるため
			{
				m_rotateNum++;
				m_rotateFrame = 45;
			}
			else if (m_rotateNum == 1)//最大回転数分したら0に戻す
			{
				m_rotateNum = 0;
				m_rotateFrame = 0;
			}
		}
	}
}

void Player::RotateFinishUpdate()
{
	m_angle = std::lerp(m_angle, 360.0f, 0.5);
}

void Player::AutoMoveStart()
{
	Character::Gravity();

	if (m_isGround)
	{
		if (m_anim != Anim::Jump)
		{
			AnimSelect(Anim::Walk);
		}
	}

	if (m_junpTimer++ > m_jumpTime)
	{
		if (m_isGround)
		{
			m_vel.y = -25.0f;
			m_isGround = false;
			AnimSelect(Anim::Jump);
		}
	}



	Rect chipRect;
	Character::CheckHitMapPlayer(chipRect);
}

void Player::AutoMove()
{
	//重力
	//m_vel.y += kGravity;
	//減衰(ほぼしない)
	//m_vel.x *= 0.999f;
	Rect chipRect;
	Character::CheckHitMapPlayer(chipRect);

	if (m_isGround)
	{
		if (m_anim == Anim::Jump)
		{
			AnimSelect(Anim::Walk);



		}
		if (m_vel.x <= 0.1f)
		{
			AnimSelect(Anim::Idle);
		}
	}
	if (m_anim == Anim::Walk)
	{
		//倍速
		AnimFrameUpdate();
		//エフェクトを出す
		if (m_animframe % 3 == 0)
			for (auto& func : onWalkEvents)
			{
				if (func)func();//呼び出し
			}


	}


}

void Player::InputUpdate(Input& input)
{
	if (isNomove)return;



	//特殊行動の入力検知
	if (input.IsTriggered("Attack"))
	{
		if (attackCoolTimer > 0)return;
		m_state = PlayerState::Attack;
		if (m_type == PlayerType::Burning)
		{
			m_vel.x = 0;//(慣性付きのバーニングは早すぎるため)
		}
		//m_vel.y = 0;

		//ジャンプ処理はすべて破棄
		{
			isJumping = false;
			m_jumpFrame = 0;
		}
		
		if (m_type == PlayerType::Archer)
		{
			if (arrowFrame >= 0)return;
			//矢を回す
			arrowFrame = arrowTime;

		}

		if (m_type == PlayerType::Burning)
		{
			if (coolTimer <= 0)
			{
				//バーニングの時はy軸の速度を0にする
				m_vel.y = 0;

				burningTimer = burningTime;
				coolTimer = 0;
			}
		}
	}
	if (input.IsTriggered("Copy"))
	{
		//ノーマル状態限定の技
		if (m_type != PlayerType::Normal)return;
		m_state = PlayerState::Copy;
	}


}

void Player::NormalUpdate(Input& input)
{
	Move(input);
	Jump(input);
	//Character::Gravity();


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


	if (burningTimer >= 0.0f)
	{
		if (m_type == PlayerType::Burning)//バーニングの攻撃処理
		{
			isBurningAttack = true;
			float dist = (m_isRight) ? kBurningSpeed : -kBurningSpeed;
			bool hit = MoveWithCollisionX(dist);//衝突判定付きで少しずつ移動(少しずつの間当たり判定)
			if (hit)
			{
				//衝突したので追加の処理
				//攻撃終了

				burningTimer = 0;

				m_state = PlayerState::Normal;
				attackCoolTimer = kAttackCool;
				//isNomove = false;
			}
			/*if (m_isRight)m_pos.x += kBurningSpeed;
			else m_pos.x -= kBurningSpeed;*/

		}
	}
	else
	{
		if (m_type == PlayerType::Burning)//減速時のbarningの攻撃処理
		{
			/*if (m_isRight)m_pos.x += kBurningSpeed / 3;
			else m_pos.x -= kBurningSpeed / 3;*/
			float dist = (m_isRight) ? kBurningSpeed / 3 : -kBurningSpeed / 3;
			bool hit = MoveWithCollisionX(dist);//衝突判定付きで少しずつ移動(少しずつの間当たり判定)
			if (hit)
			{
				//衝突したので追加の処理
				//攻撃終了

				burningTimer = 0;

				m_state = PlayerState::Normal;
				attackCoolTimer = kAttackCool;
				//isNomove = false;
				//ここの処理を変えたら
				//バーニングの終わり際が変わる
			}
		}
	}

	//バーニングだけ攻撃中は重力を受けない
	//微妙だったため一旦なし
	/*if (!(m_type == PlayerType::Burning))
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

void Player::DamageUpdate()
{
	damageCount--;
	m_vel.x = m_isRight ? -1 : 1;

	if (m_isGround)
	{
		m_vel.x = 0;
	}

	//Character::Gravity();
	
	//damageCountが0になったらstateをNormalに戻す、
	//damageを食らっているときは無敵判定
	if (damageCount <= 0)
	{
		isNomove = false;
		m_state = PlayerState::Normal;
		AnimSelect(Anim::Idle);

	}
}



void Player::ClearAttackRect()
{
	// 判定を消す
	m_attackRect.SetLT(0, 0, 0, 0);
	m_copyRect.SetLT(0, 0, 0, 0);
	m_burningRect.SetLT(0, 0, 0, 0);
	m_frozenRect.SetLT(0, 0, 0, 0);
	m_archerRect.SetLT(0, 0, 0, 0);
}

void Player::Move(Input& input)
{
	AnimSelect(m_anim);
	if (isNomove)
	{
		//減速処理(慣性)
		{
		if (m_vel.x > 0)//右に移動しているとき
		{
			m_vel.x += -decel;
			if (m_vel.x < 0)m_vel.x = 0;
		}
		else if (m_vel.x < 0)//左に移動しているとき
		{
			m_vel.x += decel;
			if (m_vel.x > 0)m_vel.x = 0;
		}
		}
		return;
	}
	//ここに処理を追加していく



	if (Pad::IsPress(PAD_INPUT_LEFT))
	{

		//そくどの方向が変わるのなら
		if (m_vel.x > 0 && m_isGround)
		{
			//エフェクトを出す
			for (auto& func : onWalkEvents)
			{
				if (func)func();//呼び出し
			}
		}
		m_vel.x += -accel;
		m_isRight = false;


	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{

		//m_vel.x = kSpeed;
		//そくどの方向が変わるのなら
		if (m_vel.x < 0 && m_isGround)
		{
			//エフェクトを出す
			for (auto& func : onWalkEvents)
			{
				if (func)func();//呼び出し
			}
		}
		m_vel.x += accel;
		m_isRight = true;

	}
	else
	{
		//減速処理(慣性)
		if (m_vel.x > 0)//右に移動しているとき
		{
			m_vel.x += -decel;
			if (m_vel.x < 0)m_vel.x = 0;
		}
		else if (m_vel.x < 0)//左に移動しているとき
		{
			m_vel.x += decel;
			if (m_vel.x > 0)m_vel.x = 0;
		}

	}






	//速度制限
	if (m_vel.x > kSpeed)m_vel.x = kSpeed;
	if (m_vel.x < -kSpeed)m_vel.x = -kSpeed;
	//---------------------------

	if (m_isDash)
	{
		//ダッシュのスピードに設定
		m_vel.x = m_isRight ? kSpeed * 1.5f : -kSpeed * 1.5f;
	}




	if (m_anim != Anim::Jump)
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

	// ジャンプが入力され、かつ接地しているならジャンプ中
	if (input.IsTriggered("Jump") && m_isGround)
	{
		isJumping = true;
		m_isGround = false;
		//AnimSelect(Anim::Jump);
	}

	if (input.IsPressed("Jump") && isJumping)
	{

		//AnimSelect(Anim::Jump);

		if (m_hitDir.top)
		{
			m_jumpFrame = kMaxJumpFrame;//天井に当たったらジャンプ力をもう加えない
		}
		//ジャンプが入力されており、かつジャンプ中ならジャンプ力を加える
		m_jumpFrame++;
		m_isGround = false;
		if (m_jumpFrame < kMaxJumpFrame)
		{
			m_vel.y = kJumpPower;

		}
		else
		{
			// ジャンプが入力されていないならジャンプ中を解除
			m_jumpFrame = 0;
			isJumping = false;

		}
	}
	else
	{
		isJumping = false;
		m_jumpFrame = 0;
	}


	if (isJumping)AnimSelect(Anim::Jump);



}

void Player::Attack()
{

	isNomove = true;

	//アニメーション
	AnimSelect(Anim::Attack);
	//判定をつける

	switch (m_type)
	{
	case PlayerType::Normal:
		//Normalの攻撃アニメーション              //右の判定：左の判定
		m_attackRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -120.0f), m_pos.y - kCharaSize, 110.0f, 80.0f);//100
		break;
	case PlayerType::Burning:                     //右の判定：左の判定
		m_burningRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -100.0f), m_pos.y - kCharaSize, 90.0f, 80.0f);//80
		break;
	case PlayerType::Frozen:                     //右の判定：左の判定
		m_frozenRect.SetLT(m_pos.x + (m_isRight ? 10.0f : -140.0f), m_pos.y - kCharaSize, 130.0f, 110.0f);//120
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
	m_copyRect.SetCenter(m_pos.x, m_pos.y - kCharaSize / 2, 120.0f, 100.0f);
}

void Player::AnimSelect(const Anim& anim)
{
	switch (m_type)
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
	//damageアニメーションは最優先で変わり、次点では攻撃アニメーション
	if (anim != Anim::Damage)
	{
		if (m_anim == Anim::Attack && charaIdx == kNormalAttackNum - 1)//攻撃アニメーション終了
		{

			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;
			attackCoolTimer = kAttackCool;

		}
		if (m_anim == Anim::Copy && charaIdx == kNormalCopyNum - 1)//コピーアニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}

		if (m_anim == Anim::Attack)return;
		if (m_anim == Anim::Copy)return;

	}

	if (m_anim != anim)
	{
		m_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectBurning(const Anim& anim)
{
	//damageアニメーションは最優先で変わり、次点では攻撃アニメーション
	if (anim != Anim::Damage)
	{
		if (m_anim == Anim::Attack && charaIdx == kBurningAttackNum - 1)//攻撃アニメーション終了
		{
			//if (m_type == PlayerType::Burning)//弱体化(いまはしていない)
			//{
			//	if (coolTimer < 0) coolTimer = coolTime;
			//}
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;
			attackCoolTimer = kAttackCool;
		}
		if (m_anim == Anim::Copy && charaIdx == kBurningCopyNum - 1)//コピーアニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}

		if (m_anim == Anim::Attack)return;
		if (m_anim == Anim::Copy)return;

	}


	if (m_anim != anim)
	{
		m_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectFrozen(const Anim& anim)
{
	//damageアニメーションは最優先で変わり、次点では攻撃アニメーション
	if (anim != Anim::Damage)
	{
		if (m_anim == Anim::Attack && charaIdx == kFrozenAttackNum - 1)//攻撃アニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;
			attackCoolTimer = kAttackCool;
		}
		if (m_anim == Anim::Copy && charaIdx == kFrozenCopyNum - 1)//コピーアニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}

		if (m_anim == Anim::Attack)return;
		if (m_anim == Anim::Copy)return;
	}



	if (m_anim != anim)
	{
		m_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::AnimSelectArcher(const Anim& anim)
{
	//damageアニメーションは最優先で変わり、次点では攻撃アニメーション
	if (anim != Anim::Damage)
	{
		if (m_anim == Anim::Attack && charaIdx == kArcherAttackNum - 1)//攻撃アニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;
			attackCoolTimer = kAttackCool;
		}
		if (m_anim == Anim::Copy && charaIdx == kArcherCopyNum - 1)//コピーアニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}

		if (m_anim == Anim::Attack)return;
		if (m_anim == Anim::Copy)return;
	}

	if (m_anim != anim)
	{
		m_anim = anim;
		m_animframe = 0;
		charaIdx = 0;
		charaIdy = 0;
	}
	return;
}

void Player::NormalAnim()
{
	switch (m_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / kNormalIdleDuration) % kNormalIdleNum;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / kNormalWalkDuration) % kNormalWalkNum;
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
			charaIdx = (m_animframe / kNormalJumpDuration) % kNormalJumpNum + 4;
			charaIdy = 3;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / kNormalAttackDuration) % kNormalAttackNum;
		charaIdy = 2;
		break;
	case Anim::Copy:
		if (charaIdx == kNormalCopyNum - 1)
		{
			charaIdx = kNormalCopyNum - 1;
			charaIdy = 4;
		}
		else
		{
			charaIdx = (m_animframe / kNormalCopyDuration) % kNormalCopyNum;//【注意】
			charaIdy = 4;
		}
		break;
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / kNormalDamageDuration) % kNormalDamageNum;
		charaIdy = 6;
		break;
	case Anim::Dead:
		//死んでいる絵
		charaIdx =3;
		charaIdy = 7;
		break;
	case Anim::StandUp:
		//死後立ち上がるとき
		charaIdx =(kNormalDeadNum-1) - (m_animframe / kNormalDeadDuration);
		if (charaIdx < 0)
		{
			m_isTriJump = true;
		}
		charaIdy = 7;
		charaIdx = (std::max)(charaIdx, 0);
		
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;
	}
}
void Player::BurningAnim()
{
	switch (m_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / kBurningIdleDuration) % kBurningIdleNum;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / kBurningIdleDuration) % kBurningWalkNum;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 2)
		{
			charaIdx = 2;
			charaIdy = 1;
		}
		else
		{
			charaIdx = (m_animframe / kBurningJumpDuration) % kBurningJumpNum + 0;
			charaIdy = 1;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / kBurningAttackDuration) % kBurningAttackNum;
		charaIdy = 5;
		break;
	case Anim::Copy:
		if (charaIdx == kBurningCopyNum - 1)
		{
			charaIdx = kBurningCopyNum - 1;
			charaIdy = 5;
		}
		else
		{
			charaIdx = (m_animframe / kBurningCopyDuration) % kBurningCopyNum;//【注意】切り替わった直後のアニメーションはここが流れてる
			charaIdy = 5;
		}
		break;
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / kBurningDamageDuration) % kBurningDamageNum;
		charaIdy = 6;
		break;
	case Anim::Dead:
		//死んでいる絵
		charaIdx = 3;
		charaIdy = 7;
		break;
	case Anim::StandUp:
		//死後立ち上がるとき
		charaIdx = (kBurningDeadNum - 1) - (m_animframe / kBurningDeadDuration);
		if (charaIdx < 0)
		{
			m_isTriJump = true;
		}
		charaIdy = 7;
		charaIdx = (std::max)(charaIdx, 0);
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
}

void Player::FrozenAnim()
{
	switch (m_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / kBrozenIdleDuration) % kFrozenIdleNum;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / kFrozenWalkDuration) % kFrozenWalkNum;
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
			charaIdx = (m_animframe / kFrozenJumpDuration) % kFrozenJumpNum + 0;
			charaIdy = 6;
		}
		break;
	case Anim::Attack:

		charaIdx = (m_animframe / kFrozenAttackDuration) % kFrozenAttackNum;
		charaIdy = 2;
		break;
	case Anim::Copy://【注意】切り替わった直後のアニメーションはここが流れてる
		if (charaIdx == kFrozenCopyNum - 1)
		{
			charaIdx = kFrozenCopyNum - 1;
			charaIdy = 4;
		}
		else
		{
			charaIdx = (m_animframe / kFrozenCopyDuration) % kFrozenCopyNum;
			charaIdy = 4;
		}

		break;
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / kFrozenDamageDuration) % kFrozenDamageNum;
		charaIdy = 8;
		break;
	case Anim::Dead:
		//死んでいる絵
		charaIdx = 3;
		charaIdy = 9;
		break;
	case Anim::StandUp:
		//死後立ち上がるとき
		charaIdx = (kFrozenDeadNum - 1) - (m_animframe / kFrozenDeadDuration);
		if (charaIdx < 0)
		{
			m_isTriJump = true;
		}
		charaIdy = 9;
		charaIdx = (std::max)(charaIdx, 0);
		break;
	default:
		// ここに来たら想定外！
		//assert(false && "Unknown animation type in switch(_anim)");
		break;

	}
}

void Player::ArcherAnim()
{
	switch (m_anim)
	{
	case  Anim::Idle:
		charaIdx = (m_animframe / kArcherIdleDuration) % kArcherIdleNum;
		charaIdy = 0;
		break;
	case Anim::Walk:
		charaIdx = (m_animframe / kArcherWalkDuration) % kArcherWalkNum;
		charaIdy = 1;
		break;
	case Anim::Jump:
		if (charaIdx == 2)
		{
			charaIdx = 2;
			charaIdy = 1;
		}
		else
		{
			charaIdx = (m_animframe / kArcherJumpDuration) % kArcherJumpNum + 0;
			charaIdy = 1;
		}
		break;
	case Anim::Attack:
		if (m_animframe == arrowAppearFrame) isArrowAttack = true;//chraIdxが9になった瞬間に矢を発射する//9 * 3;
		charaIdx = (m_animframe / kArcherAttackDuration) % kArcherAttackNum;
		charaIdy = 3;
		break;
	case Anim::Copy://【注意】切り替わった直後のアニメーションはここが流れてる
		if (charaIdx == kArcherCopyNum - 1)
		{
			charaIdx = kArcherCopyNum - 1;
			charaIdy = 3;
		}
		else
		{
			charaIdx = (m_animframe / kArcherCopyDuration) % kArcherCopyNum;
			charaIdy = 3;
		}
		break;
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / kArcherDamageDuration) % kArcherDamageNum;
		charaIdy = 4;
		break;
	case Anim::Dead:
		//死んでいる絵
		charaIdx = 3;
		charaIdy = 5;
		break;
	case Anim::StandUp:
		//死後立ち上がるとき
		charaIdx = (kArcherDeadNum - 1) - (m_animframe / kArcherDeadDuration);
		if (charaIdx < 0)
		{
			m_isTriJump = true;
		}
		charaIdy = 5;
		charaIdx = (std::max)(charaIdx, 0);
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
	//バーニングの前座標
	BurningPrevPos = m_pos;
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
			BurningAfterPos = m_pos;
			return true;
		}
		BurningAfterPos = m_pos;

	}


	//衝突なし
	return false;
}



void Player::ChangeNormal()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Game/player.png");
	assert(m_handle >= 0);

	m_type = PlayerType::Normal;
	m_state = PlayerState::Normal;
	m_animframe = 0;
}

void Player::ChangeBurning()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Game/Burning.png");
	assert(m_handle >= 0);

	m_type = PlayerType::Burning;
	m_state = PlayerState::Normal;
	m_animframe = 0;
}

void Player::ChangeFrozen()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Game/Frozen.png");
	assert(m_handle >= 0);

	m_type = PlayerType::Frozen;
	m_state = PlayerState::Normal;
	m_animframe = 0;
}

void Player::ChangeArcher()
{
	// 1. 現在の画像を解放
	DeleteGraph(m_handle);

	// 2. 新しい画像を読み込む
	m_handle = LoadGraph("data/Game/Archer.png");
	assert(m_handle >= 0);

	m_type = PlayerType::Archer;
	m_state = PlayerState::Normal;
	m_animframe = 0;
}

std::shared_ptr<Arrow> Player::ShotArrow()
{
	std::shared_ptr<Arrow> arrow;
	arrow = std::make_shared<Arrow>();

	arrow->ChangePos() = m_pos;

	arrow->m_playerdir = m_isRight;


	return arrow;

}
