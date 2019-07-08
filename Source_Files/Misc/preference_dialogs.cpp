/*

	Copyright (C) 2006 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
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
	
*/

#include "preference_dialogs.h"
#include "preferences.h"
#include "binders.h"
#include "OGL_Setup.h"
#include "screen.h"

#include <sstream>

class TexQualityPref : public Bindable<int>
{
public:
	TexQualityPref (int16& pref, int16 normal) : m_pref (pref), m_normal (normal) {}
	
	virtual int bind_export ()
	{
		int result = 0;
		int temp = m_pref;
		while (temp >= m_normal) {
			temp = temp >> 1;
			++result;
		}
		return result;
	}
	
	virtual void bind_import (int value)
	{
		m_pref = (value == 0) ? 0 : m_normal << (value - 1);
	}
	
protected:
	int16& m_pref;
	int16 m_normal;
};

class ColourPref : public Bindable<RGBColor>
{
public:
	ColourPref (RGBColor& pref) : m_pref (pref) {}
	
	virtual RGBColor bind_export () { return m_pref; }
	virtual void bind_import (RGBColor value) { m_pref = value; }
	
protected:
	RGBColor& m_pref;
};

class FarFilterPref : public Bindable<int>
{
public:
	FarFilterPref (int16& pref) : m_pref(pref) { }

	int bind_export() {
		if (m_pref == 5)
		{
			return 3;
		} 
		else if (m_pref == 3)
		{
			return 2;
		}
		else
		{
			return m_pref;
		}
	}

	void bind_import(int value) {
		if (value == 2)
		{
			m_pref = 3;
		}
		else if (value == 3)
		{
			m_pref = 5;
		}
		else
		{
			m_pref = value;
		}
	}

protected:
	int16& m_pref;
};

class TimesTwoPref : public Bindable<int>
{
public:
	TimesTwoPref (int16& pref) : m_pref (pref) {}
	
	virtual int bind_export ()
	{
		return (m_pref / 2);
	}
	
	virtual void bind_import (int value)
	{
		m_pref = value * 2;
	}

protected:
	int16& m_pref;
};

class AnisotropyPref : public Bindable<int>
{
public:
	AnisotropyPref (float& pref) : m_pref (pref) {}
	
	virtual int bind_export ()
	{
		int result = 0;
		int temp = static_cast<int> (m_pref);
		while (temp >= 1) {
			temp = temp >> 1;
			++result;
		}
		return result;
	}
	
	virtual void bind_import (int value)
	{
		m_pref = (value == 0) ? 0.0 : 1 << (value - 1);
	}

protected:
	float& m_pref;
};

OpenGLDialog::OpenGLDialog() {  }

OpenGLDialog::~OpenGLDialog()
{
	delete m_cancelWidget;
	delete m_okWidget;
	delete m_fogWidget;
	delete m_colourEffectsWidget;
	delete m_transparentLiquidsWidget;
	delete m_3DmodelsWidget;
	delete m_blurWidget;
	delete m_bumpWidget;
	delete m_colourTheVoidWidget;
	delete m_voidColourWidget;
	delete m_fsaaWidget;
	delete m_anisotropicWidget;
	delete m_sRGBWidget;
	delete m_geForceFixWidget;
	delete m_useNPOTWidget;
	delete m_vsyncWidget;
	delete m_wallsFilterWidget;
	delete m_spritesFilterWidget;

	for (int i=0; i<OGL_NUMBER_OF_TEXTURE_TYPES; ++i) {
		delete m_textureQualityWidget [i];
		delete m_nearFiltersWidget[i];
		delete m_textureResolutionWidget [i];
		delete m_textureDepthWidget[i];
	}

}

void OpenGLDialog::OpenGLPrefsByRunning ()
{
	m_cancelWidget->set_callback (boost::bind (&OpenGLDialog::Stop, this, false));
	m_okWidget->set_callback (boost::bind (&OpenGLDialog::Stop, this, true));
	
	BinderSet binders;
	
	BitPref fogPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_Fog);
	binders.insert<bool> (m_fogWidget, &fogPref);
	BitPref colourEffectsPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_Fader);
	binders.insert<bool> (m_colourEffectsWidget, &colourEffectsPref);
	BitPref transparentLiquidsPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_LiqSeeThru);
	binders.insert<bool> (m_transparentLiquidsWidget, &transparentLiquidsPref);
	BitPref modelsPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_3D_Models);
	binders.insert<bool> (m_3DmodelsWidget, &modelsPref);
	BitPref blurPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_Blur);
	binders.insert<bool> (m_blurWidget, &blurPref);
	BitPref bumpPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_BumpMap);
	binders.insert<bool> (m_bumpWidget, &bumpPref);
	BitPref perspectivePref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_MimicSW, true);
	binders.insert<bool> (m_perspectiveWidget, &perspectivePref);
	
	BitPref colourTheVoidPref (graphics_preferences->OGL_Configure.Flags, OGL_Flag_VoidColor);
	binders.insert<bool> (m_colourTheVoidWidget, &colourTheVoidPref);
	ColourPref voidColourPref (graphics_preferences->OGL_Configure.VoidColor);
	binders.insert<RGBColor> (m_voidColourWidget, &voidColourPref);
	
	TimesTwoPref fsaaPref (graphics_preferences->OGL_Configure.Multisamples);
	binders.insert<int> (m_fsaaWidget, &fsaaPref);
	
	AnisotropyPref anisotropyPref (graphics_preferences->OGL_Configure.AnisotropyLevel);
	binders.insert<int> (m_anisotropicWidget, &anisotropyPref);

	BoolPref sRGBPref (graphics_preferences->OGL_Configure.Use_sRGB);
	binders.insert<bool> (m_sRGBWidget, &sRGBPref);
	
	BoolPref geForceFixPref (graphics_preferences->OGL_Configure.GeForceFix);
	binders.insert<bool> (m_geForceFixWidget, &geForceFixPref);
	
	BoolPref useNPOTPref (graphics_preferences->OGL_Configure.Use_NPOT);
	binders.insert<bool> (m_useNPOTWidget, &useNPOTPref);
	
	BoolPref vsyncPref (graphics_preferences->OGL_Configure.WaitForVSync);
	binders.insert<bool> (m_vsyncWidget, &vsyncPref);
	
	FarFilterPref wallsFarFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList [OGL_Txtr_Wall].FarFilter);
	binders.insert<int> (m_wallsFilterWidget, &wallsFarFilterPref);
	FarFilterPref spritesFarFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList [OGL_Txtr_Inhabitant].FarFilter);
	binders.insert<int> (m_spritesFilterWidget, &spritesFarFilterPref);

	Int16Pref wallsNearFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList[OGL_Txtr_Wall].NearFilter);
	binders.insert<int> (m_nearFiltersWidget[0], &wallsNearFilterPref);
	Int16Pref landscapeNearFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList[OGL_Txtr_Landscape].NearFilter);
	binders.insert<int> (m_nearFiltersWidget[1], &landscapeNearFilterPref);
	Int16Pref spriteNearFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList[OGL_Txtr_Inhabitant].NearFilter);
	binders.insert<int> (m_nearFiltersWidget[2], &spriteNearFilterPref);
	Int16Pref weaponNearFilterPref (graphics_preferences->OGL_Configure.TxtrConfigList[OGL_Txtr_WeaponsInHand].NearFilter);
	binders.insert<int> (m_nearFiltersWidget[3], &weaponNearFilterPref);
	
	TexQualityPref wallQualityPref (graphics_preferences->OGL_Configure.TxtrConfigList [0].MaxSize, 128);
	binders.insert<int> (m_textureQualityWidget [0], &wallQualityPref);
	TexQualityPref landscapeQualityPref (graphics_preferences->OGL_Configure.TxtrConfigList [1].MaxSize, 256);
	binders.insert<int> (m_textureQualityWidget [1], &landscapeQualityPref);
	TexQualityPref spriteQualityPref (graphics_preferences->OGL_Configure.TxtrConfigList [2].MaxSize, 256);
	binders.insert<int> (m_textureQualityWidget [2], &spriteQualityPref);
	TexQualityPref weaponQualityPref (graphics_preferences->OGL_Configure.TxtrConfigList [3].MaxSize, 256);
	binders.insert<int> (m_textureQualityWidget [3], &weaponQualityPref);
	TexQualityPref modelQualityPref (graphics_preferences->OGL_Configure.ModelConfig.MaxSize, 256);
	binders.insert<int> (m_modelQualityWidget, &modelQualityPref);
	
	Int16Pref wallResoPref (graphics_preferences->OGL_Configure.TxtrConfigList [0].Resolution);
	binders.insert<int> (m_textureResolutionWidget [0], &wallResoPref);
	Int16Pref landscapeResoPref (graphics_preferences->OGL_Configure.TxtrConfigList [1].Resolution);
	binders.insert<int> (m_textureResolutionWidget [1], &landscapeResoPref);
	Int16Pref spriteResoPref (graphics_preferences->OGL_Configure.TxtrConfigList [2].Resolution);
	binders.insert<int> (m_textureResolutionWidget [2], &spriteResoPref);
	Int16Pref weaponResoPref (graphics_preferences->OGL_Configure.TxtrConfigList [3].Resolution);
	binders.insert<int> (m_textureResolutionWidget [3], &weaponResoPref);

	Int16Pref wallDepthPref(graphics_preferences->OGL_Configure.TxtrConfigList[0].ColorFormat);
	binders.insert<int> (m_textureDepthWidget[0], &wallDepthPref);
	Int16Pref landscapeDepthPref(graphics_preferences->OGL_Configure.TxtrConfigList[1].ColorFormat);
	binders.insert<int> (m_textureDepthWidget[1], &landscapeDepthPref);
	Int16Pref spriteDepthPref(graphics_preferences->OGL_Configure.TxtrConfigList[2].ColorFormat);
	binders.insert<int> (m_textureDepthWidget[2], &spriteDepthPref);
	Int16Pref weaponDepthPref(graphics_preferences->OGL_Configure.TxtrConfigList[3].ColorFormat);
	binders.insert<int> (m_textureDepthWidget[3], &weaponDepthPref);
	
	
	// Set initial values from prefs
	binders.migrate_all_second_to_first ();
	
	bool result = Run ();
	
	if (result) {
		// migrate prefs and save
		binders.migrate_all_first_to_second ();
		write_preferences ();
	}
}

static const char *far_filter_labels[5] = {
	"なし", "リニア", "バイリニア", "トリニア", NULL
};

static const char *near_filter_labels[3] = {
	"なし", "リニア", NULL
};

class w_aniso_slider : public w_slider {
public:
	w_aniso_slider(int num_items, int sel) : w_slider(num_items, sel) {
		init_formatted_value();
	}
	
	virtual std::string formatted_value(void) {
		std::ostringstream ss;
		ss << ((selection == 0) ? 0 : 1 << (selection - 1));
		return ss.str();
	}
};


class SdlOpenGLDialog : public OpenGLDialog
{
public:
	SdlOpenGLDialog (int theSelectedRenderer)
	{

		vertical_placer *placer = new vertical_placer;
		placer->dual_add(new w_title(_SJIS("OpenGLのオプション")), m_dialog);
		placer->add(new w_spacer(), true);
		
		// horizontal_placer *tabs_placer = new horizontal_placer;
		// w_button *w_general_tab = new w_button("GENERAL");
		// w_general_tab->set_callback(choose_generic_tab, static_cast<void *>(this));
		// tabs_placer->dual_add(w_general_tab, m_dialog);
		// w_button *w_advanced_tab = new w_button("ADVANCED");
		// w_advanced_tab->set_callback(choose_advanced_tab, static_cast<void *>(this));
		// tabs_placer->dual_add(w_advanced_tab, m_dialog);
		// placer->add(tabs_placer, true);

		m_tabs = new tab_placer();

		std::vector<std::string> labels;
		labels.push_back(_SJIS("一般"));
		labels.push_back(_SJIS("拡張"));
		w_tab *tabs = new w_tab(labels, m_tabs);
		placer->dual_add(tabs, m_dialog);
		
		placer->add(new w_spacer(), true);

		table_placer *general_table = new table_placer(2, get_theme_space(ITEM_WIDGET), true);
		general_table->col_flags(0, placeable::kAlignRight);
		
		w_toggle *fog_w = new w_toggle(false);
		general_table->dual_add(fog_w->label(_SJIS("フォグ")), m_dialog);
		general_table->dual_add(fog_w, m_dialog);

		w_toggle *fader_w = new w_toggle(false);
		general_table->dual_add(fader_w->label(_SJIS("カラーエフェクト")), m_dialog);
		general_table->dual_add(fader_w, m_dialog);

		w_toggle *liq_w = new w_toggle(false);
		general_table->dual_add(liq_w->label(_SJIS("液面を半透明化")), m_dialog);
		general_table->dual_add(liq_w, m_dialog);

		w_toggle *models_w = new w_toggle(false);
		general_table->dual_add(models_w->label(_SJIS("3Dモデル")), m_dialog);
		general_table->dual_add(models_w, m_dialog);

		w_toggle *perspective_w = new w_toggle(false);
		general_table->dual_add(perspective_w->label(_SJIS("3D 遠近法")), m_dialog);
		general_table->dual_add(perspective_w, m_dialog);
		
		w_toggle *blur_w = new w_toggle(false);
		general_table->dual_add(blur_w->label(_SJIS("ブルームエフェクト")), m_dialog);
		general_table->dual_add(blur_w, m_dialog);
		
		w_toggle *bump_w = new w_toggle(false);
		general_table->dual_add(bump_w->label(_SJIS("バンプマッピング")), m_dialog);
		general_table->dual_add(bump_w, m_dialog);
		
		general_table->add_row(new w_spacer(), true);

		w_toggle *vsync_w = new w_toggle(false);
		general_table->dual_add(vsync_w->label(_SJIS("垂直同期")), m_dialog);
		general_table->dual_add(vsync_w, m_dialog);

		w_select_popup *fsaa_w = new w_select_popup ();
		general_table->dual_add(fsaa_w->label(_SJIS("フルシーンアンチエイリアス")), m_dialog);
		general_table->dual_add(fsaa_w, m_dialog);
		vector<string> fsaa_strings;
		fsaa_strings.push_back (_SJIS("なし"));
		fsaa_strings.push_back (_SJIS("２倍"));
		fsaa_strings.push_back (_SJIS("４倍"));
		fsaa_w->set_labels (fsaa_strings);
		
		w_aniso_slider* aniso_w = new w_aniso_slider(6, 1);
		general_table->dual_add(aniso_w->label(_SJIS("異方性フィルタリング")),m_dialog);
		general_table->dual_add(aniso_w, m_dialog);

		w_toggle *srgb_w = new w_toggle(false);
//		general_table->dual_add(srgb_w->label(_SJIS("ガンマ補正ブレンディング")), m_dialog);
//		general_table->dual_add(srgb_w, m_dialog);


		general_table->add_row(new w_spacer(), true);

		general_table->dual_add_row(new w_static_text(_SJIS("外部テスクチャのクオリティ")), m_dialog);
	
		w_select_popup *texture_quality_wa[OGL_NUMBER_OF_TEXTURE_TYPES];
		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; i++) texture_quality_wa[i] = NULL;
		
		texture_quality_wa[OGL_Txtr_Wall] =  new w_select_popup ();
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Wall]->label(_SJIS("壁")), m_dialog);
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Wall], m_dialog);
		
		texture_quality_wa[OGL_Txtr_Landscape] = new w_select_popup ();
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Landscape]->label(_SJIS("背景")), m_dialog);
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Landscape], m_dialog);

		texture_quality_wa[OGL_Txtr_Inhabitant] = new w_select_popup ();
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Inhabitant]->label(_SJIS("スプライト")), m_dialog);
		general_table->dual_add(texture_quality_wa[OGL_Txtr_Inhabitant], m_dialog);

		texture_quality_wa[OGL_Txtr_WeaponsInHand] = new w_select_popup ();
		general_table->dual_add(texture_quality_wa[OGL_Txtr_WeaponsInHand]->label(_SJIS("手中の武器")), m_dialog);
		general_table->dual_add(texture_quality_wa[OGL_Txtr_WeaponsInHand], m_dialog);

		w_select_popup *model_quality_w = new w_select_popup();
		general_table->dual_add(model_quality_w->label(_SJIS("3Dモデルのスキン")), m_dialog);
		general_table->dual_add(model_quality_w, m_dialog);
	
		vector<string> tex_quality_strings;
		tex_quality_strings.push_back (_SJIS("無制限"));
		tex_quality_strings.push_back (_SJIS("通常"));
		tex_quality_strings.push_back (_SJIS("高い"));
		tex_quality_strings.push_back (_SJIS("高品位"));
		tex_quality_strings.push_back (_SJIS("最高品位"));
	
		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; i++) {
			if (texture_quality_wa[i]) {
				texture_quality_wa[i]->set_labels (tex_quality_strings);
			}
		}
		model_quality_w->set_labels(tex_quality_strings);

		vertical_placer *advanced_placer = new vertical_placer;

		table_placer *advanced_table = new table_placer(2, get_theme_space(ITEM_WIDGET), true);
		advanced_table->col_flags(0, placeable::kAlignRight);
	
		w_toggle *geforce_fix_w = new w_toggle(false);
		advanced_table->dual_add(geforce_fix_w->label(_SJIS("GeForce 1～4のテスクチャの問題を解決")), m_dialog);
		advanced_table->dual_add(geforce_fix_w, m_dialog);
		
		w_toggle *use_npot_w = new w_toggle(false);
		advanced_table->dual_add(use_npot_w->label(_SJIS("非２乗テスクチャ")), m_dialog);
		advanced_table->dual_add(use_npot_w, m_dialog);
		advanced_table->dual_add_row(new w_static_text(_SJIS("非２乗テスクチャは、メモリの使用量を抑制します。")), m_dialog);
		advanced_table->dual_add_row(new w_static_text(_SJIS("ただし、マシンによっては問題が発生することがあります。")), m_dialog);

		advanced_table->add_row(new w_spacer(), true);
		advanced_table->dual_add_row(new w_static_text(_SJIS("テスクチャのフィルタリング")), m_dialog);
		advanced_placer->add(advanced_table, true);

		w_select* near_filter_wa[OGL_NUMBER_OF_TEXTURE_TYPES];
		w_select* far_filter_wa[OGL_NUMBER_OF_TEXTURE_TYPES];
		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; ++i)
		{
			near_filter_wa[i] = new w_select(0, near_filter_labels);
			if (i == OGL_Txtr_Wall || i == OGL_Txtr_Inhabitant)
				far_filter_wa[i] = new w_select(0, far_filter_labels);
			else
				far_filter_wa[i] = NULL;
		}
		
		w_label* near_filter_labels[OGL_NUMBER_OF_TEXTURE_TYPES];
		near_filter_labels[OGL_Txtr_Wall] = new w_label(_SJIS("壁"));
		near_filter_labels[OGL_Txtr_Inhabitant] = new w_label(_SJIS("スプライト"));
		near_filter_labels[OGL_Txtr_Landscape] = new w_label(_SJIS("背景"));
		near_filter_labels[OGL_Txtr_WeaponsInHand] = new w_label(_SJIS("手中の武器／HUD"));
	
		table_placer *ftable = new table_placer(3, get_theme_space(ITEM_WIDGET));
		
		ftable->col_flags(0, placeable::kAlignRight);
		ftable->col_flags(1, placeable::kAlignLeft);
		ftable->col_flags(2, placeable::kAlignLeft);
		
		ftable->add(new w_spacer(), true);
		ftable->dual_add(new w_label(_SJIS("近距離")), m_dialog);
		ftable->dual_add(new w_label(_SJIS("遠距離")), m_dialog);
		
		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; ++i)
		{
			ftable->dual_add(near_filter_labels[i], m_dialog);
			ftable->dual_add(near_filter_wa[i], m_dialog);
			near_filter_wa[i]->associate_label(near_filter_labels[i]);

			if (far_filter_wa[i])
			{
				ftable->dual_add(far_filter_wa[i], m_dialog);
				far_filter_wa[i]->associate_label(near_filter_labels[i]);
			}
			else
				ftable->add(new w_spacer(), true);
		}
		
		ftable->col_min_width(1, (ftable->col_width(0) - get_theme_space(ITEM_WIDGET)) / 2);
		ftable->col_min_width(2, (ftable->col_width(0) - get_theme_space(ITEM_WIDGET)) / 2);
		
		advanced_placer->add(ftable, true);

		advanced_placer->add(new w_spacer(), true);
		w_select_popup *texture_resolution_wa[OGL_NUMBER_OF_TEXTURE_TYPES];
		w_select_popup *texture_depth_wa[OGL_NUMBER_OF_TEXTURE_TYPES];
		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; i++) 
		{
			texture_resolution_wa[i] = new w_select_popup();
			texture_depth_wa[i] = new w_select_popup();
		}

		w_label *texture_labels[OGL_NUMBER_OF_TEXTURE_TYPES];
		texture_labels[OGL_Txtr_Wall] = new w_label(_SJIS("壁"));
		texture_labels[OGL_Txtr_Landscape] = new w_label(_SJIS("背景"));
		texture_labels[OGL_Txtr_Inhabitant] = new w_label(_SJIS("スプライト"));
		texture_labels[OGL_Txtr_WeaponsInHand] = new w_label(_SJIS("手中の武器／HUD"));

		advanced_placer->dual_add(new w_static_text(_SJIS("ビルドインのテスクチャのサイズと色深度")), m_dialog);
		advanced_placer->dual_add(new w_static_text(_SJIS("（VRAMの量が少ないマシンでは、減らすことを推奨します）")), m_dialog);

		vector<string> tex_reso_strings;
		tex_reso_strings.push_back (_SJIS("完全"));
		tex_reso_strings.push_back (_SJIS("1/2"));
		tex_reso_strings.push_back (_SJIS("1/4"));

		vector<string> tex_depth_strings;
		tex_depth_strings.push_back (_SJIS("32ビット"));
		tex_depth_strings.push_back (_SJIS("16ビット"));
		tex_depth_strings.push_back (_SJIS("8ビット"));

		table_placer *table = new table_placer(3, get_theme_space(ITEM_WIDGET));

		table->col_flags(0, placeable::kAlignRight);
		table->col_flags(1, placeable::kAlignLeft);
		table->col_flags(2, placeable::kAlignLeft);

		table->add(new w_spacer(), true);
		table->dual_add(new w_label(_SJIS("サイズ")), m_dialog);
		table->dual_add(new w_label(_SJIS("色深度")), m_dialog);

		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; i++)
		{
			table->dual_add(texture_labels[i], m_dialog);
			table->dual_add(texture_resolution_wa[i], m_dialog);
			table->dual_add(texture_depth_wa[i], m_dialog);

			texture_resolution_wa[i]->associate_label(texture_labels[i]);
			texture_resolution_wa[i]->set_labels(tex_reso_strings);

			texture_depth_wa[i]->associate_label(texture_labels[i]);
			texture_depth_wa[i]->set_labels(tex_depth_strings);
		}

		table->col_min_width(1, (table->col_width(0) - get_theme_space(ITEM_WIDGET)) / 2);
		table->col_min_width(2, (table->col_width(0) - get_theme_space(ITEM_WIDGET)) / 2);

		advanced_placer->add(table, true);

		m_tabs->add(general_table, true);
		m_tabs->add(advanced_placer, true);
		placer->add(m_tabs, false);
	
		placer->add(new w_spacer(), true);

		horizontal_placer *button_placer = new horizontal_placer;
		w_button* ok_w = new w_button("OK");
		button_placer->dual_add(ok_w, m_dialog);
		
		w_button* cancel_w = new w_button(_SJIS("キャンセル"));
		button_placer->dual_add(cancel_w, m_dialog);
		placer->add(button_placer, true);

		m_dialog.set_widget_placer(placer);

		m_cancelWidget = new ButtonWidget (cancel_w);
		m_okWidget = new ButtonWidget (ok_w);
		
		m_fogWidget = new ToggleWidget (fog_w);
		m_colourEffectsWidget = new ToggleWidget (fader_w);
		m_transparentLiquidsWidget = new ToggleWidget (liq_w);
		m_3DmodelsWidget = new ToggleWidget (models_w);
		m_blurWidget = new ToggleWidget (blur_w);
		m_bumpWidget = new ToggleWidget (bump_w);
		m_perspectiveWidget = new ToggleWidget (perspective_w);

		m_colourTheVoidWidget = 0;
		m_voidColourWidget = 0;

		m_fsaaWidget = new PopupSelectorWidget (fsaa_w);

		m_anisotropicWidget = new SliderSelectorWidget (aniso_w);

		m_sRGBWidget = new ToggleWidget(srgb_w);

		m_geForceFixWidget = new ToggleWidget (geforce_fix_w);
		m_useNPOTWidget = new ToggleWidget (use_npot_w);
		m_vsyncWidget = new ToggleWidget (vsync_w);
		
		m_wallsFilterWidget = new SelectSelectorWidget (far_filter_wa[OGL_Txtr_Wall]);
		m_spritesFilterWidget = new SelectSelectorWidget (far_filter_wa[OGL_Txtr_Inhabitant]);

		for (int i = 0; i < OGL_NUMBER_OF_TEXTURE_TYPES; ++i) {
			m_textureQualityWidget [i] = new PopupSelectorWidget (texture_quality_wa[i]);
			m_nearFiltersWidget[i] = new SelectSelectorWidget(near_filter_wa[i]);
			m_textureResolutionWidget [i] = new PopupSelectorWidget (texture_resolution_wa[i]);
			m_textureDepthWidget [i] = new PopupSelectorWidget(texture_depth_wa[i]);
		}
		m_modelQualityWidget = new PopupSelectorWidget(model_quality_w);
	}

	~SdlOpenGLDialog() {
		delete m_tabs;
	}

	virtual bool Run ()
	{	
		return (m_dialog.run () == 0);
	}

	virtual void Stop (bool result)
	{
		m_dialog.quit (result ? 0 : -1);
	}

	static void choose_generic_tab(void *arg);
	static void choose_advanced_tab(void *arg);

private:
	enum {
		TAB_WIDGET = 400,
		BASIC_TAB,
		ADVANCED_TAB
	};
	
	tab_placer* m_tabs;
	dialog m_dialog;
};

void SdlOpenGLDialog::choose_generic_tab(void *arg)
{
	SdlOpenGLDialog *d = static_cast<SdlOpenGLDialog *>(arg);
	d->m_tabs->choose_tab(0);
	d->m_dialog.draw();
}

void SdlOpenGLDialog::choose_advanced_tab(void *arg)
{
	SdlOpenGLDialog *d = static_cast<SdlOpenGLDialog *>(arg);
	d->m_tabs->choose_tab(1);
	d->m_dialog.draw();
}

std::unique_ptr<OpenGLDialog>
OpenGLDialog::Create(int theSelectedRenderer)
{
	return std::unique_ptr<OpenGLDialog>(new SdlOpenGLDialog(theSelectedRenderer));
}
