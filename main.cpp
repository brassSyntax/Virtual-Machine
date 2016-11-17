#include <iostream>
#include <fstream>

using namespace std;

bool runCommand(unsigned int& widx, char code, char operands, char* registers);

ifstream pFile("q1_encr.txt", ios::binary);

int main()
{
	ifstream readBin("decryptor.bin", ios::binary);

	unsigned int widx = 0; // working index

	int pLength = 0; // program length

	char pMemory[256], cReg[16];

	if (readBin.fail())
	{
		cout << "Failed to open program.";

		return 0;
	}
	else
	{
		// Read to memory
		while (!readBin.eof())
		{
			readBin >> noskipws >> pMemory[pLength];
			if (readBin.good()) pLength++;
		}

		readBin.close();
	}

	// Execute program
	while (widx < pLength)
	{
		if (runCommand(widx, pMemory[widx], pMemory[widx + 1], cReg))
		{
			widx += 2;
		}
		else break;
	}

	return 0;
}

bool runCommand(unsigned int& widx, char code, char operands, char* registers)
{
	char* reg1, *reg2, *reg0;

	reg1 = &registers[operands & 0x0F];
	reg2 = &registers[operands >> 4];
	reg0 = &registers[0];

	switch (code)
	{
		case 0x01: // INC Rx
			*reg1 += 1;
			break;

		case 0x02: // DEC Rx
			*reg1 -= 1;
			break;

		case 0x03: // MOV Rx, Ry
			*reg1 = *reg2;
			break;

		case 0x04: // MOVC const
			*reg0 = operands;
			break;

		case 0x05: // LSL Rx
			*reg1 <<= 1;
			break;

		case 0x06: // LSR Rx
			*reg1 >>= 1;
			break;

		case 0x07: // JMP addr
			widx += operands - 2; // -2 to ignore the passive jump
			break;

		case 0x08: // JZ addr

			break;
		case 0x09: // JNZ addr

			break;
		case 0x0A: // JFE
			if (pFile.eof())
				widx += operands - 2; // -2 to ignore the passive jump
			break;

		case 0x0B: // RET
			return false;

		case 0x0C: // ADD Rx, Ry
			*reg1 += *reg2;
			break;

		case 0x0D: // SUB Rx, Ry
			*reg1 -= *reg2;
			break;

		case 0x0E: // XOR Rx, Ry
			*reg1 = *reg1 ^ *reg2;
			break;

		case 0x0F: // OR Rx
			*reg1 = *reg1 | *reg2;
			break;

		case 0x10: // IN Rx
			if(pFile.good() && !pFile.eof())
			pFile >> noskipws >> *reg1;
			break;

		case 0x11: // OUT Rx
			cout << *reg1;
			break;

		default:
			cout << hex << int(code) << " " << "Unrecognized command code." << endl;
			break;
	}

	return true;
}
