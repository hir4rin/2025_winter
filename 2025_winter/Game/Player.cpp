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
#include "Player.h"

namespace
{
	const Vec2 kInitPos = { 100.0f,100.0f };//初期位置
	constexpr float kSpeed = 10.0f;//移動速度
	constexpr float kCharaSize = 64.0f;//キャラクターサイズ//当たり判定の幅高さに使われている
	constexpr int player_cut_w = 100;
	constexpr int player_cut_h = 100;
	constexpr float  player_scale = 3.0f;

	constexpr float kJumpPower = -15.0f;	// ジャンプ力
	constexpr int kMaxJumpFrame = 15;	// ジャンプを長押しできる最大時間

	constexpr float kGravity = 1.5f;//重力
	constexpr float kGround = 900.0f;//地面位置

	constexpr float kBurningSpeed = 30.0f;//バーニングのスピード

	constexpr float damageFrame = 40.0f;
	constexpr float burningTime = 9.0f;  //バーニングの移動時間


}



Player::Player(PlayerType type) :
	m_frame(0),
	charaIdx(0),
	charaIdy(0),
	m_animframe(0),
	isNomove(false),
	arrowFrame(-1),
	isArrowAttack(false),
	isJumping(false),
	BurningPrevPos{0,0},
	BurningAfterPos{0,0},
	isBurningAttack(false),
	damageCount(0)
	//m_pBg(nullptr)
{
	m_pos = kInitPos;
	m_handle = LoadGraph("data/Game/player.png");
	assert(m_handle >= 0);
	m_anim = Anim::Idle;
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
	/*if (input.IsPressed("ok"))
	{
		DamageHit(false);
	}*/



	//当たり判定更新
	Character::SetRect();

	AnimSelect(m_anim);

	InputUpdate(input);//特殊行動の入力検知

	switch (m_state)
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
	case PlayerState::Damage:
		//ダメージを食らったときの関数を作る
		DamageUpdate();
		break;
	}
	m_hitDir = CheckHitMapPlayer(chipRect);
	//着地時にアニメーションを帰るところ
	if (m_isGround)
	{


		m_jumpFrame = 0;
		if(m_anim != Anim::Damage)m_vel.y = 0.0f;
		if (m_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}

	if (m_pos.x < 0)//画面外に出ないようにする
	{
		m_pos.x = 0;
	}

}

void Player::Draw()//使わない
{

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
	DrawFormatString(1000, 10, GetColor(255, 0, 0), "_animは%dです", m_anim);
	DrawFormatString(1000, 20, GetColor(255, 0, 0), "_stateは%dです", m_state);
	DrawFormatString(1000, 30, GetColor(255, 0, 0), "_typeは%dです", m_type);
	DrawFormatString(10, 20, GetColor(255, 0, 0), "charaIdxは%dです", charaIdx);
	DrawFormatString(10, 30, GetColor(255, 0, 0), "charaIdyは%dです", charaIdy);
	DrawFormatString(10, 60, GetColor(255, 0, 0), "m_pos.yは%fです", m_pos.y);
#endif
}

void Player::DamageHit(bool ans)
{
	m_isRight = ans;
	//動けないようにする
	isNomove = true;
	m_vel.y = 0;
	m_vel.y = -10.0f;
	m_state = PlayerState::Damage;
	AnimSelect(Anim::Damage);
	damageCount = damageFrame;
}

void Player::InputUpdate(Input& input)
{
	if (isNomove)return;
	//特殊行動の入力検知
	if (input.IsTriggered("Attack"))
	{
		m_state = PlayerState::Attack;
		m_vel.x = 0;//攻撃中は動けないようにするため
		m_vel.y = 0;

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
				burningTimer = burningTime;
			
			}
		}
	}
	if (input.IsTriggered("Copy"))
	{
		m_state = PlayerState::Copy;
	}

	if (input.IsTriggered("CopyOut"))
	{
		if (!(m_type == PlayerType::Normal))
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
				//isNomove = false;
			}
		}
	}
	//着地時にアニメーションを帰るところ
	/*if (m_isGround)
	{
		m_pos.y = kGround;


		if (m_isJumpPreparing)return;
		m_jumpFrame = 0;
		m_vel.y = 0.0f;
		if (_anim == Anim::Jump)
		{
			AnimSelect(Anim::Idle);
		}
	}*/
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

void Player::DamageUpdate()
{
	damageCount--;
	m_vel.x = m_isRight ? -2 : 2;
	
	Character::Gravity();
	//damageCountが0になったらstateをNormalに戻す、
	//damageを食らっているときは無敵判定
	if (damageCount <= 0)
	{
		isNomove = false;
		m_state = PlayerState::Normal;
		AnimSelect(Anim::Idle);
		
	}
}

void Player::Move(Input& input)
{
	AnimSelect(m_anim);
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
		if (m_anim == Anim::Attack && charaIdx == 6)//攻撃アニメーション終了
		{
			if (m_type == PlayerType::Burning)
			{
				coolTimer = coolTime;
			}
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;


		}
		if (m_anim == Anim::Copy && charaIdx == 5)//コピーアニメーション終了
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
		if (m_anim == Anim::Attack && charaIdx == 7)//攻撃アニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}
		if (m_anim == Anim::Copy && charaIdx == 7)//コピーアニメーション終了
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
		if (m_anim == Anim::Attack && charaIdx == 14)//攻撃アニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}
		if (m_anim == Anim::Copy && charaIdx == 14)//コピーアニメーション終了
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
		if (m_anim == Anim::Attack && charaIdx == 11)//攻撃アニメーション終了
		{
			m_anim = Anim::Idle;
			isNomove = false;
			m_state = PlayerState::Normal;

		}
		if (m_anim == Anim::Copy && charaIdx == 8)//コピーアニメーション終了
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
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / 5) % 4;
		charaIdy = 6;
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
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / 5) % 4;
		charaIdy = 6;
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
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / 5) % 4;
		charaIdy = 8;
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
	case Anim::Damage:
		//ダメージを食らったときのアニメーション
		charaIdx = (m_animframe / 5) % 4;
		charaIdy = 4;
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
}

std::shared_ptr<Arrow> Player::ShotArrow()
{
	std::shared_ptr<Arrow> arrow;
	arrow = std::make_shared<Arrow>();

	arrow->ChangePos() = m_pos;

	arrow->m_playerdir = m_isRight;


	return arrow;

}
