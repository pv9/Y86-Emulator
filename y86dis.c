#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>



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




void print_register(int n){
	switch(n){
		case 0:printf("%%eax");break;
		case 1:printf("%%ecx");break;
		case 2:printf("%%edx");break;
		case 3:printf("%%ebx");break;
		case 4:printf("%%esp");break;
		case 5:printf("%%ebp");break;
		case 6:printf("%%esi");break;
		case 7:printf("%%edi");break;
		default:break;
	}
}
void determine_inst(char *text){
	int pc=0;char opCode[2];
	char long_n[9];int long_val;
	int i,r1,r2,k;i=0;
	 	  while(pc<strlen(text)){
		 opCode[0]=text[pc];
		 opCode[1]=text[pc+1];
		//printf("OpCode %c %c\n",opCode[0],opCode[1]);
		 if(opCode[0]=='0'&&opCode[1]=='0'){
			printf("nop\n");
			pc+=2;
		 }
		 else if(opCode[0]=='1'&&opCode[1]=='0'){
			//halt
			printf("halt\n");
			pc+=2;
		 }
		 else if(opCode[0]=='2'&&opCode[1]=='0'){
		 	//load the registers
			r1=text[pc+2]-'0';
			r2=text[pc+3]-'0';
			printf("rrmovl ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
			//rrmovl
		}else if(opCode[0]=='3'&&opCode[1]=='0'){
			//irmovl
			r2=text[pc+3]-'0';
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("irmovl $ %x ,",long_val);
			print_register(r2);
			printf("\n");
			
			pc+=12;//6 byte instruction
			
		 }else if(opCode[0]=='4'&&opCode[1]=='0'){
			//rmmovl
			r1=text[pc+2]-'0';
			r2=text[pc+3]-'0';
			
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[pc+4+i*2];
				long_n[k++]=text[pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			printf("rmmovl ");
			print_register(r1);
			printf(", %x(",long_val);print_register(r2);
			printf(")\n");			
             
			pc+=12;//6 byte instruction
		 }else if(opCode[0]=='5'&&opCode[1]=='0'){
			//mrmovl
			
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
			printf("mrmovl ");
			printf("%x (",long_val);
			print_register(r2);
			printf("), ");
			print_register(r1);
			printf("\n");
			
		     pc+=12;//6 byte instruction
		}
		else if(opCode[0]=='6'&&opCode[1]=='0'){
			//addl
		
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			printf("addl ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
			
		}else if(opCode[0]=='6'&&opCode[1]=='1'){
			//subl
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			printf("subl ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
		}else if(opCode[0]=='6'&&opCode[1]=='2'){
			//andl
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			printf("andl ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
			
			
		}else if(opCode[0]=='6'&&opCode[1]=='3'){
            //xorl
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			
			printf("xorl ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
			
		}else if(opCode[0]=='6'&&opCode[1]=='4'){
            //mull
            
			
			r1=text[2*pc+2]-'0';
			r2=text[2*pc+3]-'0';
			printf("mull ");
			print_register(r1);
			printf(",");
			print_register(r2);
			printf("\n");
			pc+=4;//2 byte instruction
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='0'){
			//jmp
			
			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			
		
			printf("jmp %x\n",long_val);
			pc+=10;
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='1'){
			//jle
			
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("jle %x\n",long_val);
			pc+=10;
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='2'){
			//jl

			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("jl %x\n",long_val);
			pc+=10;

		}
		else if(opCode[0]=='7'&&opCode[1]=='3'){
			//je
			
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("je %x\n",long_val);
			pc+=10;
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='4'){
			//jne 
			
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("jne %x\n",long_val);
			pc+=10;
		}
		else if(opCode[0]=='7'&&opCode[1]=='5'){
			//jge

			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("jge %x\n",long_val);
			pc+=10;
			
		}
		else if(opCode[0]=='7'&&opCode[1]=='6'){
			//jg
			
			
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("jg %x\n",long_val);
			pc+=10;
		}
		else if(opCode[0]=='8'&&opCode[1]=='0'){
			//call
			
			//read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+2+i*2];
				long_n[k++]=text[2*pc+2+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("call %x\n",long_val);
			pc+=10;
			
		}else if(opCode[0]=='9'&&opCode[1]=='0'){
			//call
			
			
			printf("ret \n");
			pc+=2;
			
		}
else if(tolower(opCode[0])=='a'&&opCode[1]=='0'){
	            //pushl
				
			    r1=text[2*pc+2]-'0';
			    printf("pushl ");
			    print_register(r1);
				printf("\n");
				pc+=4;//2 byte instruction
			
		}else if(tolower(opCode[0]=='b')&&opCode[1]=='0'){
			//popl
			
			r1=text[2*pc+2]-'0';
			printf("pushl ");
			    print_register(r1);
			printf("\n");
				pc+=4;//2 byte instruction
		}else if(tolower(opCode[0]=='c')&&opCode[1]=='0'){
			
		r1=text[2*pc+2]-'0';
			  //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("readb %x(",long_val);
			print_register(r1);
			printf(")");
			pc+=12;//6 byte instruction

		}else if(tolower(opCode[0]=='c')&&opCode[1]=='1'){
		r1=text[2*pc+2]-'0';
			  //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("readl %x(",long_val);
			print_register(r1);
			printf(")");
			pc+=12;//6 byte instruction

		}else if(tolower(opCode[0]=='d')&&opCode[1]=='0'){
			
			r1=text[2*pc+2]-'0';
			  //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("writeb %x(",long_val);
			print_register(r1);
			printf(")\n");
			pc+=12;//6 byte instruction
		}else if(tolower(opCode[0]=='d')&&opCode[1]=='1'){
		r1=text[2*pc+2]-'0';
			  //read immediate value
   			k=0;
			for(i=3;i>=0;i--){
				long_n[k++]=text[2*pc+4+i*2];
				long_n[k++]=text[2*pc+4+i*2+1];
			}
			long_n[8]='\0';
			sscanf(long_n, "%x", &long_val);
			printf("writel %x(",long_val);
			print_register(r1);
			printf(")\n");
			pc+=12;//6 byte instruction
		}
		else{
			//Invalid Instruction encountered
			printf("Invalid Instruction\n");		
	pc+=2;
		}
		
		
 	}
}
void print_dis(FILE* in){
char line[600];
char first[600];
char size_s[600];
char argument[600];
int text=0;
		while(fgets(line, 600, in) != NULL){
			sscanf(line,"%s %s %s",first,size_s,argument);
			if(strcmp(first,".text")==0){
				text=1;determine_inst(argument);	
break;	
			}	
		}
	if(text==0){printf("ERROR: Missing .text directive\n");}
}
int main(int argc,char* argv[])
{
 if(argc<2)	{
	printf("ERROR: Invalid Arguments\n");
	exit(1);
 }
 if((argv[1][0]=='-')&&(argv[1][1]=='h')){
	printf("Usage: y86dis <input_file>\n");
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
 print_dis(in_file);

 fclose(in_file);	
 
 return 0;
}

