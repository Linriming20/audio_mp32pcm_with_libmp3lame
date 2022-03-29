#include <stdio.h>
#include <stdlib.h>

#include "lame/lame.h"


/* just for debug */
#ifdef ENABLE_DEBUG
    #define DEBUG(fmt, args...)     printf(fmt, ##args)
#else
    #define DEBUG(fmt, args...)
#endif


#define INBUF_SIZE 		(4096)
#define BUF_SIZE 		(512)
#define MP3BUF_SIZE 	((int)(1.25 * BUF_SIZE) + 7200)


int main(int argc, char *argv[])
{
	char *inMp3FileName = NULL;
	char *outPcmFileName = NULL;
	FILE *fpInMP3 = NULL;
	FILE *fpOutPcm = NULL;
	short pcm_l[INBUF_SIZE];
	short pcm_r[INBUF_SIZE];
	unsigned char mp3Buf[MP3BUF_SIZE];
	hip_t hip = NULL;

	if (argc != 3)
	{
		printf("Usage: \n"
			   "    %s <in MP3 file> <out PCM file>\n"
			   "Examples: \n"
			   "    %s audio/test1_44100_stereo.mp3 out1_44100_16bit_stereo.pcm\n"
			   "    %s audio/test2_22050_stereo.mp3 out2_22050_16bit_stereo.pcm\n"
			   "    %s audio/test3_22050_mono.mp3   out3_22050_16bit_mono.pcm\n"
			   "    %s audio/test4_8000_mono.mp3    out4_8000_16bit_mono.pcm\n",
			   argv[0], argv[0], argv[0], argv[0], argv[0]);

		return -1;
	}
	else
	{
		inMp3FileName = argv[1];
		outPcmFileName = argv[2];
	}

	
	fpInMP3 = fopen(inMp3FileName, "rb");
	if(!fpInMP3)
	{
		perror("open input MP3 file failed");
		goto exit;
	}
	fpOutPcm = fopen(outPcmFileName, "wb");
	if(!fpOutPcm)
	{
		perror("open output PCM file failed");
		goto exit;
	}

	/* MP3 decode 1/4: init handler */
	hip = hip_decode_init();
	if(!hip)
	{
		printf("init mp3 decoder failed!\n");
		goto exit;
	}

	int readMP3Bytes = -1;
	int samples = -1;
	int samplesRate = -1;
	int channels = -1;
	mp3data_struct mp3Info = {};

	/* MP3 decode 2/4: read MP3 header info */
	do{
		readMP3Bytes =  fread(mp3Buf, 1, 16, fpInMP3);
		hip_decode_headers(hip, mp3Buf, readMP3Bytes, pcm_l, pcm_r, &mp3Info);

		samplesRate = mp3Info.samplerate;
		channels = mp3Info.stereo;		
	}while(!mp3Info.header_parsed && readMP3Bytes > 0);

	// don't need to reset file position
	//fseek(fpInMP3, 0, SEEK_SET);

	printf("Input MP3 Info: \n"
		   "    samplesRate: %d\n"
		   "    channels: %d\n",
		   samplesRate, channels);

	while(1)
	{
		readMP3Bytes = fread(mp3Buf, 1, 418, fpInMP3);
		if(readMP3Bytes <= 0)
			break;

		/* MP3 decode 3/4: decode MP3 data */
		samples = hip_decode(hip, mp3Buf, readMP3Bytes, pcm_l, pcm_r);
		if(samples > 0)
		{
			DEBUG("read MP3 bytes: %d \t decode output samples: %d\n", readMP3Bytes, samples);
			for(int i = 0; i < samples; i++)
			{
				fwrite(pcm_l + i, 2, 1, fpOutPcm); /* pcm_l/pcm_r type is 'short', so the 'i' don't to multi 2 */
				if(channels == 2)
				{
					fwrite(pcm_r + i, 2, 1, fpOutPcm);
				}
			}
		}
	}

	printf("\e[32m%s -> %s success\e[0m\n", inMp3FileName, outPcmFileName);

exit:
	/* MP3 decode 4/4: decode exit */
	if(hip) hip_decode_exit(hip);

	if(fpOutPcm) {fflush(fpOutPcm);fclose(fpOutPcm);}
	if(fpInMP3) fclose(fpInMP3);

	return 0;
}
