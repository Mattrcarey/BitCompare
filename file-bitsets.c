#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


const size_t SETSIZE = sizeof( uint64_t) << 3 ;
const size_t BUFSIZE = 256;



uint64_t file_set_encode( FILE * fp );
	//converts everything in the file into a uint64_t


uint64_t encodechar( char ch){
	char * reference = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789,abcdefghijklmnopqrstuvwxyz";
	int x = 0;
	for(int i =0; i<strlen(reference); i++){
		if(reference[i]==ch){
			x=(-i-65)%64;
		}
	}
	if(ch!='z' && x==0){
		return (uint64_t)0;
	}
	uint64_t code = 1;
	code = code << x;
	return code;
}

uint64_t set_encode( char * st ) {
	//turns the string into a binary
	uint64_t code  = 0;
	for(int i = 0; i<strlen(st); i++){
		code = code | encodechar(st[i]);
	}
	return code;
}
uint64_t set_intersect( uint64_t set1, uint64_t set2 ) ;
uint64_t set_union( uint64_t set1, uint64_t set2 ) ;
uint64_t set_complement( uint64_t set1 ) ;
uint64_t set_difference( uint64_t set1, uint64_t set2 ) ;
uint64_t set_symdifference( uint64_t set1, uint64_t set2 ) ;
size_t set_cardinality( uint64_t set ) ;
char * set_decode( uint64_t set ) ;


int main(int argc, char* argv[]){
	//takes two command line arguments, which are strings
	//it creates a set of each of the inputs and applies set operations.
	//Then it reports the result of each operation.
	//
	//check if the args are files, if not uses the string. 

	printf("0x%lx\n",set_encode("Ski@Do"));
	//if(argc<3){
	//	perror("Usage: file-bitsets string1 string2");
	//	exit(0);
	//}

	
}
