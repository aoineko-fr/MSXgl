# -*- coding: utf-8 -*-

#
# wav2pcm.py
#
# 11Khz or 8KHzの8bitモノラルのwavファイルからPCMデータを抽出して保存する。
# なお、wavコンテナの各情報は以下。
#
# RIFFチャンク（アドレス固定）
# - +0x0000〜+0x0003 4byte  0x52, 0x49, 0x46, 0x46 ('RIFF')
# - +0x0004〜+0x0007 4byte  +0x0008以降のデータサイズ
# - +0x0008〜+0x000B 4vyte  0x57, 0x41, 0x56, 0x45 ('WAVE')
# fmtチャンク（開始アドレスはRIFFチャンク直後、0x000cで固定）
# - +0x0000〜+0x0003 4byte  0x66, 0x6d, 0x74, 0x20 ('fmt ')
# - +0x0004〜+0x0007 4byte  fmtチャンクバイト数
# - +0x0008〜+0x0009 2byte  フォーマットコード (0x0001=PCM)
# - +0x000A〜+0x000B 2byte  チャンネル数
# - +0x000C〜+0x000F 4byte  サンプリングレート 0x2b11=11025(11KHz),0x1f40=8000(8KHz) 
# - +0x0010〜+0x0013 4byte  データ速度 ステレオだとサンプリングレートx2となる
# - +0x0014〜+0x0015 2byte  ブロックサイズ
# - +0x0016〜+0x0017 2byte  サンプルあたりのビット数 0x0008=8bit,0x0010=16bit
# - +0x0018〜+0x0019 2byte  拡張部分のサイズ、リニアPCMは存在しない
# - +0x001A〜               拡張部分
# dataチャンク（開始アドレスは任意）
# - +0x0000〜+0x0003 4byte  0x64, 0x61, 0x74, 0x61 ('data')
# - +0x0004〜+0x0007 4byte  波形データのバイト数
# - +0x0008〜               波形データ
# LISTチャンク（ない場合もある）
# - +0x0000〜+0x0003 4byte  0x4c, 0x49, 0x53, 0x54 ('LIST')
# - +0x0004〜+0x0007 4byte  +0x0008以降のデータサイズ
# - +0x0008〜               データ

import os
import sys
import traceback
import argparse
import binascii
import ffmpeg

# wavデータ
data = ""

# 現在の処理対象位置
pos = 0x0000

def main():
    '''
    メイン処理\n
    '''
    global data
    global pos

    # 変換要否
    _isNeedConv = False

    # 引数パース
    _argparser = argparse.ArgumentParser(description="Extract PCM data from a WAV file.")
    _argparser.add_argument("infile", help="WAV file name to input.")
    _argparser.add_argument("--outfile", "-o", help="PCM file name for output.", default="out.pcm")
    _argparser.add_argument("--rate", "-r", default="8", help="Specify the sampling rate of the PCM data to be created as 11 or 8. The default is 8 KHz.")
    _argparser.add_argument("--force", "-f", action="store_const", const="", help="Ignore output even if the file exists.")
    _argparser.add_argument("--ver", "-v", action="version", version="%(prog)s 0.4.0")
    _args = _argparser.parse_args()

    # 入力ファイルのフルパスを設定
    _inFilePath = filePathUtil(_args.infile)

    # 出力ファイルのフルパスを設定
    _outFilePath = filePathUtil(_args.outfile)

    # 引数チェック
    try:
        # 入力ファイルの拡張子は.wav以外はエラー
        if os.path.splitext(_args.infile)[1] != ".wav":
            raise Exception("File extension is not '.wav'.")

        # 入力ファイルの存在チェック
        if os.path.exists(_inFilePath) == False:
            raise Exception("File not found.")

        # 存在チェック
        # オプション --force(-f)が設定されている場合はエラーとしない
        if os.path.exists(_outFilePath) and _args.force == None:
            raise FileExistsError("Specified file already exists.")

        # rateは8,11以外はエラー
        if _args.rate not in["8", "11"]:
            raise Exception("Rate Error.")
        else:
            _rate = int(_args.rate)

    except Exception as e:
        print(traceback.format_exception_only(type(e), e)[0])
        sys.exit()

    # 入力ファイル取得
    getData(_inFilePath)

    # 取得したデータの長さ分処理する
    while pos < len(data)/2:

        # Chank名取得
        _chankName = getStringData()
        if _chankName == "RIFF":
            checkRIFFChank()
        elif _chankName == "fmt ":
            _isNeedConv = checkFmtChank(_rate)
            if _isNeedConv:
                # サンプリングレートを変更する必要がある場合は、
                # サンプリングレートを変更してデータを読み直し、現在の処理対象位置をリセット
                _tmpFilePath = convertRate(_inFilePath, _rate)
                getData(_tmpFilePath)
                pos = 0
                os.remove(_tmpFilePath)
        elif _chankName == "data":
            checkDataChank(_outFilePath)
        elif _chankName == "LIST":
            checkListChank()


def getData(filePath:str):
    '''
    入力ファイル取得処理\n
    \n
    Parameters\n
    ----------\n
    filePath : str
        入力ファイルパス
    '''
    global data

    try:
        # ファイル読み込み
        with open(filePath, mode="rb") as f:
            data = f.read().hex()
            f.close()

    except Exception as e:
        print(traceback.format_exception_only(type(e), e)[0])
        sys.exit()

    return


def checkRIFFChank():
    '''
    RIFFチャンクの処理\n
    '''
    global data
    global pos

#    # 全体のデータサイズ取得
#    _size = getInt4byteData()
    pos += 4

    return     


def checkFmtChank(rate:int) -> bool:
    '''
    fmtチャンクの処理\n
    \n
    Parameters\n
    ----------\n
    rate : int\n
        出力PCMデータのサンプリングレート\n
    \n
    Returns\n
    -------\n
    bool\n
        サンプリングレートを変更する必要がある場合はTrue\n
    '''
    global data
    global pos

    # 変換要否
    _isNeedConv = False

    # fmtチャンクバイト数取得
    _size = getInt4byteData()
    _pos_sv = pos

    try:
        # フォーマットコードチェック
        if getInt2byteData() != 0x0001:
            # PCMでなければエラー
            raise Exception("This is not PCM data.")

        # チャンネル数はスキップ
        pos += 2

        # サンプリングレートチェック
        _v = getInt4byteData()
        if _v != 0xac44 and _v != 0x2b11 and _v != 0x1f40: # ac44は44KHz,2b11は11KHz,1f40は8KHz
            # 8000以外であればエラー
            raise Exception("Sampling rate is not 44KHz/11Khz/8KHz.")

        # 変換要否チェック
        print("samplingrate=" + str(_v))
        if (_v == 0xac44) or (_v == 0x2b11 and rate != 11) or (_v == 0x1f40 and rate != 8):
            _isNeedConv = True
        else:
            _isNeedConv = False

        # データ速度はスキップ
        pos += 4

        # ブロックサイズはスキップ
        pos += 2

        # サンプルあたりのビット数はスキップ
        pos += 2

        # 次のチャンクの先頭にセット
        pos = _pos_sv + _size

    except Exception as e:
        print(traceback.format_exception_only(type(e), e)[0])
        sys.exit()

    return _isNeedConv


def checkDataChank(filePath:str):
    '''
    dataチャンクの処理\n
    \n
    Parameters\n
    ----------\n
    filePath : str\n
        出力ファイルパス
    '''
    global data
    global pos

    _size = getInt4byteData()
    _pos_sv = pos

    # 波形データのサイズを取得
    _datasize = getInt4byteData()

    try:
        # 波形データをバイトデータとして一時ファイルに出力
        with open(filePath, mode="wb") as _f:
            _f.write( bytes.fromhex( data[pos*2:(pos+_datasize)*2] ) )
            _f.close()
            print("output data size " + str(_size - 4) + "bytes.")

        # 次のチャンクの先頭にセット
        pos = _pos_sv + _size

    except Exception as e:
        print(traceback.format_exception_only(type(e), e)[0])
        sys.exit()

    return     


def checkListChank():
    '''
    LISTチャンクの処理\n
    '''
    global data
    global pos

    _size = getInt4byteData()

    # 次のチャンクの先頭にセット
    pos += _size

    return     


def getStringData() -> str:
    '''
    現在の処理対象位置から4byteのバイナリデータを取得して、キャラクタ文字列として返却します。\n
    この関数を実行すると、現在の処理対象位置が+4されます。\n
    \n
    Returns\n
    -------\n
    str\n
        4バイトのバイナリデータをキャラクタ文字列に変換した値\n
    '''
    global data
    global pos

    _result = ""
    for i in range(0, 4):
        _result += chr(int(data[(pos)*2:(pos+1)*2], 16))
        pos += 1

    return _result


def getInt2byteData() -> int:
    '''
    現在の処理対象位置から2byteのバイナリデータを取得して、数値として返却します。\n
    この関数を実行すると、現在の処理対象位置が+2されます。\n
    \n
    Returns\n
    -------\n
    int\n
        2バイトのバイナリデータを10進に変換した値\n
    '''
    global data
    global pos

    _result = getIntValue(pos, pos+1)
    pos += 2

    return _result


def getInt4byteData() -> int:
    '''
    現在の処理対象位置から4byteのバイナリデータを取得して、数値として返却します。\n
    この関数を実行すると、現在の処理対象位置が+4されます。\n
    \n
    Returns\n
    -------\n
    int\n
        4バイトのバイナリデータを10進に変換した値\n
    '''
    global data
    global pos

    _result = getIntValue(pos, pos+3)
    pos += 4

    return _result


def getIntValue(startPos, endPos):
    '''
    dataに格納された16進数文字列の指定位置の値をint型の数値に変換して返却します。\n
    \n
    Parameters\n
    ----------\n
    startPos : int\n
        開始位置\n
    endPos : int\n
        終了位置\n
    '''
    global data

    return int.from_bytes(binascii.a2b_hex(data[startPos*2:(endPos+1)*2]), "little")


def filePathUtil(path:str) -> str:
    '''
    ファイルパスユーティリティ\n
    引数のパスにディレクトリを含んでいない場合、カレントディレクトリを付与したフルパスを生成して返却します。\n
    \n
    Parameters\n
    ----------\n
    path : str\n
        ファイルパス
    \n
    Returns\n
    -------\n
    str\n
        フルパスに編集後の文字列\n
    '''
    # 入力ファイルのフルパスからファイル名を取得
    _filename = os.path.basename(path)

    # 入力ファイルのフルパスからファイルパスを取得
    _filepath = os.path.dirname(path)
    if _filepath == "" or _filepath == None:
        # ファイルパスが取得できなかった場合（ファイル名のみ指定された場合）は現在のパスを設定
        _filepath = os.path.dirname(__file__)

    return _filepath + os.sep + _filename


def convertRate(filePath:str, rate:int) -> str:
    '''
    wavファイルのサンプリングレートを変更する\n
    fmtチャンクのサンプリングレートと引数のサンプリングレートが違う場合は、
    引数のサンプリングレートに変更します。\n
    \n
    Parameters\n
    ----------\n
    filePath : str\n
        入力ファイルパス
    rate : int\n
        変換対象のサンプリングレート
    \n
    Returns\n
    -------\n
    str\n
        一時ファイルのフルパス\n
    '''

    # 一時ファイルパス生成
    _tmpFilePath = filePathUtil("tmp.wav")

    # ここでffmpegを実行
    try:
        _stream = ffmpeg.input(filePath)
        _stream = ffmpeg.output(
                            _stream, \
                            _tmpFilePath, \
                            ac=1, \
                            ar=(8000 if rate == 8 else 11025), \
                            acodec="pcm_u8")
        _stream = ffmpeg.overwrite_output(_stream)
        ffmpeg.run(_stream)

    except Exception as e:
        print(traceback.format_exception_only(type(e), e)[0])
        sys.exit()

    # 生成した一時ファイルのフルパスを返却
    return _tmpFilePath


if __name__ == "__main__":
    main()
