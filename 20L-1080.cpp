							/* Nabeeha Mudassir 20L-1080 */
									/* PROJECT */
#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <stdlib.h>
#include <ctime>
using namespace std;

//----------------------------------------------------------------------------------------//
class User
{
private:
	int Key; //Key = Priority
	int UserID;
	char InfoType;
	 //Read = R; Write = W;
public:
	template <class T>
	friend class PriorityQueue;


	friend class File;
	User()
	{
		Key = -1; UserID = 0; InfoType = 0;
	}
	User(int _Key, int _UserID, char _InfoType)
	{
		Key = _Key; UserID = _UserID; InfoType = _InfoType;
	}
	void SetValues(int _Key, int _UserID, char _InfoType)
	{
		Key = _Key; UserID = _UserID; InfoType = _InfoType;
	}
	void Print()
	{
		cout  <<" ID = " << UserID << "Priorty " << Key << ","<<InfoType;
	}
	int Priority()
	{
		return Key;
	}
	int getID()
	{
		return UserID;
	}
	char getInfotype()
	{
		return InfoType;
	}
};

//----------------------------------------------------------------------------------------//


template<class T>
class PriorityQueue
{
private:
	vector<T> Queue; //this is actually vector of Users
	int CurrentSize;

public:
	template <class T>
	friend class FileRepository;

	friend class File;

	PriorityQueue()
	{
		CurrentSize = 0;
	}
	
	void BuildQueue(User Arr[], int ArraySize)
	{
		CurrentSize = ArraySize;

		User* thisNode = 0;
		User* CurrNode = Arr;
		for (int i = 0; i < ArraySize; i++)
		{
			if (CurrNode) //protects against null nodes if any.
			{
				//thisNode = new User(CurrNode->Key,CurrNode->UserID,CurrNode->InfoType);
				User thisUser(CurrNode->Key,CurrNode->UserID,CurrNode->InfoType);
				CurrNode++;

				Queue.push_back(&thisUser); 

				HeapUp(0,i);
			}
		}
	}

	void HeapUp(int IndexofRoot, int LastIndex)
	{
		int ParentIndex = 0;

		if (LastIndex > IndexofRoot)
		{
			ParentIndex = (LastIndex-1)/2;
			if (Queue[ParentIndex].Key < Queue[LastIndex].Key)
			{
				SwapRows(Queue[ParentIndex],Queue[LastIndex]);
				HeapUp(IndexofRoot,ParentIndex);
			}
		}

	}

	//erase function is added.
	void ExtractMax(User & MaxNode)
	{
		if (CurrentSize > 0)
		{
			MaxNode.SetValues(Queue[0].Key,Queue[0].UserID,Queue[0].InfoType);

			Queue[0].Key = Queue[CurrentSize-1].Key;
			Queue[0].UserID = Queue[CurrentSize-1].UserID;
			Queue[0].InfoType = Queue[CurrentSize-1].InfoType;

			Queue.erase(Queue.begin()+CurrentSize-1);
			
			CurrentSize--;
		
			HeapDown(0,CurrentSize-1);
		}
	}

	User& FindMax()
	{
		if (CurrentSize > 0)
			return Queue[0];
	}
	
	User& FindMin()
	{
		if (CurrentSize > 0)
			return Queue[CurrentSize-1];
	}
	
	void SwapRows(User& First, User& Second)
	{
		if (&First == &Second)
			return;
		int tempUserId = First.UserID; char tempInfo = First.InfoType; int tempKey = First.Key;
		First.SetValues(Second.Key,Second.UserID,Second.InfoType);
		Second.SetValues(tempKey,tempUserId,tempInfo);
	}

	void Print()
	{
		cout << endl;
		for (int i = 0; i < CurrentSize; i++)
		{
			cout << Queue[i].Key << " ";
		}
	}

	void HeapDown(int CurrentIndex, int LastIndex)
	{
		int MaxChild = 0; 
		int RightChild = CurrentIndex*2 + 2; 
		int LeftChild = CurrentIndex*2 + 1;

		if (LeftChild <= LastIndex)
		{
			if (LeftChild == LastIndex)
			{
				MaxChild = LeftChild;
			}
			else if (Queue[LeftChild].Key <= Queue[RightChild].Key)
				MaxChild = RightChild;
			else
				MaxChild = LeftChild;

			if (Queue[MaxChild].Key > Queue[CurrentIndex].Key)
			{
				SwapRows(Queue[MaxChild],Queue[CurrentIndex]);
				HeapDown(MaxChild,LastIndex);
			}
		}
	}

	void Insert(T & newUser)
	{
		CurrentSize++;
		Queue.push_back(newUser);
		HeapUp(0,CurrentSize-1);
	}

	int Size()
	{
		return CurrentSize;
	}
	void Delete()
	{
		for (int i = 0; i < Queue.size(); i++)
			delete Queue[i];
	}
	
	~PriorityQueue()
	{
		
	}
	
	void Empty()
	{
		Queue.clear();
	}
};


//------------------------------------------------------------------------------------------//
class File
{
private:
	int FileID;
	PriorityQueue<User> UserWaitingQueue; 
	int CurrentUsers;
	vector<User> CurrentAccess;
public: 
	template <class T>
	friend class FileRepository;

	//Adds User to CurrentAccess Vector
	void GiveAccess(User& AccessingUser)
	{
		CurrentAccess.push_back(AccessingUser);
	}
	
	//Removes User ID from CurrentAccess Vector
	void RemoveAccess(int UserID, bool & deleted)
	{
		deleted  = false;

		vector<User> :: iterator it = CurrentAccess.begin();
		int count = 0; 
		while (it != CurrentAccess.end())
		{
			if (it->getID() == UserID)
			{
				CurrentAccess.erase(CurrentAccess.begin() + count);
				deleted = true;
				break;
			}
			count++;
			it++;
		}
		if (deleted == false)
		{
			cout << "\nUser " << UserID << " is not currently accessing this File so cannot release file.";
		}
	}
	
	int SizeofWaitingQueue()
	{
		return UserWaitingQueue.Size();
	}
	
	File(int ID)
	{
		FileID = ID;
		CurrentUsers = 0;
	}
	
	~File()
	{
		for (int i = 0; i < UserWaitingQueue.Size() ;i++)
		{
			//cout << "\nDELETING\n";
			//delete UserWaitingQueue.Queue[i];
		}
	}

	//Inserts a User in the Waiting Queue
	void InsertUser(User*& thisUser)
	{
		CurrentUsers++;
		//User* uptr = new User(thisUser->Key,thisUser->getID(),thisUser->getInfotype());
		UserWaitingQueue.Insert(*thisUser);
	}
	
	void PrintCurrentFileInfo()
	{
		cout << FileID << " ... Access granted to User "; 
		User MaxUser;
		UserWaitingQueue.ExtractMax(MaxUser);
		MaxUser.Print();
	}
};


//------------------------------------------------------------------------------------------//
template<class T>
class FileRepository
{
private:
	vector<list<T>> FileHashTable; //T is File actually
	int CurrentFiles;
	int HTableSize;

public:
	FileRepository()
	{
		CurrentFiles = 0;
		//Insert 50 Dummy Files
		list<T> dummyFileList;
		HTableSize = 50;
		for (int i = 0; i < HTableSize; i++)
			FileHashTable.push_back(dummyFileList);
	}
	
	~FileRepository()
	{
	}
	
	void ReadMyFile(ifstream & FileRead)
	{
		if (!FileRead)
		{
			cout << "\nFile does not exist. Try again...\n";
			return;
		}

		char TexttoIgnore[200]; char Text[80]; 
		
		FileRead.getline(TexttoIgnore,80); //Ignore First line.

		while (!FileRead.eof())
		{
			FileRead.getline(Text,80);
			Segment(Text);
			
			if (FileRead.eof())
				break;
		}

		FileRead.close();
	}

	//-------------------Segments Read File into different parts--------------------
	void Segment(string WholeLine)
	{
		string FileID; string UserId; string Priorty; char Access;
		
		//---------------------------------Read File ID---------------------------------
		int i = 0;
		for (i = 0; WholeLine.at (i) != ','; i++)
		{
			if (WholeLine.at(i)!=' ')
			FileID.push_back(WholeLine[i]);
		}
		i++;
		//---------------------------------Read User ID---------------------------------
		for (; WholeLine.at(i) != ',' ; i++)
		{
			if (WholeLine.at(i)!=' ')
			UserId.push_back(WholeLine[i]);
		}
		i++;
		//---------------------------------Read Priority---------------------------------
		for (; WholeLine.at(i)!= ','; i++)
		{
			if (WholeLine.at(i)!=' ')
			Priorty.push_back(WholeLine[i]);
		}
		i++; i++;
		//---------------------------------Read Access----------------------------------
		Access = WholeLine.at(i);

		//---------------------------------Creates File and User Objects---------------------
		int F_Id = stoi(FileID); int Priority = stoi(Priorty); int U_Id = stoi(UserId);
		File CurrFile(F_Id);
		
		//---------------------Validate All 4 fields---------------------------
		if (F_Id < 5000 || F_Id > 99999)
		{
			cout << "\nThis File ID is not valid. It will be stored as 8888.";
			F_Id = 8888;
		}
		if (U_Id < 0 || U_Id > 10000)
		{
			cout << "\nThis User ID is not valid. It will be stored as 888.";
			U_Id = 888;
		}
		if (Access != 'R' && Access!= 'W')
		{
			cout << "\nThis Access is not valid. It will be stored as R.";
			Access = 'R';
		}
		if (Priority < 0 || Priority > 100)
		{
			cout << "\nThis Priorty is not valid. It will be stored as -1.";
			Priority = -1;
		}

		User CurrUser(Priority,U_Id,Access);
		//cout << "\nCreating User with "; CurrUser.Print();

		//---------------------------------Inserts File in HashTable------------------------------------
		this->InsertFile(CurrFile.FileID);

		//---------------------------------Request File Access------------------------------
		RequestFileAccess(U_Id,F_Id,Access,Priorty);
	}
	
	void PrintHashTable()
	{
		cout << endl;

		for (int F = 0; F < FileHashTable.size(); F++)
		{
			list<File> :: iterator it;
			it = FileHashTable[F].begin();
			
			if (FileHashTable[F].size() >0)
				cout << "\n\tH" << F << "-> ";
		
			
			while (it != FileHashTable[F].end())
			{
				cout << "File " << it->FileID << "... Access granted to: ";
				if (it->CurrentAccess.size() > 0)
				{
					for (int u = 0; u < it->CurrentAccess.size(); u++)
					{
						cout << "User ";

						cout << it->CurrentAccess[u].getID() << ",";
						if (it->CurrentAccess[u].getInfotype() == 'R')
							cout<< "Read"<< endl;
						else cout << "Write" << endl;
						cout << "\t\t\t\t\t  ";
					}
				}
				else
					cout << " none currently. \n";
		
				cout << "\n\t\tNext User ";
			
				if (it->SizeofWaitingQueue() > 0)
				{
					
					cout << it->UserWaitingQueue.Queue[0].getID() << ",";
					if (it->UserWaitingQueue.Queue[0].getInfotype() == 'R')
						cout << "Read";
					else
						cout << "Write";

					cout << "\n\t\tWaiting " << it->SizeofWaitingQueue() << " Users ";
					
				}
				else
					cout << " none currently. \n\t\tWaiting: none currently";
			
				it++;
			}
		}
	}
	
	int ReturnPriority(string Priority, int FileId)
	{
		int FileLocation = hashFunction(FileId);
		if (FileHashTable[FileLocation].size() == 0)
		{
			cout << "\nThis File with ID = "<< FileId << " does NOT exist. ";
			return -1;
		}

		list<File>::iterator it; 
		it = FileHashTable[FileLocation].begin();
		
		int count = 0; int ListSize = FileHashTable[FileLocation].size();
		
		bool FileFound = false;
		count = 0;
		while (count < FileHashTable[FileLocation].size())
		{
			if (it->FileID == FileId)
			{
				FileFound = true;
				break;
			}
			it++;
			count++;
		}
		if (FileFound == false)
		{
			cout << "\nCannot release File "<<FileId << " as it does not exist. ";
			return -1;
		}
		int qsize = it->UserWaitingQueue.Size();
		if (qsize > 0)
		{
			if (Priority.at(0) == 'H')
				return it->UserWaitingQueue.FindMax().Priority() + 1;
			else 
				return it->UserWaitingQueue.FindMin().Priority() - 1;
		}
		else
		{
			if (Priority.at(0) == 'H')
				return 2;
			else
				return 1;
		}
	}
	
	//----------------------Gives Read or Write Access------------------------------------
	void RequestFileAccess(int UserId, int FileId, char AccessType, string priority)
	{
		//Validation for ID
		//------------------------Validate Priority--------------------------
		if (priority.at(0)!='H' && priority.at(0)!='L')
			if (priority.at(0) < 47 || priority.at(0) > 57)
			{
				cout << "\nInvalid Priority. ";
				return;
			}

		//--------------------------Validate Priority---------------------------------
		int UserPriority = 0; list<File>::iterator it; int UserPri;
		if (priority.at(0) >= 47 && priority.at(0) < 58)
		{
			UserPri = priority.at(0) - 48;
		}
		else
			UserPri = ReturnPriority(priority,FileId); 
		

		//----------------------to validate that the file in fact exists------------------
		int FileLocation = hashFunction(FileId);
		
		it = FileHashTable[FileLocation].begin();
		if (FileHashTable[FileLocation].size() == 0)
		{
			cout << "\nAccess denied to File "<< FileId << " because it does not exist.";
			return;
		}
		
		int count = 1;
		int ListSize = FileHashTable[FileLocation].size();
		while (it != FileHashTable[FileLocation].end())
			{
				if (count > ListSize)
				{
					cout << "\nYou cannot access this File "<< FileId << " because it does not exist.";
					return;
				}
				if (it->FileID == FileId) 
					break;
				
				it++;
				count++;
			}

		if (UserPriority == -1)
		{
			cout << "\nYou cannot access this File "<< FileId << " because it does not exist.";
			return; 
		}

		//------------------------Move User to File's Waiting Queue---------------
		
		//-------------insert a User in the file waiting queue -----------------
		//User* NewUser = new User (UserPri,UserId,AccessType); 
		User NewUser(UserPri,UserId,AccessType);
		User* userptr = &NewUser;

		it->InsertUser(userptr); 

		
		//if the AQ is empty, give access
		//if the AQ is WRITING, dont give access, place in WQ
		//if the AQ is READING, give access

		if (it->CurrentAccess.size() == 0)
		{
			cout << "\nUser "<< UserId <<" is granted immediate access to File "<< FileId;
				
			//--------------Remove from Waiting Q and Move to Access Q-----------
			User MaxNode;
			it->UserWaitingQueue.ExtractMax(MaxNode);
			it->GiveAccess(MaxNode); 
		}
		else
		{
			if (it->CurrentAccess[0].getInfotype() == 'R' && AccessType == 'W')
			{
				cout << "\nUser "<<UserId << " placed in the Waiting Queue of File" << FileId;
			}
			else if (it->CurrentAccess[0].getInfotype() == 'W' && AccessType == 'R')
			{
				cout << "\nUser "<<UserId << " placed in the Waiting Queue of File"<<FileId;
			}
			else if (it->CurrentAccess[0].getInfotype() == 'R' && AccessType == 'R')
			{
				if (it->UserWaitingQueue.Queue.size() == 1)
				{
					cout << "\nUser "<< UserId <<" is granted immediate access to File "<< FileId;
				
					//--------------Remove from Waiting Q and Move to Access Q-----------
					User MaxNode;
					it->UserWaitingQueue.ExtractMax(MaxNode);
					it->GiveAccess(MaxNode); 
				}
				else
					cout << "\nUser "<<UserId << " placed in the Waiting Queue of File"<<FileId;
			}			
		}
	}
	
	void InsertFile(int CurrFileID) 
	{
		int HashIndex = hashFunction(CurrFileID);
		File NewFile(CurrFileID);

		//----if this File Already exists in Hashtable, dont add it Hash it again----
		bool DontHash = false;
		for (auto it = FileHashTable[HashIndex].begin(); it!=FileHashTable[HashIndex].end(); it++)
		{
			if (it->FileID == CurrFileID)
				DontHash = true;
		}
		
		//-------Add File to Hash Table--------
		if (DontHash == false)
		{
			FileHashTable.at(HashIndex).push_back(NewFile);
			CurrentFiles++;

			cout << "\n\t\t\tFile "<<CurrFileID << " added. \n";
		}
	}

	int hashFunction(int key)
	{
		return (key % 51) % 49;
		
	}

	//---------------------Deletes a File if Access Queue is Empty------------------
	void DeleteFile(int FileID)
	{
		//-------------------Validates if File Actually Exists----------------
		int hashIndex = hashFunction(FileID);
		list<File> :: iterator it = FileHashTable[hashIndex].begin();

		bool FileFound = false;
		int count = 0;
		while (count < FileHashTable[hashIndex].size())
		{
			if (it->FileID == FileID)
			{
				FileFound = true;
				break;
			}
			it++;
			count++;
		}
		if (FileFound == false)
		{
			cout << "\nCannot delete File "<<FileID << " as it does not exist. ";
			return;
		}

		//---------------------------Remove File from HashTable-----------------
		if (it->CurrentAccess.size() == 0)
			{
				FileHashTable[hashIndex].erase(FileHashTable[hashIndex].begin());
				cout << "\nDeleted File "<<FileID;
			}
		else
			cout << "\nCannot delete File "<<FileID << " because it is currently being accessed.";
		
	}

	bool Menu()
	{
		cout << "\n\n\t\t\t------------------------\n";
		cout << "\n\t\t\tChoose from the Menu: \n";
		cout << "\n\t\tPress 1 to Load Data.\n\t\tPress 2 to Insert a File.\n\t\tPress 3 to Delete a File";
		cout << "\n\t\tPress 4 to Access a File.\n\t\tPress 5 to Release a File.";
		cout << "\n\t\tPress 6 to Print the HashTable.\n";
		cout << "\n\t\tPress 7 to Exit the Program. \n";
		cout << "\n\t\t\tYour Choice: ";
		int option = 0;
		cin >> option;

		while (option <= 0 || option > 7)
		{
			cout << "\nInvalid option. Enter again: ";
			cin >> option;
		}

		if (option == 1) // Load Data
		{
			cout << "\nYou have chosen to Load Data .\n";
			cout << "\nEnter file Name: ";
			string FileName;
			//FileName = "Files.txt"; //comment this later and uncomment next line.
			cin >> FileName;
			
			string FileExtension = ".txt";
			while (FileName.find(FileExtension) == string::npos)
			{
				cout << "\nFile must include .txt extension. Enter again: ";
				cin >> FileName;
			}

			ifstream Fin(FileName);
			cout << "\n\nLoading File...\n\n";
			ReadMyFile(Fin);
		}
		else if (option == 2) //Insert File
		{
			cout << "\n\t\t\tYou have chosen to Insert a File. \n";
			cout << "\n\t\t\tEnter File ID: ";
			int FileID = 0;
			cin >> FileID;
			while (FileID < 5000 || FileID > 99999)
			{
				cout << "\n\t\t\tInvalid File ID. Please enter again: ";
				cin >> FileID;
			}
			InsertFile(FileID);	
		}
		else if (option == 3) //Delete a File
		{
			cout << "\n\t\t\tYou have chosen to Delete a File. \n";
			cout << "\n\t\t\tEnter File ID: ";
			int FileID = 0;
			cin >> FileID;
			while (FileID < 5000 || FileID > 99999)
			{
				cout << "\n\t\t\tInvalid File ID. Please enter again: ";
				cin >> FileID;
			}

			DeleteFile(FileID);
		}
		else if (option == 4) //access a file
		{
			cout << "\n\t\t\tYou have chosen to Access a File.\n";
			cout << "\n\t\t\tEnter File ID: ";
			int FileID = 0;
			cin >> FileID;
			while (FileID < 5000 || FileID > 99999)
			{
				cout << "\n\t\t\tInvalid File ID. Please enter again: ";
				cin >> FileID;
			}
			cout << "\n\t\t\tPlease Enter User ID: ";
			int UserID = 0;
			cin >> UserID;
			
			while (UserID < 0 || UserID > 10000)
			{
				cout << "\n\t\t\tInvalid User ID. Please enter again: ";
				cin >> UserID;
			}
			cout << "\n\t\t\tPlease Enter your Priority. "; 
			cout <<"If you would like to enter Highest or Lowest, Press 1. Else Press 2: ";
			
			int Num = 0; string FilePriority;
			cin >> Num;
			while (Num!=1 && Num!=2)
			{
				cout << "\n\t\t\tOnly enter 1 or 2: ";
				cin >> Num;
			}
			if (Num == 1)
			{
				cout << "\n\t\t\tEnter H for Highest or L for Lowest Priority: ";
				
				cin >> FilePriority;

				while (FilePriority.at(0)!='H' && FilePriority.at(0)!='L')
				{
					cout << "\n\t\t\tOnly enter H or L: ";
					cin >> FilePriority;
				}
			}
			else
			{
				cout << "\n\t\t\tSpecify Priority as an integer: ";
				int Pri = 0;
				cin >> Pri;
				while (Pri < 0 || Pri > 100)
				{
					cout << "\n\t\t\tOnly enter between 0 and 100 : ";
					cin >> Pri;
				}
			}

			cout << "\n\t\t\tEnter the type of Access you would like to perform.";
			cout << " Enter R for Read and W for Write: ";
			char access = 0;
			cin >> access;

			while (access!='R' && access!='W')
			{
				cout << "\n\t\t\tOnly enter R or W: ";
				cin >> access;
			}
			
			RequestFileAccess(UserID,FileID,access,FilePriority);

		}
		else if (option == 5) //Release File
		{
			cout << "\n\t\t\tYou have chosen to Release a File. \n\t\t\t";
			cout << "Enter File ID: ";
			int FileID = 0;
			cin >> FileID;
			while (FileID < 5000 || FileID > 99999)
			{
				cout << "\n\t\t\tInvalid File ID. Please enter again: ";
				cin >> FileID;
			}
			cout << "\n\t\t\tPlease Enter User ID: ";
			int UserID = 0;
			cin >> UserID;
			
			while (UserID < 0 || UserID > 10000)
			{
				cout << "\n\t\t\tInvalid User ID. Please enter again: ";
				cin >> UserID;
			}

			ReleaseFile(UserID,FileID);
		}
		else if (option == 6)
		{
			cout << "\n\t\t\tYou have chosen to Print the Hashtable. \n";
			PrintHashTable();
		}
		else 
		{
			cout << "\n\t\t\tYou have chosen to Exit the Program.\n";
			return false;
		}

		return true;
	}

	//-------------------Releases a File from a User---------------------------------
	void ReleaseFile(int UserID, int FileId)
	{
		//-----------Validate that this User is in fact in the waiting queue of this File
		int hashIndex = hashFunction(FileId);
		list<File>:: iterator it = FileHashTable[hashIndex].begin();
		if (FileHashTable[hashIndex].size() == 0)
			{
				cout << "\nFile "<< FileId << " does not exist. ";
				return;
			}

		//---------------------------Validate the File Exists---------------------
		bool FileFound = false;
		int count = 0;
		while (count < FileHashTable[hashIndex].size())
		{
			if (it->FileID == FileId)
			{
				FileFound = true;
				break;
			}
			it++;
			count++;
		}
		if (FileFound == false)
		{
			cout << "\nCannot release File "<<FileId << " as it does not exist. ";
			return;
		}
		//if (it->FileID!=FileId) return;
		//count = 0; int ListSize = FileHashTable[hashIndex].size();
		
		/*while (it->FileID != FileId)
		{
			it++; 
			if (count > ListSize)
			{
				cout << "\nYou cannot release this File "<< FileId << " because it does not exist.";
				return;
			}
			if (it->FileID == FileId)
				break;
			count++;
		}*/
		
		//---------------Check is User is part of the Access Queue (Not Waiting Q)---------------
		//---------------Remove User from Access Queue-------------------------
		bool NotFound = false;
		it->RemoveAccess(UserID,NotFound);
		if (NotFound == false)
			return;

		cout << "\nReleasing File "<<FileId << " from User "<<UserID;

		//---------------Move to Next User-----------------------------
		
		User CurrPriUser; //NEWLY ADDED.

		//Now we will give access to the Next Person in the waiting queue.
		if (it->UserWaitingQueue.FindMax().getInfotype() == 'W')
		{
			if (it->CurrentAccess.size() != 0)
			{
				cout << "\nWrite Access cannot be given to User ";
				cout << it->UserWaitingQueue.FindMax().getID();
				cout << " because it is currently being accessed by other Users.";
				return;
			}

			//-------------Rempve from Waiting Queue to Access Queue-------------

			User HighestPriorityUser;
			it->UserWaitingQueue.ExtractMax(HighestPriorityUser);
			it->GiveAccess(HighestPriorityUser);
			cout << "\nExclusive Write Access for File "<< FileId << " granted to User ";
			cout << HighestPriorityUser.getID();
		}
		//--------------------Grant access to all Read Access Users till Write User found
		else if (it->UserWaitingQueue.FindMax().getInfotype() == 'R')
		{
			//User* CurrPriorityUser = new User;
			
			//Remove Current High Priority User from WQ to AQ
			it->UserWaitingQueue.ExtractMax(CurrPriUser);
			it->GiveAccess(CurrPriUser);
			cout << "\nGiving Read Access to " << CurrPriUser.getID();
			
			while (CurrPriUser.getInfotype() == 'R')
			{
				//User CurrPriorityUser;
				it -> UserWaitingQueue.ExtractMax(CurrPriUser);
				it -> GiveAccess(CurrPriUser);
				
				cout << "\nGiving Read Access to "<<CurrPriUser.getID();
				
				if (it->UserWaitingQueue.FindMax().getInfotype() == 'W' || it->SizeofWaitingQueue() == 0)
					break;
			}
		}
	}
};


int main()
{
	FileRepository<File> FRepo;

	while(FRepo.Menu() == true)
	{
		if (FRepo.Menu() == false)
			break;
	}
	
	/*
	ifstream Fin("Files.txt");
	
	
	FRepo.ReadMyFile(Fin);
	
	FRepo.InsertFile(20000);

	FRepo.RequestFileAccess(0,10003,'R',"H");
	
	FRepo.ReleaseFile(10,10003);
	FRepo.ReleaseFile(0,10003);
	FRepo.ReleaseFile(11,10003);
	FRepo.ReleaseFile(1,10003);
	FRepo.ReleaseFile(0,10003);
	FRepo.ReleaseFile(0,10003);

	FRepo.DeleteFile(10003);
	FRepo.PrintHashTable();
	*/
	
	cout << endl;


	return 0;
}