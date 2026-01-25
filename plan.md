# AlephOne 日本語対応版 上流マージ計画

## 概要

このプロジェクトは2020年ごろから開発が止まっている日本語対応版AlephOneを、上流リポジトリ（Aleph-One-Marathon/alephone）の最新版とマージする計画です。約5年分の上流の改善を取り込みながら、日本語対応機能を維持します。

**現在のバージョン:** Aleph One JP 1.4 (2021-04-08)  
**上流リポジトリ:** https://github.com/Aleph-One-Marathon/alephone

---

## 日本語対応の主要な変更点

### 1. 文字エンコーディング対応

**実装ファイル:**

- `Source_Files/Misc/converter.h`
- `Source_Files/Misc/converter.cpp`

**機能:**

- Shift_JIS ⇔ UTF-8 変換
- UTF-8 ⇔ UTF-16 変換
- Boost.Locale を使用したエンコーディング処理
- iconvによるフォールバック処理（MacRoman対応）
- 2バイト文字（日本語）の検出関数

**関連する変更:**

- `Source_Files/shell.cpp` - `setlocale(LC_ALL, "")`を追加

### 2. 日本語フォント対応

**フォントディレクトリ:**

- `data/Japanese_Font/` - 日本語対応フォント一式

**使用フォント:**
| 用途 | オリジナル | 日本語版 |
|------|-----------|---------|
| HUD | Monaco/ProFontAO | DotGothic16 |
| ターミナル | Courier Prime | 源ノ等幅 (Source Han Mono) |
| ダイアログ | AlephSansMono-Bold | AlephJPSansMono-Bold |

**フォントヘッダー:**

- `Source_Files/RenderOther/jp-fonts/` 配下に7つのフォントヘッダー
  - `DotGothic16-Regular.h`
  - `SourceHanMono-Regular.h`
  - `AlephJPSansMono-Medium.h` / `Bold.h` / `RegularIt.h` / `BoldIt.h`
  - `AlephJPSerifMono-Bold.h`

**フォント読み込み:**

- `Source_Files/RenderOther/sdl_fonts.cpp` - 日本語フォントをインクルードし、フォントマッピングを定義

**文字コード範囲:**

- Shift_JISの範囲に限定（機種依存文字の一部も含む）
- サブセット化により各フォント約1.6MB程度に最適化

### 3. UIの日本語化

**翻訳済みファイル:**

- `Source_Files/Misc/DefaultStringSets.cpp` - エラーメッセージ、システムメッセージを日本語化
- `Source_Files/Misc/interface.cpp` - About画面に日本語化タブとクレジット追加
- `Source_Files/shell.cpp` - 日本語化スタッフのクレジット表示
- `data/default_theme/resources/Themes.mml` - テーマ名を日本語化
- `data/default_theme/Plugin.xml` - フォント設定

**日本語化スタッフ:**

- Logue Takayama
- Pfhore (ookawa_mi)
- marathon
- HogePiyo (EviHoge)
- saiten

### 4. ビルドシステムの変更

**configure.ac:**

- プロジェクト名を "Aleph One JP" に変更
- Boost.Locale を必須依存に追加
- iconv を必須依存に追加

**Makefile.am:**

- `Source_Files/Makefile.am` - `Japanese_Font`配下のフォントファイルをdistに追加

**Xcode プロジェクト:**

- `PBProjects/AlephOne.xcodeproj` - `converter.h/cpp` を追加、`boost_locale.framework` をリンク
- ローカライゼーションに "Japanese" を追加

**Windows Visual Studio:**

- `Source_Files/AlephOne.vcxproj.filters` にconverter関連ファイルの参照

---

## コンフリクトが予想される主要ファイル

### 高リスク（大規模な変更）

1. **`Source_Files/RenderOther/sdl_fonts.cpp`**
   - フォントシステム全体が変更されている
   - 日本語フォントヘッダーのインクルード
   - フォントマッピング処理の追加

2. **`Source_Files/Misc/DefaultStringSets.cpp`**
   - 全文が日本語化

3. **`Source_Files/Misc/interface.cpp`**
   - About画面の構造変更
   - 日本語化タブの追加

4. **`Source_Files/shell.cpp`**
   - クレジット表示とロケール設定
   - `converter.h`のインクルード

5. **`configure.ac`**
   - プロジェクト名と依存関係の変更

### 中リスク（部分的な変更）

6. **`Source_Files/Makefile.am`**
   - 配布ファイルリスト

7. **`data/default_theme/`配下のテーマファイル**
   - `Plugin.xml`
   - `resources/Themes.mml`

8. **Xcodeプロジェクト設定**
   - `PBProjects/AlephOne.xcodeproj/project.pbxproj`

---

## マージ戦略

### 推奨アプローチ: 機能別ブランチ戦略

```
上流最新版
    ↓
[Phase 1] エンコーディング層の追加
    - converter.h/cpp の追加
    - Boost.Locale依存の追加（条件付き）
    ↓
[Phase 2] 日本語フォント対応
    - jp-fonts/ヘッダー追加
    - sdl_fonts.cpp の拡張（条件付きコンパイル）
    - Japanese_Font/ディレクトリ追加
    ↓
[Phase 3] UI日本語化（オプション機能として）
    - DefaultStringSets.cpp
    - interface.cpp
    - shell.cpp
    - テーマファイル
    ↓
[Phase 4] 統合とテスト
```

### Phase 1: 基盤準備（低コンフリクト）

1. converter層を独立したモジュールとして追加
2. ビルドシステムにBoost.Locale依存を条件付きで追加
3. `#ifdef ENABLE_JAPANESE`でコンパイル時に切り替え可能に

**具体的なタスク:**

- [ ] 上流リポジトリをremoteに追加: `git remote add upstream https://github.com/Aleph-One-Marathon/alephone.git`
- [ ] 最新版をfetch: `git fetch upstream`
- [ ] 差分を確認: `git diff upstream/master`
- [ ] `converter.h/cpp`に条件付きコンパイルのガードを追加
- [ ] `configure.ac`に`--enable-japanese`オプションを追加

### Phase 2: フォント対応（中コンフリクト）

1. jp-fontsディレクトリとヘッダーファイルを追加
2. sdl_fonts.cppの拡張を条件付きコンパイルで実装
3. Japanese_Fontディレクトリを追加

**具体的なタスク:**

- [ ] `sdl_fonts.cpp`の日本語フォント関連コードを`#ifdef ENABLE_JAPANESE`で保護
- [ ] `Makefile.am`でJapanese_Fontを条件付きで配布リストに追加
- [ ] Xcodeプロジェクトに条件付きコンパイル設定を追加

### Phase 3: UI日本語化（高コンフリクト）

1. DefaultStringSetsを言語別に分離（将来的な拡張性のため）
2. interface.cppの日本語化タブをローカライゼーション機能として実装
3. テーマの日本語化を条件付きで適用

**具体的なタスク:**

- [ ] `DefaultStringSets.cpp`の日本語文字列を条件分岐で処理
- [ ] `interface.cpp`のAbout画面を条件付きで日本語対応
- [ ] `shell.cpp`のクレジット表示を条件付きに
- [ ] テーマファイルの日本語化を別ファイルとして分離（オプション）

### Phase 4: 上流マージと統合

1. 作業ブランチで上流masterをマージ
2. コンフリクトを段階的に解決
3. 全プラットフォームでビルドテスト
4. 動作確認

**具体的なタスク:**

- [ ] マージブランチを作成: `git checkout -b merge-upstream-2026`
- [ ] 上流をマージ: `git merge upstream/master`
- [ ] コンフリクト解決（ファイルごとに慎重に）
- [ ] macOSでビルドテスト
- [ ] Linuxでビルドテスト（可能であれば）
- [ ] Windowsでビルドテスト（可能であれば）
- [ ] 日本語版として動作確認
- [ ] 英語版（`--disable-japanese`）として動作確認

---

## ビルド時の切り替え

### 日本語版ビルド

```bash
./autogen.sh
./configure --enable-japanese
make
```

### 英語版ビルド（デフォルト）

```bash
./autogen.sh
./configure
make
```

---

## コード内の条件分岐例

```cpp
#ifdef ENABLE_JAPANESE
    #include "converter.h"
    #include "jp-fonts/DotGothic16-Regular.h"
    #include "jp-fonts/SourceHanMono-Regular.h"
    // 日本語フォントの処理
#endif
```

---

## 技術的な注意点

### 1. Converter層の汎用化

- Shift_JIS専用ではなく、他の言語にも対応可能な設計に
- エンコーディング設定を外部から指定可能に

### 2. フォント埋め込みの条件化

- ビルド時に埋め込み or 外部ファイルを選択可能に
- メモリ使用量への配慮

### 3. UI翻訳の外部化

- 将来的にはハードコードではなく、リソースファイルベースに
- `.po/.mo`形式への移行を検討

### 4. テストの整備

- 日本語環境でのリグレッションテスト
- 文字化けの確認
- フォント表示の確認

### 5. ドキュメント更新

- 日本語対応のビルド方法を英語でドキュメント化
- README更新

---

## 長期的な統合パス

### 理想的な最終形態

- 上流リポジトリに日本語対応がマージされる
- ローカライゼーション機能として統合
- 他言語対応への道を開く

### 現実的なアプローチ

1. まず日本語版を独立したforkとして維持
2. 上流の変更を定期的にマージ
3. 日本語対応部分を上流にPR形式で提案
4. 段階的に統合を進める

---

## タイムライン（想定）

| フェーズ | 作業内容                             | 想定期間   |
| -------- | ------------------------------------ | ---------- |
| Phase 1  | 基盤準備とエンコーディング層の条件化 | 1-2日      |
| Phase 2  | フォント対応の条件化                 | 1-2日      |
| Phase 3  | UI日本語化の条件化                   | 2-3日      |
| Phase 4  | 上流マージとコンフリクト解決         | 3-5日      |
| テスト   | マルチプラットフォームでの動作確認   | 2-3日      |
| **合計** |                                      | **9-15日** |

---

## 次のステップ

1. ✅ 調査完了 - 日本語対応の変更点と上流との差分を特定
2. ⬜ Phase 1の実装開始
3. ⬜ 段階的なマージとテスト
4. ⬜ ドキュメント更新
5. ⬜ 上流へのコントリビューション検討

---

## 参考情報

- **上流リポジトリ:** https://github.com/Aleph-One-Marathon/alephone
- **日本語版配布:** https://mjolnir.logue.be/
- **プロジェクト管理:** https://github.com/logue/alephone

---

_最終更新: 2026年1月25日_
