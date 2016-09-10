#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

//Program Status Codes
#define STATUS_AOK 0
#define STATUS_HLT 1
#define STATUS_ADR 2
#define STATUS_INS 3


//preprocessors for debugging
#define DEBUG 0
#define IF_DEBUG if(DEBUG)


//stores the 8 registers of the y86 cpu
int registers[8];
//flag variables
int zf,sf,of;
//program counter
int pc=-1;
//program status
int program_status=STATUS_AOK;
//memory block which will be allocated dynamically after reading the input
char *memory;

//used to track instructions in memory for DEBUGGING PURPOSE.
int memory_size=0;
int max_address=0;

int text_start=0;
int text_length=0;


/**
	Used for testing the contents of instructions in emulated memory.
*/
void print_instr_mem(){
	int i;
	//printf("printing text\n");
	for(i=text_start;i<text_start+text_length;i++){
	//	printf("%c",memory[i]);
	}
//	printf("\n");
}

//stack for the calls of the procedure
 int stack[512];
 //stack for pushl and popl instructions
 int stack2[512];
 int top2=0;
 int top=0;

 void push(int val){
 	stack[top++]=val;
 	if(top==512){
 		printf("ERROR Stack Overflow Exception");
 	}
 }

 int pop(){
 	if(top==0){
 		printf("ERROR Stack Underflow Exception");
 		return 0;
 	}else{
 		return stack[--top];
 	}
 }

 void pushl(int val){
 	stack2[top2++]=val;
 	if(top2==512){
 		printf("ERROR Stack Overflow Exception");
 	}
 }

 int popl(){
 	if(top2==0){
 		printf("ERROR Stack Underflow Exception");
 		return 0;
 	}else{
 		return stack2[--top2];
 	}
 }

//initialize the registers
void clean_registers(){
	int i;
	for(i=0;i<8;i++)
		registers[i]=0;
}



/**
	Prints the machine state {Program Status, Flags , Registers}
*/
void print_output_status(){
	int i;
	//1.program status

	switch(program_status){
		case STATUS_ADR:
			printf("\nADR: Program Ended With Invalid Address Exception\n");
		break;
		case STATUS_AOK:
			printf("\nAOK: Program Executing Normally\n");
		break;
		
		case STATUS_HLT:
			printf("\nHLT: Program Ended With halt Operation\n");
		break;
		case STATUS_INS:
			printf("\nINS: Program Ended With Invalid Instruction Code Encountered\n");
		break;
	}
	
	//2. Flags
	printf("Flags ZF=%d SF=%d OF=%d\n",zf,sf,of);
	//3. Registers
	printf("Register Contents\n");
	for(i=0;i<8;i++){
		printf("Register %d:\t%x\n",i,registers[i]);
	}

}



/**
	This function executes the instructions in .text directive 
	after reading the whole source code once.
*/
 void execute_instructions(){
 	  char opCode[2];
 	  //for opcode of current instruction being executed by the CPU.
 	  
 	  char long_n[9];// for storing hexadecimal long integer in character format
 	  
	  int long_val;
	  
 	  int op_res;//result of oper operations with opcode 6 and fn 1,2,3,4
 	  
	  int r1,r2;//registers for currently executed instructions.
 	  
 	  char temp[9];//temporary variable to store long values in character form. specially used for rmmovl and mrmovl
 	  
 	  
 	  int i,k;//loop variables
 	  
 	  //Checks if PC is initialized . If not then print the array with apt message.
	  if(pc==-1){
 		printf("ERROR: CPU Instructions Not Found. Missing .text directive.\n");
 		exit(1);
 	  }

 		IF_DEBUG
	 		printf("\nStarting program Execution at %d\n",pc);
 	
	  //For each instruction do this 
	  //1. Fetch
 	  //2. Decode
 	  //3. Execute
 	  while(1){
 		 //Get the OpCode
	//Check for Address Overflow Error
		 if(pc<0||max_address<2*pc){
   				program_status=STATUS_ADR;
   				break;
   		 }
		 opCode[0]=memory[2*pc];
		 opCode[1]=memory[2*pc+1];
		 
		 
   		 
   		 //prints memory area where instructions are stored. Used for debugging only.
		 IF_DEBUG 
		 print_instr_mem();
		
		 IF_DEBUG
			printf("PC=%X OpCode %c %c\t",pc,opCode[0],opCode[1]);
		
		 if(opCode[0]=='0'&&opCode[1]=='0'){
			//nop
			IF_DEBUG
			printf("No Operation\n");
			pc+=1;
		 }
		 else if(opCode[0]=='1'&&opCode[1]=='0'){
			//halt
			IF_DEBUG
			printf("Halt\n");
			pc+=1;
			program_status=STATUS_HLT;//program exited normally
			break;
		 }
		 else if(opCode[0]=='2'&&opCode[1]=='0'){
		 	//load the registers
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			IF_DEBUG
			printf("rrmovl r1=%d,r2=%d\n",r1,r2);
			registers[r2]=registers[r1];
			pc+=2;//2 byte instruction
			//rrmovl
		}else if(opCode[0]=='3'&&opCode[1]=='0'){
			//irmovl
			r2=memory[2*pc+3]-'0';
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+4+i*2];
				long_n[k++]=memory[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
		
			//fetch complete
			//now execute it
			registers[r2]=long_val;
			IF_DEBUG
			printf("irmovl $ %x ,r2= %d\n",long_val,r2);
			
			pc+=6;//6 byte instruction
			
		 }else if(opCode[0]=='4'&&opCode[1]=='0'){
			//rmmovl
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			//read immediate value
   			//check for address error
			if(pc<0||max_address<2*pc+11){
   				program_status=STATUS_ADR;
   				break;
   			}
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+4+i*2];
				long_n[k++]=memory[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			//fetch complete
			IF_DEBUG
			printf("rmmovl r1=%d ,long_val = %d  ( r2=%d)\n",r1,long_val,r2);
			
			sprintf(temp,"%08x",registers[r1]);
			k=0;
            int offset=long_val+registers[r2];
             
			if(offset<0||max_address<offset){
   				program_status=STATUS_ADR;
   				break;
   			}	
			 
			for(i=3;i>=0;i--){
             	memory[2*offset+i*2]=temp[k++];
				memory[2*offset+i*2+1]=temp[k++];
            }
             
			pc+=6;//6 byte instruction
		 }else if(opCode[0]=='5'&&opCode[1]=='0'){
			//mrmovl
			
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			//read immediate value
   			if(pc<0||max_address<2*pc+11){
   				program_status=STATUS_ADR;
   				break;
   			}
   			
			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+4+i*2];
				long_n[k++]=memory[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			//fetch complete
			IF_DEBUG
			printf("mrmovl long_val= %d( r2=%d),r1=%d\n",long_val,r2,r1);
			
			k=0;
            int offset=long_val+registers[r2];
          
		  	if(offset<0||max_address<offset){
   				program_status=STATUS_ADR;
   				break;
   			}
		     for(i=3;i>=0;i--){
             	
             	long_n[k++]=memory[2*offset+i*2];
				long_n[k++]=memory[2*offset+i*2+1];
             }
             long_n[8]='\0';
             sscanf(long_n,"%x",&registers[r1]);
			
		     pc+=6;//6 byte instruction
		}
		else if(opCode[0]=='6'&&opCode[1]=='0'){
			//addl
		
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			op_res=registers[r2]+registers[r1];
			if(op_res==0){
				zf=1;
			}
			else{
				zf=0;
			}
			if(op_res<0){
				sf=1;
			}
			else{
				sf=0;
			}
			if(( registers[r1] > 0 && registers[r2] > 0 && op_res < 0) || (registers[r1] < 0 && registers[r2] < 0 && op_res > 0)){
				of=1;
			}
			else{
				of=0;
			}
			IF_DEBUG
				printf("Addl r1=%d,r2=%d\n",r1,r2);
			registers[r2]=op_res;
			pc+=2;//2 byte instruction
			
		}else if(opCode[0]=='6'&&opCode[1]=='1'){
			//subl
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			
			op_res=registers[r2]-registers[r1];
			if(op_res==0){
				zf=1;
			}
			else{
				zf=0;
			}
			if(op_res<0){
				sf=1;
			}
			else{
				sf=0;
			}
			if((registers[r1] > 0 && registers[r2] > 0 && op_res < 0) || (registers[r1] < 0 && registers[r2] < 0 && op_res > 0)){
				of=1;
			}
			else{
				of=0;
			}
			IF_DEBUG
				printf("subl r1=%d,r2=%d\n",r1,r2);
			registers[r2]=op_res;
			
			
			pc+=2;//2 byte instruction
		}else if(opCode[0]=='6'&&opCode[1]=='2'){
			//andl
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			
			op_res=registers[r2]&registers[r1];
			if(op_res==0){
				zf=1;
			}
			else{
				zf=0;
			}
			if(op_res<0){
				sf=1;
			}
			else{
				sf=0;
			}
			
			of=0;
			
			registers[r2]=op_res;
			IF_DEBUG
			printf("andl r1=%d,r2=%d\n",r1,r2);
			
			
			pc+=2;//2 byte instruction
		}else if(opCode[0]=='6'&&opCode[1]=='3'){
            //xorl
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			
			op_res=registers[r2] ^ registers[r1];
			if(op_res==0){
				zf=1;
			}
			else{
				zf=0;
			}
			if(op_res<0){
				sf=1;
			}
			else{
				sf=0;
			}
			of=0;
		
			registers[r2]=op_res;
			IF_DEBUG
			printf("xorl r1=%d,r2=%d\n",r1,r2);
			pc+=2;//2 byte instruction
		}else if(opCode[0]=='6'&&opCode[1]=='4'){
            //mull
            
			
			r1=memory[2*pc+2]-'0';
			r2=memory[2*pc+3]-'0';
			
			op_res=registers[r2] ^ registers[r1];
			if(op_res==0){
				zf=1;
			}
			else{
				zf=0;
			}
			if(op_res<0){
				sf=1;
			}
			else{
				sf=0;
			}
			
			
			if((registers[r1] > 0 && registers[r2] > 0 && op_res < 0) || (registers[r1] < 0 && registers[r2] < 0 && op_res < 0) ||
                (registers[r1] < 0 && registers[r2] > 0 && op_res > 0) || (registers[r1] > 0 && registers[r2] < 0 && op_res > 0)){
				of=1;
			}
			else{
				of=0;
			}
		
			registers[r2]=op_res;
			IF_DEBUG
			printf("mull r1=%d,r2=%d\n",r1,r2);
			pc+=2;//2 byte instruction
		}
		else if(opCode[0]=='7'&&opCode[1]=='0'){
			//jmp
			
			//read immediate value
   			
			 //checks for invalid address.  
			   if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			   }
   			
			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			pc=long_val; //jump the execution to new location
			IF_DEBUG
			printf("jmp %x\n",long_val);
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='1'){
			//jle
			
			//check for invalid address error
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			IF_DEBUG
			printf("jle %x\n",long_val);
			
			if((sf^of)|zf){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='2'){
			//jl
			
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			
			sscanf(long_n, "%x", &long_val);
			IF_DEBUG
			printf("jl %x\n",long_val);
			if((sf^of)){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
		}
		else if(opCode[0]=='7'&&opCode[1]=='3'){
			//je
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			IF_DEBUG
			printf("je %x\n",long_val);
			if(zf){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='4'){
			//jne
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			   //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
   			sscanf(long_n, "%x", &long_val);
			IF_DEBUG
			printf("jne %x\n",long_val);
			if(zf==0){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='5'){
			//jge
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			IF_DEBUG
			printf("jge %x\n",long_val);
			if(~(sf^of)){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
		}
		else if(opCode[0]=='7'&&opCode[1]=='6'){
			//jg
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			IF_DEBUG
			printf("jg %x\n",long_val);
			if(~(sf^of)&~zf){
				pc=long_val;
			}else{
				pc+=5;//5 byte instruction
			}
		}
		else if(opCode[0]=='8'&&opCode[1]=='0'){
			//call
			if(pc<0||max_address<2*pc+9){
   				program_status=STATUS_ADR;
   				break;
   			}
			   //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+2+i*2];
				long_n[k++]=memory[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			IF_DEBUG
			printf("call %x\n",long_val);
			push(pc+5);
			pc=long_val;
		
		}
		else if(opCode[0]=='9'&&opCode[1]=='0'){
			
			//ret
			long_val=pop();
			IF_DEBUG
			printf("ret %x\n",long_val);
			pc=long_val;
			
			
		}else if(tolower(opCode[0])=='a'&&opCode[1]=='0'){
	            //pushl
				if(pc<0||max_address<2*pc+2){
   					program_status=STATUS_ADR;
   					break;
   				}
			    r1=memory[2*pc+2]-'0';
				pushl(registers[r1]);
				IF_DEBUG
				printf("pushl r1=%d\n",r1);
				pc+=2;//2 byte instruction
			
		}else if(tolower(opCode[0]=='b')&&opCode[1]=='0'){
			//popl
			
			r1=memory[2*pc+2]-'0';
			registers[r1]=popl();
			IF_DEBUG
			printf("popl r1=%d\n",r1);
			pc+=2;//2 byte instruction
		}else if(tolower(opCode[0]=='c')&&opCode[1]=='0'){
			r1=memory[2*pc+2]-'0';
			//readb
			IF_DEBUG
			printf("readb\t");
			zf=!scanf("%d",&registers[r1]);
			of=sf=0;
			pc+=6;//6 byte instruction
		}else if(tolower(opCode[0]=='c')&&opCode[1]=='1'){
			r1=memory[2*pc+2]-'0';
			//readl
			IF_DEBUG
			printf("readl\t");
			
			long_val = 0;
			
			zf=!scanf("%d",&registers[r1]);
			of=sf=0;
			
			pc+=6;//6 byte instruction
		}else if(tolower(opCode[0]=='d')&&opCode[1]=='0'){
			
			r1=memory[2*pc+2]-'0';
			IF_DEBUG
			printf("registers[r1]=");
			
				   //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=memory[2*pc+4+i*2];
				long_n[k++]=memory[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
		if(long_val+registers[r1]<0||2*(registers[r1]+long_val)>max_address){
			program_status=STATUS_ADR;break;			
			}
			char val[2];
			int val_i;
			val[0]=memory[2*(registers[r1]+long_val)];
			val[1]=memory[2*(registers[r1]+long_val)+1];
			sscanf(val,"%x",&val_i);
			printf("%c",val[1]);
			IF_DEBUG
			printf("\nWriteb\n");
			pc+=6;//6 byte instruction
		}else if(tolower(opCode[0]=='d')&&opCode[1]=='1'){
			r1=memory[2*pc+2]-'0';
			//writel
			
			IF_DEBUG
			printf("registers[r1]=%d\n",registers[r1]);
			printf("%x",registers[r1]);
			//IF_DEBUG
			printf("\nWritel\n");
			pc+=6;//6 byte instruction
		}
		else{
			//Invalid Instruction encountered
			 program_status=STATUS_INS;
		}
		//clear the value again
		for(i=0;i<8;i++){
			long_n[i]='0';
		}
		long_n[8]='\0';
		
 	}
 }

/*
	First reads the input program to allocate and assign actual memory and then calls run instructions
	function to start the execution.
*/
void run_y86_emul(FILE *in){
	int i,k;
	clean_registers();
	
	program_status=STATUS_AOK;
	pc=-1;
	//default state initially
	
	char line[512];
	char first[512];
	char size_char[20];
	int size=0;
	int hex_addr=-1;
	char directive_arg[512];
	//determine the size to be allocated
	while(fgets(line, 512, in) != NULL)
   {
	 /* get a line, up to 512 chars from fr.  done if NULL */
	 sscanf (line, "%s", first);
	 
	 IF_DEBUG
	 	printf("Line:\t%s\n",line);
	 IF_DEBUG
	 	printf ("First %s\n", first);
   	 if(strcmp(first,".size")==0){
   	 	sscanf (line, "%s %s", first,size_char);
   	 	IF_DEBUG
			printf ("Size %s\n", size_char);
   	 	size=strtol(size_char,NULL,16);
   	 	IF_DEBUG
			printf("Size in decimal %d",size);
		break;	
   	 }	
   }
   //check for memory size determined or not?
   if(size==0){
   		printf("ERROR: No Size directive provided.\n");
   		exit(1);
   }
   
   
	//size is determined
	max_address=2*size*sizeof(char);
	memory=(char *)malloc(2*size*sizeof(char));
	rewind(in);
	//restart the pointer to assign the values to the memory
	while(fgets(line, 512, in) != NULL)
   {
	 sscanf (line, "%s", first);
	 if(strcmp(first,".string")==0){
	 	//STORE THE STRING LITERAL
   	 	sscanf (line, "%s %s", first,size_char);
   	 	IF_DEBUG
   	 	printf ("\nSize %s\n", size_char);
   	    hex_addr=strtol(size_char,NULL,16);
   	 
		char *start=memory+2*hex_addr;
   	 	k=0;
   	 	while(line[k]!='"') k++;
   	 	k++;
   	 	//error
   	 	if(k>=strlen(line))
   	 		break;
		
		start++;
		while((line[k])!='"'){
//char temp[3];
//			int temp_i=line[k++];
//			sprintf(temp,"%d",temp_i);

//			printf("temp_i %d temp %s",temp_i,temp);
			(*start)=line[k++];
			start++;

			(*start)='$';
			start++;
		
		}
//		(*start)='\0';
   	 		
  	 } else if(strcmp(first,".long")==0){
	   //STORE SIGNED LONG AT SPECIFIED ADDRESS
	   sscanf (line, "%s %s %s", first,size_char,directive_arg);
	   //1. GET HEX Address and convert it to decimal
	    hex_addr=strtol(size_char,NULL,16);
   	 	char *start=memory+hex_addr;
   	 	k=0;
   	 	if(strlen(directive_arg)==2){
   	 		
   	 		(*start)=directive_arg[0];
   	 		start++;
   	 		(*start)=directive_arg[1];
   	 	}else if(strlen(directive_arg)==1){
   	 		(*start)='0';
   	 		 start++;
   	 		(*start)=directive_arg[0];
   	 	}
   	 	else{
   	 		printf("ERROR: Invalid Long Integer %s \n",directive_arg);
   	 		exit(1);
   	 	}
   	 	
		//2. get the decimal value convert it to character string 
	   //3. store the character string.
  	 }
  	 else if(strcmp(first,".bss")==0){
  	 	//CHUNK OF UNINITIALIZED MEMORY
	   
  	 }
  	 else if(strcmp(first,".byte")==0){
	   	  //STORES HEXADECIMAL BYTE AT SPECIFIED POSITION
	   	   //1. GET HEX Address and convert it to decimal
	   	    sscanf (line, "%s %s %s", first,size_char,directive_arg);
	  // 		IF_DEBUG
		//	   printf ("\nSize %s\n", size_char);
   	    	hex_addr=strtol(size_char,NULL,16);
   	 //		IF_DEBUG
	//			printf("Address in decimal %d",hex_addr);
   	 		char *start=memory+2*hex_addr;
   	 		k=0;
			sscanf(directive_arg,"%x",&k);
			memory[2*hex_addr]='0';
			memory[2*hex_addr+1]=k;
   	 		/*if(strlen(directive_arg)==2){
   	 		
   	 									 (*start)=directive_arg[0];
   	 									 start++;
   	 									 (*start)=directive_arg[1];
	       }else if(strlen(directive_arg)==1){
   	 			 							  (*start)='0';
   	 		 	 							  start++;
   	 										  (*start)=directive_arg[0];
		   }
   	 	   else{
   	 										  printf("ERROR: Invalid Byte %s \n",directive_arg);
		  				 					  exit(1);
			  }*/
   	
  	 }
  	 else if(strcmp(first,".text")==0){
	   	  //STORE THE INSTRUCTION BYTECODE OF CPU
	   	  //INITIALIZE PC
	   	   sscanf (line, "%s %s %s", first,size_char,directive_arg);
	  	   IF_DEBUG
			 printf ("\nSize %s\n", size_char);
  	   	   	hex_addr=strtol(size_char,NULL,16);
   	 		pc=hex_addr;
		    IF_DEBUG
			printf("Address in decimal %d",hex_addr);
   	 		char *start=memory+2*hex_addr;
   	 		k=0;
   	 		text_start=2*hex_addr;
   	 		text_length=strlen(directive_arg);
   	 		//while(line[k]!='"') k++;
   	 		//k++;
   	 		IF_DEBUG
				printf("directive_arg\t%s\t text\n\n",directive_arg);
  	 		
   	 		for(i=0;i<strlen(directive_arg);i++){
   	 			(*start)=directive_arg[i];
   	 			start++;
   	 		}
   	  }
  	   else if(strcmp(first,".size")==0){
  	 			//job already done
	   }
	   else{
  	 	printf("ERROR: Unknown Directive encountered %s ",first);
  	 	exit(1);
  	  }
   }
   //EXECUTE THE CODE FROM PROGRAM COUNTER
    execute_instructions();
    //print the output status
    print_output_status();
    //deallocate the memory
	free(memory);
}
int main(int argc,char* argv[])
{
 if(argc<2)	{
	printf("ERROR: Invalid Arguments\n");
	exit(1);
 }
 if((argv[1][0]=='-')&&(argv[1][1]=='h')){
	printf("Usage: y86emul <input_file>\n");
	exit(0);	
 }	
 if(argc!=2){
	printf("ERROR: Invalid Arguments\n");
	exit(1);
 }
 
 FILE *in_file;
 in_file =fopen(argv[1], "r");
 if (!in_file){
		printf("ERROR:Unable to location input file.\n");
		return 1;
 }
 run_y86_emul(in_file);
 fclose(in_file);	
 return 0;
}

