/*
Brett Rothberg
CS547
Karatsuba's Algorithm
*/

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>


//make the two input strings the same length
int sameLength(std::string &s1, std::string &s2){
	int s1_size = s1.size();
	int s2_size = s2.size();
	if(s1_size > s2_size){
		//while(0 < s1.size() - s2.size()){
		for(int i = 0 ; i < s1_size - s2_size; i++){
			//appened 0 bit to front of string 2
			s2 = '0' + s2;
		}
		return s1_size;
	}else if(s2_size > s1_size){
		//while( 0 < s2.size() - s1.size()){
		for(int i = 0; i < s2_size - s1_size; i++){
			// appened 0 bit to the front of string 1
			s1 = '0' + s1;
		}
	}
	return s2_size; //is s2>=s1
}

std::string addBits(std::string s1, std::string s2){
	std::string result;
	int c = 0;
	int size = sameLength(s1, s2);

	for(int i = size - 1; i >= 0; i--){
		//convert from char to int
		int bit1 = s1.at(i) - '0';
		int bit2 = s2.at(i) - '0';

		int sum = (bit1 ^ bit2 ^ c) + '0';

		result = (char)sum + result;

		c = (bit1&bit2) | (bit2&c) | (bit1&c);  //determine carry
	}
	
	if(c){
		result = '1' + result;
	}
	return result;

}

unsigned long long int karaMultiply(std::string s1, std::string s2){

	int size = sameLength(s1,s2);

	if(size == 0) return 0;
	if(size == 1) return (s1.at(0) - '0') * ( s2.at(0) - '0');

	int half1 = size / 2;
	int half2 = size-half1;

	//get the substrings for each half of the two strings
	std::string s1_l = s1.substr(0,half1);
	std::string s1_r = s1.substr(half1, half2); //gets remaining second half of string starting at half1

	std::string s2_l = s2.substr(0,half1);
	std::string s2_r = s2.substr(half1, half2);


	unsigned long long int x = karaMultiply(s1_l, s2_l);
	unsigned long long int y = karaMultiply(s1_r, s2_r);
	unsigned long long int z = karaMultiply(addBits(s1_l, s1_r), addBits(s2_l, s2_r));


	return x*(1<<(2*half2)) + (z - x - y)*(1<<half2) + y;
}


unsigned long long int karaMultiply_base10(unsigned long long int s1, unsigned long long int s2){


	std::string s1_s = std::to_string(s1);
	std::string s2_s = std::to_string(s2);

	int length;
	length = sameLength(s1_s,s2_s);

	s1 = stoull(s1_s);
	s2 = stoull(s2_s);


	if(length == 0) return 0;
	if(length == 1) return (s1) * (s2);

	int half1 = length / 2;
	int half2 = length-half1;

	//split the integers
	int power =  std::pow(10,half2);
	int s1_l = s1 / power;//(10^half1);
	int s1_r = s1 % power;
	int s2_l = s2 / power;
	int s2_r = s2 % power;


	unsigned long long int x = karaMultiply_base10(s1_l, s2_l);
	unsigned long long int y = karaMultiply_base10(s1_r, s2_r);
	unsigned long long int z = karaMultiply_base10((s1_l+s1_r), (s2_l + s2_r));

	return x*(std::pow(10,2*half2)) + (z - x - y)*(std::pow(10,half2)) + y;
}



int main(){

	std::cout << "Binary Multiplication  " << std::endl<< std::endl;

	std::string one = "1100";
	std::string two = "1010";
	std::cout << "result  " << karaMultiply(one,two) << std::endl;
	std::cout << "result  " << karaMultiply("1101", "10110") << std::endl;
	std::cout << "result  " << karaMultiply("101", "11010") << std::endl;
	std::cout << "result  " << karaMultiply("11", "1010") << std::endl;
	std::cout << "result  " << karaMultiply("10", "1010") << std::endl;
	std::cout << "result  " << karaMultiply("111", "111") << std::endl;
	std::cout << "result  " << karaMultiply("1001", "11111") << std::endl;
	std::cout << "result  " << karaMultiply("1000001", "11") << std::endl;
	std::cout << "result  " << karaMultiply("01010101010101010101111110110101", "01010101010101010101010100001010101") << std::endl;

	std::cout<<std::endl << "Base 10 Multiplication  " << std::endl<< std::endl;

	std::cout << "result  " << karaMultiply_base10(13, 14) << std::endl;
	std::cout << "result  " << karaMultiply_base10(1313, 1414) << std::endl;
	std::cout << "result  " << karaMultiply_base10(131, 141) << std::endl;
	std::cout << "result  " << karaMultiply_base10(1234567899, 1234567899) << std::endl;






}
