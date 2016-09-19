#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <queue>

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
		rightEncoding.push_back(false);

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


			initializeFrequencyList(frequencyList);
			generateFrequencyList(fin, frequencyList);
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

		}
	}

	cin.get();
	cin.get();

	return 0;
}