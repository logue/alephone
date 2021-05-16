#!/usr/bin/fontforge -script

# 新しいフォント名
family = "AlephJPSansMono"


Open("mplus-2m-bold.otf")

# IPAゴシックの高さをDejaVu Sans Monoに揃える
ScaleToEm(2048)

# 別ファイルとして保存
Generate("mplus-2m-bold-scaled.ttf")

Close()


Open("../AlephSansMono-Bold.ttf")

# DejaVu Sans Monoの横幅を縮小する処理
SelectWorthOutputting()
Scale(75,100)
SetWidth(1024)
SelectNone()

# 高さを揃えたIPAゴシックをマージ
MergeFonts("mplus-2m-bold-scaled.ttf")

#フォント名等を指定して保存
SetFontNames(family, "AlephJP Sans Mono", family, "Bold")
Generate("AlephJPSansMono-Bold.ttf")

Close()