/*  DefaultStringSets.cpp - support for compiled-in text strings

	Copyright (C) 2002 and beyond by the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

	Default text StringSets, in case no MML provides them.
	by Woody Zenfell, III
	June 3, 2002

    June 3, 2002: captured text_strings.mml into C++ form for this file.
*/

#include    "cseries.h"
#include    "DefaultStringSets.h"
#include    "TextStrings.h"


static inline void BuildStringSet(short inStringSetID,
                                  const char** inStrings,
                                  short inNumStrings) {
	for (int i = 0; i < inNumStrings; i++)
		TS_PutCString(inStringSetID, i, inStrings[i]);
}

#define NUMBER_OF_STRINGS(sa)   (sizeof(sa) / sizeof((sa)[0]))

#define BUILD_STRINGSET(id, strs)   BuildStringSet(id, strs, NUMBER_OF_STRINGS(strs))



// StringSets from original Bungie resources
// -----------------------------------------------------------------------------------------
// <!-- STR# Resource: "Errors" -->
static const char* sStringSetNumber128[] = {
    "$appName$には、68040以上のプロセッサが必要です。",
    "$appName$には、Color QuickDraw が必要です。",
    "$appName$には、漢字Talk 7.1 以降のシステムが必要です。",
    "$appName$には、3000k 以上の空きRAMが必要です。",
    "$appName$には、13インチ以上(640x480)そして256色(またはグレー)が表示できるモニタが必要です。",
    "Map, Shapes, Images, Sounds のファイルが$appName$と同じフォルダに入っているか確認してください。",
    "$appName$は、サウンドを初期化できませんでした。",
    "$appName$の実行中にファイルシステムエラーが発生しました。ディスク上の空要領とロックされていないか確認してください。",
    "この$appName$のコピーはウイルスなどによって変更されています。オリジナルのディスクから再度インストールしてください。",
    "This beta copy of $appName$ has expired and is no longer functional. Call Bungie at (312) 563-6200 ext. 21 for more information.",
    "そのキーは既にボリューム調整に使われています。",
    "そのキーは既に地図のズームに使われています。",
    "そのキーは既にアイテムのスクロールに使われています。",
    "そのキーは既にゲームの機能に使われています。",
    "$appName$は、この地図をロードするためのメモリが足りません。メモリ量を増やしたり、解像度を落としたりサウンドのビットレートを落としたり止めたりしてもう一度実行してください。",
    "$appName$は、これから別の物理設定ファイルをロードします。そのため性能が落ちたり、クラッシュしたり、保存ゲームファイルが悪化したり、ネットワークゲームが不安定になるおそれがあります。ご注意ください！",
    "$appName$は Bungie製以外の地図を使っています。そのため性能が落ちたり、クラッシュしたり、保存ゲームファイルが悪化するおそれがあります。ご注意ください！",
    "地図または保存ゲームファイルの読み込み時に致命的なエラーが発生しました。",
    "地図または保存ゲームファイルの読み込み時にシステムエラーが発生しました。",
    "ゲームファイルの保存中にエラーが発生しました。（ハードディスクが一杯か、ロックされていませんか？）",
    "シリアル番号が無効です。もう一度入力してください。",
    "ネットワークゲームの際に同じシリアル番号を複数のマシンでご使用いただくことはできません。新たに製品をご購入いただく必要があります。詳しくは日本国内の販売代理店まで連絡してください。",
    "この$appName$のコピーはネットワーク専用のシリアル番号で登録されています。ネットワーク専用のシリアル番号では一人用のプレーはできません。",
    "読み込もうとしている地図がこわれているぞ。バックアップコピーから再度インストールしよう。",
    "チェックポイント %d は見つかりませんでした！",
    "画像 %dは見つかりませんでした！",
    "このバージョンの$appName$ はプレビュー版につき、ネットワークゲームをサポートしていません。Bungie社からは、まもなくネットワーク対応版が出ます。（この他多くのクールな特徴を持っています）",
    "プレーヤーを集めたプレーヤーが、勝手にみんなをおいてゲームを終了してしまった。張本人は処刑した方がよいかも…。",
    "すまん。$appName$はネットワークゲームからの終了に失敗してしまった。その結果、このレベルでの君の努力は水の泡となってしまった。",
    "この保存されたゲームのシナリオファイルが見つかりません。レベルを替えるとき、デフォルトの地図に戻ります。",
    "$appName$は、この映画を記録した地図を見つけることができませんでした。このため映画を再生することができません。",
    "$appName$をネットワーク上で遊ぶには 6000k 以上の空きRAMが必要です。$appName$の割当メモリ量を増やしてもう一度実行してください。",
    "スクリプト同士が競合を起こしているようです。MMLとネットスクリプトで誰をLuaでコントロールするか異なっていると思われます。予期しない動きや同期乱れが起こるかもしれません。",
    "This replay was created with a newer version of $appName$ and cannot be played with this version. Upgrade $appName$ and try again.",
    "すまん。スクロールホイールは、武器の切り替えで使用されている。",
};

// STR# Resource: "Filenames"
static const char* sStringSetNumber129[] = {
    "Shapes.shpA",
    "Shapes.16",
    "Sounds.sndA",
    "Sounds.16",
    "$appName$ Preferences",
    "Map.sceA",
    "Untitled Game",
    "Marathon",
    "$appName$ Recording",
    "Physics Model",
    "Music.ogg",
    "Images.imgA",
    "Movie",
    "Default",
    "Marathon.appl",
};

// STR# Resource: "Top-Level Interface Items"
static const char* sStringSetNumber130[] = {
	"新規ゲーム",
	"既存のゲームを開く",
    "",
	"最後のゲームを再生",
	"フィルムを開く",
	"フィルムの保存",
    "",
	"ネットゲーム集合",
	"ネットゲーム参加",
    "",
	"設定",
	"デモ開始",
	"終了",
};

// STR# Resource: "Prompts"
static const char* sStringSetNumber131[] = {
    "ゲーム保存",
    "映画の保存",
    "映画の選択：",
    "デフォルト",
};

// STR# Resource: "Network Errors"
static const char* sStringSetNumber132[] = {
	"そのプレーヤーはネットワーク上で見つかりません。ゲーム参加のダイアログからキャンセルしたかもしれません。",

	"いくつかのプレーヤーがネットワーク上で見つからないため、"
	"ゲームが中断されました。",

	"ホストをしているコンピュータからの応答がありません。"
	"ファイヤーウォールが適切に設定されているか、アドレスが間違っていないかを確認して下さい。",

	"プレーヤーを集めていたプレーヤーが、勝手にみんなをおいてゲームを終了してしまった。（張本人を処罰した方がよいかも…。）",

	"マップを全員に送ることが出来なかったため、ゲームがキャンセルされました。",

	"呼び出し側のコンピュータが地図を送らなかったため、ゲームがキャンセルされました。他のコンピュータの接続が切れたかもしれません。",
	"AlephOneは、ゲームを開始できません。他のコンピュータの接続が切れたかもしれません。",
	"ゲームの参加中にエラーが発生しました（アプリケーションのバージョンが違うかもしれません）。もう一度試してみてください。",
	"ネットワークエラーのためAlephOneは続行できません。",

	"バージョンが異なるためそのプレーヤーはゲームに参加できませんでした。",

	"このプレイヤーは、現在募集をかけているゲームのいくつかの拡張機能が使用できない古いバージョンのAlephOneを使用しているようです。このため、このゲームを開始することができません。",
	"現在募集をかけているゲームに、拡張機能に対応していないAlephOneを使っているプレイヤーが参加しようとしました。",
	"AlephOneは、このレベルのマップファイルを見つけることができませんでした。いくつかのターミナルが正常に表示されないかもしれません。また、このコンピューター上でゲームを保存することは推奨できません。",
	"募集をかけていた人がいなくなりました。",

	"募集をかけている人を見つけることができません。アドレスが正しく入力されているかを確認ください。",

	"サーバーからマップを送るときにエラーが発生しました。ゲーム終了します。",

	"募集をかけている人は、スタープロトコルを使用していますが、あなたの設定はリングになっています。このため、対戦可能な他のプレイヤーのリストに表示されません。",

	"募集をかけている人は、リングプロトコルを使用していますが、あなたの設定はスターになっています。このため、対戦可能な他のプレイヤーのリストに表示されません。",

	"募集をかけている人はLuaのネットスクリプトを使用していますが、このバージョンは、ビルド時にLuaが有効になっていません。このため、対戦可能な他のプレイヤーのリストに表示されません。",
	
	"インターネット上のゲームを募集しているサーバーへの接続に問題が発生しました。お手数ですが、もう一度やり直してください。",
	
	"あなたが募集しようとしているゲームは、インターネット上に通知することができませんでした。他の方法で、あなたのIPアドレスを知らせるか、もう一度やり直してください。",
	
	"Aleph Oneは、ファイヤーウォールやルーターの設定を変更することができませんでした。プレイヤーを募集することができません。"
	
};

// STR# Resource: "Key Codes To Names"
static const char* sStringSetNumber133[] = {
    "A",
    "S",
    "D",
    "F",
    "H",
    "G",
    "Z",
    "X",
    "C",
    "V",
    "0x0A",
    "B",
    "Q",
    "W",
    "E",
    "R",
    "Y",
    "T",
    "1",
    "2",
    "3",
    "4",
    "6",
    "5",
    "=",
    "9",
    "7",
    "-",
    "8",
    "0",
    "]",
    "O",
    "U",
    "[",
    "I",
    "P",
    "リターン",
    "L",
    "J",
    "'",
    "K",
    ";",
    "\\",
    ",",
    "/",
    "N",
    "M",
    ".",
    "Tab",
    "スペース",
    "`",
    "Delete",
    "0x34",
    "Escape",
    "0x35",
    "Command",
    "Shift",
    "Caps Lock",
    "Option",
    "Control",
    "0x3c",
    "0x3d",
    "0x3e",
    "0x3f",
    "0x40",
    "テンキーの.",
    "0x42",
    "テンキーの*",
    "0x44",
    "テンキーの+",
    "0x46",
    "Clear",
    "0x48",
    "0x49",
    "0x4a",
    "テンキーの/",
    "Enter",
    "0x4d",
    "テンキーの-",
    "0x4f",
    "0x50",
    "テンキーの=",
    "テンキーの0",
    "テンキーの1",
    "テンキーの2",
    "テンキーの3",
    "テンキーの4",
    "テンキーの5",
    "テンキーの6",
    "テンキーの7",
    "0x5a",
    "テンキーの8",
    "テンキーの9",
    "0x5d",
    "0x5e",
    "0x5f",
    "F5",
    "F6",
    "F7",
    "F3",
    "F8",
    "F9",
    "0x66",
    "F11",
    "0x68",
    "F13",
    "0x6a",
    "F14",
    "0x6c",
    "F10",
    "0x6e",
    "F12",
    "0x70",
    "F15",
    "Help",
    "Home",
    "Page Up",
    "Forw. Del.",
    "F4",
    "End",
    "F2",
    "Page Down",
    "F1",
    "←",
    "→",
    "↓",
    "↑",
    "Power",
};

// STR# Resource: "Preferences Advice"
static const char* sStringSetNumber134[] = {
    "外部スピーカー又はヘッドフォンが接続され、サウンドのコントロールパネルでステレオ出力がオンになっていることを確認してください。",
    "Cybermaxx ヘルメットがシリアルポートに接続され、オンになっていることを確認してください。",
    "機能拡張フォルダに QuickTime Musical Instruments ファイルが入っていることを確認してくさい。入っていないとBGMはすごくバカらしく聞こえるぞ…。",
};

// STR# Resource: "Computer Interface"
static const char* sStringSetNumber135[] = {
    "U.E.S.C. Marathon",
    "β.4.5-23 と接続開始中…",
    "CAS.qterm//CyberAcme Systems Inc.",
    "<931.461.60231.14.vt920>",
    "UESCTerm 802.11（リモートオーバーライド）",
    "PgUp/PgDown/矢印キーでスクロール",
    "Return/Enterキーで了解",
    "接続切断…",
    "接続終了",
    "%H%M %m.%d.%Y",
};

// STR# Resource: "Join Dialog Messages"
static const char* sStringSetNumber136[] = {
    "「参加」をクリックすると$appName$のネットワークゲームに参加できます。",
    "サーバーからネットワークゲームに呼び出されるのを待っています。「キャンセル」をクリックすると中断します。",
    "ネットワークゲームに参加しました。サーバーは他のプレーヤーを集めています…。",
};

// STR# Resource: "Weapon Names"
static const char* sStringSetNumber137[] = {
    "拳骨",
    ".44 マグナム・メガ級 A1",
    "ゼウス級核融合ピストル",
    "MA-75B 機関銃/グレネード砲",
    "SPNKR-X18型ミサイルランチャー",
    "TOZT-7 火炎放射機",
    "武器類不明 システムエラー 0xfded",
    "WSTE-M 戦闘用ショットガン",
    "(somehow related to time of applicability)",
    "KKV-7 10MMフルチエット小型軽機関銃",
};

// STR# Resource: "file search path"
static const char* sStringSetNumber138[] = {
    "Marathon Trilogy:Marathon Infinity \xc4:Marathon Infinity:",
};

// STR# Resource: "Preferences Groupings"
static const char* sStringSetNumber139[] = {
    "グラフィック",
    "プレイヤー",
    "サウンド",
    "コントロール",
    "環境",
};

// STR# Resource: "Postgame network game stats"
static const char* sStringSetNumber140[] = {
    "%d つの旗",
    "%d:%02d",
    "%d ポイント",
    "チーム",
    "ボールの保持時間",
    "旗の捕獲",
    "Time 'It'",
    "ゴール",
    "陣地にいた時間",
    "陣地にいた時間",
    "ポイント",
    "時間",
};

// STR# Resource: "Net Game Setup"
static const char* sStringSetNumber141[] = {
    "殺傷制限",
    "殺傷",
    "捕獲制限",
    "つの旗",
    "点数制限",
    "点",
    "陣地にいた時間：",
    "分",
    "ポイント",
    "時間",
};

// STR# Resource: "New Join Dialog Messages"
static const char* sStringSetNumber142[] = {
    "君は「%s」プレーに受け入れられました。サーバーが残りのプレーヤーを追加するまで少々おまちください…。",
    "個人対戦",
    "君は「共同対戦」プレーに受け入れられました。サーバーが残りのプレーヤーを追加するまで少々おまちください…。",
    "旗取り",
    "陣地取り",
    "ボールの奪い合い",
    "防衛戦",
    "ラクビー",
    "タグ",
    "君は「カスタム」プレーに受け入れられました。サーバーが残りのプレーヤーを追加するまで少々おまちください…。",
};

// STR# Resource: "Progress strings for networking"
static const char* sStringSetNumber143[] = {
    "リモートのプレーヤーへ地図を転送しています。",
    "リモートのプレーヤーへ地図を転送しています。",
    "サーバーから地図を受け取っています。",
    "サーバーから地図が送られてくるのを待っています。",
    "環境情報を他のプレーヤーへ転送しています。",
    "環境情報を他のプレーヤーへ転送しています。",
    "サーバーから環境情報を受け取っています。",
    "読み込んでいます…。",
    "ルーターのポートを開いています…。",
    "ルーターのポートを閉じています…。",
    "更新をチェックしています…。"
};


// Stringsets for SDL w_select widgets.
// -----------------------------------------------------------------------------------------
#include    "network_dialogs.h"  // for stringset ID's

static const char*    sDifficultyLevelsStrings[] = {
	"ちびっ子",
	"簡単",
	"普通",
	"難しい",
	"虐殺"
};

static const char*    sNetworkGameTypesStrings[] = {
	"個人対戦",
	"チームプレイ",
	"旗取り",
	"陣地取り",
	"鬼ごっこ",
	"ラグビー",
	"タグ",
	"ネットスクリプト"
};

static const char*    sEndConditionTypeStrings[] = {
//    "No Limit (Alt+Q to quit)",
	"無制限",
	"時間制限",
	"スコア上限"
};

static const char*	sSingleOrNetworkStrings[] = {
	"ソロでゲーム",
	"ネットワークでゲーム"
};

// More Mac OS string-resource stringsets.
// -----------------------------------------------------------------------------------------
// STR# Resource: "Item names"
static const char* sStringSetNumber150[] = {
    "拳骨",
    ".44 マグナム・メガ級",
    ".44 マグナム・メガ級",
    ".44 弾丸 (x8)",
    ".44 弾丸 (x8)",
    "ゼウス級核融合ピストル",
    "核融合バッテリ",
    "核融合バッテリ",
    "MA-75B 機関銃",
    "MA-75B 弾丸 (x52)",
    "MA-75B 弾丸 (x52)",
    "MA-75B グレネード (x7)",
    "MA-75B グレネード (x7)",
    "SPNKR-X18 SSM ミサイル発射機",
    "SSM ミサイル (x2)",
    "SSM ミサイル (x2)",
    "エイリアンの武器",
    "ショットガン 弾丸 (x2)",
    "ショットガン 弾丸 (x2)",
    "TOZT-7 背負い火炎放射機",
    "ナパーム散弾",
    "ナパーム散弾",
    "POWERPC 620 チップ",
    "POWERPC 620 チップ",
    "エイリアンのエネルギー変換機",
    "波動砲",
    "設計図",
    "WSTE-M 戦闘用ショットガン",
    "WSTE-M 戦闘用ショットガン",
    "スフトカードキー",
    "スフトカードキー",
    "アップリンクチップ",
    "アップリンクチップ",
    "ライアンの空色のボール",
    "頭蓋骨",
    "紫色のボール",
    "黄色のボール",
    "茶色のボール",
    "橙色のボール",
    "青のボール",
    "緑のボール",
    "KKV-7 10MM 小型軽機関銃",
    "10MM フレチェット弾倉",
    "10MM フレチェット弾倉",
};

// STR# Resource: "Item types"
static const char* sStringSetNumber151[] = {
    "武器",
    "弾丸",
    "パワーアップ",
    "アイテム",
    "武器 パワーアップ",
    "ボール",
    "ネットワーク状況",
};

// STR# Resource: "Net Statistics Strings"
static const char* sStringSetNumber153[] = {
    "殺傷：%d",
    "死亡：%d",
    "自爆：%d",
    "虐殺合計",
    "敵",
    "殺傷の合計： %d （１分当たり %.2f人）",
    "死亡の合計： %d （１分当たり %.2f人）",
    " - 自爆を含む",
    "チーム虐殺の合計",
    " - 味方による殺傷 %d人を含む",
    "成績",
    "チームの成績",
    // ZZZ: added for team vs. team carnage in postgame report
    "%sのチーム",
    // ZZZ: added for legend/key in SDL postgame report
    "殺傷",
    "死亡",
    "自爆",
    "味方による殺傷",
};

// STR# Resource: "OpenGL-Option Color-Picker Prompts"
static const char* sStringSetNumber200[] = {
    "何もない空間の色は？",
    "日中の地面の色は？",
    "日中の空の色は？",
    "夜間の地面の色は？",
    "夜間の空の色は？",
    "月の地面の色は？",
    "月の空の色は？",
    "宇宙空間の地面の色は？",
    "宇宙空間の空の色は？",
    "フォッグの色は？",
};

#include "player.h" // for kTeamColorsStringSetID

static const char* sTeamColorNamesStrings[] = {
    "灰",
    "赤",
    "紫",
    "黄",
    "白",
    "橙",
    "青",
    "緑",
};


void InitDefaultStringSets() {
	BUILD_STRINGSET(128, sStringSetNumber128);
	BUILD_STRINGSET(129, sStringSetNumber129);
	BUILD_STRINGSET(130, sStringSetNumber130);
	BUILD_STRINGSET(131, sStringSetNumber131);
	BUILD_STRINGSET(132, sStringSetNumber132);
	BUILD_STRINGSET(133, sStringSetNumber133);
	BUILD_STRINGSET(134, sStringSetNumber134);
	BUILD_STRINGSET(135, sStringSetNumber135);
	BUILD_STRINGSET(136, sStringSetNumber136);
	BUILD_STRINGSET(137, sStringSetNumber137);
	BUILD_STRINGSET(138, sStringSetNumber138);
	BUILD_STRINGSET(139, sStringSetNumber139);
	BUILD_STRINGSET(140, sStringSetNumber140);
	BUILD_STRINGSET(141, sStringSetNumber141);
	BUILD_STRINGSET(142, sStringSetNumber142);
	BUILD_STRINGSET(143, sStringSetNumber143);
	BUILD_STRINGSET(kDifficultyLevelsStringSetID,   sDifficultyLevelsStrings);
	BUILD_STRINGSET(kNetworkGameTypesStringSetID,   sNetworkGameTypesStrings);
	BUILD_STRINGSET(kEndConditionTypeStringSetID,   sEndConditionTypeStrings);
	BUILD_STRINGSET(kSingleOrNetworkStringSetID,	sSingleOrNetworkStrings);
	BUILD_STRINGSET(150, sStringSetNumber150);
	BUILD_STRINGSET(151, sStringSetNumber151);
	BUILD_STRINGSET(153, sStringSetNumber153);
	BUILD_STRINGSET(200, sStringSetNumber200);
	BUILD_STRINGSET(kTeamColorsStringSetID, sTeamColorNamesStrings);
}
