#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

std::vector<char> letters = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };  

enum letterenum { A, B, C ,D ,E ,F, G, H, I, J, K, L ,M ,N ,O, P, Q, R, S, T, U, W, X, Y, Z };

class enigma
{
private:
	std::string io;
	std::string RightRotor1;
	std::string RightRotor2;
	std::string CenterRotor1;
	std::string CenterRotor2;
	std::string LeftRotor1;
	std::string LeftRotor2;
	std::string Reflector;  
	int rotorIndex1 = 0;
	int turnindex = 0;
	std::string settingkey;
	// Letters should be distributed randomly with one duplicates.
public:
	enigma(std::string settingkey);
	//~enigma();
	void setRotors();
	char input(char ch);
	std::string test(std::string message);
};

