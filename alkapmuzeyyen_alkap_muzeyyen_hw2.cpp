// Muzeyyen ALKAP / 25046
#include <iostream>
#include <string>
#include <fstream>
#include "bst.h"
#include "avl.h"
#include <chrono>

using namespace std;

string upper(string &word) {for (auto & c: word) {c = toupper(c);}; return word;}
string makeAlphabetic(string& s)
{
	for(string::iterator i = s.begin(); i != s.end(); i++)
	{
		if(!isalpha(s.at(i - s.begin())) && s.at(i - s.begin()) != ' ' )
		{
			s.erase(i);
			i--;
		}
	}
	s = upper(s);
	return s;
}
string lower(string &word)
{
	toupper(word[0]);
	for (int i = 1; i < word.length(); i++)
		tolower(word[i]);
	return word;
}

bst<Node>* bstree = new bst<Node>(nullptr);
avl<Node>* avltree = new avl<Node>(nullptr);


void chooseAction()
{
	cout << "\nChoose which action to perform from 1 to 6:"<< endl;
	cout << "1 - Search a phonebook contact\n2 - Adding a phonebook contact"<< endl;
	cout << "3 - Deleting a phonebook contact\n4 - Print the phonebook to a file(inorder)"<< endl;
	cout << "    Print the phonebook to a file(postorder)\n5 - Draw the Phonebook as a Tree to a file"<< endl;
	cout << "6 - Press 6 to exit\n" << endl;
}

template <class T>
void inOrderPrintToFileHelper(ofstream& file, T* temp)
{
	if (temp)
	{
		inOrderPrintToFileHelper(file, temp->right);
		file << temp->element << " " << temp->phone << " " << temp->city << endl;
		inOrderPrintToFileHelper(file,temp->left);
	}
}

template <class T>
void preOrderPrintToFileHelper(ofstream& file, T* temp)
{
	if (temp)
	{
		file << temp->element << " " << temp->phone << " " << temp->city << endl;
		preOrderPrintToFileHelper(file,temp->left);
		preOrderPrintToFileHelper(file,temp->right);
	}
}

template <class T>
void searchContactHelper(T newNode, T* root, bool& exist) // newNode = which node will be searched; root = root of the tree, then it will move
{
	if (!root) //all nodes are travelled but couldn't find
		return;
	if (root->element == newNode.element)
	{ //exactly desired node is found
		root->print();
		exist = true;
		return;
	}
	size_t found = root->element.find(newNode.element);
	if (found != string::npos)
	{ //some part of desired element is found
		root->print();
		exist = true;
	}
	searchContactHelper( newNode,root->left, exist);
	searchContactHelper( newNode, root->right, exist);
	// the node already exists which is caught above
}

template <class T>
T* findMin(T*& temp)  //finds the minimum element in the given subtree
{// check if u used it
	if (temp == NULL)
		return nullptr;
	if (temp->left == nullptr)
		return temp;

	return  findMin(temp->left);
} 

template <class T>
T* findMax(T*& temp) //finds the maximum element in the given subtree 
{// check if u used it
	if (temp== NULL)
		return nullptr;
	if (temp->right == NULL)
		return temp;

	return findMax(temp->right );
}

template <class T>
void drawTreeHelper(const string& prefix, const T* node, bool isLeft, ofstream& file)
{
    if( node != nullptr )
    {
        file << prefix;

	    file << (!isLeft ? "|--" : "|__" );

		// print the value of the node
       	file << node->element << endl;

        // enter the next tree level - left and right branch
        drawTreeHelper( prefix + (!isLeft ? "|   " : "    "), node->right, false, file);
		drawTreeHelper( prefix + (!isLeft ? "|   " : "    "), node->left, true, file);
    }
}

template <class T>
void drawTreeFirst(const T* node, ofstream& file)
{
    drawTreeHelper("", node, true, file);    
}

int main()
{
	int COUNT = 0;
	clock_t beginA, beginB, endA, endB;
	double elapsed_secsB, elapsed_secsA;

	ifstream file;
	ofstream fileOut;
	string fileName = "PhoneBook-sample3.txt";
	//cout << "Please enter the contact file name:" << endl; cin >> fileName;

	file.open(fileName);
	string name,  phn, city;

	beginB = clock();
	while (!file.eof())
	{
		string sname;		file >> name >> sname >> phn >> city;
		name = makeAlphabetic(name+" "+sname); lower(city);
		
		Node* temp = new Node(name, phn, city);

		bstree->addContact(*temp);
	}
	endB = clock(); elapsed_secsB = double(endB - beginB);
	cout << "\nPhonebook creation in BST took "<< elapsed_secsB <<" milliseconds\n" << endl; bstree->isBalanced(bstree->root);
	cout << "\n\nThe heights of BST are for left: " << getHeightBST(bstree->root->left) << " and right: " << getHeightBST(bstree->root->right) << " loading the phonebook into an AVL" << endl;
	file.clear(); file.seekg(0, ios::beg);

	beginA = clock();
	while (!file.eof())
	{
		string sname;
		file >> name >> sname >> phn >> city;
		name = makeAlphabetic(name+" "+sname);  lower(city);

		Node* temp2 = new Node(name, phn, city);

		avltree->addContact(*temp2);

	} file.close();
	endA = clock(); elapsed_secsA = double(endA - beginA);
	cout << "\nPhonebook creation in AVL took "<< elapsed_secsA <<" milliseconds\n" << endl; avltree->isBalanced(avltree->root);
	cout << "\nThe heights of AVL tree for left: " << getHeight(avltree->root->left) << " and right: "<< getHeight(avltree->root->right)<<endl;

	int action;    bool cont = true, done = false;

	Node tempBST;	Node *ptrBST = nullptr;
	Node tempAVL;	Node *ptrAVL = nullptr;
	while (cont)
	{
		chooseAction();
		cin >> action;
		switch (action)
		{
		case(1):  //search a phonebook contact
			cout << "Search for a contact: ";	cin.ignore();
			getline(cin, name);		name = makeAlphabetic(name);
			tempBST.element = name; tempAVL.element = name;
			done = false;
			beginB = clock();
			cout << "\nPhonebook: Searching for: ("<< name <<")"<< endl;
			cout << "======================================" << endl;
			bstree->searchContact(tempBST, done);
			endB = clock();		elapsed_secsB = double(endB - beginB);

			beginA = clock();
			cout << "Searching an item in the phonebook (AVL) ..."<< endl;
			cout << "Phonebook: Searching for: ("<< name <<")"<< endl;
			cout << "======================================" << endl;
			avltree->searchContact(tempAVL, done);
			endA = clock();		elapsed_secsA = double(endA - beginA);
			
			if(done){
				cout << "\nThe serach in BST took " << elapsed_secsB << " milliseconds..." << endl;
				cout << "The serach in AVL took " << elapsed_secsA << " milliseconds..." << endl;}
			else
				cout << "There is no such a node !" << endl;

			break;

		case(2):  //Adding a phonebook contact
			cout << "Enter the information of the contact to be added:\nName: ";	cin.ignore();
			getline(cin, name); name = makeAlphabetic(name);
			cout << "\nTel: " ; cin >> phn;
			cout << "City: "; cin >> city;  lower(city);

			ptrBST = new Node(name, phn, city);
			ptrAVL = new Node(name, phn, city);

			beginB = clock();
			done = bstree->addContact(*ptrBST);	endB = clock();

			if (done){
				cout << "Contact has been added successfully to the BST" << endl;
				elapsed_secsB = double(endB - beginB) / 1000000;
				cout << "Adding a contact to the Binary Tree took " << elapsed_secsB << " nanoseconds..." << endl;

				beginA = clock();
				cout << "Adding an item to the phonebook (AVL) ..." << endl;
				avltree->addContact(*ptrAVL);
				endA = clock();
				cout <<	"Contact has been added successfully to the AVL tree" << endl;

				elapsed_secsA = double(endA - beginA) / 1000000;
				cout << "Adding a contact to the AVL Tree took " << elapsed_secsA << " nanoseconds..." << endl;}
			else
				cout << "There is already such a person" << endl;

			break;

		case(3):  //Deleting a phonebook contact
			cout << "Deleting an item from the notebook ..." << endl;
			cout << "======================================" << endl;
			cout <<	"Enter the fullname of the contact to be deleted: "<< endl;		cin.ignore();
			getline(cin, name);		name = makeAlphabetic(name);

			beginB = clock(); 
			done = bstree->deleteContact(name); 
			endB=clock();
			elapsed_secsB = double(endB - beginB) / 1000000;
			beginA = clock(); avltree->deleteContact(name); endA=clock();
			elapsed_secsA = double(endA - beginA) / 1000000;

			if (done){
				cout << "Deleted successfully.." << endl;
				cout << "Deleted from Binary Tree took " <<  elapsed_secsB << "nanoseconds" << endl;
				cout << "Deleted from AVL Tree took " <<  elapsed_secsA << "nanoseconds" << endl;}

			break;

		case(4):  //Printing
			fileOut.open("phonebookInOrder.txt");
			bstree->inOrderPrintToFile(fileOut);
			fileOut.close();

			fileOut.open("phonebookPreOrder.txt");
			bstree->preOrderPrintToFile(fileOut);
			fileOut.close();

			/* you can also use avl tree to print in and pre orders

			ofstream fileOut;
			fileOut.open("phonebookTreeAVL.txt");
			rootAVL->inOrderPrintToFile(fileOut);
			fileOut.close();

			ofstream fileOut;
			fileOut.open("phonebookTreeAVL.txt");
			rootAVL->preOrderPrintToFile(fileOut);
			fileOut.close();*/ 

			break;

		case(5):  //Drawing the Tree
			fileOut.open("phonebookTreeBST.txt");
			bstree->drawTree(fileOut);
			fileOut.close();

			fileOut.open("phonebookTreeAVL.txt");
			avltree->drawTree(fileOut);
			fileOut.close();

			break;
		case(6):  //exit
			cont = false;

			break;

		default:
			cout << "Only 1 to 6 are valid cases" << endl;
		}
	}

	return 0;
}
