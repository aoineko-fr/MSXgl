[ [Engligh](README.md) | [日本語](README.ja.md) ]

---
# PCMPlayer for MSX

## Overview

This program plays PCM data for MSX.  
The PCM data is 8-bit mono PCM data at 8KHz/11KHz.
This source code uses z80asm of z88dk. 

## License

MIT Lisence

## Execution Sample

You can check the operation of the sample program at the following URL.  
The program stops once it is played, so if you want to play it again, use WebMSX to perform the Reset operation.

https://webmsx.org/?MACHINE=MSX1J&ROM=https://github.com/aburi6800/msx-PSGPCMPlayer/raw/main/dist/sample.rom&FAST_BOOT

## Specification

- The PCM data that can be used with this PCM player is in the following format.  
    - Sampling rate: 11KHz or 8KMz (default 8KHz, 11KHz requires source modification)
    - Bit rate: 8bit
    - monoral
    > The default setting is 8 KHz playback.  
    > For 11KHz playback, enable line 87 and comment line 88.
- Prohibit any interruptions during playback.

## How to build

### To use cmake:

Edit the `CMakeLists.txt` included in this project to specify the source for this player (`psmplayer.asm`) and the source you created.   
For example, the sample program defines the following.  
```
add_source_files(
    ./src/msx/pcmplayer.asm
    ./src/msx/sample.asm
)
```

Next, run the following command in the project root directory to generate the make file.   
(First time only; not necessary after the second time)  
```
$ mkdir build && cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/z88dk.cmake ..
```

Execute the following command in the `build` directory to build.  
```
$ make clean && make
```

The `.rom` file will be created in the `dist` directory.  

### To use the zcc command:

Enter the source directory and execute the following command (replace the source file name with your own)  
This command is the same for both assembler and C.  
For details on other options, such as specifying the path to the include file, see the help displayed by `zcc -h`.  
```
$ zcc +msx -create-app -subtype=rom pcmplayer.asm sample.asm -o=../../dist/build.rom 
```

## How to use

- Prepare the data to be played back. (See "Create PCM Data" below)  
- Copy `pcmplayer.asm` to your project directory.  
- At the beginning of the program source, specify the following.  
```
    EXTERN PCMPLAY
```
- Specify the first address of the PCM data in the HL register and the data length (in bytes) in the DE register, and then execute the following.  
```
    CALL PCMPLAY
```
- During playback, interrupts are disabled and other processes are stopped. Interrupt is turned back on when playback is finished.  

## Create PCM data

A Python script (`wav2pcm.py`) is provided in this project to extract PCM data from WAV files.   
This script works in an environment with ffmpeg installed, so please install it beforehand.  
 (It can be downloaded [here](https://ffmpeg.org/))  

Follow the steps below to make.  

1. Create a wav file  
    The wav file to be created can be 44.1KHz 16bit stereo.  
    Cutting out the recorded data can be done using a variety of tools, but can also be done using the following sites.  
    https://audiotrimmer.com/

1. Extract PCM data  
    When running for the first time, install the required modules.
    ```
    $ cd src/python
    $ pip install -r requirements.txt
    ```
    It can then be used for  
    ```
    $ python wav2pcm.py [Input filename]
    ```
    By default, PCM data is created in the same directory as the script with the file name `out.pcm`.
    The following options are optional.  
    - -o Specifies the output file name
    - -f Force overwriting of existing files  
    - -r Specify sampling rate (8 or 11)  
    - -h help  

1. Include PCM data in the program to be used
    ```
    PCMDATA:
        INCBIN "[PCM filename]"
    PCMDATA_END:
        DB $7F
    ```
    > This `PCMDATA` will be a label indicating the starting address of the PCM data.  
    > `PCMDATA_END` is the label set to calculate the PCM data length.  
    > This results in `PCMDATA_END - PCMDATA` for the data length.  

## Release notes

### pcmplayer.asm

- 2022/11/27  Version 0.2.0
    - Added save/restore and initialization of PSG registers
    - Fixed a bug that Ch1-3 tones were not initialized when executed in WebMSX

- 2022/11/26  Version 0.1.0
    - initial creation

### wav2pcm.py

- 2022/12/03  Version 0.4.0
    - Supports sampling rate conversion of WAV files
    - Add -r option (specifies the sampling rate of the output PCM file)

- 2022/11/28  Version 0.3.0
    - Supported arguments
    - Supports processing of files in arbitrary paths

- 2022/11/27  Version 0.2.0
    - Fixed to correctly retrieve and process chunks of wav container

- 2022/11/26  Version 0.1.0
    - initial creation

## Thanks
[h1romas4](https://github.com/h1romas4)  
[超人MSX](http://hp.vector.co.jp/authors/VA054130/pcm1dm.html)  
[MSX Assembly Page](http://map.grauw.nl/articles/psg_sample.php)  
