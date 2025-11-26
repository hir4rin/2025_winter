#include "Input.h"
#include "Dxlib.h"

Input::Input() :inputData_{}, lastInputData_{}, inputTable_{}
{
    //イベント名を添え字にして、右辺値に実際の入力種別と入力コードの配列をおく
    inputTable_["ok"] = { { PeripheralType::keyboard,KEY_INPUT_RETURN },
                          { PeripheralType::pad1,PAD_INPUT_L } };//SELECTキー

    inputTable_["Attack"] = { { PeripheralType::keyboard,KEY_INPUT_X },
                          { PeripheralType::pad1,PAD_INPUT_A } };//PADのAボタン

    inputTable_["Copy"] = { { PeripheralType::keyboard,KEY_INPUT_C },
                          { PeripheralType::pad1,PAD_INPUT_X } };//PADのAボタン
    inputTable_["CopyOut"] = { { PeripheralType::keyboard,KEY_INPUT_V },
                          { PeripheralType::pad1,PAD_INPUT_6 } };//PADのSボタン
    inputTable_["Jump"] = { { PeripheralType::keyboard,KEY_INPUT_Z },
                          { PeripheralType::pad1,PAD_INPUT_1 } };//PADの?ボタン

    



    inputTable_["up"] = { { PeripheralType::keyboard,KEY_INPUT_UP },
                          { PeripheralType::pad1,PAD_INPUT_UP} };
    inputTable_["down"] = { { PeripheralType::keyboard,KEY_INPUT_DOWN },
                            { PeripheralType::pad1,PAD_INPUT_DOWN} };
    inputTable_["left"] = { { PeripheralType::keyboard,KEY_INPUT_LEFT },
                            { PeripheralType::pad1,PAD_INPUT_LEFT} };
    inputTable_["right"] = { { PeripheralType::keyboard,KEY_INPUT_RIGHT },
                             { PeripheralType::pad1,PAD_INPUT_RIGHT} };

    // あらかじめ枠を開けておく
    //ここで枠を開けておかないと、チェックの際にAt関数でクラッシュする可能性がある
    for (const auto& inputInfo : inputTable_) {
        inputData_[inputInfo.first] = false;
        lastInputData_[inputInfo.first] = false;
    }
}

void Input::Update()
{
    // まず現在の入力情報を取得
    char keyState[256];
    GetHitKeyStateAll(keyState);//生のキーボード情報
    int padState = GetJoypadInputState(DX_INPUT_PAD1);//生のPAD1情報
    lastInputData_ = inputData_;//直前のフレームを更新(前のフレーム情報をコピー)

    // すべての入力イベントをチェックします
    //ここでInputData_が更新される
    //inputTable_を回して各イベントをチェックする
    for (const auto&inputInfo : inputTable_)
    {
        auto& input=inputData_[inputInfo.first];//inputInfo.firstには"ok"等が入っている
        //inputを書き換えると、inputData_のそのイベントが押されているかどうかを
        //書き換えることになる
        //InputStateのベクタを回す
        for (const auto& state : inputInfo.second){
            //このループはInputState配列のループなので
            //まず、入力種別をチェックします
            switch (state.type)
            {
            case PeripheralType::keyboard://キーボードだったら
                //GethitKeyStateAllでとってきた配列の中身を見て
                //入力されているかどうかをチェックします
                input = keyState[state.id];
                break;
            case PeripheralType::pad1:
                //GethitKeyStateAllでとってきたビット情報を見て
                //登録されているビット情報と&をとり、そのビットが立っているかどうかをチェック
                input = (padState & state.id);
                break;
            }
            if (input) {//必須!
                break;//ここでbreakしないと、最後のチェックで押されていないとfalseになる
            }
        }
    }

}

bool Input::IsPressed(const char* name) const
{
    //もし「ない」イベントを送られるとクラッシュします
    //クラッシュがいやだったら
    //if (inputData_.contains(name))
    //{
    //    return false;//これで回避できます
    //}
    return inputData_.at(name);//const関数内部なので[]ではなくatを使用している
}

bool Input::IsTriggered(const char* name) const
{
    return inputData_.at(name) && !lastInputData_.at(name);
}
