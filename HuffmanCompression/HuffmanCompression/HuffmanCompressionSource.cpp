#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <bitset>

using namespace std;

struct huffNode
{
	int frequency;
	char character;
	huffNode* left;
	huffNode* right;
};

struct node_comparison
{
	bool operator () (const huffNode* A, const huffNode* B) const
	{
		return A->frequency > B->frequency;
	}
};

void generateEncodings(huffNode *root, vector<bool> & encoding, vector<vector<bool>> & encodingTable)
{
	if (root != NULL) {
		if (root->character != -1)
		{
			encodingTable[root->character] = encoding;
		}
		vector<bool> leftEncoding = encoding;
		leftEncoding.push_back(false);
		vector<bool> rightEncoding = encoding;
		rightEncoding.push_back(true);

		generateEncodings(root->left,leftEncoding,encodingTable);
		generateEncodings(root->right,rightEncoding,encodingTable);
	}
}

void generateInitialPQueue(int frequencyList[256], priority_queue<huffNode*, vector<huffNode*>, node_comparison> &nodeHeap)
{
	for (int i = 0; i < 256; i++)
	{
		if (frequencyList[i] != 0)
		{
			huffNode* tempNode;
			tempNode = new huffNode;
			tempNode->frequency=frequencyList[i];
			tempNode->character = (char)i;
			tempNode->left = NULL;
			tempNode->right = NULL;
			nodeHeap.push(tempNode);
		}
	}
}

void printPQueue(priority_queue<huffNode*, vector<huffNode*>, node_comparison> &nodeHeap)
{
	if (nodeHeap.empty())
	{
		cout << "There's nothing in the heap!" << endl;
	}
	while (!nodeHeap.empty())
	{
		huffNode* tempNode;
		tempNode = nodeHeap.top();
		cout << tempNode->character << endl;
		nodeHeap.pop();
	}
}

void initializeFrequencyList(int frequencyList[256])
{
	for (int i = 0; i < 256; i++)
		frequencyList[i] = 0;
}

void generateFrequencyList(ifstream& fin, int frequencyList[256])
{
	string line = "";
	while (!fin.eof())
	{
		getline(fin,line);
		for (int i = 0; i < line.length(); i++)
		{
			frequencyList[line[i]] += 1;
		}
		frequencyList[10] += 1;
		line = "";
	}
}

void printFrequencyList(int frequencyList[256])
{
	for (int i = 0; i < 256; i++)
	{
		if (frequencyList[i] != 0)
		{
			cout << "Character " << (char)i << ": " << frequencyList[i] << endl;
		}
	}
}

void getEncoding(vector<bool> bitString, string &encoding)
{
	for (int i = 0; i < bitString.size(); i++)
	{
		encoding += bitString[i];
	}
}

void printEncodings(vector<vector<bool>> encodingTable)//vector<bool> encodingTable[256])
{
	string encoding = "";
	for (int i = 0; i < 256; i++)
	{
		if (!(encodingTable[i].empty()))
		{
			getEncoding(encodingTable[i], encoding);
			cout << "Character " << (char)i << " encoding: " << encoding << endl;
			encoding = "";
		}
	}
}

void generateHuffmanTree(priority_queue <huffNode*, vector<huffNode*>, node_comparison> &heap)
{
	if (heap.size() <= 1)
		return;
	else
	{
		huffNode *node1, *node2;
		node1 = new huffNode;
		node2 = new huffNode;

		node1 = heap.top();
		heap.pop();
		node2 = heap.top();
		heap.pop();
		int totalFreq = node1->frequency + node2->frequency;

		huffNode* newNode;
		newNode = new huffNode;
		newNode->frequency = totalFreq;
		newNode->character = (char)255;
		newNode->left = node1;
		newNode->right = node2;

		heap.push(newNode);
		totalFreq = 0;
		generateHuffmanTree(heap);
	}
}

void convertBuffer(vector<bool> & ogBuffer, bitset<8> &newBuffer)
{
	for (int i = 0; i < ogBuffer.size(); i++)
	{
		newBuffer[i] = ogBuffer[i];
	}
}

void checkBuffer(vector<bool> & bitBuffer, vector<bool> & encoding, ofstream &fout)
{
	while (encoding.size() != 0 )
	{
			int availableSpace = 8 - bitBuffer.size();
			for (int i = 0; i < availableSpace; i++)
			{
				if (!encoding.empty())
				{
					bitBuffer.push_back(encoding[0]);
					encoding.erase(encoding.begin());
				}
			}

			if (bitBuffer.size() == 8)
			{
				//convert vector<bool> to bitbuffer for output
				bitset<8> outputBuffer;
				convertBuffer(bitBuffer, outputBuffer);
				unsigned char n = outputBuffer.to_ulong();
				fout << n;
				bitBuffer.clear();
			}

		//cout << "You're Stuck" << endl;
	}
}

void streamEncoding(ifstream &fin, ofstream &fout, vector<bool> & bitBuffer, vector<vector<bool>> encodingTable)
{
	string line = "";
	vector<bool> currentEncoding;
	while (!fin.eof())
	{
		getline(fin, line);
		for (int i = 0; i < line.length(); i++)
		{
			currentEncoding = encodingTable[line[i]];
			checkBuffer(bitBuffer, currentEncoding, fout);
			while (!currentEncoding.empty())
			{
				bitBuffer.push_back(currentEncoding[0]);
				currentEncoding.erase(currentEncoding.begin());
			}
		}
		line = "";
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Please rerun the program and type a source file to be compressed." << endl;
	}

	else
	{
		//argv[1] contains the filename to be provided
		string inFileName = argv[1];
		ifstream fin(inFileName);
		if (fin.fail())
			cout << "Could not open target file, please rerun the program with a valid text file (.txt)." << endl;
		else
		{
			string outFileName = inFileName.substr(0, inFileName.length() - 3) + "mcp"; //assumes a .txt file (changes output to a .mcp) 
			ofstream fout(outFileName, ios::binary | ios::out);
			int frequencyList[256];
			priority_queue<huffNode*, vector<huffNode*>, node_comparison> nodeHeap;
			vector<bool> startEncoding;
			vector<vector<bool>> encodingTable(256, vector<bool>(0));
			vector<bool> bitBuffer;
			


			initializeFrequencyList(frequencyList);
			generateFrequencyList(fin, frequencyList);
			fin.close();
			//printFrequencyList(frequencyList);
			generateInitialPQueue(frequencyList, nodeHeap);
			generateHuffmanTree(nodeHeap);

			huffNode *root;
			root = nodeHeap.top();
			nodeHeap.pop();

			//create the encoding for each leaf node (character)
			generateEncodings(root, startEncoding, encodingTable);
			//printEncodings(encodingTable);

			//run the compression algorithm
			fin.open(inFileName);
			streamEncoding(fin, fout, bitBuffer, encodingTable);

		}
	}

	cout << "Done." << endl;

	cin.get();
	cin.get();

	return 0;
}