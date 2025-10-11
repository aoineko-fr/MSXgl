;// [NDP] - PSG Music Driver for MSX - Programmed by naruto2413

;// Adapted to SDCC assembler by Aoineko for use in MSXgl.
;// - v1.0: Original version
;// - v1.1: Removed BASIC interface

	.module NDP

;// =====================================
;// ドライバ本体
;// （NDP.ASM と NDP_WRK.ASM が別途必要）
;// =====================================

	.area _CODE

;// ------	曲データのアドレス設定

;// USR関数用 (HL<-曲データをアドレスを格納しているメモリのアドレス-2)

; U_ADR:
; 	INC	HL
; 	INC	HL
; 	LD	E,(HL)
; 	INC	HL
; 	LD	D,(HL)

;// 直接コール用 (DE<-曲データのアドレス)

NDP_ADRSET::
ADRSET:
	LD	(BGMADR),DE
	RET

;// ------	マスター音量セット

;// USR関数用 (HL<-マスター音量を保存しているメモリのアドレス-2)

; U_MV:
; 	INC	HL
; 	INC	HL
; 	LD	A,(HL)

;// 直接コール用 (A<-マスター音量)

NDP_MVSET::
MVSET:
	LD	(MVOL),A
	RET

;// ------	フェードアウトセット

;// USR関数用 (HL<-フェードアウトのフレーム数を保存しているメモリのアドレス-2)

; U_MFO:
; 	INC	HL
; 	INC	HL
; 	LD	A,(HL)

;// 直接コール用 (A<-フェードのフレーム数)

NDP_MFOSET::
MFOSET:
	LD	(MFADE),A
	LD	A,#1
	LD	(LCOUNT),A
	XOR	A
	LD	(FINOUT),A
	RET

;// ------	フェードインセット

;// USR関数用 (HL<-フェードインのフレーム数を保存しているメモリのアドレス-2)

; U_MFI:
; 	INC	HL
; 	INC	HL
; 	LD	A,(HL)

;// 直接コール用 (A<-フェードのフレーム数)

NDP_MPLAYF::
MPLAYF:
	PUSH	AF
	CALL	MSTART
	POP	AF
	LD	(MFADE),A
	LD	A,#1
	LD	(LCOUNT),A
	LD	(FINOUT),A
	LD	A,#15
	LD	(MVOL),A
	RET

;// ------	チャンネルミュート

;// USR関数用 (ミュートするフレーム数を格納しているメモリのアドレス-2)

; U_OFF1:
; 	INC	HL
; 	INC	HL
; 	LD	D,(HL)
; 	JR	CH1OFF
; U_OFF2:
; 	INC	HL
; 	INC	HL
; 	LD	D,(HL)
; 	JR	CH2OFF
; U_OFF3:
; 	INC	HL
; 	INC	HL
; 	LD	D,(HL)
; 	JR	CH3OFF

;// 直接コール用 (D<-ミュートするフレーム数)

NDP_CH1OFF::
CH1OFF:
	LD	E,#8
	LD	C,#3
	LD	HL,#CH1WRK+WSIZE+10
	JR	CHMUTE

NDP_CH2OFF::
CH2OFF:
	LD	E,#9
	LD	C,#2
	LD	HL,#CH1WRK+WSIZE*2+10
	JR	CHMUTE

NDP_CH3OFF::
CH3OFF:
	LD	E,#10
	LD	C,#1
	LD	HL,#CH1WRK+WSIZE*3+10

CHMUTE:
	LD	A,D
	OR	A
	RET	Z

	LD	A,(RITRK)
	CP	C
	LD	A,D
	JR	NZ,CHMUT1	;// リズム割り込まれトラックでなければスキップ
	LD	(CH1WRK+10),A	;// 割り込まれトラックならリズムトラックのミュートするカウント数を設定
CHMUT1:
	LD	(HL),A		;// ノーマルトラックのミュートするカウント数を設定
	XOR	A
	DI
	CALL	WPSG

	LD	A,#11
	SUB	E
	LD	B,A
	CALL	MIXT
	AND	#0b00111111	;// 最上位ビットを0にすると、
	LD	(MIXWRK),A	;// ミックスモードの書き込み発動（ミュート時はトーンにする）

	EI
	RET

;// ------	フック接続
NDP_NDPINI::
NDPINI:
	DI

	LD	A,(HKFLG)	;// 簡易的な多重実行対策
	OR	A
	RET	NZ

	; LD	HL,#HTIMI
	; LD	DE,#OLDTH
	; LD	BC,#5
	; LDIR

	; LD	A,#0xC3		;// JP
	; LD	HL,#INTRPT	;// ドライバ割り込みルーチンアドレス
	; LD	(HTIMI),A
	; LD	(HTIMI+1),HL

	LD	A,#1
NDP_SETHF::
SETHF: ;// Set Hook Flag (A<-1:Set,0:Clear)
	LD	(HKFLG),A

	EI
	RET

;// ------	フック切り離し・発音停止

NDP_NDPOFF::
NDPOFF:
	LD	A,(HKFLG)	;// 簡易的な多重実行対策
	OR	A
	RET	Z

	DI

	LD	HL,#OLDTH
	LD	DE,#HTIMI
	LD	BC,#5
	LDIR

	XOR	A
	LD	(HKFLG),A

	CALL	MSTOP

	EI
	RET

;// ------	演奏停止（フック操作せず）

NDP_MSTOP::
MSTOP:
	LD	B,#CHNUM
	LD	HL,#CH1WRK+10
MSTP2L:
	LD	(HL),#0
	LD	DE,#WSIZE
	ADD	HL,DE
	DJNZ	MSTP2L

	XOR	A
	LD	(MFADE),A
	LD	(STATS),A

;// (PSG初期化)

PSGINI:
	LD	E,#7		;// R#7
	LD	A,#0b10111000	;// 10NNNTTT 0=ON/1=OFF
	LD	(MIXWRK),A
	LD	(MIXWRS),A
	CALL	WPSG

	XOR	A
	LD	B,#4
PSGINL:
	INC	E		;// R#8～11=0
	CALL	WPSG
	DJNZ	PSGINL

	INC	E		;// R#12=4
	LD	A,#4
	JP	WPSG

;// ------	演奏開始（フック操作せず）

NDP_MSTART::
MSTART:
	DI

	CALL	PSGINI

	XOR	A		;// うにスキーさんによるバイト数削減を適用
	LD	HL,#CH1WRK
	LD	DE,#CH1WRK+1
	LD	BC,#CLREND-CH1WRK-1
	LD	(HL),A		;// LD (HL),0
	LDIR			;// CH1WRK～CLRENDまでのワークをクリア

	INC	A		;// LD A,1
	LD	(RITRK),A	;// リズム割り込まれトラック=1
	LD	(STATS),A	;// 演奏状態=1
	LD	(MIXNMH),A	;// 音量半減時のミックスモード退避用
	LD	A,#5
	LD	(RPREG),A	;// 5
	ADD	A,A
	LD	(RVREG),A	;// 10

	LD	IX,#CH1WRK
	LD	HL,(BGMADR)
	LD	B,#CHNUM
	CALL	INIADR		;// 曲データ先頭アドレス設定
	CALL	VSET0		;// 音色アドレス設定

	EI
	RET

;// (音色アドレス設定)

VSET0:
	LD	HL,(BGMADR)
	PUSH	HL
	LD	DE,#8
	ADD	HL,DE
	LD	E,(HL)		;// 音色トラックアドレスを読み出す
	INC	HL
	LD	A,(HL)
	CP	#0x40		;// 音色トラックアドレスが4000H以上なら実アドレス、4000H以下ならBGM先頭アドレスからのオフセット値
	POP	HL
	JR	NC,VSETA
	LD	D,A
	ADD	HL,DE		;// HL=音色トラック実アドレス (BGMアドレス先頭アドレス+オフセット)
	JR	VSET
VSETA:
	LD	H,A
	LD	L,E		;// HL=音色トラック実アドレス

NDP_VSET::
VSET:
	LD	A,(HL)		;// 音色ヘッダを読む (255なら音色データ終了)
	CP	#255
	RET	Z

	INC	HL
	EX	DE,HL

	ADD	A,A
	LD	B,#0
	LD	C,A

	LD	HL,#VADTBL	;// 音色ヘッダが0～15ならノーマル音色番号
	CP	#32
	JR	C,VSET1
	LD	HL,#RADTBL-32	;// 音色ヘッダが16～47ならリズム音色番号
	CP	#96
	JR	C,VSET1
	LD	HL,#PADTBL-96	;// 音色ヘッダが48～63ならピッチエンベ番号
	CP	#128
	JR	C,VSET1
	LD	HL,#NADTBL-128	;// 音色ヘッダが64～ならノートエンベ番号
VSET1:
	ADD	HL,BC
	LD	A,(DE)		;// データ長を読む
	INC	DE
	LD	(HL),E
	INC	HL
	LD	(HL),D
	EX	DE,HL
	LD	D,#0
	LD	E,A
	ADD	HL,DE
	JR	VSET

;// ------	曲データのアドレス初期設定 (IX←ワーク先頭 HL←曲データ先頭アドレス B←トラック数)

INIADR::
	PUSH	HL
	POP	IY		;// IYに曲データ読み込みアドレスを入れる
	LD	C,B		;// トラック数をCに退避

INIWRK:
	LD	E,(HL)		;// 各CHオフセット値を読み出す (HL=曲データ実アドレス)
	INC	HL
	LD	D,(HL)
	INC	HL
	LD	A,E
	OR	D
	LD	A,#1
	JR	NZ,INIW1	;// オフセット値が0000Hでなければトラック有効

	LD	A,C		;// 指定トラック数を確認して
	CP	#CHNUM		;// 4トラック未満なら効果音なので
	JR	C,INIW0		;// トラック無効化だけ行う（終了フラグは触らない）

	LD	E,#16		;// オフセット値が0000Hなら
	LD	D,B		;// 終了フラグ更新
INIWR:
	SRL	E
	DJNZ	INIWR
	LD	B,D
	LD	A,(ENDTR)
	OR	E
	LD	(ENDTR),A	;// 未使用トラックのフラグを立てておく
	LD	(ENDTRR),A	;// リセット時に書き戻す値を退避
INIW0:
	XOR	A		;// トラック無効
INIW1:
	LD	10(IX),A	;// トラック有効/無効をセット
	LD	9(IX),#8	;// Q8

	PUSH	HL

	PUSH	IY
	POP	HL
	ADD	HL,DE
	LD	0(IX),L	;// CH開始アドレス
	LD	1(IX),H

	POP	HL
INIW2:
	LD	DE,#WSIZE
	ADD	IX,DE
	DJNZ	INIWRK

	RET

; ;// ------	演奏状態取得 (0=停止中 1=演奏中 ->A)

; NDP_RDSTAT::
; RDSTAT:
; 	LD	A,(STATS)
; 	RET

; ;// ------	終了トラック取得 (0000321R で終端まで達したトラックのビットが立つ ->A)

; NDP_RDENDT::
; RDENDT:
; 	LD	A,(ENDTRW)
; 	RET

; ;// ------	ループ回数取得 （ループしない曲では255を返す)

; NDP_RDLOOP::
; RDLOOP:
; 	LD	A,(MCOUNT)
; 	RET

;// ------	バージョン取得

NDP_SYSVER::
SYSVER:
	LD	HL,#0x0104	;// v1.04

	;// 上位バイトがメジャーバージョン、下位バイトがマイナーバージョン
	;// 上位バイトが0なら0.9として扱い、下位バイトはビルドバージョンとする
	;// 下位バイトは常に2桁として扱う

	RET

;// ------	曲データのアドレス設定

;// USR関数用 (HL<-曲データをアドレスを格納しているメモリのアドレス-2)

; U_SE:
; 	INC	HL
; 	INC	HL
; 	LD	E,(HL)
; 	INC	HL
; 	LD	D,(HL)

;// 直接コール用 (DE<-曲データのアドレス)

NDP_SEPLAY::
SEPLAY:
	JP	SEPSUB

;// ------	PSG書き込み (E←レジスタ A←データ C破壊)

;// (リズムトラック無効時のみPSGレジスタに書き込む)

WPSGMR:
	LD	C,A

	LD	A,(RITRK)
	CP	B
	JR	NZ,WPSGM1	;// 割り込まれトラックでなければPSGレジスタに書き込む

	LD	A,(RNON)
	OR	A
	RET	NZ		;// 割り込まれトラックでリズム発音中ならRET

	LD	A,C

;// (メインルーチン用PSG書き込み)

WPSGM:
	LD	C,A
WPSGM1:
	LD	A,10(IX)
	CP	#2
	LD	A,C
	RET	NC		;// トラック有効フラグが2未満の時のみPSGレジスタに書き込む

;// (通常用PSG書き込み)

WPSG:
	LD	C,#0xA0		;// PSGポート
	OUT	(C),E
	INC	C
	OUT	(C),A

	RET

;// ------	タイマ割り込みルーチン呼び出し＆旧フックコール

; NDP_INTRPT::
; INTRPT:
; 	DI
; 	CALL	IMAIN
; 	CALL	OLDTH
; 	EI
; 	RET

NDP_IMAIN::
IMAIN:
	; PUSH	AF
	; PUSH	BC
	; PUSH	DE
	; PUSH	HL
	; PUSH	IX
	; PUSH	IY

	LD	A,(SLWPRM)
	RRCA
	LD	(SLWPRM),A
	JR	C,INTEND	;// スロー再生処理（ビットが立ってたらドライバ処理しない）

INT0:
	;// (BGM処理)
MINT:
	XOR	A
	LD	(SEMODE),A	;// 0=BGMモード

	LD	B,#CHNUM
	LD	IX,#CH1WRK
MLOOP:
	PUSH	BC

	LD	A,10(IX)	;// チャンネル有効ならNMAINを呼ぶ
	OR	A
	CALL	NZ,NMAIN

	LD	DE,#WSIZE	;// CHワークのインデックスをワークサイズ分進める
	ADD	IX,DE

	POP	BC
	DJNZ	MLOOP

	LD	A,(FFFLG)	;// 早送りフラグが
	OR	A		;// 立っていたら
	JR	NZ,INT0		;// 割り込みの頭に戻す

	LD	A,(MFADE)	;// フェード値が
	OR	A		;// 0なら
	JR	Z,MINTED	;// フェード処理は飛ばす

	LD	C,A		;// Cにフェード値を退避
	LD	A,(FCOUNT)
	OR	A
	JR	NZ,MFADE1

	LD	A,(FINOUT)	;// 0ならフェードアウト、1ならフェードイン
	OR	A
	LD	A,(MVOL)
	JR	Z,MFOUT
	OR	A
	JR	Z,MFEND
	DEC	A
	JR	MFADE0
MFEND:
	LD	(MFADE),A	;// フェード値を0に
	JR	MINTED
MFOUT:
	INC	A
	CP	#15
	CALL	NC,MSTOP
MFADE0:
	LD	(MVOL),A
	LD	A,C
MFADE1:
	DEC	A
	LD	(FCOUNT),A
MINTED:

	;// (効果音処理)
SINT:
	XOR	A
	LD	(SECNT),A	;// SEの使用トラック数をゼロクリア
	INC	A
	LD	(SEMODE),A	;// 1=SEモード

	LD	B,#CHNUM-1
	LD	IX,#SE1WRK
SECHK1:
	LD	A,10(IX)	;// チャンネル有効ならSE用MAINを呼ぶ
	OR	A
	CALL	NZ,MAINSE

	LD	DE,#WSIZE	;// CHワークのインデックスをワークサイズ分進める
	ADD	IX,DE

	DJNZ	SECHK1
SINTED:

INTEND:
	; POP	IY
	; POP	IX
	; POP	HL
	; POP	DE
	; POP	BC
	; POP	AF

	RET

;// ------	ドライバ本体

MAINSE:
	LD	HL,#SECNT
	INC	(HL)		;// 効果音の使用トラック数を加算

NMAIN:
	CP	#2		;// トラック有効フラグを確認 (IX+10の内容がAに入ってくる)
	JR	C,MAIN01	;// 2未満なら通常処理にジャンプ

	CP	#255
	JR	NC,MAIN01	;// トラック有効フラグが255ならミュートカウンタを更新しない

	DEC	A		;// トラック有効フラグが2～254なら一時ミュートカウンタとして扱う
	LD	10(IX),A
	CP	#2		;// 減算結果を再確認
	JR	NC,MAIN01	;// 2以上ならレジスタ復帰しない

	LD	A,(RITRK)
	CP	B
	JR	NZ,MAINR	;// リズムトラックと同じチャンネルでなければレジスタ復帰

	LD	A,(RNON)
	OR	A
	JR	Z,MAINR		;// リズム発音中でなければレジスタ復帰

	LD	A,#255
	LD	(SEBAKR),A	;// リズム発音中ならリズムキーオフ後にレジスタ復帰するフラグをセット

	JR	MAIN01

MAINR:
	CALL	PRRET1		;// レジスタ復帰
	CALL	PRRET0

	LD	A,60(IX)
	CALL	MIXSUB		;// ミックスモード復帰

	LD	A,2(IX)	;// レジスタ復帰時に
	OR	A		;// 休符でなければ
	JR	NZ,MAINR1	;// スキップ
	LD	26(IX),#1	;// 休符ならリリースカウンタを強制的に1にする
MAINR1:
	LD	A,6(IX)
	CP	#0x90
	JR	NC,MAIN01
	LD	A,#11
	SUB	B
	LD	E,A
	LD	A,#16
	CALL	WPSGM		;// ハードエンベなら音量レジスタを16に

	LD	A,17(IX)	;// 音量半減カウンタ設定
	OR	A
	JR	Z,MAIN01	;// 0ならスキップ

	LD	A,16(IX)	;// 音量半減カウンタ
	OR	A
	CALL	Z,PITCH0	;// 0なら周波数を0に

MAIN01:
	LD	A,4(IX)	;// 音長カウンタチェック
	OR	A
	JP	Z,MAIN1

	LD	A,(RNON)
	OR	A
	JR	NZ,MAIN0A	;// リズム発音中ならR#7への書き込みはスキップ

	CALL	MIX_IY
	LD	A,(IY)		;// ミックスモード確認
	BIT	7,A
	JR	NZ,MAIN0A	;// 最上位ビットが0でなければスキップ
	OR	#0x80		;// 0なら1にして
	LD	(IY),A		;// ワークに保存し
	LD	E,#7		;// R#7にも
	CALL	WPSGM		;// 書き込む
MAIN0A:
	LD	A,(NFREQW)	;// ノイズ周波数確認
	CP	#32
	JR	C,MAIN0B	;// 32未満ならスキップ
	AND	#31
	LD	(NFREQW),A	;// 32以上なら31以下に丸めてワークに保存
	LD	E,#6
	CALL	WPSGM		;// ノイズ周波数書き込み
MAIN0B:
	LD	A,B		;// リズムチャンネルならリズムモード用のエンベに
	CP	#CHNUM
	JP	Z,RENV

;// 音長カウント・エンベロープ

	LD	A,2(IX)	;// 休符チェック
	OR	A
	JP	NZ,ENV		;// 休符でなければエンベロープ処理に
REST:
	CALL	RDELAY

	LD	A,25(IX)	;// リリースカウンタ設定が0ならリリース処理スキップ
	OR	A
	JR	Z,ENVRR1

	DEC	26(IX)		;// リリースカウンタ
	JP	NZ,ENVRR4

	INC	A
	LD	26(IX),A	;// リリースカウンタが0になったらリリースカウンタ設定値に戻す

	LD	A,6(IX)	;// 音色が
	CP	#0x90		;// ハードエンベかどうかを確認して
	JR	C,REST0		;// ハードエンベならキーオフ時にミックスモードをトーンにする
	CALL	MIXTWT
REST0:
	LD	A,24(IX)	;// リリース音量
	OR	A
	LD	D,A		;// 音量をDに退避
	JR	Z,ENVRR1
	DEC	A
	LD	24(IX),A	;// リリース音量更新
ENVRR1:
	LD	A,(RITRK)
	CP	B
	JR	NZ,ENVRR2	;// 割り込まれトラックじゃなければ無条件で音量レジスタ更新

	LD	A,D
	LD	(RSVOL),A	;// 割り込まれトラックなら退避音量を保存

	LD	A,(RNON)
	OR	A
	JP	NZ,ENVRR4	;// リズム発音中なら音量レジスタ更新しない
ENVRR2:
	CALL	REGV
	LD	A,(MVOL)
	LD	C,A
	LD	A,D		;// 退避しておいた音量をPSGに書き込み
	SUB	C
	JR	NC,ENVRR3
	XOR	A
ENVRR3:
	LD	15(IX),A
	CALL	WPSGM
ENVRR4:
	LD	A,11(IX)	;// ピッチエンベが有効なら休符でピッチエンベ駆動
	OR	A
	JP	Z,COUNTL
	AND	#0x80
	JP	NZ,COUNTL	;// ピッチエンベのビット7が立っていたら掛けない

	LD	A,29(IX)
	OR	A
	JP	Z,PENV		;// ピッチエンベ有効ならキーオフ時のピッチ設定しない
	LD	E,27(IX)	;// キーオフ時のピッチ設定
	LD	A,28(IX)
	LD	D,A
	OR	E
	JP	Z,PENV		;// 0なら処理しない
	LD	13(IX),E
	LD	14(IX),D
	JP	PENV

;// リリースディレイ処理（設定ピッチをレジスタに書き込み）

RDELAY:
	LD	A,(RNON)
	OR	A
	RET	NZ		;// リズム発音中ならリリースディレイ処理しない

	LD	A,29(IX)
	OR	A
	RET	Z		;// リリースディレイフラグが0ならリリースディレイ処理しない

	LD	A,59(IX)
	OR	A
	RET	Z
	DEC	59(IX)

	CALL	REGP
	LD	A,28(IX)
	OR	27(IX)
	JR	NZ,RDELY2	;// ピッチが0じゃなければジャンプ

PWRITE:
	LD	A,14(IX)
	CALL	WPSGM		;// 音程レジスタ上位に現在のピッチを書き込み
	DEC	E
	LD	A,13(IX)
	JP	WPSGM		;// 音程レジスタ下位に現在のピッチを書き込み

RDELY2:
	LD	A,28(IX)
	LD	55(IX),A
	CALL	WPSGM		;// 音程レジスタ上位にリリースディレイ用ピッチを書き込み
	DEC	E
	LD	A,27(IX)
	LD	54(IX),A
	CALL	WPSGM		;// 音程レジスタ下位にリリースディレイ用ピッチを書き込み

	LD	A,(RITRK)
	CP	B
	RET	NZ

	LD	A,(SEMODE)
	OR	A
	RET	NZ

	LD	A,27(IX)	;// 割り込まれトラックならリズム発音後に音程を戻すためのピッチ値を退避
	LD	(RPITCH),A
	LD	A,28(IX)
	LD	(RPITCH+1),A
	RET

;// ソフトエンベ (音量インターバル)

ENV:
	LD	A,42(IX)	;// 音量インターバル
	OR	A
	JR	Z,VENV

	LD	C,A
	AND	#0x7F
	CP	#64
	JR	C,VIE0

	LD	A,(VISPAN)
	INC	A
	LD	(VISPAN),A
	CP	#2
	JR	C,VENV
	XOR	A
	LD	(VISPAN),A
VIE0:
	LD	A,47(IX)	;// 音量インターバル用カウンタ
	DEC	A
	JR	Z,VIE
	LD	47(IX),A	;// 0じゃなければワークに保存して
	JR	VENV		;// スキップ
VIE:
	LD	A,C
	AND	#0x7F
	LD	47(IX),A	;// 0ならカウンタ再設定

	LD	A,48(IX)	;// 音量加算値
	INC	A
	CP	#15
	JR	C,VIE1
	LD	A,#15
VIE1:
	LD	48(IX),A

;// ソフトエンベ (音色)

VENV:
	LD	A,52(IX)	;// ゲートタイム
	OR	A
	JR	NZ,ENV0		;// 0じゃなければエンベ処理

	LD	A,7(IX)
	OR	A
	JP	Z,REST		;// レガート中でなければ休符処理

ENV0:
	LD	A,6(IX)	;// 音色番号が
	CP	#0x90		;// ハードエンベでなければ
	JR	C,ENV00		;// ソフトエンベにジャンプ

	LD	A,16(IX)	;// ハードエンベなら音量半減カウンタを確認
	OR	A
	JP	Z,ENVEND	;// カウンタが設定されてなければエンベ処理せずにジャンプ
	DEC	16(IX)
	JP	NZ,ENVEND	;// カウントが0になっていなければエンベ処理せずにジャンプ

	LD	A,(RITRK)
	CP	B
	JR	NZ,ENVH00	;// 割り込まれトラックでなければ無条件に音量半減用のレジスタ書き込み

	LD	A,(RNON)
	OR	A
	JR	Z,ENVH00	;// 割り込まれトラックでリズム発音中でなければ音量半減用のレジスタ書き込み

	CALL	MIX_IY
	CALL	MIXT		;// 割り込まれトラックでリズム発音中なら退避音量ワークのみ書き込み
	LD	(IY),A
	JR	ENVH01
ENVH00:
	CALL	PITCH0		;// トーン有効・ノイズ無効に
	CALL	MIXTWT

	LD	A,(RITRK)
	CP	B
	JP	NZ,ENVEND	;// 割り込まれトラックでなければ処理スキップ
ENVH01:
	XOR	A
	LD	(RPITCH),A
	LD	(RPITCH+1),A	;// 割り込まれトラックなら退避音程を0に
	JP	ENVEND

ENV00:
	LD	A,18(IX)	;// エンベロープウェイトカウンタをチェック
	OR	A
	JR	Z,ENV1
	DEC	18(IX)
	JP	ENVEND
ENV1:
	LD	L,16(IX)	;// エンベロープポインタをHLに
	LD	H,17(IX)
ENV10:
	LD	A,(HL)		;// エンベロープデータをAに
	CP	#0xF0
	JR	C,ENV11
	LD	D,#0		;// 上位4ビットがFならデータ終端、下位4ビットはエンベカウンタを戻す値
	AND	#0x0F
	LD	E,A
	SBC	HL,DE
	JR	ENV10
ENV11:
	INC	HL

	CP	#0xA0
	JR	NZ,ENV1Z0

	LD	16(IX),L
	LD	17(IX),H	;// エンベロープポインタを保存
	JP	ENVEND

ENV1Z0:
	CP	#0xA1		;// 0A1H=周波数を0にする
	JR	NZ,ENV1Z1

	CALL	PITCH0
	XOR	A
	LD	13(IX),A
	LD	14(IX),A
	
	LD	A,(RITRK)
	CP	B
	JP	NZ,ENV10

	XOR	A
	LD	(RPITCH),A
	LD	(RPITCH+1),A

	JP	ENV10
ENV1Z1:
	CP	#0xA3		;// 0A3H=ピッチエンベ適用前の音程に戻す
	JR	NZ,ENV11I

	CALL	REGP
	LD	A,55(IX)
	LD	D,A
	CALL	WPSGM
	DEC	E
	LD	A,54(IX)
	CALL	WPSGM

	LD	E,A

	LD	A,(RITRK)
	CP	B
	JP	NZ,ENV10

	LD	A,E
	LD	(RPITCH),A
	LD	A,D
	LD	(RPITCH+1),A

	JP	ENV10
ENV11I:
	CP	#0xA5		;// 0A5H=音量遷移インターバル
	JR	NZ,ENV11N

	LD	A,(HL)
	INC	HL
	LD	42(IX),A	;// 設定値
	LD	C,A
	AND	#0b01111111
	LD	47(IX),A	;// カウンタ
	JR	ENV10

ENV11N:
	CP	#0xA4		;// 0A4H=ノートエンベロープ
	JR	NZ,ENV11P
	LD	C,#1
	CALL	SETNES
	JR	ENV10

ENV11P:
	CP	#0xA2		;// 0A2H=ピッチエンベロープ
	JR	NZ,ENV11A
	LD	C,#1
	CALL	SETPES
	JP	ENV10

ENV11A:
	CP	#0xD0
	JR	C,ENV11B	;// D0-EFH=ノイズ周波数

	SUB	#0xD0
	LD	(NFREQW),A	;// ノイズ周波数を保存
	LD	E,#6
	CALL	WPSGM		;// 書き込み
	JP	ENV10

ENV11B:
	CP	#0xC0
	JR	C,ENV11C	;// C0～C3H=ミックスモード

	AND	#3
	CALL	MIXSWT

	LD	A,(RNON)
	OR	A
	JP	NZ,ENV10	;// リズム発音中ならレジスタ書き込みは行わない
	LD	A,C
	OR	#0x80
	LD	E,#7
	CALL	WPSGM		;// ミックスモード書き込み
	JP	ENV10

ENV11C:
	LD	16(IX),L
	LD	17(IX),H	;// エンベロープポインタを進めて保存

	CP	#0xB0		;// B0～BFH=ハードエンベ形状
	JR	C,ENV11Z

	AND	#0x0F		;// ハードエンベ形状を
	LD	(HENVSW),A	;// ワークエリアにセットして
	LD	E,#13		;// R#13に
	CALL	WPSG		;// 書き込む
ENV11X:
	CALL	REGV
	LD	A,(RITRK)
	CP	B
	JR	NZ,ENV11H	;// 割り込まれトラックでなければレジスタ書き込み

	LD	A,(RNON)
	OR	A
	JR	NZ,ENV11R	;// リズム発音中ならリズム終了時の値のみ設定する
ENV11H:
	LD	A,#16		;// 音量レジスタに16を
	CALL	WPSGM		;// 書き込む
	JR	ENVEND
ENV11R:
	LD	A,E
	LD	(RVREG),A
	LD	A,#16
	LD	(RSVOL),A
	JR	ENVEND

ENV11Z:
	LD	D,A
	SRL	A
	SRL	A
	SRL	A
	SRL	A
	LD	18(IX),A	;// エンベロープウェイトカウンタを保存
	LD	A,D
	AND	#0x0F
	LD	D,3(IX)	;// チャンネル音量をDに入れて
	SUB	D		;// エンベロープ音量から引く
	JR	NC,ENV12
	XOR	A		;// 0を下回ったら0に
ENV12:
	LD	D,A		;// 結果をDに退避
	LD	A,42(IX)	;// 音量インターバル設定値の
	AND	#0x80		;// ビット7が立っているか
	JR	NZ,ENV3M	;// ビット7が立っていなければ減算

	LD	A,56(IX)	;// 音量インターバル到達値が
	OR	A		;// 0でなければ
	JR	NZ,ENV3P	;// 設定値を最大値に反映
	LD	A,3(IX)	;// 0なら
	XOR	#15		;// 設定音量を最大値に反映
ENV3P:
	LD	C,A
	LD	A,D		;// 退避した音量をAに戻す
	ADD	A,48(IX)	;// 音量インターバルのワーク分を加算
	CP	C
	JR	C,ENV4
	LD	A,C
	JR	ENV4
ENV3M:
	LD	A,D		;// 退避した音量をAに戻す
	SUB	48(IX)		;// 音量インターバルのワーク分を減算
	JR	NC,ENV4
	XOR	A
ENV4:
	LD	15(IX),A	;// 結果を音量ワークに保存
	LD	D,A		;// 結果をDに退避

	LD	A,(RITRK)
	CP	B
	JR	NZ,ENV2		;// 割り込まれトラックじゃなければエンベをレジスタに書き込む

	LD	A,D
	LD	(RSVOL),A	;// リズム発音前の音量を退避

	LD	A,(RNON)
	OR	A
	JR	NZ,ENVEND	;// リズム発音中フラグが立っていたらエンベ処理スキップ
ENV2:
	CALL	REGV
	LD	A,(MVOL)
	LD	C,A
	LD	A,D		;// Dに退避してある音量を読み込んで
	SUB	C		;// MVOL分を減らす
	JR	NC,ENV3
	XOR	A
ENV3:
	CALL	WPSGM
ENVEND:
	LD	A,7(IX)
	OR	A
	JP	NZ,PORWT	;// レガート中ならゲートタイムは減らさない

	DEC	52(IX)		;// ゲートタイムを減らす
	JP	NZ,PORWT	;// 0じゃなければキーオフ時のリリース設定はスキップ

	CALL	NOTER

	LD	A,6(IX)
	CP	#0x90
	JR	C,ENVED1	;// ハードエンベでなければスキップ

	LD	A,25(IX)
	OR	A
	JR	Z,ENVED1	;// リリースOFFならスキップ

	LD	A,17(IX)	;// 音量半減カウンタ
	OR	A
	JR	Z,ENVEH1	;// 0ならスキップ

	LD	A,29(IX)	;// リリースディレイ
	OR	A
	JR	NZ,ENVEH1	;// 0以外ならスキップ

	CALL	REGP
	CALL	PWRITE		;// ピッチを戻す

ENVEH1:
	LD	A,(RITRK)
	CP	B
	JR	NZ,ENVED1	;// 割り込まれトラックでなければピッチ退避しない

	LD	A,13(IX)
	LD	(RPITCH),A
	LD	A,14(IX)
	LD	(RPITCH+1),A

ENVED1:
	LD	A,11(IX)	;// ピッチエンベ番号
	AND	#0x80		;// 最上位ビットによってキーオフ時に音程を戻すかどうかを決める
	JP	Z,PORWT

	LD	L,54(IX)	;// キーオフ時に音程を戻す
	LD	H,55(IX)
	LD	13(IX),L
	LD	14(IX),H

	JP	PSET

;// リリース設定

NOTER:
	LD	A,6(IX)
	CP	#0x90
	JR	C,NOTERS
	LD	A,#15
	JR	NOTER0
NOTERS:
	LD	A,15(IX)	;// ソフトエンベなら現在の音量を基準にする
NOTER0:
	SUB	30(IX)		;// 上記の音量-リリース音量設定をリリース音量に
	JR	NC,NOTER1
	XOR	A
NOTER1:
	LD	24(IX),A	;// リリース音量を保存
NOTES:
	LD	A,8(IX)	;// リリース時間
	LD	25(IX),A	;// リリースカウンタ設定値
	LD	26(IX),#1	;// リリースカウンタ
	RET

;// ポルタメント加減算

PORWT:
	LD	A,34(IX)	;// ポルタメント値(整数部)
	OR	A
	JP	Z,POREND	;// ポルタメント値が0ならポルタメント処理をスキップ
PORWT1:
	PUSH	BC

	LD	L,35(IX)	;// 現在音程
	LD	H,36(IX)

	LD	C,13(IX)	;// 目的音程
	LD	B,14(IX)

	LD	A,H
	CP	B		;// 現在値と到達値の上位を比較
	JR	C,PPLUS		;// 現在値が小さければ加算処理へ
	JR	NZ,PMINUS	;// でなければ減算処理へ
	LD	A,L
	CP	C		;// 現在値と到達値の下位を比較
	JR	Z,PJUST		;// 到達してたら終了
	JR	C,PPLUS		;// 現在値が小さければ加算処理へ

PMINUS:
	LD	D,#0
	LD	E,34(IX)
	SBC	HL,DE		;// 減算

	JR	C,PJUST

	LD	A,B		;// 目的値の上位バイトと
	CP	H		;// 現在値の上位バイトを比較
	JR	NZ,PSETP
	LD	A,C		;// 目的値の下位バイトと
	CP	L		;// 現在値の下位バイトを比較
	JR	NC,PJUST
	JR	PSETP

PPLUS:
	LD	D,#0
	LD	E,34(IX)
	ADD	HL,DE		;// 加算

	LD	A,L
	SUB	C
	LD	A,H
	SBC	A,B
	JR	C,PSETP

PJUST:
	LD	34(IX),#0
	LD	33(IX),#0

	LD	A,31(IX)	;// キーオン時の音程レジスタ更新フラグが立っていたら
	OR	A		;// ポルタメントの音程を設定しない
	JR	NZ,PSETP

	LD	L,C		;// HLレジスタに到達音程を設定
	LD	H,B

PSETP:
	POP	BC

	LD	A,(RITRK)
	CP	B
	JR	NZ,PSETP1	;// 割り込まれトラック以外なら常にポルタメントのレジスタ書き込み実行

	LD	A,(RNON)	;// 割り込まれトラックでリズム発音中ならレジスタ書き込みはスキップ
	OR	A
	JR	NZ,PSEND
PSETP1:
	CALL	REGP
	LD	A,H
	CALL	WPSGM		;// 音程レジスタ上位に書き込み
	DEC	E
	LD	A,L
	CALL	WPSGM		;// 音程レジスタ下位に書き込み

PSEND:
	LD	35(IX),L
	LD	36(IX),H	;// 現在値を更新

POREND:

;// ノートエンベロープ

NENV:
	LD	A,43(IX)
	OR	A
	JP	Z,NEEND		;// ノートエンベスイッチがOFFならスキップ
	LD	C,A

	AND	#0x80
	JR	NZ,NEEND	;// ノートエンベ終了フラグが立っていたらスキップ

	LD	L,44(IX)	;// ノートエンベポインタ
	LD	H,45(IX)
NENV1:
	LD	A,(HL)		;// データを読む
	INC	HL
	CP	#0x80
	JR	NZ,NENV3	;// データが80Hならポインタを指定バイト戻す
	LD	A,(HL)		;// 戻すバイト数
	OR	A
	JR	NZ,NENV2
	LD	A,C
	OR	#0x80
	LD	43(IX),A	;// 戻すバイト数が0ならノートエンベ終了フラグを立てる
	JP	NEEND
NENV2:
	LD	D,#0		;// 指定バイト数戻す
	LD	E,A
	SBC	HL,DE
	JR	NENV1
NENV3:
	LD	44(IX),L	;// ノートエンベポインタ更新
	LD	45(IX),H
	JR	NC,NENV4

	LD	D,2(IX)
	ADD	A,D		;// ノート番号を加算
	CP	#95
	JR	C,NENV5
	LD	A,#95
	JR	NENV5

NENV4:
	NEG
	LD	D,A
	LD	A,2(IX)
	SUB	D		;// ノート番号を減算
	JR	C,NENV4A
	OR	A
	JR	NZ,NENV5
NENV4A:
	LD	A,#1
NENV5:
	CALL	NTOP
	LD	13(IX),L	;// ピッチ更新
	LD	14(IX),H
	LD	54(IX),L
	LD	55(IX),H
	JP	PSET
NEEND:

;// ピッチエンベロープ

PENV:
	LD	A,11(IX)
	OR	A
	JP	Z,PEEND		;// ピッチエンベスイッチがOFFならスキップ
	LD	C,A

	LD	A,6(IX)
	CP	#0x90
	JR	C,PENV0		;// ハードエンベ無効ならピッチエンベ駆動

	LD	A,17(IX)
	OR	A
	JP	NZ,PEEND	;// ハードエンベ有効で音量半減カウンタが設定されていたらスキップ

PENV0:
	DEC	23(IX)		;// ピッチエンベディレイカウンタ
	JP	NZ,PEEND	;// ピッチディレイカウンタが0でなければスキップ
	LD	23(IX),#1	;// ピッチエンベディレイカウンタを1にする（上のDECで毎回発動するようになる）

	BIT	6,C		;// ピッチエンベ番号のビット6が立っていたら
	JP	NZ,PSET		;// ディレイカウンタだけ進めてピッチエンベ発動しない

	BIT	5,C		;// ピッチエンベ番号のビット5が立っていなければ
	JR	Z,PENVF		;// 前フレームのピッチを維持

	LD	L,54(IX)
	LD	H,55(IX)	;// 元の音程を
	LD	13(IX),L
	LD	14(IX),H	;// 加工後ピッチに反映

PENVF:
	LD	L,21(IX)	;// ピッチエンベポインタ
	LD	H,22(IX)
PENV1:
	LD	A,(HL)		;// データを読む
	INC	HL
	CP	#0x80
	JR	NZ,PENV3	;// データが80Hならポインタを指定バイト戻す
	LD	D,#0
	LD	E,(HL)		;// 戻すバイト数
	SBC	HL,DE
	JR	PENV1
PENV3:
	LD	21(IX),L	;// ピッチエンベポインタ更新
	LD	22(IX),H

	PUSH	AF

	LD	A,C		;// ピッチエンベ番号をAに戻して
	AND	#0b00100000	;// ビット5が立っていたら
	JR	NZ,PENVZ	;// 前フレームでなく元のピッチからの遷移

	LD	L,13(IX)
	LD	H,14(IX)
	JR	PENV3A
PENVZ:
	LD	L,54(IX)
	LD	H,55(IX)
PENV3A:
	POP	AF
	JR	C,PENVA		;// 加減算の分岐

	NEG			;// 減算のために符号反転
	LD	D,A
	LD	A,L		;// ピッチ下位を読み込んで
	SUB	D		;// ピッチ減算
	LD	13(IX),A	;// ピッチ下位ワーク更新
	JP	NC,PSET
	LD	A,H		;// 桁あふれ時は
	SUB	#1		;// ピッチ上位も減算
	JR	C,PENV4		;// 上位の桁あふれを確認
	LD	14(IX),A
	JP	PSET
PENV4:
	XOR	A
	LD	13(IX),A
	LD	14(IX),A
	JR	PSET

PENVA:
	LD	D,A
	LD	A,L		;// 現在のピッチ下位を読み込んで
	ADD	A,D		;// ピッチ加算
	LD	13(IX),A	;// ピッチ下位ワーク更新
	JR	NC,PSET
	LD	A,H		;// 桁あふれ時は
	INC	A		;// ピッチ上位も加算
	CP	#16
	JR	C,PENV5		;// 上位の桁あふれを確認
	LD	13(IX),#255
	LD	14(IX),#15
	JR	PSET
PENV5:
	LD	14(IX),A
	JR	PSET
PEEND:

;// キーオン時のピッチ更新処理

KONCHK:
	LD	A,31(IX)	;// キーオン時のピッチ更新フラグを確認
	OR	A
	JP	Z,COUNT

;// ピッチとハードエンベをレジスタに設定

PSET:
	LD	A,6(IX)	;// 音色番号がハードエンベ用かどうか
	CP	#0x90
	JR	C,PSETS
	LD	D,A

	LD	A,52(IX)	;// ハードエンベ時にゲートタイムが
	OR	A		;// 0でなければ
	JP	NZ,PSET0	;// ハードエンベをセットするかどうか確認

	LD	A,31(IX)	;// ゲートタイムが0なら
	OR	A		;// キーオンフラグを確認して
	JP	Z,PSETS		;// キーオフ(%1の音符でない)なら音程のみセット
PSET0:
	LD	A,2(IX)	;// 休符なら
	OR	A		;// ハードエンベはセットせずに
	JP	Z,PSETS		;// 音程をセット

	LD	A,12(IX)	;// レガート中でなければ
	OR	A		;// ハードエンベを
	JP	Z,PSETH0	;// セット

	LD	A,17(IX)	;// レガート中なら音量半減が有効かどうかを
	OR	A		;// 確認して
	JR	Z,PSETS		;// 音源半減を設定してなければ音程をセット
	LD	31(IX),#0	;// していたらキーオンフラグだけリセット
	JP	COUNTL
PSETH0:
	LD	A,31(IX)	;// キーオン時レジスタ更新フラグが
	OR	A		;// 立っていなければ
	JR	Z,PSET1		;// ハードエンベはセットしない

	LD	A,D
	AND	#0x0F
	LD	(HENVSW),A	;// ハードエンベ番号を退避
	LD	E,#13
	CALL	WPSGMR		;// R#13にエンベロープ番号を設定

PSET1:
	LD	A,(MVOL)
	OR	A
	JR	NZ,PSET2

	LD	A,#11
	SUB	B
	LD	E,A
	LD	A,#16
	CALL	WPSGMR		;// ハードエンベなら音量レジスタに16を設定

	LD	A,(RITRK)
	CP	B
	JP	NZ,PSET2	;// 割り込まれトラック以外ならリズム用の音量退避はスキップ
	LD	A,#16
	LD	(RSVOL),A
PSET2:
	LD	A,17(IX)	;// 音量半減カウンタ設定を確認
	LD	16(IX),A	;// ビット7のフラグを消してカウンタ設定
	OR	A
	JR	NZ,PSETH	;// 設定されていたらジャンプ

	LD	A,25(IX)
	OR	A
	JR	Z,PSETS		;// リリース設定されてなければスキップ

	LD	A,(MIXNMH)	;// 音量半減が設定されていなければミックスモードを戻す
	CALL	MIXSUB
	JR	PSETH1
PSETH:
	CALL	MIXOFF		;// 音量半減が設定されていたらキーオン時トーンノイズOFF
PSETH1:
	CALL	MIXWT

PSETS:
	LD	31(IX),#0	;// キーオンフラグをリセット
PSETS1:
	CALL	REGP
	LD	A,34(IX)	;// ポルタメント中かどうか
	OR	A
	JP	NZ,PSETPO

	LD	A,14(IX)
	CALL	WPSGMR		;// 音程レジスタ上位に書き込み(通常時)
	DEC	E
	LD	A,13(IX)
	CALL	WPSGMR		;// 音程レジスタ下位に書き込み(通常時)
	JP	COUNTL
PSETPO:
	LD	A,36(IX)
	CALL	WPSGMR		;// 音程レジスタ上位に書き込み(ポルタメント時)
	DEC	E
	LD	A,35(IX)
	CALL	WPSGMR		;// 音程レジスタ下位に書き込み(ポルタメント時)
	JP	COUNTL

;// リズムエンベロープ

RENV:
	LD	A,2(IX)	;// 休符チェック
	OR	A
	JP	Z,COUNTR

	LD	A,(RNON)
	CP	#2
	JP	Z,RENV0		;// リズム発音中フラグが2ならエンベ処理
	OR	A
	CALL	NZ,RHYOFF	;// リズム発音中フラグが1なら通常処理に戻す
	JP	COUNTR
RENV0:
	LD	L,16(IX)	;// エンベロープポインタをHLに
	LD	H,17(IX)
RENV1:
	LD	A,(HL)		;// エンベロープデータをAに

	INC	HL
	LD	16(IX),L
	LD	17(IX),H	;// エンベロープポインタを進めて保存

	CP	#255		;// データ終了か
	JP	Z,REEND

RENV2:
	CP	#0x10		;// 1フレーム分の処理終了
	JP	Z,COUNTR

	LD	E,A		;// Eレジスタに退避 (レジスタ番号を兼ねる)

	AND	#0x20
	JR	Z,RENV2S	;// 2xHじゃなければスキップ

	LD	C,B		;// 2xHならミックスモード
	LD	A,(RITRK)
	LD	B,A
	PUSH	HL
	LD	A,E
	AND	#3
	CALL	MIXSUB
	POP	HL
	LD	B,C
	LD	E,#7
	CALL	WPSGM		;// ミックスモード書き込み
	JR	RENV1

RENV2S:
	LD	A,E
	CP	#6		;// レジスタ#6以降かどうか
	JR	C,RENV3		;// 6未満ならスキップ
	CP	#13
	JR	NZ,RENV2D	;// 13以外（6～12）ならフラグは操作せずに直レジスタ書き込み
	LD	(RHENV),A	;// 13ならリズム内ハードエンベ使用フラグを立てて直レジスタ書き込み
RENV2D:
	LD	A,(HL)		;// データを読み込む
	JR	RENVZ		;// レジスタ#6以降なら直に値を書き込む

RENV3:
	CP	#1
	JR	Z,RENVV		;// 1なら音量、2なら音程

	LD	A,(RPREG)
	LD	E,A		;// ピッチレジスタ上位を設定
	LD	A,(HL)		;// ピッチデータ上位を読み込み
	CALL	WPSGM
	INC	HL
	LD	A,(HL)		;// ピッチデータ下位を読み込み
	DEC	E		;// ピッチレジスタ下位を設定
	JR	RENVZ

RENVV:
	LD	A,(RVREG)
	LD	E,A		;// 音量レジスタを設定

	LD	A,(HL)		;// データを読み込んでおく
	CP	#16
	JR	Z,RENVZ		;// ハードエンベなら音量指定やマスター音量を反映させない
	LD	D,A
	LD	A,(RVWRK)
	LD	C,A
	LD	A,(MVOL)
	ADD	A,C
	LD	C,A
	LD	A,D
	SUB	C
	JR	NC,RENVZ
	XOR	A
RENVZ:
	CALL	WPSGM
	INC	HL		;// エンベロープポインタを進める
	JP	RENV1

;// リズム初期化

RHYOFF:
	XOR	A
	LD	(RNON),A

	LD	A,(NFREQW)	;// ノイズ周波数書き込みフラグを立てる
	OR	#32
	LD	(NFREQW),A

	LD	A,(RPREG)	;// リズム発音前の音程に戻す
	LD	E,A
	LD	A,(RPITCH+1)
	CALL	WPSGM
	DEC	E
	LD	A,(RPITCH)
	CALL	WPSGM

	LD	E,#7		;// ミックスモードを戻す
	LD	A,(MIXWRK)
	OR	#0x80
	CALL	WPSGM

	LD	A,(RVREG)	;// リズム発音前の音量に戻す
	LD	E,A
	LD	A,(MVOL)
	LD	C,A
	LD	A,(RSVOL)
	SUB	C
	JR	NC,RHYOFM
	XOR	A
RHYOFM:
	CALL	WPSGM

	CP	#16		;// 音量が16(ハードエンベ)なら
	JR	Z,RHYOFH	;// ハードエンベ関連レジスタを戻す

	LD	A,(RHENV)	;// リズム音色内で
	OR	A		;// R#13への書き込みを実行していなければ
	JR	Z,RHYOF0	;// 以下処理しない
RHYOFH:
	CALL	HEPWT		;// ハードエンベ周期を戻す
	INC	E		;// ハードエンベ形状を戻す
	LD	A,(HENVSW)
	CALL	WPSGM

RHYOF0:
	LD	A,(SEBAKR)
	OR	A
	JR	Z,RHYOF1

	CALL	PRRET1		;// レジスタ復帰
	CALL	PRRET0

	XOR	A
	LD	(SEBAKR),A
RHYOF1:
	RET

HEPWT:
	LD	E,#11		;// ハードエンベ周期を戻す
	LD	A,(HENVPW)
	CALL	WPSGM
	INC	E
	LD	A,(HENVPW+1)
	JP	WPSGM

REEND:
	LD	A,#1		;// データ終端なら
	LD	(RNON),A	;// リズム発音フラグを1に (1=音程リセット)

;// 音長カウント

COUNTR:
	DEC	4(IX)		;// 音長カウンタを減らす（リズム用）
	RET	NZ

	LD	A,2(IX)
	OR	A
	CALL	NZ,RHYOFF	;// 休符以外かつカウンタが0になったらリズム初期化

	JR	MAIN1

COUNTL:
	LD	A,7(IX)
	LD	12(IX),A	;// レガート更新
COUNT:
	LD	A,51(IX)	;// リリースディレイカウンタが
	OR	A		;// 0なら
	JR	Z,COUNT1	;// スキップ
	CP	#255
	JR	Z,COUNT1	;// 255でもスキップ

	DEC	51(IX)		;// リリースディレイカウンタをデクリメントして
	JR	NZ,COUNT1	;// 0でなければ処理しない

	LD	A,29(IX)
	LD	51(IX),A	;// カウンタを戻す

	LD	A,2(IX)
	OR	A
	JR	NZ,COUNT0
	LD	A,41(IX)
COUNT0:
	CALL	NTOP
	LD	27(IX),L
	LD	28(IX),H	;// 音程を進める
	LD	59(IX),#1

COUNT1:
	DEC	4(IX)		;// 音長カウンタを減らす
	RET	NZ

;// データ読み込み

MAIN1:
	LD	L,0(IX)
	LD	H,1(IX)	;// 演奏中アドレス
MAIN1A:
	LD	A,5(IX)
	OR	A
	JR	Z,READ		;// 次の1バイトがカウンタじゃなければ次のデータを読む
	XOR	A
	LD	5(IX),A	;// 次の1バイトもカウンタかどうかのフラグをリセットして
	JP	NOTE2		;// カウント設定に飛ぶ

READ:
	LD	A,B
	CP	#4		;// 通常チャンネルかどうか
	JR	NZ,READ1

;// リズムチャンネル用データ読み込み

READR:
	LD	A,(HL)
	INC	HL

	OR	A
	JP	Z,RREST		;// 00H	休符

	CP	#0x40
	JP	C,RNOTE		;// 001nnnnnb (20-3FH) n番リズムを発音（続く任意バイトが音長）

	CP	#0x60
	JP	C,RVOLAD	;// 010nnnnnb (40-5FH) n番リズムの音量減算（続く1バイトが相対値）

	CP	#0x80
	JP	C,RVOLS		;// 011nnnnnb (60-7FH) n番リズムの音量加算（続く1バイトが相対値）

	CP	#0xC0
	JP	C,RVOL		;// 101nnnnnb (A0-BFH) n番リズムの音量（続く1バイトが音量）

	JP	TBLJFX

;// 通常チャンネル用データ読み込み

READ1:
	LD	A,(HL)
	INC	HL

	CP	#0x60
	JP	C,NOTE		;// 00-5FH	音程 (続く任意バイトが音長)

	CP	#0x70
	JP	C,VOL		;// 60-6FH	音量

	CP	#0x80
	JP	C,TONE		;// 70-7FH	音色番号

	CP	#0x90
	LD	DE,#CTBL8-0x80*2
	JP	C,TBLJ		;// 80-8FH	テーブルの各コマンド

	CP	#0xA0
	JP	C,HENV		;// 90-9FH	ハードエンベ

	CP	#0xB0
	LD	DE,#CTBLA-0xA0*2
	JR	C,TBLJ		;// A0-AFH	各コマンド

	CP	#0xC0
	JP	C,VOLP		;// B0-BFH 音量加算

	CP	#0xD0
	JP	C,VOLM		;// C0-CFH 音量減算

TBLJFX:
	LD	DE,#CTBLF-0xF0*2	;// F0-FFH テーブルの各コマンド
TBLJ:
	PUSH	HL
	LD	H,#0
	LD	L,A
	ADD	HL,HL
	ADD	HL,DE
	LD	E,(HL)
	INC	HL
	LD	D,(HL)
	PUSH	DE
	POP	IY
	POP	HL
	JP	(IY)

CTBL8:	.DW	SETPED,MIXMOD,SETPEV,NFREQ ,LEGATO,LEGATO,QGATE ,DETUNE,PORTA ,DTUNER,SDELAY,SDELAY,SDLY_S,RRSET ,DTUNE2,PENV6B	;// 80-8F
CTBLA:	.DW	HENVP ,PORTA2,Q2GATE,SUS   ,SETNEV,SETVI ,FRQSET,VIMAX ,PORTAP,RDPRST,Q3GATE,PRMSAV,SEPLY			;// A0-AC
CTBLF:	.DW	FSET  ,REPSTA,REPESC,REPEND,RITSET,CHEND ,CHEND ,CHEND ,CHEND ,CHEND ,CHEND ,CHEND ,YCMD  ,SLWSET,FFSET ,CHEND	;// F0-FF

;// キーオン

NOTE:
	PUSH	HL

	LD	D,A		;// 新しいノート番号をDに退避

	LD	A,2(IX)	;// 古いノート番号を読む
	OR	A
	JR	Z,NOTE0
	LD	41(IX),A	;// 休符でなければ保存
NOTE0:
	LD	C,A		;// 古いノート番号をCに退避
	LD	A,D		;// 新しいノート番号を読む
	LD	2(IX),A	;// 新しいノート番号を保存
	OR	A
	LD	D,A
	JP	Z,NOTE1P	;// 休符ならキーオンフラグなどをセットしない

	LD	59(IX),#1
	LD	31(IX),A	;// キーオンフラグをセット

	LD	A,29(IX)	;// リリースディレイ設定値を
	LD	51(IX),A	;// リリースカウンタにセット

NOTE1:
	LD	A,38(IX)	;// ポルタメント値が
	OR	A		;// 設定されていれば
	JR	Z,NOTE11	;// ポルタメント加算値の
	LD	34(IX),A	;// 設定をする

NOTE11:
	LD	A,39(IX)	;// ポルタメント単発フラグ
	OR	A		;// が
	JR	Z,NOTE1P	;// 0ならスキップ

	INC	39(IX)		;// 加算しつつ…
	CP	#1		;// 加算前の値が1でも
	JR	Z,NOTE1P	;// スキップ

	XOR	A		;// 単発フラグが立っていて
	LD	39(IX),A	;// 前回と違う音程なら
	LD	38(IX),A	;// ポルタメントオフ
	LD	33(IX),A
	LD	34(IX),A

NOTE1P:
	LD	A,12(IX)	;// レガート確認
	OR	A
	JR	NZ,NOTE1R	;// レガート中ならスキップするかどうかの判定に飛ぶ

	CALL	DTNSAV
	JR	NOTE12		;// レガート中でなければキーオン時の各種設定に飛ぶ

NOTE1R:
	LD	A,13(IX)
	OR	14(IX)
	JR	Z,NOTE1E	;// 今の音程レジスタ値のワークが0ならピッチ更新せずスキップ

	LD	A,D
	CP	C
	JR	NZ,NOTE13	;// （レガート中で）前回と違う音程ならピッチ更新

	LD	A,19(IX)
	CP	49(IX)
	JR	NZ,NOTE1C	;// （レガート中で）前回と違うデチューン値ならピッチ更新

	LD	A,20(IX)
	CP	50(IX)
	JR	Z,NOTE1E	;// レガート中で前回と同じデチューン値ならピッチ更新せずスキップ
NOTE1C:
	CALL	DTNSAV

	LD	A,D
NOTE13:
	CALL	NPSET		;// ピッチ設定

	LD	A,11(IX)	;// ピッチエンベが
	AND	#0b00100000	;// 前のフレームからの相対値でなく元の音程からの場合は
	JR	NZ,NOTE1E	;// ビブラートの再設定をしない

	LD	C,#1		;// ビブラートディレイを書き込まない
	CALL	SETPA2

	JR	NOTE1E
NOTE12:
	LD	A,D
	CALL	NPSET		;// レガート中でなければピッチ設定

NOTE2S:
	LD	A,6(IX)	;// 音色番号
	LD	E,A
	CP	#0x90
	JR	C,NOTES0	;// 90H未満ならソフトエンベ

	LD	A,2(IX)
	OR	A
	JR	Z,NOTE2R	;// 休符ならミックスモードの設定をスキップ、ピッチエンベの再設定もしない

	LD	A,17(IX)	;// 音量半減
	OR	A		;// が
	JR	Z,NOTES2	;// 設定されていなければスキップ
	CALL	MIXOFF
	CALL	MIXWT		;// 音量半減が設定されていたらキーオン時のミックスモードを設定
	JR	NOTES2

NOTES0:
	CALL	SETVAD		;// 音色アドレス設定
NOTES1:
	LD	A,2(IX)
	OR	A
	JR	Z,NOTE2R	;// 休符ならピッチエンベの再設定をしない

NOTES2:
	LD	C,#0		;// ビブラートディレイ有効
	CALL	SETPAD		;// ピッチエンベアドレス設定
	CALL	SETNAD		;// ノートエンベアドレス指定
NOTE2R:

	XOR	A
	LD	48(IX),A	;// 音量インターバルをリセット
	LD	(VISPAN),A	;// 音量インターバル周期をリセット

	LD	A,42(IX)	;// 音量インターバルカウンタを設定
	AND	#0b01111111
	LD	47(IX),A

NOTE1E:
	POP	HL
NOTE2:
	LD	A,(HL)
	INC	HL
	LD	4(IX),A	;// 音長セット
	OR	A
	JP	Z,MAIN1A	;// 音長が0フレームでなければゲートタイム等を設定せずに次のデータを読む
	CP	#255
	JP	C,NOTE3
	LD	5(IX),A	;// 次の1バイトも音長
	LD	52(IX),A	;// ゲートタイムを255に
	LD	A,(HL)
	CP	#255		;// 更に次の1バイトが255ならクオンタイズは計算しない
	JP	Z,MEND
	LD	A,#255
NOTE3:
	PUSH	HL

	LD	H,A		;// 音長をHに

	LD	A,B
	CP	#CHNUM
	JR	Z,NOTE2E	;// リズムトラックならゲートタイム計算しない

	LD	A,57(IX)	;// 固定ゲートタイム
	OR	A		;// が
	JR	Z,NOTEQ		;// 0なら通常のクオンタイズ設定に

	CP	H		;// 音長と固定ゲートタイムを比較
	JR	C,NOTEGT	;// 固定ゲートタイムのほうが短ければそのままジャンプ
	LD	A,H		;// 長ければ固定ゲートタイムを音長と同じにする
	JR	NOTEGT
NOTEQ:
	LD	A,9(IX)	;// Qの値
	CP	#8
	JR	NZ,NOTE3A

	LD	A,40(IX)	;// @Qの値が
	CP	#129		;// 129以下なら
	JR	C,NOTE40	;// 減算へ
	JR	NOTEGT		;// 129以上でもQ8のときは加算しない
NOTE3A:
	LD	D,H
	LD	E,#0		;// 固定小数用
	SRL	D
	RR	E		;// /2
	SRL	D
	RR	E		;// /4
	SRL	D
	RR	E		;// /8
	LD	HL,#0x0000
	LD	C,B		;// Bレジスタを退避
	LD	B,A		;// Qの値
NOTE4:
	ADD	HL,DE
	DJNZ	NOTE4
	LD	B,C		;// Bレジスタを戻す

	LD	A,40(IX)	;// @Qの値を確認して
	CP	#129		;// 129以下なら
	JR	C,NOTE40	;// 減算へ

	SUB	#128		;// 129以上なら128引いて
	ADD	A,H		;// 加算
	JR	NC,NOTEGT
	LD	A,#254
	JR	NOTEGT

NOTE40:
	LD	A,H
	SUB	40(IX)		;// ゲートタイム減算(@Q)
	JR	C,NOTE41
	JR	NZ,NOTEGT
NOTE41:
	LD	A,#1
NOTEGT:
	LD	52(IX),A	;// ゲートタイム

	CP	29(IX)		;// リリースディレイの設定フレームと比較して
	JR	C,NOTE22	;// 音長のほうが小さければジャンプ
	LD	53(IX),#1	;// 小さくなければリリースディレイスキップフラグをセット
NOTE22:
	LD	53(IX),#0	;// 小さければリリースディレイスキップフラグをリセット

	LD	A,(RITRK)
	CP	B
	JR	NZ,NOTE2E

	LD	A,(SEMODE)
	OR	A
	JR	NZ,NOTE2E

	LD	A,13(IX)	;// 割り込まれトラックならリズム発音後に音程を戻すためのピッチ値を退避
	LD	(RPITCH),A
	LD	A,14(IX)
	LD	(RPITCH+1),A
NOTE2E:
	POP	HL
NOTE5:
	JP	MEND

NPSET:
	OR	A
	RET	Z

	CALL	NTOP		;// ノート番号からピッチを取得してHLに

	LD	A,53(IX)	;// リリースディレイスキップフラグが
	OR	A		;// 0なら
	JR	Z,NPSET1	;// 通常のピッチ設定

	LD	27(IX),L	;// リリースディレイ用の音程を
	LD	28(IX),H	;// 最新の音程にして
	LD	53(IX),#0	;// リリースディレイスキップフラグを下げる
	JR	NPSET2
NPSET1:
	PUSH	HL
	LD	A,41(IX)
	CALL	NTOP
	LD	27(IX),L
	LD	28(IX),H
	POP	HL
NPSET2:
	LD	13(IX),L	;// ワークにピッチを保存
	LD	14(IX),H
	LD	54(IX),L
	LD	55(IX),H

	RET

;// リズム休符

RREST:
	PUSH	HL
	LD	2(IX),#0
	JP	NOTE1E

;// リズムキーオン

RNOTE:
	PUSH	HL
	AND	#0b00011111
	INC	A
RNSET1:
	LD	2(IX),A	;// ノート番号を保存

	LD	D,#0
	LD	E,A
	LD	HL,#RVOLW-1
	ADD	HL,DE		;// リズム音量のワークを算出
	LD	A,(HL)
	LD	(RVWRK),A	;// 反映用のリズム音量を保存

	SLA	E
	LD	HL,#RADTBL-2
	ADD	HL,DE		;// リズム音色のテーブルを算出
	LD	A,(HL)
	LD	16(IX),A
	INC	HL
	LD	A,(HL)
	LD	17(IX),A	;// エンベロープポインタを設定

	LD	A,#2
	LD	(RNON),A	;// リズム発音中フラグを2に (2=リズム発音中)
	XOR	A
	LD	(RHENV),A	;// リズム内ハードエンベ使用フラグをリセット

	JP	NOTE1E

;// 音量

VOL:
	AND	#0x0F
VOL1:
	LD	3(IX),A	;// 音量を設定
	JP	READ1

VOLP:
	AND	#0x0F		;// 音量を加算
	ADD	A,3(IX)
	CP	#15
	JR	C,VOL1
	LD	A,#15
	JR	VOL1

VOLM:
	AND	#0x0F		;// 音量を減算
	LD	D,A
	LD	A,3(IX)
	SBC	A,D
	JR	NC,VOL1
	XOR	A
	JR	VOL1

;// リズム音量 (101xxxxxb 続く1バイトが音量）

RVOL:
	LD	C,(HL)		;// 音量値
	INC	HL

	PUSH	HL

	AND	#0b00011111
	CP	#31
	JR	Z,RVOL0

	CALL	RVADDR
	LD	(HL),C

	POP	HL
	JP	READR

RVOL0:
	LD	D,B
	CALL	RVOLA
	LD	B,D

	POP	HL
	JP	READR

;// 全リズム音量 (C<-音量)

RVOLA:
	LD	B,#26
	LD	HL,#RVOLW
RVOLA0:
	LD	(HL),C
	INC	HL
	DJNZ	RVOLA0
	RET

;// リズム音量加算 (011xxxxxb 続く1バイトが音量）

RVOLAD:
	LD	C,(HL)		;// 音量加算値
	INC	HL

	PUSH	HL

	AND	#0b00011111
	CP	#31
	JR	Z,RVOLA2

	CALL	RVADDR
	LD	A,(HL)
	ADD	A,C
	AND	#0x0F
	LD	(HL),A
	POP	HL
	JP	READR

;// 全リズム音量加算 (C<-音量)

RVOLA2:
	LD	D,B

	LD	B,#26
	LD	HL,#RVOLW
RVOLAA0:
	LD	A,(HL)
	ADD	A,C
	AND	#0x0F
	LD	(HL),A
	INC	HL
	DJNZ	RVOLAA0

	LD	B,D

	POP	HL
	JP	READR

;// リズム音量減算 (111xxxxxb 続く1バイトが音量）

RVOLS:
	LD	C,(HL)		;// 音量減算値
	INC	HL

	PUSH	HL

	AND	#0b11111
	CP	#31
	JR	Z,RVOLS2

	CALL	RVADDR
	LD	A,(HL)
	SUB	C
	AND	#0x0F
	LD	(HL),A
	POP	HL
	JP	READR

;// 全リズム音量減算 (C<-音量)

RVOLS2:
	LD	D,B

	LD	B,#26
	LD	HL,#RVOLW
RVOLSA0:
	LD	A,(HL)
	SUB	C
	AND	#0x0F
	LD	(HL),A
	INC	HL
	DJNZ	RVOLSA0

	LD	B,D

	POP	HL
	JP	READR

;// リズム音量アドレスをHLに返す (D<-リズム番号)

RVADDR:
	LD	HL,#RVOLW
	LD	D,#0
	LD	E,A
	ADD	HL,DE
	RET

;// 音色指定

TONE:
	LD	C,6(IX)	;// 旧音色番号をCに退避

	AND	#0x0F
	LD	6(IX),A	;// 音色番号を設定

	PUSH	HL
	LD	E,A
	LD	A,12(IX)	;// レガート確認
	OR	A
	CALL	NZ,SETVAD	;// レガート中なら即座に音色アドレス設定

	LD	A,C		;// 旧音色番号が
	CP	#0x90		;// ハードエンベでなければ
	JR	C,TONE1		;// スキップ
	LD	A,17(IX)	;// 音量半減カウンタ設定が
	OR	A		;// 0なら
	JR	Z,TONE1		;// スキップ
	CALL	MIXTWT		;// 0以外ならトーン有効・ノイズ無効に
TONE1:
	POP	HL

	JP	READ1

;// ハードエンベ指定

HENV:
	LD	6(IX),A	;// 音色番号を設定

	LD	A,(HL)
	INC	HL
	LD	16(IX),A	;// 減衰カウンタ
	LD	17(IX),A	;// 減衰カウンタ設定

	JP	READ1

;// ハードエンベ周期

HENVP:
	LD	A,(HL)
	LD	(HENVPW),A
	INC	HL
	LD	A,(HL)
	LD	(HENVPW+1),A
	CALL	HEPWT
	INC	HL
	JP	READ1

;// サスティン

SUS:
	LD	A,(HL)		;// サスティン値
	INC	HL
	LD	8(IX),A
	CALL	NOTES
	JP	READ1

;// 音量インターバル

SETVI:
	LD	A,(HL)		;// ビット7が立ってたらマイナス、そうでなければプラス、ビット0～6でフレーム数
	OR	A
	JR	NZ,SETVI1
	LD	56(IX),A	;// 設置値が0なら到達音量も0にする
SETVI1:
	LD	42(IX),A	;// 設定値
	AND	#0b01111111
	LD	47(IX),A	;// カウンタ
	INC	HL
	JP	READ1

;// ノイズ周波数

NFREQ:
	LD	A,(HL)
	LD	(NFREQW),A
	INC	HL
	JP	READ1

;// ピッチエンベ指定

SETPEV:
	CALL	SETPES
	LD	C,12(IX)	;// レガート中なら即座にピッチエンベアドレス設定
	JP	READ1

SETPES:
	LD	A,(HL)		;// ピッチエンベ値
	INC	HL
	LD	11(IX),A

	LD	A,C
	OR	A
	LD	C,#0
	PUSH	HL
	CALL	NZ,SETPAD	;// Cが0以外なら即座にピッチエンベアドレス設定
	POP	HL
	RET

;// ノートエンベ指定

SETNEV:
	CALL	SETNES
	LD	C,12(IX)	;// レガート中なら即座にノートエンベアドレス設定
	JP	READ1

SETNES:
	LD	A,(HL)		;// ノートエンベ値
	INC	HL
	LD	43(IX),A

	LD	A,C
	OR	A
	PUSH	HL
	CALL	NZ,SETNAD	;// Cが0以外なら即座にノートエンベアドレス設定
	POP	HL
	RET

;// レガート

LEGATO:
	AND	#1		;// レガートON/OFF
	LD	7(IX),A
	JP	READ1

;// クオンタイズ（8段階ゲートタイム）

QGATE:
	LD	57(IX),#0	;// ;// 固定ゲートタイムオフ

	LD	A,9(IX)
	OR	A
	JR	NZ,QGATE2
	LD	7(IX),#0	;// 実行時にQ0ならレガートオフ
QGATE2:
	LD	A,(HL)		;// クオンタイズ値
	INC	HL
	LD	9(IX),A
	OR	A
	JP	NZ,READ1

	INC	A
	LD	7(IX),A	;// Q0ならレガートオン

	JP	READ1

;// クオンタイズ2（減算ゲートタイム）

Q2GATE:
	LD	57(IX),#0	;// ;// 固定ゲートタイムオフ

	LD	A,(HL)
	INC	HL
	LD	40(IX),A
	JP	READ1

;// クオンタイズ3（固定ゲートタイム）

Q3GATE:
	LD	A,(HL)
	INC	HL
	LD	57(IX),A
	JP	READ1

;// デチューン

DETUNE:
	CALL	DTNSAV

	LD	A,(HL)
	INC	HL
	LD	19(IX),A	;// デチューン(0=OFF)

	CP	#0x80
	JR	C,DETUN1

	LD	20(IX),#255
	JP	READ1
DETUN1:
	LD	20(IX),#0
	JP	READ1

;// デチューン(2バイト用)

DTUNE2:
	CALL	DTNSAV
	LD	A,(HL)
	LD	19(IX),A
	INC	HL
	LD	A,(HL)
	LD	20(IX),A
	INC	HL

	JP	READ1

;// 前回のデチューン値を保存

DTNSAV:
	LD	A,19(IX)
	LD	49(IX),A
	LD	A,20(IX)
	LD	50(IX),A
	RET

;// 相対デチューン

DTUNER:
	LD	E,(HL)
	INC	HL
	LD	D,(HL)
	INC	HL

	PUSH	HL

	LD	L,19(IX)
	LD	49(IX),L
	LD	H,20(IX)
	LD	50(IX),H	;// 前回のデチューン値をHLに入れつつワークに保存

	ADD	HL,DE

	LD	19(IX),L
	LD	20(IX),H

	POP	HL

	JP	READ1

;// ピッチエンベ番号のビット6を設定

PENV6B:
	LD	A,(HL)
	INC	HL
	OR	A
	LD	A,11(IX)
	JR	Z,PENV6Z
	OR	#0b01000000
	JR	PENV6S
PENV6Z:
	AND	#0b10111111
PENV6S:
	LD	11(IX),A
	JP	READ1

;// ピッチエンベのディレイ値を書き換え

SETPED:
	LD	C,(HL)
	INC	HL

	LD	A,11(IX)
	OR	A
	JP	Z,READ1

	PUSH	HL
	CALL	GETPAD
	LD	(HL),C
	POP	HL

	JP	READ1

;// ポルタメント

PORTA:
	LD	A,(HL)
	INC	HL
	LD	37(IX),#0	;// ポルタメント値(小数部)
	LD	38(IX),A	;// ポルタメント値(整数部, 0=OFF)

	OR	A
	JP	Z,PORTA1	;// P0ならリセット処理

	LD	A,13(IX)
	LD	E,14(IX)
	OR	E
	JR	NZ,PORTA0

	LD	A,(O4C+1)
	LD	E,A
	LD	A,(O4C)
PORTA0:
	LD	35(IX),A
	LD	36(IX),E	;// 開始ピッチを設定

	LD	39(IX),#0	;// ポルタメント単発フラグはOFF
	JP	READ1
PORTA1:
	LD	34(IX),A	;// ポルタメント加算値を
	LD	33(IX),A	;// 0にする
	JP	READ1

;// ポルタメント2

PORTA2:
	LD	39(IX),#1	;// ポルタメント単発フラグをON

	LD	A,(HL)
	INC	HL
	LD	37(IX),A	;// ポルタメント設定値(小数部)

	LD	A,(HL)
	INC	HL
	LD	38(IX),A	;// ポルタメント設定値(整数部, 0=OFF)

;// ポルタメントピッチ設定

PORTAP:
	LD	A,(HL)		;// ノート番号を取得
	INC	HL

	PUSH	HL
	CALL	NTOP		;// 音程データを取得して
	LD	35(IX),L	;// 開始ピッチを設定
	LD	36(IX),H
	POP	HL

	JP	READ1

;// リリースディレイのピッチをリセット

RDPRST:
	LD	53(IX),#1
	JP	READ1

;// パラメータ退避・復帰

PRMSAV:
	LD	A,(HL)
	INC	HL

	AND	#0x80
	JR	Z,PRMSV1

	LD	A,58(IX)	;// 退避値を
	LD	3(IX),A	;// 設定音量に復帰
	JP	READ1
PRMSV1:
	LD	A,3(IX)	;// 設定音量を
	LD	58(IX),A	;// 退避
	JP	READ1

;// 音量インターバル最大値を設置

VIMAX:
	LD	A,(HL)
	INC	HL
	LD	56(IX),A
	JP	READ1

;// 周波数テーブル書き換え

FRQSET:
	LD	D,#0
	LD	E,(HL)
	INC	HL
	PUSH	HL
	LD	HL,#PTABLE
	ADD	HL,DE
	EX	DE,HL		;// 音程データから音程テーブルのアドレスを求めてHLに
	POP	HL

	LD	A,B
	LDI
	LDI
	LD	B,A
	JP	READ1

;// リリースディレイ

SDELAY:
	AND	#1
	JR	Z,SDLY1
	LD	A,#255		;// デフォルトでは音程を進ませるフレーム数を255にする（255=進ませない）
SDLY1:
	LD	29(IX),A	;// リリースディレイ値
	JP	READ1

;// リリースディレイ（ディレイ値を設定）

SDLY_S:
	LD	A,(HL)		;// ディレイ値を指定する場合
	INC	HL
	JR	SDLY1

;// ミックスモード

MIXMOD:
	LD	D,(HL)
	LD	A,6(IX)
	CP	#0x90
	LD	A,D
	JR	C,MIXMD1
	LD	(MIXNMH),A
MIXMD1:
	INC	HL
	CALL	MIXSWT
	JP	READ1

MIXTWT:
	CALL	MIXT
	CALL	MIXWT
	RET
MIXSWT:
	PUSH	HL
	CALL	MIXSUB
	CALL	MIXWT
	POP	HL
	RET

MIXWT:
	LD	C,A
	CALL	MIX_IY
	LD	(IY),C
	RET

MIX_IY:
	LD	IY,#MIXWRK	;// 効果音モードかどうかでミックスモードのワークを判断してIYに返す
	LD	A,(SEMODE)
	OR	A
	RET	Z
	INC	IY
	RET

MIXSUB:
	OR	A
	JR	Z,MIXOFF
	CP	#1
	JR	Z,MIXT
	LD	60(IX),A	;// *nの値を保存
	CP	#2
	JR	Z,MIXN
MIXON:
	LD	HL,#MIXTB3-1
	LD	E,B
MIXAND:
	LD	D,#0
	ADD	HL,DE
	CALL	MIX_IY
	LD	A,(IY)
	AND	(HL)
	RET
MIXOFF:
	LD	HL,#MIXTB0-1
	LD	E,B
	LD	D,#0
	LD	60(IX),D	;// *0を保存
	ADD	HL,DE
	CALL	MIX_IY
	LD	A,(IY)
	AND	#0b00111111
	JR	MIXAO1
MIXT:
	LD	HL,#MIXTB1-2
	LD	60(IX),#1	;// *1を保存
MIXAO:
	LD	A,B
	ADD	A,A
	LD	E,A
	CALL	MIXAND
	INC	HL
MIXAO1:
	OR	(HL)
	RET
MIXN:
	LD	HL,#MIXTB2-2
	JR	MIXAO

;// 		ChCBACBA
;// 		10NNNTTT
MIXTB0:
	.DB	0b00100100;// 	CH.C OR トーンOFF ノイズOFF
	.DB	0b00010010;// 	CH.B OR トーンOFF ノイズOFF
	.DB	0b00001001;// 	CH.A OR トーンOFF ノイズOFF
MIXTB1:
	.DB	0b00111011;// 	CH.C AND トーンON
	.DB	0b00100000;// 	CH.C  OR ノイズOFF
	.DB	0b00111101;// 	CH.B AND トーンON
	.DB	0b00010000;// 	CH.B  OR ノイズOFF
	.DB	0b00111110;// 	CH.A AND トーンON
	.DB	0b00001000;// 	CH.A  OR ノイズOFF
MIXTB2:
	.DB	0b00011111;// 	CH.C AND ノイズON
	.DB	0b00000100;// 	CH.C  OR トーンOFF
	.DB	0b00101111;// 	CH.B AND ノイズON
	.DB	0b00000010;// 	CH.B  OR トーンOFF
	.DB	0b00110111;// 	CH.A AND ノイズON
	.DB	0b00000001;// 	CH.A  OR トーンOFF
MIXTB3:
	.DB	0b00011011;// 	CH.C AND トーンON ノイズON
	.DB	0b00101101;// 	CH.B AND トーンON ノイズON
	.DB	0b00110110;// 	CH.A AND トーンON ノイズON

;// Yコマンド

YCMD:
	LD	E,(HL)
	INC	HL
	LD	A,(HL)
	INC	HL
	CALL	WPSGM
	JP	READRT

;// リリース音量設定

RRSET:
	LD	A,(HL)
	INC	HL

	CP	#128
	JR	C,RRSET1	;// ビット7が立っていなければ設定値に反映
	AND	#15
	LD	24(IX),A	;// 立っていたら現在の値を書き換え
	LD	26(IX),#1
	JP	READ1
RRSET1:
	LD	30(IX),A
	CALL	NOTER
	JP	READ1

;// フェードアウト設定

FSET:
	LD	E,(HL)		;// フェード値
	INC	HL
	LD	D,(HL)		;// ループ数
	INC	HL

	LD	A,(LCOUNT)	;// ワークのループ数が0なら値セット
	OR	A
	JR	NZ,MFSET1

	LD	A,(MFADE)
	OR	A
	JP	NZ,READRT

	LD	A,D
MFSET1:
	DEC	A
	LD	(LCOUNT),A
	JP	NZ,READRT

	LD	A,E
	LD	(MFADE),A
	JP	READRT

;// スロー設定

SLWSET:
	LD	A,(HL)
	INC	HL
	LD	(SLWPRM),A
	JP	READRT

;// 早送り設定

FFSET:
	LD	A,(HL)
	INC	HL
	LD	(FFFLG),A
	JP	READRT

;// 割り込みトラック選択

RITSET:
	LD	A,(HL)
	INC	HL
	LD	(RITRK),A	;// 割り込まれトラック (1=CH.C 2=CH.B 3=CH.A)

	LD	C,A
	LD	A,#11
	SUB	C
	LD	(RVREG),A	;// リズムトラックの音量レジスタ

	SUB	#8
	ADD	A,A
	INC	A
	LD	(RPREG),A	;// リズムトラックの周波数レジスタ(上位)

	JP	READRT

;// SEを発音

SEPLY:
	LD	E,(HL)
	INC	HL
	LD	D,(HL)
	INC	HL

	CALL	SEPSUB

	JP	READRT

;// SE発音サブ（DE←効果音データ先頭アドレス）

SEPSUB:
	PUSH	IX
	PUSH	HL
	PUSH	BC

	PUSH	DE

	LD	IX,#SE1WRK
	LD	IY,#CH2WRK
	LD	HL,#SEBAKT
	LD	DE,#WSIZE
	LD	B,#CHNUM-1
SEPLY1:
	LD	A,10(IX)	;// 効果音が
	OR	A		;// 発音中でなければ
	JR	Z,SEPLY2	;// スキップする

	LD	A,(HL)		;// 効果音発音中なら
	LD	10(IY),A	;// BGM側のトラック有効フラグを戻す
SEPLY2:
	ADD	IX,DE
	ADD	IY,DE
	INC	HL
	DJNZ	SEPLY1

	POP	DE

	LD	A,D
	CP	#0x40
	JR	C,SEPLY3	;// 効果音アドレスが4000H未満なら相対値として扱う

	LD	H,D
	LD	L,E
	JR	SEPLY4
SEPLY3:
	LD	HL,(BGMADR)
	ADD	HL,DE
SEPLY4:
	LD	IX,#SE1WRK
	LD	B,#CHNUM-1
	CALL	INIADR		;// 効果音データ先頭アドレス設定

	LD	IX,#CH2WRK
	LD	IY,#SE1WRK
	LD	HL,#SEBAKT
	LD	DE,#WSIZE
	LD	B,#CHNUM-1
SEMOF1:
	LD	A,10(IX)	;// BGMのチャンネル有効フラグ
	LD	(HL),A
	OR	A
	JR	Z,SEMOFL	;// 0ならスキップ

	LD	A,10(IY)	;// SEのチャンネル有効フラグ
	OR	A
	JR	Z,SEMOFL	;// 0ならスキップ
	LD	10(IX),#255	;// 0でなければBGM側をミュート状態へ

SEMOFL:
	ADD	IX,DE
	ADD	IY,DE
	INC	HL
	DJNZ	SEMOF1

	LD	A,(MIXWRK)
	AND	#0b00111111	;// 最上位ビットを0にして
	LD	(MIXWRS),A	;// 効果音用ミックスワークに転送
	POP	BC
	POP	HL
	POP	IX

	RET

;// リピート開始

REPSTA:
	INC	32(IX)		;// ネスト加算
	CALL	REPADD
	LD	(IY),L
	LD	1(IY),H	;// リピート開始アドレス設定
	LD	4(IY),#255	;// リピート回数を255に仮設定

	JP	READRT

;// リピート終了

REPEND:
	CALL	REPADD
	LD	A,4(IY)	;// リピート回数確認
	CP	#255
	JR	NZ,REPEN1

	LD	A,(HL)		;// リピート回数が仮設定の状態(255)なら指定した回数を読み出して
	LD	4(IY),A	;// ワークに保存する

	INC	HL
	LD	2(IY),L	;// 終了アドレスを保存
	LD	3(IY),H

REPEN1:
	DEC	A		;// リピート回数を減算して
	LD	4(IY),A	;// ワークに保存する
	JR	Z,REPEN2

	LD	L,(IY)		;// リピート回数が0じゃなければ
	LD	H,1(IY)	;// ポインタをリピート開始アドレスに戻す
	JP	READRT

REPEN2:
	LD	L,2(IY)	;// ポインタをリピート終了アドレスにする
	LD	H,3(IY)
	DEC	32(IX)		;// ネスト減算
	JP	READRT

;// リピート脱出

REPESC:
	CALL	REPADD
	LD	A,4(IY)
	CP	#2
	JR	C,REPEN2	;// リピート回数が1以下ならポインタをリピート終了アドレスに
	JP	READRT

;// リピートのネスト数からワークエリア参照アドレス加算

REPADD:
	LD	IY,#CH1RWK

	LD	A,#4
	SUB	B
	JR	Z,REPAD2

	LD	C,B
	LD	B,A
	LD	DE,#RWSIZE
REPAD1:
	ADD	IY,DE
	DJNZ	REPAD1
	LD	B,C
REPAD2:
	LD	A,32(IX)
	DEC	A
	LD	D,A
	ADD	A,A
	ADD	A,A
	ADD	A,D
	LD	D,#0
	LD	E,A
	ADD	IY,DE

	RET

;// トラックに応じて読み込み処理にジャンプ

READRT:
	LD	A,B
	CP	#4		;// 通常チャンネルかどうか
	JP	Z,READR
	JP	READ1

;// 音色アドレス設定 (E<-音色番号)

SETVAD:
	LD	HL,#VADTBL	;// 音色アドレステーブルに
	LD	D,#0
	SLA	E		;// 音色番号*2
	ADD	HL,DE		;// を加算
	LD	A,(HL)
	LD	16(IX),A
	INC	HL
	LD	A,(HL)
	LD	17(IX),A	;// 音色アドレスを設定
	RET

;// ピッチエンベアドレス設定
;// (C<-0 ディレイ値を設定する C<-1 ディレイ値を設定しない、C<-2以上 ディレイをこの値にする)

SETPAD:
	XOR	A
	LD	18(IX),A	;// エンベロープウェイトカウンタをリセット

SETPA2:
	LD	A,11(IX)	;// ピッチエンベ番号
	OR	A
	RET	Z

	CALL	GETPAD

	LD	A,C
	CP	#1
	JR	C,SETPA4
	JR	Z,SETPA1
	JR	SETPA3
SETPA4:
	LD	A,(HL)
SETPA3:
	LD	23(IX),A	;// ピッチエンベディレイカウンタを設定
SETPA1:
	INC	HL
	LD	21(IX),L	;// ピッチエンベポインタ下位を設定
	LD	22(IX),H	;// ピッチエンベポインタ上位を設定
	RET

;// ピッチエンベの実アドレスを取得

GETPAD:
	AND	#0b00011111
	DEC	A
	ADD	A,A
	LD	D,#0
	LD	E,A
	LD	HL,#PADTBL	;// ピッチエンベ番号からアドレスを求める
	ADD	HL,DE

	LD	E,(HL)
	INC	HL
	LD	D,(HL)
	EX	DE,HL
	RET

;// ノートエンベアドレス指定 (A<-ノートエンベ番号)

SETNAD:
	LD	A,43(IX)
	OR	A
	RET	Z

	AND	#0x7F
	LD	43(IX),A

	DEC	A
	ADD	A,A
	LD	D,#0
	LD	E,A
	LD	HL,#NADTBL
	ADD	HL,DE

	LD	A,(HL)
	LD	44(IX),A	;// ノートエンベポインタ下位を設定
	INC	HL
	LD	A,(HL)
	LD	45(IX),A	;// ノートエンベポインタ上位を設定

	RET

;// A→ノート番号 周波数レジスタ値→HL

NTOP:
	OR	A
	RET	Z

	ADD	A,A
	LD	D,#0
	LD	E,A
	LD	HL,#PTABLE-2
	ADD	HL,DE		;// 音程データから音程テーブルのアドレスを求めてHLに

	LD	E,(HL)		;// 音程データを読み込む
	INC	HL
	LD	D,(HL)
	EX	DE,HL

	LD	E,19(IX)	;// デチューン値を
	LD	D,20(IX)	;// DEに読み込み
	ADD	HL,DE		;// 加算
	RET

;// 周波数レジスタを0に

PITCH0:
	CALL	REGP
	XOR	A
	CALL	WPSGM
	DEC	E
	JP	WPSGM		;// 周波数を0に

;// Bから各レジスタを求めてEに

REGP:
	LD	A,#3
	SUB	B
	ADD	A,A
	LD	E,A
	INC	E		;// 音程レジスタ
	RET

REGV:
	LD	A,#3
	SUB	B
	ADD	A,#8
	LD	E,A		;// 音量レジスタ
	RET

;// 全PSGレジスタを戻す（未使用）
;// 	IX=CHワーク先頭アドレス

PRRETA:
	LD	IX,#CH1WRK
	LD	B,#CHNUM-1
PRRETL:
	CALL	PRRET1

	LD	DE,#WSIZE
	ADD	IX,DE

	DJNZ	PRRETL

PRRET0:
	LD	E,#6		;// R#6
	LD	A,(NFREQW)	;// ノイズ周波数
	CALL	WPSG

	LD	A,(MIXWRK)	;// ミックスモードのワークを読んで
	AND	#0b00111111	;// 最上位ビットを0にして
	LD	(MIXWRK),A	;// ワークに戻す（メイン先頭でレジスタ書き込み発動）

	LD	A,6(IX)
	CP	#0x90
	RET	C

	LD	E,#11		;// R#11
	LD	A,(HENVPW)	;// ハードエンベ周期下位
	CALL	WPSG
	INC	E		;// R#12
	LD	A,(HENVPW+1)	;// ハードエンベ周期上位
	CALL	WPSG

	INC	E		;// R#13
	LD	A,(HENVSW)	;// ハードエンベ番号
	CALL	WPSG
	RET

PRRET1:
	LD	A,B
	CP	#CHNUM
	RET	NC

	CALL	REGP
	LD	A,14(IX)	;// 発音中の音程レジスタ上位
	CALL	WPSG

	DEC	E		;// R#1 R#3 R#5
	LD	A,13(IX)	;// 発音中の音程レジスタ下位
	CALL	WPSG

	CALL	REGV
	XOR	A		;// 音量は0に
	CALL	WPSG

	RET

;// トラック終了

CHEND:
	LD	A,(SEMODE)
	OR	A
	JR	Z,CHEBGM	;// BGMモードならBGM終端処理へ

	LD	10(IX),#255	;// SEの終端ならトラック有効フラグを255に

	PUSH	BC
	PUSH	IX
SEMON:
	LD	IX,#CH2WRK
	LD	IY,#SE1WRK
	LD	HL,#SEBAKT

	LD	DE,#WSIZE
	LD	B,#CHNUM-1
SEMON1:
	LD	A,10(IY)	;// SEトラック有効フラグ
	CP	#255
	JR	NZ,SEMON3	;// 255でなければスキップ
	XOR	A
	LD	10(IY),A	;// 255なら0にする

	LD	A,(HL)
	CP	#1
	JR	NZ,SEMON2
	INC	A
SEMON2:
	LD	10(IX),A	;// BGM側のトラック有効フラグを退避しておいた値に戻す
	OR	A
	;// JR	NZ,SEMON3

	LD	A,#11		;// もしBGM側が
	SUB	B		;// 未使用トラックなら
	LD	E,A		;// 音量レジスタを
	XOR	A		;// 0にする
	CALL	WPSGM

SEMON3:
	ADD	IX,DE
	ADD	IY,DE
	INC	HL

	DJNZ	SEMON1

	POP	IX
	POP	BC

	RET

CHEBGM:
	LD	E,#16
	LD	C,B
CHEND0:
	SRL	E
	DJNZ	CHEND0
	LD	B,C
	LD	A,(ENDTRW)	;// ユーザー参照用の終端到達フラグを
	OR	E		;// 立てて
	LD	(ENDTRW),A	;// 保存

	LD	A,(ENDTR)
	OR	E
	CP	#0b1111
	JR	NZ,CHEND1	;// すべてのチャンネルが終わっているかどうか

	EX	DE,HL
	LD	HL,#MCOUNT	;// 曲が終わっていたらループ回数を操作

	LD	IY,(BGMADR)
	LD	A,11(IY)	;// BGMデータから無限ループフラグを得る
	AND	#1
	JR	Z,CHEL1
	LD	(HL),#255	;// 無限ループしない曲ならループ回数=255
	JR	CHEL2
CHEL1:
	INC	(HL)		;// 無限ループする曲ならループ回数+1
CHEL2:
	EX	DE,HL
	LD	A,(ENDTRR)	;// ループするときはフラグを曲開始の状態にリセット
CHEND1:
	LD	(ENDTR),A	;// 終了トラックを更新

	LD	E,(HL)		;// ループアドレス下位
	INC	HL
	LD	D,(HL)		;// ループアドレス上位

	LD	A,E
	OR	D
	JR	Z,CHEND2	;// ループアドレスが0なら非ループの終端処理に

	LD	HL,(BGMADR)
	ADD	HL,DE		;// ループアドレスが0でなければ演奏アドレスのオフセットとして扱う
	LD	0(IX),L	;// 演奏アドレスを更新
	LD	1(IX),H

	LD	A,B
	CP	#4
	JP	Z,READR		;// リズムトラックかどうかで戻り先を選択
	JP	READ1

CHEND2:
	LD	HL,#RESTDATA	;// 休符を走らせるためのデータ

	LD	A,B
	CP	#4
	JP	Z,RREST		;// リズムトラックかどうかで戻り先を選択
	XOR	A
	JP	NOTE

;// メインルーチン終了

MEND:
	LD	0(IX),L	;// 演奏アドレスを更新
	LD	1(IX),H
	RET

;// ------	終端用データ

RESTDATA:
	.DB	254,255	;// 254=音長254フレーム, 255=トラック終了
	.DW	0	;// 0=ループアドレスなし(非ループ用終端処理にジャンプ)
