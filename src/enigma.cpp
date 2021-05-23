#include "enigma.h"

enigma::enigma(std::string settingkey) : settingkey(settingkey)  {
	setRotors();
}

char enigma::input(char ch) {
	// Standard enigma machine keyboard
	std::string keyboard = "ABCDEFGHIJKLMNOPQRSTUWXYZ";  
	if (ch == ' ') {
		return ch;
	}
	// Rotors
	rotorIndex1 = keyboard.find(ch);
	// For every keyboard press we increment turnindex and that causes randomness on the rotors. 
	int smallturnindex = turnindex;
	if (turnindex > 50) { // To avoid string overflow.
		smallturnindex = turnindex % 50;
	}	

	char tempchar = RightRotor1[0];
	RightRotor1.erase(0, 1);
	RightRotor1.push_back(tempchar);
	tempchar = RightRotor2[0]; 
	RightRotor2.erase(0, 1);
	RightRotor2.push_back(tempchar);

	// Center and left rotors also rotate by 1 as turnindex increases.
	if (turnindex % 3 == 0) {
		char turnChar = CenterRotor1[0];
		CenterRotor1.erase(0, 1);
		CenterRotor1.push_back(turnChar);
		turnChar = CenterRotor2[0];
		CenterRotor2.erase(0, 1);
		CenterRotor2.push_back(turnChar);
	}
	if (turnindex % 11 == 0) {
		char turnChar = LeftRotor1[0];
		LeftRotor1.erase(0, 1);
		LeftRotor1.push_back(turnChar);
		turnChar = LeftRotor2[0];
		LeftRotor2.erase(0, 1);
		LeftRotor2.push_back(turnChar);
	}

	// Here enigma machine's emulation.
	rotorIndex1 = RightRotor2.find(RightRotor1[rotorIndex1]);
	rotorIndex1 = CenterRotor2.find(CenterRotor1[rotorIndex1]);
	rotorIndex1 = LeftRotor2.find(LeftRotor1[rotorIndex1]);
	int temp = rotorIndex1;

	// if index points out to the farther element find() will return -1.
	rotorIndex1 = Reflector.find(Reflector[rotorIndex1], rotorIndex1 + 1);  
	if (rotorIndex1 < 0) {
		rotorIndex1 = Reflector.find(Reflector[temp]);
	}
	rotorIndex1 = LeftRotor1.find(LeftRotor2[rotorIndex1]);
	rotorIndex1 = CenterRotor1.find(CenterRotor2[rotorIndex1]);
	rotorIndex1 = RightRotor1.find(RightRotor2[rotorIndex1]);

	return keyboard[rotorIndex1];
}

void enigma::setRotors() {
	// I tried to randomize the order of the letters on the rotors. 
	// We can change their layouts at any time we want.
	// Letters should be distributed randomly with one duplicates.
	RightRotor1 = "APDEFKXYGHRSTCIBLMNOUWJQZAPDEFKXYGHRSTCIBLMNOUWJQZ";
	RightRotor2 = "ABCDEFGHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZ";
	CenterRotor1 = "CDEFIJKLMNOPQGRSAHBYZTUWXCDEFIJKLMNOPQGRSAHBYZTUWX";
    CenterRotor2 = "GHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZABCDEF";
	LeftRotor1 = "HQRSMNAXYKLDOWZPBCTUIJEFGHQRSMNAXYKLDOWZPBCTUIJEFG";
	LeftRotor2 = "ABCDEFGHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZ";
	Reflector = "ABCDEFGHIJKLMABCDEFGHIJKLABCDEFGHIJKLMABCDEFGHIJKL";

	// Setting rotors according to the given letters, like 'CAT'.
	// So, the machine will turn rotors until it sees the given letter.
	// C for the right rotor, A for the center rotor and T for the left rotor.
	int random = 0;
	while (true) {
		if (RightRotor2[random] == settingkey[0]) {
			break;
		}
		else {
			char tempchar = RightRotor2[0];
			RightRotor2.erase(0, 1);
			RightRotor2.push_back(tempchar);
		}
	}
	while (true) {
		if (CenterRotor2[random] == settingkey[1]) {
			break;
		}
		else {
			char tempchar = CenterRotor2[0];
			CenterRotor2.erase(0, 1);
			CenterRotor2.push_back(tempchar);
		}
	}
	while (true) {
		if (LeftRotor2[random] == settingkey[2]) {
			break;
		}
		else {
			char tempchar = LeftRotor2[0];
			LeftRotor2.erase(0, 1);
			LeftRotor2.push_back(tempchar);
		}
	}
	rotorIndex1 = 0;
	turnindex = 0;
}

// Operating enigma machine for the message that is going to be encrypted.
std::string enigma::operateEnigma(std::string message) { 
	std::ostringstream encryptedMessage;
	for (char c : message) {
		if(c == ' '){
			encryptedMessage << ' ';
			continue;
		}
		encryptedMessage << input(c);
		turnindex++;
	}
	setRotors();
	return encryptedMessage.str();
}

void simpleExampleCase(){
	enigma encryption("CAT");
	std::string encrypedMessage = encryption.operateEnigma("TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE");
	std::cout << encrypedMessage << std::endl;
	enigma decryption("CAT");
	std::string decryptedMessage = decryption.operateEnigma(encrypedMessage);
	std::cout << decryptedMessage << std::endl;
	// Output of this test case is:
	// Encryption: WBTIUCTUCWTNTLKWPGYOBAZBECNLK IGSQUCNDMZCDBCGNSUFEDIWSALNKR CRZPUIGTRZXKFHOKJALGSMKAUJKQM FFYPENJMBNLOSROKJGXZSHJRXLQYR PYSNFBADOYERCAODCFHEWPEZLJWII TZWQEQFSJFCCYSUORKUABAZTCCXZU DEMISBDQKGRYWBTPMUMTJKSGWTKHQ
	// Decryption: TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE TESTENCRYPTIONOFENIGMAMACHINE
	// We observe that encryption of the same plaintext changes as it continues because rotors are turning at every cycle.
	
	std::cout << "---------------------------------------------" << std::endl;
	enigma encryption2("RED");
	encrypedMessage = encryption2.operateEnigma("WENEEDADECIPHER WENEEDADECIPHER WENEEDADECIPHER WENEEDADECIPHER");
	std::cout << encrypedMessage << std::endl;
	decryptedMessage = encryption2.operateEnigma(encrypedMessage);
	std::cout << decryptedMessage << std::endl;

	std::cout << "---------------------------------------------" << std::endl;
	encrypedMessage = encryption2.operateEnigma("TESTENCRYPTIONOFENIGMAMACHINE");
	std::cout << encrypedMessage << std::endl;
	decryptedMessage = encryption2.operateEnigma(encrypedMessage);
	std::cout << decryptedMessage << std::endl;
}

int main() {
	simpleExampleCase();
	return 0;
}
