#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<inttypes.h>
//@author: Matthew Carey

const size_t SETSIZE = sizeof( uint64_t) << 3 ;
const size_t BUFSIZE = 256;
uint64_t set_encode(char* ch);


//this function reads everything in file fp into a string and then 
//returns set_encode on that string.
uint64_t file_set_encode( FILE * fp ){
	char string[BUFSIZE];// = "hello";
	int counter = 0;
	for(int ch = fgetc(fp); ch!=EOF; ch=fgetc(fp)){
		string[counter]=ch;
		counter++;
	}
	string[counter]='\0';
	return set_encode(string);
}


//this function is a helper for set_encode it converts one char to a 
//uint64_t using the reference string and returns the uint64_t 
uint64_t encodechar( char ch){
	
	char * reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789,abcdefghijklmnopqrstuvwxyz";
	int x = -1;
	for(int i =0; (unsigned)i<strlen(reference); i++){
		if(reference[i]==ch){
			x=(-i+63);//this is because the first letters in the 
			//reference string are actually the highest bit order.
		}
	}
	if(x==-1){//checks if nothing recognisable was found and returns 0
		return (uint64_t)0;
	}
	uint64_t code = 1;
	code = code << x;
	return code;
}


//this function encodes a string by calling encodechar on each char
//and or'ing them all together then returning the result. 
uint64_t set_encode( char * st ) {
	//static int i = 0;
	//i++;
	//printf("string%d:	%s",i,st);
	//printf("	Encoding the string:	%s\n",st);
	int64_t code  = 0;
	for(int i = 0; (unsigned)i<strlen(st); i++){
		code = code | encodechar(st[i]);
	}
	return code;
}


//returns the intersect of two sets
uint64_t set_intersect( uint64_t set1, uint64_t set2 ){
	return (set1 & set2);
}


//returns the union of two sets
uint64_t set_union( uint64_t set1, uint64_t set2 ){
	return (set1 | set2);
}


//returns the complement of a set
uint64_t set_complement( uint64_t set1 ){
	return ~set1;
}


//returns the result of subtracting the XOR of two sets by everything
//in set2 thats not in set1;
uint64_t set_difference( uint64_t set1, uint64_t set2 ){
	uint64_t long1 =  (set1 ^ set2);
	uint64_t long2 = (set2 - (set1 & set2));
	return long1-long2;
}


//returns the XOR of two sets
uint64_t set_symdifference( uint64_t set1, uint64_t set2 ){
	return set1 ^ set2;
}


//shifts right by 1 over and over checking each time if the resulting 
//number is odd, if so increments cardinality. Returns final cardinality.
size_t set_cardinality( uint64_t set ) {
	int cardinality = 0;
	while(set!=0){
		set=set>>1;
		if(set%2==1){
			cardinality++;
		}
	}
	return cardinality;
}


//shifts right by 1 over and over checking each time if the resulting
//number is odd, if so it adds the char that is at index [-(number of
//shifts that have occured)+63] to the string. Returns the resulting 
//sting
char * set_decode( uint64_t set ) {
	char* reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789,abcdefghijklmnopqrstuvwxyz";
	char* result = malloc(BUFSIZE);
	char temp;
	int x = 0;
	for(int i=0; set!=0; i++){
		//set=set>>1;
		if(set%2==1){
			result[x]=reference[-i+63];
			x++;
		}
		set=set>>1;
	}
	int len = x-1;
	for(int i=0; i<x-1;i++){//this reverses the order of the string that I just build
				// to match the output of the try tests. 
		temp = result[len];
		result[len] = result[i];
		result[i]=temp;
		len--;
		if(len==(x-1)/2){
			break;
		}	
	}
	result[x]='\0';
	return result;
}


//takes two command line arguments, either strings or filenames. It then encodes
//them accordingly and prints the result of multiple set operations. 
int main(int argc, char* argv[]){ 
	if(argc<3){//checks that there are enough arguments
		fprintf(stderr,"Usage: file-bitsets string1 string2");
		exit(1);
	}
		
	FILE* file1;
	FILE* file2;
	uint64_t long1 = 0;
	uint64_t long2 = 0;
	printf("string1:	%s",argv[1]);
	if((file1=fopen(argv[1],"r"))){//checks if the first argument is a file
		printf("	Encoding the file:	%s\n",argv[1]);
		long1 = file_set_encode(file1);
		fclose(file1);	
	}
	else{//if not a file uses set_encode
		long1 = set_encode(argv[1]);
		printf("	Encoding the string:	%s\n",argv[1]);
	}
	printf("string2:	%s",argv[2]);
	if((file2=fopen(argv[2],"r"))){//checks if the second argument is a file
		printf("	Encoding the file:	%s\n",argv[2]);	
		long2 = file_set_encode(file2);
		fclose(file2);
	}
	else{//if not a file uses set_encode
		long2 = set_encode(argv[2]);
		printf("	Encoding the string:	%s\n",argv[2]);
	}
	printf("\n");
	printf("set1:	%#018lx\n",long1);
	printf("set2:	%#018lx\n\n",long2);
	printf("set_intersect:	%#018lx\n",set_intersect(long1,long2));
	printf("set_union:	%#018lx\n\n",set_union(long1,long2));
	printf("set1 set_complement:	%#018lx\n",set_complement(long1));
	printf("set2 set_complement:	%#018lx\n\n",set_complement(long2));
	printf("set_difference:		%#018lx\n",set_difference(long1,long2));
	printf("set_symdifference:	%#018lx\n\n",set_symdifference(long1,long2));
	printf("set1 set_cardinality:	%ld\n",set_cardinality(long1));
	printf("set2 set_cardinality:	%ld\n\n",set_cardinality(long2));
	char* decode1 = set_decode(long1);//declares these two out of print statement so  
	char* decode2 = set_decode(long2);//they can be freed later.
	printf("members of set1:	'%s'\n",decode1);
	printf("members of set2:	'%s'\n",decode2);
	free(decode1);
	free(decode2);
}
