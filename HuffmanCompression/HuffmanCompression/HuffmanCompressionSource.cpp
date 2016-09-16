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

	bool friend operator>(const huffNode& A,const huffNode& B)
	{
		return (A.frequency > B.frequency);
	}
};

void inOrderTrav(huffNode *root)
{
	if (root != NULL) {
		inOrderTrav(root->left);
		cout << root->frequency << endl;
		inOrderTrav(root->right);
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

void generateHuffmanTree(priority_queue <huffNode*, vector<huffNode*>, greater<huffNode*>> &heap)
{
	if (heap.size() <= 1)
		return;
	else
	{
		huffNode temp1, temp2;
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

			//run the compression algorithm

		}
	}

	cin.get();
	cin.get();

	return 0;
}