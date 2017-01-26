#include"utili.h"
using namespace std;

extern "C" int ConvertComment(FILE *inputfile, FILE *outputfile);

int main(int argc, char *argv[])
{
	FILE *inputfile = fopen("input.c", "r");
	if(NULL == inputfile)
	{
		cout<<"Open input.c file Fail."<<endl;
		return -1;
	}
	FILE *outputfile = fopen("output.c", "w");
	if(NULL == outputfile)
	{
		cout<<"Open output.c file Fail."<<endl;
		fclose(inputfile);
		return -1;
	}

	//////////////////////////////////////////////////////////
	//
	ConvertComment(inputfile, outputfile);

	fclose(inputfile);
	fclose(outputfile);
	return 0;
}
