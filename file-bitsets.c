#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


const size_t SETSIZE = sizeof( uint64_t) << 3 ;
const size_t BUFSIZE = 256;
uint64_t set_encode(char* ch);


uint64_t file_set_encode( FILE * fp ){
	//reads the first two 2 words of the file and calls set_encode on them.
	char string[BUFSIZE];// = "hello";
	int counter = 0;
	for(int ch = fgetc(fp); ch!=EOF; ch=fgetc(fp)){
		string[counter]=ch;
		counter++;
	}
	return set_encode(string);
}

uint64_t encodechar( char ch){
	char * reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789,abcdefghijklmnopqrstuvwxyz";
	int x = 0;
	for(int i =0; i<strlen(reference); i++){
		if(reference[i]==ch){
			x=(-i-65)%64;
		}
	}
	if(ch!='z' && x==0){
		return (uint64_t)0x0000000000000000;
	}
	uint64_t code = 0x0000000000000001;
	code = code << x;
	return code;
}

uint64_t set_encode( char * st ) {
	//turns the string into a binary
	static int i = 0;
	i++;
	printf("string%d:	%s",i,st);
	printf("	Encoding the string:	%s\n",st);
	int64_t code  = 0x0000000000000000;
	for(int i = 0; i<strlen(st); i++){
		code = code | encodechar(st[i]);
	}
	return code;
}


uint64_t set_intersect( uint64_t set1, uint64_t set2 ){
	return (set1 & set2);
}


uint64_t set_union( uint64_t set1, uint64_t set2 ){
	return (set1 | set2);
}


uint64_t set_complement( uint64_t set1 ){
	return ~set1;
}


uint64_t set_difference( uint64_t set1, uint64_t set2 ){
	uint64_t long1 =  (set1 ^ set2);//-set2;
	uint64_t long2 = (set2 - (set1 & set2));
	return long1-long2;
}
uint64_t set_symdifference( uint64_t set1, uint64_t set2 ){
	return set1 ^ set2;
}
size_t set_cardinality( uint64_t set ) {
	int x = 0;
	while(set!=0){
		set=set>>1;
		if(set%2==1){
			x++;
		}
	}
	return x;
}

char * set_decode( uint64_t set ) {
	char* reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789,abcdefghijklmnopqrstuvwxyz";
	char* result = malloc(BUFSIZE);
	int x = 0;
	for(int i=0; set!=0; i++){
		//set=set>>1;
		if(set%2==1){
			result[x]=reference[-i+63];
			x++;
		}
		set=set>>1;
	}
	result[x]='\0';
	return result;
}


int main(int argc, char* argv[]){
	//takes two command line arguments, which are strings
	//it creates a set of each of the inputs and applies set operations.
	//Then it reports the result of each operation.
	//
	//check if the args are files, if not uses the string. 
	if(argc<3){
		perror("Usage: file-bitsets string1 string2");
		exit(0);
	}
	FILE* file1;
	FILE* file2;
	uint64_t long1 = 0;// = set_encode(argv[1]);
	uint64_t long2 = 0;// = set_encode(argv[2]);
	if(file1=fopen(argv[1],"r")){
		if(file2=fopen(argv[2],"r")){
			long1 = file_set_encode(file1);
			long2 = file_set_encode(file2);
			fclose(file2);
		}
		fclose(file1);
	}
	else{
		long1 = set_encode(argv[1]);
		long2 = set_encode(argv[2]);
	}
	//for(int i = 1; i < 3; i++){
	//	printf("string%d:	%s",i,argv[i]);
	//	printf("	Encoding the string:	%s\n",argv[i]);
	//}	
	printf("\n");
	printf("set1:	%#.16lx\n",long1);
	printf("set2:	%#.16lx\n\n",long2);
	printf("set_intersect:	%#.16lx\n",set_intersect(long1,long2));
	printf("set_union:	%#.16lx\n\n",set_union(long1,long2));
	printf("set1: set_complement:	%#.16lx\n",set_complement(long1));
	printf("set2: set_complement:	%#.16lx\n\n",set_complement(long2));
	printf("set_difference:		%#.16lx\n",set_difference(long1,long2));
	printf("set_symdifference:	%#.16lx\n\n",set_symdifference(long1,long2));
	printf("set1 set_cardinality:	%ld\n",set_cardinality(long1));
	printf("set2 set_cardinality:	%ld\n\n",set_cardinality(long2));
	char* decode1 = set_decode(long1);
	char* decode2 = set_decode(long2);
	printf("members of set1:	'%s'\n",decode1);
	printf("members of set2:	'%s'\n",decode2);
	free(decode1);
	free(decode2);
}
