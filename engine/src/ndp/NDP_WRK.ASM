; ;// [NDP] - PSG Music Driver for MSX - Programmed by naruto2413

;// Adapted to SDCC assembler by Aoineko for use in MSXgl.
;// - v1.0: Original version
;// - v1.1: Removed BASIC interface

	.module NDP

; ;// =====================================
; ;// コールエントリ／ワークエリア／テーブル
; ;// （NDP.ASM と NDP_DRV.ASM が別途必要）
; ;// =====================================

; 	JP	NDPINI
; 	JP	MSTART
; 	JP	MSTOP
; 	JP	INTRPT

; 	JP	U_ADR
; 	JP	U_OFF1
; 	JP	U_OFF2
; 	JP	U_OFF3
; 	JP	U_MV
; 	JP	U_MFO
; 	JP	U_MFI
; 	JP	U_SE

; 	JP	CH1OFF
; 	JP	CH2OFF
; 	JP	CH3OFF
; 	JP	MVSET
; 	JP	MFOSET
; 	JP	RDSTAT
; 	JP	RDENDT
; 	JP	RDLOOP
; 	JP	ADRSET
; 	JP	MPLAYF
; 	JP	SEPLAY
; 	JP	VSET

; 	JP	SYSVER
; 	JP	NDPOFF
; 	JP	SETHF

; 	JP	IMAIN

;// ------	ワークエリア
	.area _DATA
_g_NDP_RAM0::

_g_NDP_CH1WRK::
CH1WRK:	;// リズムトラック
	.ds		2	;// 0-1	演奏データ読み込みアドレス
	.ds		1	;// 2	ノート番号
	.ds		1	;// 3
	.ds		1	;// 4	音長カウンタ
	.ds		1	;// 5	次の1バイトも音長かどうか
	.ds		1	;// 6	255ならリズム
	.ds		1	;// 7	レガート
	.ds		1	;// 8
	.ds		1	;// 9
	.ds		1	;// 10	チャンネル有効/無効
	.ds		1	;// 11
	.ds		1	;// 12	レガート遅延フラグ
	.ds		1	;// 13
	.ds		1	;// 14
	.ds		1	;// 15
	.ds		2	;// 16 17	ソフトエンベロープポインタ
	.ds		1	;// 18
	.ds		1	;// 19
	.ds		1	;// 20
	.ds		1	;// 21
	.ds		1	;// 22
	.ds		1	;// 23
	.ds		1	;// 24
	.ds		1	;// 25
	.ds		1	;// 26
	.ds		1	;// 27
	.ds		1	;// 28
	.ds		1	;// 29
	.ds		1	;// 30
	.ds		1	;// 31
	.ds		1	;// 32	リピートネスト数
_g_NDP_RVOLW::
RVOLW:
	.ds		26	;// 33-58	リズム26種類の音量
	.ds		1	;// 59
	.ds		1	;// 60

_g_NDP_CH2WRK::
CH2WRK:	;// 通常トラック
	.ds		2	;// +0 1	演奏データ読み込みアドレス
	.ds		1	;// +2	ノート番号
	.ds		1	;// +3	設定音量 (0=v15, 1=V14, 2=V13 … 15=v0)
	.ds		1	;// +4	音長カウンタ
	.ds		1	;// +5	次の1バイトも音長かどうか
	.ds		1	;// +6	音色番号 (90～9FHの場合はハードエンベ番号)
	.ds		1	;// +7	レガート
	.ds		1	;// +8	サスティン
	.ds		1	;// +9	Q(クオンタイズ)の値
	.ds		1	;// +10	チャンネル有効/無効 0=無効 1=有効 2=効果音発音中 3～255=一時ミュート(フレーム数)
	.ds		1	;// +11	ピッチエンベ番号 (bit7=ピッチを戻す？ bit6=ピッチ更新する？ bit5=元音程からの相対値？)
	.ds		1	;// +12	レガート確認用フラグ（+7のレガート設定の値が1音あとに入る）
	.ds		2	;// +13 14	発音中の音程（レジスタ値）
	.ds		1	;// +15	発音中の音量
	.ds		2	;// +16 17	ソフトエンベロープポインタ / ハードエンベ音量半減カウンタ
	.ds		1	;// +18	ソフトエンベロープウェイトカウンタ
	.ds		2	;// +19 20	デチューン
	.ds		2	;// +21 22	ピッチエンベロープポインタ
	.ds		1	;// +23	ピッチエンベロープディレイカウンタ
	.ds		1	;// +24	リリース音量
	.ds		1	;// +25	リリースカウンタ設定値
	.ds		1	;// +26	リリースカウンタ
	.ds		2	;// +27 28	リリースディレイ用ピッチ
	.ds		1	;// +29	リリースディレイ用スイッチ兼指定フレーム数
	.ds		1	;// +30	リリース音量設定
	.ds		1	;// +31	キーオン時の音程レジスタ更新フラグ
	.ds		1	;// +32	リピートネスト数
	.ds		1	;// +33	ポルタメント加算値(小数)
	.ds		1	;// +34	ポルタメント加算値
	.ds		2	;// +35 36	ポルタメント用の現在音程 (13-14に向かって加減算)
	.ds		1	;// +37	ポルタメント設定値(小数)
	.ds		1	;// +38	ポルタメント設定値兼スイッチ (0=OFF)
	.ds		1	;// +39	ポルタメント単発フラグ
	.ds		1	;// +40	@Q(後ろを削るクオンタイズ)の値
	.ds		1	;// +41	キーオン直前のノート番号（リリースディレイ用）
	.ds		1	;// +42	音量インターバル設定値
	.ds		1	;// +43	ノートエンベ番号
	.ds		2	;// +44 45	ノートエンベロープポインタ
	.ds		1	;// +46	ノートエンベロープウェイトカウンタ
	.ds		1	;// +47	音量インターバルのカウンタ
	.ds		1	;// +48	音量インターバルの音量値
	.ds		2	;// +49 50	前回のデチューン値
	.ds		1	;// +51	リリースディレイ用カウンタ
	.ds		1	;// +52	ゲートタイム (Qから計算してキーオン時に設定)
	.ds		1	;// +53	リリースディレイスキップ用カウンタ
	.ds		2	;// +54 55	ピッチエンベ適用前の音程（レジスタ値）
	.ds		1	;// +56	音量インターバル到達値
	.ds		1	;// +57	固定ゲートタイム
	.ds		1	;// +58	パラメータの退避領域
	.ds		1	;// +59	リリースディレイ発動フラグ
	.ds		1	;// +60	*n（ミックスモード）の値
_g_NDP_CH3WRK::
CH3WRK: 
	.ds		NDP_WSIZE
_g_NDP_CH4WRK::
CH4WRK:
	.ds		NDP_WSIZE

_g_NDP_SE1WRK::
SE1WRK:
	.ds		NDP_WSIZE*3

_g_NDP_CH1RWK::
CH1RWK:
	.ds		2	;// 0-1	リピート1開始アドレス
	.ds		2	;// 2-3	リピート1終了アドレス
	.ds		1	;// 4	リピート1回数
	.ds		2	;// 5-6	リピート2開始アドレス
	.ds		2	;// 7-8	リピート2終了アドレス
	.ds		1	;// 9	リピート2回数
	.ds		2	;// 10-11	リピート3開始アドレス
	.ds		2	;// 12-13	リピート3終了アドレス
	.ds		1	;// 14	リピート3回数
	.ds		2	;// 15-16	リピート4開始アドレス
	.ds		2	;// 17-18	リピート4終了アドレス
	.ds		1	;// 19	リピート4回数
_g_NDP_CH2RWK::
CH2RWK:
	.ds		NDP_RWSIZE
_g_NDP_CH3RWK::
CH3RWK:
	.ds		NDP_RWSIZE
_g_NDP_CH4RWK::
CH4RWK:
	.ds		NDP_RWSIZE

;// ->*************** MSTART 初期化時に0クリアする物

_g_NDP_FFFLG::
FFFLG:	.ds		1	;// 早送りフラグ
_g_NDP_ENDTR::
ENDTR:	.ds		1	;// 演奏データが終端まで達したトラックのビットが立つ (0000321RB) ※1ループでリセット
_g_NDP_ENDTRW::
ENDTRW:	.ds		1	;// 〃 ※1ループ後もリセットしない
_g_NDP_ENDTRR::
ENDTRR:	.ds		1	;// 〃 リセット時に書き戻す値 (曲開始時に未使用トラックのフラグを立てておいた値)
_g_NDP_MCOUNT::
MCOUNT:	.ds		1	;// ループ回数
_g_NDP_MVOL::
MVOL:	.ds		1	;// マスター音量 (この値を減算)
_g_NDP_MFADE::
MFADE:	.ds		1	;// フェード設定 (0=フェードしない 1～255=フェードカウント)
_g_NDP_FCOUNT::
FCOUNT:	.ds		1	;// フェードカウンタ
_g_NDP_LCOUNT::
LCOUNT:	.ds		1	;// フェードループカウンタ
_g_NDP_SLWPRM::
SLWPRM:	.ds		1	;// スロー設定
_g_NDP_SLWCNT::
SLWCNT:	.ds		1	;// スロー再生用カウンタ
_g_NDP_NFREQW::
NFREQW:	.ds		1	;// ノイズ周波数
_g_NDP_VISPAN::
VISPAN:	.ds		1	;// 音量インターバル周期

_g_NDP_RNON::
RNON:	.ds		1	;// リズム発音中かどうか (0=発音しない 1=発音終了してピッチを戻す 2=発音中 3=1フレだけ通常トーン)
_g_NDP_RSVOL::
RSVOL:	.ds		1	;// リズムを鳴らす前の音量退避
_g_NDP_RPITCH::
RPITCH:	.ds		2	;// リズムを鳴らす前の音程退避

_g_NDP_HENVSW::
HENVSW:	.ds		1	;// ハードエンベ番号

CLREND:

	.area _DATA

_g_NDP_SEMODE::
SEMODE:	.ds		1		;// Playback Mode (0:BGM 1:SE) ※Set within interrupt
_g_NDP_SEBAKT::
SEBAKT:	.ds		NDP_CHNUM	;// Save the track enable flag for the track being interrupted by sound effects here
_g_NDP_SEBAKR::
SEBAKR:	.ds		1		;// Whether to trigger register reset at SE end during rhythm key off
_g_NDP_SECNT::
SECNT:	.ds		1		;// Sound Effect Track Counter (Number of sound effect tracks currently playing)

_g_NDP_VADTBL::
VADTBL:	.ds		32	;// Volume Embedded Address Table
_g_NDP_RADTBL::
RADTBL:	.ds		64	;// Rhythm Tone Address Table
_g_NDP_PADTBL::
PADTBL:	.ds		32	;// Pitch-Embedded Dressing Table
_g_NDP_NADTBL::
NADTBL:	.ds		32	;// Note Envelope Dress Table

_g_NDP_RAM0END::

;// <-***************

; RVWRK:	DB	0	;// For rhythm volume adjustment
; RITRK:	DB	1	;// Interrupted Track (1=CH.C 2=CH.B 3=CH.A)
; RVREG:	DB	10	;// Rhythm track volume register number
; RPREG:	DB	5	;// Rhythm track frequency register number (high)
; RHENV:	DB	0	;// If no S command is executed within the rhythm, it becomes 0.

; OLDTH:	DS	5,0xC9	;// Legacy Timer Interrupt Hook
; HKFLG:	DB	0	;// Is the driver connected to the hook?

; STATS:	DB	0	;// Playback Status (0: Stopped 1: Playing)

; FINOUT:	DB	0	;// 0=Fade out; 1=Fade in

; MIXNMH:	DB	1	;// Value of *n (mix mode) when using hard envelope

; MIXWRK:	DB	10111000B	;// 10NNNTTT 0=ON/1=OFF
; MIXWRS:	DB	10111000B	;// Sound Effects Mixing Work

; HENVPW:	DW	1024	;// Hard Envelope Period

; BGMADR:	DW	04000H	;// Song Data Start Address

; PTABLE:
; ;//		c      c+     d      d+     e      f      f+     g      g+     a      a+     b
; 	.dw	0x0D5D,0x0C9C,0x0BE7,0x0B3C,0x0A9B,0x0A02,0x0973,0x08EB,0x086B,0x07F2,0x0780,0x0714	;// O1
; 	.dw	0x06AF,0x064E,0x05F4,0x059E,0x054E,0x0501,0x04BA,0x0476,0x0436,0x03F9,0x03C0,0x038A	;// O2
; 	.dw	0x0357,0x0327,0x02FA,0x02CF,0x02A7,0x0281,0x025D,0x023B,0x021B,0x01FD,0x01E0,0x01C5	;// O3
; O4C:
; 	.dw	0x01AC,0x0194,0x017D,0x0168,0x0153,0x0140,0x012E,0x011D,0x010D,0x00FE,0x00F0,0x00E3	;// O4
; 	.dw	0x00D6,0x00CA,0x00BE,0x00B4,0x00AA,0x00A0,0x0097,0x008F,0x0087,0x007F,0x0078,0x0071	;// O5
; 	.dw	0x006B,0x0065,0x005F,0x005A,0x0055,0x0050,0x004C,0x0047,0x0043,0x0040,0x003C,0x0039	;// O6
; 	.dw	0x0035,0x0032,0x0030,0x002D,0x002A,0x0028,0x0026,0x0024,0x0022,0x0020,0x001E,0x001C	;// O7
; 	.dw	0x001B,0x0019,0x0018,0x0016,0x0015,0x0014,0x0013,0x0012,0x0011,0x0010,0x000F,0x000E	;// O8

	.area _DATA

_g_NDP_RAMVAR::

_g_NDP_RVWRK::
RVWRK:	.ds		1	;// For rhythm volume adjustment
_g_NDP_RITRK::
RITRK:	.ds		1	;// Interrupted Track (1=CH.C 2=CH.B 3=CH.A)
_g_NDP_RVREG::
RVREG:	.ds		1	;// Rhythm track volume register number
_g_NDP_RPREG::
RPREG:	.ds		1	;// Rhythm track frequency register number (high)
_g_NDP_RHENV::
RHENV:	.ds		1	;// If no S command is executed within the rhythm, it becomes 0.

_g_NDP_OLDTH::
OLDTH:	.ds		5	;// Legacy Timer Interrupt Hook
_g_NDP_HKFLG::
HKFLG:	.ds		1	;// Is the driver connected to the hook?

_g_NDP_STATS::
STATS:	.ds		1	;// Playback Status (0: Stopped 1: Playing)

_g_NDP_FINOUT::
FINOUT:	.ds		1	;// 0=Fade out; 1=Fade in

_g_NDP_MIXNMH::
MIXNMH:	.ds		1	;// Value of *n (mix mode) when using hard envelope

_g_NDP_MIXWRK::
MIXWRK:	.ds		1	;// 10NNNTTT 0=ON/1=OFF
_g_NDP_MIXWRS::
MIXWRS:	.ds		1	;// Sound Effects Mixing Work

_g_NDP_HENVPW::
HENVPW:	.ds		2	;// Hard Envelope Period

_g_NDP_BGMADR::
BGMADR:	.ds		2	;// Song Data Start Address

_g_NDP_PTABLE::
PTABLE:
	.ds 2*12
	.ds 2*12
	.ds 2*12
_g_NDP_O4C::
O4C:
	.ds 2*12
	.ds 2*12
	.ds 2*12
	.ds 2*12
	.ds 2*12

_g_NDP_RAMVAREND::

	.area _CODE

_g_NDP_RAMVAL::

	.db		0	;// For rhythm volume adjustment
	.db		1	;// Interrupted Track (1=CH.C 2=CH.B 3=CH.A)
	.db		10	;// Rhythm track volume register number
	.db		5	;// Rhythm track frequency register number (high)
	.db		0	;// If no S command is executed within the rhythm, it becomes 0.
	.db		0xC9	;// Legacy Timer Interrupt Hook
	.db		0xC9	;// Legacy Timer Interrupt Hook
	.db		0xC9	;// Legacy Timer Interrupt Hook
	.db		0xC9	;// Legacy Timer Interrupt Hook
	.db		0xC9	;// Legacy Timer Interrupt Hook
	.db		0	;// Is the driver connected to the hook?
	.db		0	;// Playback Status (0: Stopped 1: Playing)
	.db		0	;// 0=Fade out; 1=Fade in
	.db		1	;// Value of *n (mix mode) when using hard envelope
	.db		0b10111000	;// 10NNNTTT 0=ON/1=OFF
	.db		0b10111000	;// Sound Effects Mixing Work
	.dw		1024	;// Hard Envelope Period
	.dw		0x4000	;// Song Data Start Address

;// ------	Pitch Table
;//		c      c+     d      d+     e      f      f+     g      g+     a      a+     b
	.dw	0x0D5D,0x0C9C,0x0BE7,0x0B3C,0x0A9B,0x0A02,0x0973,0x08EB,0x086B,0x07F2,0x0780,0x0714	;// O1
	.dw	0x06AF,0x064E,0x05F4,0x059E,0x054E,0x0501,0x04BA,0x0476,0x0436,0x03F9,0x03C0,0x038A	;// O2
	.dw	0x0357,0x0327,0x02FA,0x02CF,0x02A7,0x0281,0x025D,0x023B,0x021B,0x01FD,0x01E0,0x01C5	;// O3
	.dw	0x01AC,0x0194,0x017D,0x0168,0x0153,0x0140,0x012E,0x011D,0x010D,0x00FE,0x00F0,0x00E3	;// O4
	.dw	0x00D6,0x00CA,0x00BE,0x00B4,0x00AA,0x00A0,0x0097,0x008F,0x0087,0x007F,0x0078,0x0071	;// O5
	.dw	0x006B,0x0065,0x005F,0x005A,0x0055,0x0050,0x004C,0x0047,0x0043,0x0040,0x003C,0x0039	;// O6
	.dw	0x0035,0x0032,0x0030,0x002D,0x002A,0x0028,0x0026,0x0024,0x0022,0x0020,0x001E,0x001C	;// O7
	.dw	0x001B,0x0019,0x0018,0x0016,0x0015,0x0014,0x0013,0x0012,0x0011,0x0010,0x000F,0x000E	;// O8

_g_NDP_RAMVALEND::