/* LZ48 simple C source file / crappy version by roudoudou - Flower Corp. 2016 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

int LZ48_encode_extended_length(unsigned char *odata, int length)
{
	int ioutput=0;

	while (length>=255) {
		odata[ioutput++]=0xFF;
		length-=255;
	}
	/* if the last value is 255 we must encode 0 to end extended length */
	odata[ioutput++]=length;
	return ioutput;
}

int LZ48_encode_block(unsigned char *odata,unsigned char *data, int literaloffset,int literalcpt,int offset,int maxlength)
{
	int ioutput=1;
	int token=0;
	int i;

	if (offset<0 || offset>255) {
		fprintf(stderr,"internal offset error!");
		exit(-2);
	}
	
	if (literalcpt<15) {
		token=literalcpt<<4; 
	} else {
		token=0xF0;
		ioutput+=LZ48_encode_extended_length(odata+ioutput,literalcpt-15);
	}

	for (i=0;i<literalcpt;i++) odata[ioutput++]=data[literaloffset++];

	if (maxlength<18) {
		if (maxlength>2) {
			token|=(maxlength-3);
		} else {
			/* endoffset has no length */
		}
	} else {
		token|=0xF;
		ioutput+=LZ48_encode_extended_length(odata+ioutput,maxlength-18);
	}

	odata[ioutput++]=offset-1;
	
	odata[0]=token;
	return ioutput;
}

unsigned char *LZ48_encode(unsigned char *data, int length, int *retlength)
{
	int i,startscan,current=1,token,ioutput=1,curscan;
	int maxoffset,maxlength,matchlength,literal=0,literaloffset=1;
	unsigned char *odata=NULL;
	
	if (!length) return NULL;

	odata=malloc(length*1.1);
	if (!odata) {
		fprintf(stderr,"memory full");
		exit(-1);
	}

	/* first byte always literal */
	odata[0]=data[0];

	/* force short data encoding */
	if (length<5) {
		token=(length-1)<<4;
		odata[ioutput++]=token;
		for (i=1;i<length;i++) odata[ioutput++]=data[current++];
		odata[ioutput++]=0xFF;
		*retlength=ioutput;
		return odata;
	}

	while (current<length) {
		maxlength=0;
		startscan=current-255;
		if (startscan<0) startscan=0;
		while (startscan<current) {
			matchlength=0;
			curscan=current;
			for (i=startscan;curscan<length;i++) {
				if (data[i]==data[curscan++]) matchlength++; else break;
			}
			if (matchlength>=3 && matchlength>maxlength) {
				maxoffset=startscan;
				maxlength=matchlength;
			}
			startscan++;
		}
		if (maxlength) {
			ioutput+=LZ48_encode_block(odata+ioutput,data,literaloffset,literal,current-maxoffset,maxlength);
			current+=maxlength;
			literaloffset=current;
			literal=0;
		} else {
			literal++;
			current++;
		}
	}
	ioutput+=LZ48_encode_block(odata+ioutput,data,literaloffset,literal,0,0);
	*retlength=ioutput;
	return odata;
}

void LZ48_decode(unsigned char *data, unsigned char *odata)
{
	int HL=0,DE=0;
	int literallength,matchlength,HLmatch;

	odata[DE++]=data[HL++];

	while (1) {
		literallength=(data[HL] & 0xF0)>>4;
		matchlength=(data[HL++] & 0xF);

		if (literallength==15) {
			while (data[HL]==255) {
				literallength+=255;
				HL++;
			}
			literallength+=data[HL++];
		}

		while (literallength>0) {
			odata[DE++]=data[HL++];
			literallength--;
		}

		/* matchkey */
		if (matchlength==15) {
			while (data[HL]==255) {
				matchlength+=255;
				HL++;
			}
			matchlength+=data[HL++];
		}
		matchlength+=3;
		if (data[HL]==0xFF) return; else HLmatch=DE-(data[HL++]+1);
		
		while (matchlength) {
			odata[DE++]=odata[HLmatch++];
			matchlength--;
		}
	}
}

unsigned char * LZ48_decrunch(unsigned char *data, int *osize)
{
	int literallength,matchlength;
	int HL=1,DE=1;
	unsigned char *odata=NULL;
	
	while (1) {
		literallength=(data[HL] & 0xF0)>>4;
		matchlength=(data[HL++] & 0xF);
		if (literallength==15) {
			while (data[HL]==255) {
				literallength+=255;
				HL++;
			}
			literallength+=data[HL++];
		}

		DE+=literallength;
		HL+=literallength;

		/* matchkey */
		if (matchlength==15) {
			while (data[HL]==255) {
				matchlength+=255;
				HL++;
			}
			matchlength+=data[HL++];
		}
		matchlength+=3;
		if (data[HL]==0xFF) break; else HL++;
		
		DE+=matchlength;
	}
	*osize=DE;
	odata=malloc(*osize);
	if (!odata) {
		fprintf(stderr,"memory full\n");
		exit(-1);
	}
	LZ48_decode(data,odata);
	return odata;
}

void Usage()
{
	printf("usage: lz48 <options>\n");
	printf("options are:\n");
	printf("-i <inputfile>\n");
	printf("-o <outputfile>\n");
	printf("-b hexatext output (enable if you forget outputfile)\n");
	printf("-d decrunch\n");
	printf("\n");
	exit(0);
}

int ParseOptions(char **argv,int argc, char **inputfilename, char **outputfilename, int *hexoutput, int *crunch)
{
	int i=0;

	if (argv[i][0]=='-')
	{
		switch (argv[i][1])
		{
			case 'O':
			case 'o':
				if (i+1<argc) *outputfilename=argv[i+1];
				i++;
				break;
			case 'I':
			case 'i':
				if (i+1<argc) *inputfilename=argv[i+1];
				i++;
				break;
			case 'D':
			case 'd':
				*crunch=0;
				break;
			case 'B':
			case 'b':
				*hexoutput=1;
				break;
			default:
				Usage();				
		}
	}
	else
		Usage();
	return i;
}

/*
 * 	GetParametersFromCommandLine	
 * 		retrieve parameters from command line and fill pointers to file names
 * 		*/
void GetParametersFromCommandLine(int argc, char **argv, char **inputfilename, char **outputfilename, int *hexoutput, int *crunch)
{
	#undef FUNC
	#define FUNC "GetParametersFromCommandLine"
	int i;
	
	for (i=1;i<argc;i++)
		i+=ParseOptions(&argv[i],argc-i,inputfilename,outputfilename,hexoutput,crunch);

	if (!*inputfilename) Usage();
	if (!*outputfilename) *hexoutput=1;
}



void main(int argc, char **argv)
{
	unsigned char *data=NULL,*newdata=NULL;
	char *inputfilename=NULL,*outputfilename=NULL;
	int hexoutput=0,isize,crunch=1;
	int newsize,i,cr;
	FILE *fin=NULL,*fout=stdout;

	fprintf(stderr,"LZ48 cruncher / roudoudou - Flower Corp. 2016\n");

	GetParametersFromCommandLine(argc,argv,&inputfilename,&outputfilename,&hexoutput,&crunch);

	fin=fopen(inputfilename,"rb");
	if (!fin) Usage();
	fseek(fin,0,SEEK_END);
	isize=ftell(fin);
	fseek(fin,0,SEEK_SET);
	data=malloc(isize);
	if (!data) {
		fprintf(stderr,"memory full\n");
		exit(-1);
	}
	if (fread(data,1,isize,fin)!=isize) {
		fprintf(stderr,"read error\n");
		exit(-1);
	}
	fclose(fin);
	switch (crunch) {
		case 0:newdata=LZ48_decrunch(data,&newsize);break;
		case 1:newdata=LZ48_encode(data,isize,&newsize);break;
	}
printf("input: %d output: %d\n",isize,newsize);
	if (hexoutput) {
		for (i=cr=0;i<newsize;i++) {
			if (!cr) {
				printf("DEFB %02X",newdata[i]);
				cr=1;
			} else {
				if (cr==15) {
					printf(",%02X\n",newdata[i]);
					cr=0;
				} else {
					printf(",%02X",newdata[i]);
					cr++;
				}
			}
		}
		if (cr) printf("\n");
	} else {
		fout=fopen(outputfilename,"wb");
		if (!fout) Usage;
		if (fwrite(newdata,1,newsize,fout)!=newsize) {
			fprintf(stderr,"write error\n");
			exit(-1);
		}
		fclose(fout);
	}
}

