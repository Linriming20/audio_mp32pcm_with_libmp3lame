#### 前言

本demo是使用开源项目lame来将MP3数据解码成PCM数据。（环境：x86\_64 Ubuntu16.04 64位）

> 注：在下面【参考文章】中的第1篇里面提到解码出现过异常情况（hip: bitstream problem, resyncing skipping xxx bytes...），或许是因为我屏蔽了main.c中第88行的fseek(...)函数，在目前demo中提供的MP3文件以及网络下载的其他MP3文件解码均未发现此提示。




### 1、编译使用

**lame的编译：**(源码下载地址：[https://sourceforge.net/projects/lame/files/lame/](https://sourceforge.net/projects/lame/files/lame/))

```bash
tar xzf lame-3.100.tar.gz
cd lame-3.100/
./configure --prefix=$PWD/_install --enable-decoder
make
make install
```

**本demo的编译与使用：**

```bash
$ make clean && make DEBUG=1
$ 
$ ./mp32pcm 
Usage: 
    ./mp32pcm <in MP3 file> <out PCM file>
Examples: 
    ./mp32pcm audio/test1_44100_stereo.mp3 out1_44100_16bit_stereo.pcm
    ./mp32pcm audio/test2_22050_stereo.mp3 out2_22050_16bit_stereo.pcm
    ./mp32pcm audio/test3_22050_mono.mp3   out3_22050_16bit_mono.pcm
    ./mp32pcm audio/test4_8000_mono.mp3    out4_8000_16bit_mono.pcm
```



### 2、参考文章

 - [使用lame解码mp3\_Griza\_J的博客-CSDN博客\_lame 解码](https://blog.csdn.net/Griza_J/article/details/111063506)

 - [lame mp3解码 - 小小程序员001 - 博客园](https://www.cnblogs.com/musicfans/archive/2012/07/16/2819299.html)



### 附录（demo目录架构）

```bash
$ tree
.
├── audio
│   ├── out1_44100_16bit_stereo.pcm
│   ├── out2_22050_16bit_stereo.pcm
│   ├── out3_22050_16bit_mono.pcm
│   ├── out4_8000_16bit_mono.pcm
│   ├── test1_44100_stereo.mp3
│   ├── test2_22050_stereo.mp3
│   ├── test3_22050_mono.mp3
│   └── test4_8000_mono.mp3
├── docs
│   ├── lame mp3解码 - 小小程序员001 - 博客园.mhtml
│   └── 使用lame解码mp3_Griza_J的博客-CSDN博客_lame 解码.mhtml
├── include
│   └── lame
│       └── lame.h
├── lib
│   └── libmp3lame.a
├── main.c
├── Makefile
└── README.md
```

