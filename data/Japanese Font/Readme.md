# AlephOne JP用フォントについて

AlephOne日本語版ではターミナルテキストなどゲーム内で使用するフォントは、以下のように日本語対応のフォントに差し替えて実装しています。

|ID |オリジナル版       |英語版                     |日本語版               |備考
|---|-------------------|---------------------------|-----------------------|------------------------------------------
|4  |Monaco             |ProFontAO                  |k8x12L                 |HUD向けフォント
|22 |Courier            |Courier Prime              |NotoSansMonoCJK        |汎用フォント（ターミナルテキストなどで使用）
|   |Courier(Bold)      |Courier Prime Bold         |NotoSansMonoBoldCJK    |汎用ボールドフォント
|   |Courier(Italic)    |Courier Prime Italic       |
|   |Courier(BoldItalic)|Courier Prime BoldItalic   |
|   |mono               |AlephSansMono-Bold         |AlephJPSansMono-Bold   |ダイアログ表示用フォント

AlephOne JPで使用できる文字は、Shift JISに含まれる文字のみです。このため、配布されているフォントをそのまま使用するのではなく、
[サブセットフォントメーカー](https://opentype.jp/subsetfontmk.htm)などで使用しない文字を削除して`bin2h`などのツールでttfをヘッダーファイル化します。これにより、フォントの容量が書体あたり1.6M程度になります。

これらのフォントの定義は、以下のソースで定義されています。

* SourceFiles/RenderOther/FontHandler.cpp - 122行目付近。Mac版Marathon2のFontリソースの定義へのTrueTypeフォントのマッピング
* SourceFiles/RenderOther/screen_drawing.cpp - 144行目付近。Mac版のMarathon2で使われていたFontリソースの定義
* SourceFiles/RenderOther/sdl_fonts.cpp - 100行目付近。ヘッダーファイル化したTrueTypeフォントのデータの読み込み

英語版では、HUDのテキストはボールドになっていますが、日本語版では文字のスペースの都合上、ボールドは解除しています。

また、NotoSansMonoBoldCJKのボールドフォントとボールドイタリックフォントはないため、SDL_Fontの機能でこれらを実装しています。

## AlephJPSansMono-Boldについて

これは、AlephSansMono-BoldにM+フォントの日本語部分を合成したフォントです。`build_AlephJPSansMono-Bold.sh`を実行すると合成処理が行われます。
なお、この状態では生成されたチェックボックス（☐と☑）が横に縮んでしまうため、fontforgeであらためてチェックボックスのグリフをAlephSansMono-Boldからコピペする必要があります。

## Shift_JISの文字コード

AlephOneJPで使用できる文字は、原則的にはShift_JISの範囲のみです。
そのままフォントを入れると余計なグリフも含まれてしまうため、ヘッダーファイル化する前にサブセット化を実行してください。

実際使用できる文字は、[Shift_JIS Character code.txt]にまとめてあります。

※純粋なShift_JISではなく、☐と☑のようなシステムで使用する文字と、①や㍻のような機種依存ではない拡張文字も含まれます。

このテキストファイルを[サブセットフォントメーカー](https://opentype.jp/subsetfontmk.htm)に読み込ませて必要な書体のみを抽出して使用します。

## ヘッダーファイル化

```
bin2h.exe -cz [変数名] <[対象ファイル名]> [出力先ファイル名].h
bin2h.exe -cz SourceHanMonoRegular <SourceHanMono-Regular-subset.otf> SourceHanMono-Regular.h
```

## リンク

* フォント
  * [M+ Font](https://mplus-fonts.osdn.jp/)
  * [NotoCJK](https://github.com/googlefonts/noto-cjk)
  * [k8x12](http://littlelimit.net/k8x12.htm)
* ツール
  * [FontForge](https://fontforge.sf.net/)
  * [サブセットフォントメーカー](https://opentype.jp/subsetfontmk.htm)
