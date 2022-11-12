// DecryptData.cpp
//
// THis file uses the input data and key information to decrypt the input data
//

#include "Main.h"

// YOU WILL DELETE ALL THIS CODE WHEN YOU DO YOUR PROJECT - THIS IS GIVING YOU EXAMPLES OF HOW TO 
// ACCESS gPasswordHash AND gKey

void decryptData_01(char *data, int sized)
{
	int round = 0;
	__asm
	{
		xor ecx, ecx									//Clearing registers
		xor eax, eax
		xor ebx, ebx
		mov edi, [round]								//Storing the value of round
		mov esi, [data]									//Data String ptr
		mov ah, byte ptr gPasswordHash[0 + edi * 4]		//Generating Starting index and storing in ax								
		mov al, byte ptr gPasswordHash[1 + edi * 4]

		xor ebx, ebx
		mov bl, byte ptr[gkey + eax]					//Stores byte in gkey

	LOOP1:
		xor eax, eax
		mov al, byte ptr[esi + ecx]						//Stores byte in data[ecx] 
		xor al, bl

		//for each data[x]
		ror al, 3										//#E rotate 3 bits LEFT

		xor ah, ah										//#A code table swap - gDecodeTable; keep
		mov al, byte ptr[gDecodeTable + eax]			

		ror al, 4										//#C nibble rotate left 1

		xor al, 0x62									//#D invert bits 1,5,6; 1-8 0x31; keep

		push ecx										//#B reverse bit order - keep
		xor ecx, ecx
		REVERSE :
			rcr al, 1
			rcl ah, 1

			inc ecx
			cmp ecx, 8
			jb REVERSE

		pop ecx
		mov al, ah


		mov[esi + ecx], al								//overwites byte in data with its xor'd value
				
		inc ecx											//increments the count
		cmp ecx, sized
		jb LOOP1										//exits loop when ecx is equal to datalength

	}

	return;
} // decryptData_01


//////////////////////////////////////////////////////////////////////////////////////////////////
// EXAMPLE code to show how to access global variables
int decryptData(char *data, int dataLength)
{
	int resulti = 0;

	gdebug1 = 0;					// a couple of global variables that could be used for debugging
	gdebug2 = 0;					// also can have a breakpoint in C code

	// You can not declare any local variables in C, but can use resulti to indicate any errors
	// Set up the stack frame and assign variables in assembly if you need to do so
	// access the parameters BEFORE setting up your own stack frame
	// Also, you cannot use a lot of global variables - work with registers

	__asm {
		// you will need to reference some of these global variables
		// (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds

		// simple example that xors 2nd byte of data with 14th byte in the key file
		lea esi,gkey				// put the ADDRESS of gkey into esi
		mov esi,gptrKey;			// put the ADDRESS of gkey into esi (since *gptrKey = gkey)

		lea	esi,gPasswordHash		// put ADDRESS of gPasswordHash into esi
		mov esi,gptrPasswordHash	// put ADDRESS of gPasswordHash into esi (since unsigned char *gptrPasswordHash = gPasswordHash)

		mov al,byte ptr [esi+0]				// get first byte of password hash
		mov al,byte ptr [esi+1]				// get 5th byte of password hash
		mov ebx,2
		mov al,byte ptr [esi+ebx]			// get 3rd byte of password hash
		mov al,byte ptr [esi+ebx*2]			// get 5th byte of password hash

		mov ax,word ptr [esi+ebx*2]			// gets 5th and 6th bytes of password hash ( gPasswordHash[4] and gPasswordHash[5] ) into ax
		mov eax,dword ptr [esi+ebx*2]		// gets 4 bytes, as in:  unsigned int X = *( (unsigned int*) &gPasswordHash[4] );

		mov al,byte ptr [gkey+ebx]			// get's 3rd byte of gkey[] data

		mov al,byte ptr [gptrKey+ebx]		// THIS IS INCORRECT - will add the address of the gptrKey global variable (NOT the value that gptrKey holds)

		mov al,byte ptr [esi+0xd];			// access 14th byte in gkey[]: 0, 1, 2 ... d is the 14th byte
		mov edi,data						// Put ADDRESS of first data element into edi
		xor byte ptr [edi+1],al				// Exclusive-or the 2nd byte of data with the 14th element of the keyfile
											// NOTE: Keyfile[14] = 0x21, that value changes the case of a letter and flips the LSB
											// Lowercase "c" = 0x63 becomes capital "B" since 0x63 xor 0x21 = 0x42
	}

	return resulti;
} // decryptData

