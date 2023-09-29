
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <stdexcept>

#include "DLXML_MessageManager.h"
#include "DLXML_MessageEncoder.h"
#include "DLXML_MessageDecoder.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::copy;
using std::vector;
using std::ostream_iterator;
using std::istream_iterator;
using std::map;

using namespace DLXML;

static const char* const CONST_STR_INPUT_FILE_1 = "SampleInputSet_1.txt";
static const char* const CONST_STR_INPUT_FILE_2 = "SampleInputSet_2.txt";
static const char* const CONST_STR_INPUT_FILE_3 = "SampleInputSet_3.txt";
static const char* const CONST_STR_INPUT_FILE_ALL = "SampleInputSet_All.txt";

vector<string> getInputMessagesToEncode(const string& arstrInputFileName)
{
	vector<string> vecDataStreams;
	std::ifstream ifsMessages(arstrInputFileName.c_str(), std::ios::in);
	if (!ifsMessages) {
		std::cerr << "Input file not found " + arstrInputFileName;
		return vecDataStreams;
	}
	
	vector<string> vecTokens;
	istream_iterator<string> isitrInFile(ifsMessages), eos;
	copy(isitrInFile, eos, back_inserter(vecTokens));

	string strMessageStream;
	for(size_t j = 0; j < vecTokens.size(); ++j) {
		strMessageStream.append(vecTokens[j]);
		strMessageStream.append(1, ' ');
		if (vecTokens[j].find("</Message>") != string::npos) {
			vecDataStreams.push_back(strMessageStream);
			strMessageStream.clear();
		}
	}
	return vecDataStreams;
}

void testDLXMLGeneral()
{
	vector<string> vecEncodedMessages;
	vector<string> vecDataStreams = getInputMessagesToEncode(CONST_STR_INPUT_FILE_ALL);

	try {
		cMessageManager& objMessageManager = cMessageManager::getInstance();
		objMessageManager.addGeneralSymbol("DLRP_Latitude", "1.534");
		cMessageEncoder* pcMessageEncoder = objMessageManager.getMessageEncoder();
		vecEncodedMessages = pcMessageEncoder->encodeMessages(vecDataStreams);
		std::cout << "\nEncoded Messages:\n";
		copy(vecEncodedMessages.begin(), vecEncodedMessages.end(), ostream_iterator<string>(cout, "\n"));
		//following two lines are just for demonstration purposes.
		cout << pcMessageEncoder->getMessageRepository();
		cout << pcMessageEncoder->getErrorRepository();

		cout << endl << string(160, '*') << endl;
		cMessageDecoder* pcMessageDecoder = objMessageManager.getMessageDecoder();
		vector<string> vecDecodedMessages = pcMessageDecoder->decodeMessages(vecEncodedMessages);
		cout << "\nDecoded Messages:\n";
		copy(vecDecodedMessages.begin(), vecDecodedMessages.end(), ostream_iterator<string>(cout, "\n"));
		cout << pcMessageDecoder->getErrorRepository();
	}
	catch (DLXML_Exception& exp) {
		cout << "Exception is caught:" << exp.what() << "\n";
	}
	catch (const std::exception& exp) {
		cout << "Exception is caught:" << exp.what() << "\n";
	}
	return;
}

void testMessageEncoding()
{
	vector<string> vecInputMessagesToEncode =\
		getInputMessagesToEncode(CONST_STR_INPUT_FILE_2);
	cMessageManager& objMessageManager = cMessageManager::getInstance();
	objMessageManager.addGeneralSymbol("DLRP_Latitude", "1.534");
	
	cMessageEncoder* pcMessageEncoder = 0;
	try {
		
	pcMessageEncoder = objMessageManager.getMessageEncoder();
	
	vector<string> vecEncodedMessages = pcMessageEncoder->\
		encodeMessages(vecInputMessagesToEncode);
	
	std::cout << "\nresult of encoding " << endl;
			copy(vecEncodedMessages.begin(), vecEncodedMessages.end(), ostream_iterator<string>(cout, "\n"));
	
	} catch(const std::exception& rExp) {
		cout << "Exception caught in DLXML:" << rExp.what() << endl;
	}
	
	cout << pcMessageEncoder->getMessageRepository();
		const cErrorRepository& refErrorRepository = pcMessageEncoder->getErrorRepository();
		cout << refErrorRepository;

	return;
}

void testMessageDecoding()
{
	string arrEncodedMessages[] = {
			"011010111101000010110110001111111110110111000111",
			"000101111110000000111001100100000000000000010100",
			"001111111100001101100110100000011110111000010101"
	};

	vector<string> vecEncodedMessages(arrEncodedMessages, arrEncodedMessages + 3);
	try {
		cMessageManager& objMessageManager = cMessageManager::getInstance();
		objMessageManager.addGeneralSymbol("DLRP_Latitude", "1.534");
		cMessageDecoder* pcMessageDecoder = objMessageManager.getMessageDecoder();
		vector<string> vecDecodedMessages = pcMessageDecoder->decodeMessages(vecEncodedMessages);
		cout << "\nDecoded Messages:\n";
		copy(vecDecodedMessages.begin(), vecDecodedMessages.end(), ostream_iterator<string>(cout, "\n"));
		const cErrorRepository& refErrorRepositoy = pcMessageDecoder->getErrorRepository();
		cout << pcMessageDecoder->getErrorRepository();
		cout << refErrorRepositoy;
	} catch(std::exception& rExp) {
		cout << "Exception caught:" << rExp.what();
	}
	return;
}

int main()
{
	try {
		//testMessageEncoding();
		//testMessageDecoding();
		testDLXMLGeneral();
	} catch(const std::exception& rExp) {
		cout << "Exception caught:" << rExp.what();
	}
	return 0;
}
