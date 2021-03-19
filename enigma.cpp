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
	if (turnindex > 25) { // To avoid string overflow.
		smallturnindex = turnindex % 10;
	}	
	RightRotor1.erase(0, 1);
	RightRotor1.push_back(keyboard[smallturnindex]);
	// Center and left rotors also rotate by 1 as turnindex increases.
	if (turnindex % 3 == 0) {
		CenterRotor1.erase(0, 1);
		CenterRotor1.push_back(keyboard[turnindex/3]);
	}
	if (turnindex % 11 == 0) {
		LeftRotor1.erase(0, 1);
		LeftRotor1.push_back(keyboard[turnindex / 11]);
	}
	// Here enigma machine's emmulation.
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
	RightRotor1 = "APDEFKXYGHRSTCIBLMNOUWJQZAPDEFKXYGHRSTCIBLMNOUWJQZ";
	RightRotor2 = "ABCDEFGHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZ";
	CenterRotor1 = "CDEFIJKLMNOPQGRSAHBYZTUWXCDEFIJKLMNOPQGRSAHBYZTUWX";
    CenterRotor2 = "GHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZABCDEF";
	LeftRotor1 = "HQRSMNAXYKLDOWZPBCTUIJEFGHQRSMNAXYKLDOWZPBCTUIJEFG";
	LeftRotor2 = "ABCDEFGHIJKLMNOPQRSTUWXYZABCDEFGHIJKLMNOPQRSTUWXYZ";
	Reflector = "ABCDEFGHIJKLMABCDEFGHIJKLABCDEFGHIJKLMABCDEFGHIJKL";

	// Setting rotors according to the given letters, like 'CAT'.
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

std::string enigma::test(std::string message) {
	std::ostringstream encryptedMessage;
	for (char c : message) {
		encryptedMessage << input(c);
		turnindex++;
	}
	return encryptedMessage.str();

}

int main() {
	enigma encryption("CAT");
	std::string encrypedMessage = encryption.test("TESTENCRPYTION OF ENIGMAMACHINE");
	std::cout << encrypedMessage << std::endl;
	enigma decryption("CAT");
	std::string decryptedMessage = decryption.test(encrypedMessage);
	std::cout << decryptedMessage << std::endl;
	/*Results:
	TWYELOOGYKDHWU TM ODBUSDFPPCJSC  -> after encryption
	TESTENCRPYTION OF ENIGMAMACHINE  -> after decryption
	*/

	return 0;
}
