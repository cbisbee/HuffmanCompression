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

	huffNode()
	{
		this->frequency = -1;
		this->character = '?';
		this->left = NULL;
		this->right = NULL;
	}

	huffNode(int freq, char ch,  huffNode* L, huffNode* R)
	{
		this->frequency = freq;
		this->character = ch;
		this->left = L;
		this->right = R;
	}

	bool friend operator> (const huffNode& n1,const huffNode& n2)
	{
		return (n1.frequency > n2.frequency);
	}
};

void generateEncodings(huffNode *root, string encoding, string encodingTable[256]) //need to adjust this function to not build a string but instead a bitstring
{
	if (root != NULL) {
		if (root->character != -1)
		{
			encodingTable[root->character] = encoding;
		}
		generateEncodings(root->left,(encoding + '0'),encodingTable);
		generateEncodings(root->right,(encoding + '1'),encodingTable);
	}
}

void generateInitialPQueue(int frequencyList[256], priority_queue<huffNode*, vector<huffNode*>, greater<huffNode*>> &nodeHeap)
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

void printPQueue(priority_queue<huffNode, vector<huffNode>, greater<huffNode>> &nodeHeap)
{
	if (nodeHeap.empty())
	{
		cout << "There's nothing in the heap!" << endl;
	}
	while (!nodeHeap.empty())
	{
		huffNode tempNode;
		tempNode = nodeHeap.top();
		cout << tempNode.character << endl;
		nodeHeap.pop();
	}
}

void initializeFrequencyList(int frequencyList[256])
{
	for (int i = 0; i < 256; i++)
		frequencyList[i] = 0;
}

void initializeEncodingTable(string encodingTable[256])
{
	for (int i = 0; i < 256; i++)
	{
		encodingTable[i] = "";
	}
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
			//cout << "ASCII " << i << ": " << frequencyList[i] << endl;
		}
	}
}

void printEncodings(string table[256])
{
	for (int i = 0; i < 256; i++)
	{
		if (table[i] != "")
		{
			cout << "Character " << (char)i << " encoding: " << table[i] << endl;
		}
	}
}

void generateHuffmanTree(priority_queue <huffNode*, vector<huffNode*>, greater<huffNode*>> &heap)
{
	if (heap.size() <= 1)
		return;
	else
	{
		//huffNode temp1, temp2;
		huffNode *node1, *node2;

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
			priority_queue<huffNode*, vector<huffNode*>, greater<huffNode*> > nodeHeap;
			string encodingTable[256]; //need to change this from a string to a bit sort of data structure

			initializeFrequencyList(frequencyList);
			generateFrequencyList(fin, frequencyList);
			printFrequencyList(frequencyList);
			generateInitialPQueue(frequencyList, nodeHeap);
			//printPQueue(nodeHeap);
			//cout << nodeHeap.size();
			generateHuffmanTree(nodeHeap);

			huffNode *root;
			root = nodeHeap.top();
			nodeHeap.pop();

			//create the encoding for each leaf node (character)
			initializeEncodingTable(encodingTable);
			generateEncodings(root, "", encodingTable);
			printEncodings(encodingTable);

			//run the compression algorithm

		}
	}

	cin.get();
	cin.get();

	return 0;
}