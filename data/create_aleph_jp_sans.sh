#!/usr/bin/fontforge -script

# AlephSansMono-Boldを日本語に対応させたAlephJPSansMono-Boldを生成するプログラム
# by Logue

# 日本語部分は、NotoSansMonoCJKjp-Bold.otfからMacのShift_JIS使用領域のみサブセット化したフォントです。
# サブセット化対応文字は
# https://github.com/nakamura001/JIS_CharacterSet/blob/master/SHIFTJIS_custom/SHIFTJIS_custom_mac_utf8.txt
# より取得してください。

# 新しいフォント名
family = "AlephJPSansMono"


Open("NotoSansMonoCJKjp-Bold-subset.otf")

# NotoSansMonoCJKjpの高さをDejaVu Sans Monoに揃える
ScaleToEm(2048)

# 別ファイルとして保存
Generate("NotoSansMonoCJKjp-Bold-modified.ttf")

Close()


Open("AlephSansMono-Bold.ttf")

# AlephSansMono-Boldの横幅を縮小する処理
SelectWorthOutputting()
Scale(80,100)
SetWidth(1024)
SelectNone()

# 高さを揃えたIPAゴシックをマージ
MergeFonts("NotoSansMonoCJKjp-Bold-modified.ttf")

#フォント名等を指定して保存
SetFontNames(family, family, family, "Bold")
Generate(family + ".ttf")

Close()