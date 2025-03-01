#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

#define head_ 0
std::ifstream Read;
std::ofstream Write;

struct Edit_Distance{
	std::string word;
	double distance;
};

int partition(Edit_Distance arr[], int low, int high);
void QuickSort(Edit_Distance arr[], int low, int high);
bool isValidWord(std::string S);
bool isLetter(char C);
std::vector<std::string> Closest_words(std::string S, std::vector<std::string> words, int return_size);
std::vector<std::string> Sort(Edit_Distance WORDS[], int size, int return_size);
std::vector<std::string> Sort_alphabetical(std::vector<std::string> Words);
void swap(std::string& a, std::string& b);
int min(int a, int b);
std::vector<std::string> Sort_alphabetical_ignoring_Capitalization(std::vector<std::string> Words);
int handle_Capitalization(char C);
int lev(std::string S1, std::string S2);
bool Exists(std::string S, std::vector<std::string> words);
int max(int a, int b);

int LevenshteinDistance(std::string s1, std::string s2);
int Same(std::string s1, std::string s2);

int main(int argc, char* argv[]){
	std::string temp = "a";
	std::vector<std::string> words;
	
	//const int MAX_READ = 30000;
	const int MAX_READ = 85000;
	int already_read = 0;
	
	Read.open("words_by_freq.txt");
	if(Read.is_open()){
		while(already_read < MAX_READ){
			Read >> temp;
			words.push_back(temp);
			//std::cout << words[already_read] << "\n";
			already_read++;
		}
		Read.close();
		printf("Done reading\n\n");
	}
	
	std::string S = "happf";
	int top = 6;
	
	return 0;
}








std::vector<std::string> Closest_words(std::string S, std::vector<std::string> words, int return_size){
	size_t size = words.size();
	
	std::vector<std::string> CLOSEST;
	Edit_Distance WORDS[size];
	
	for(int i = 0; i < size; i++){
		//WORDS[i].distance = lev(S, words[i]); // old lev approach
		WORDS[i].distance = LevenshteinDistance(S, words[i]) - (Same(S, words[i])/(i+1)); // faster lev approach
		WORDS[i].word = words[i];
	}
	
	// USING QUICK SORT
//	QuickSort(WORDS, 0, size);
//	for(int i = 0; i < return_size; i++){
//		CLOSEST.push_back(WORDS[i].word);
//	}
	// USING SELECTION SORT AND STOPPING ON THE MAX RETURN SIZE
	CLOSEST = Sort(WORDS, size, return_size);
		
	return CLOSEST;
}

std::vector<std::string> Sort(Edit_Distance WORDS[], int size, int return_size){
	if(size < 0){
		exit(0);
	}
	std::vector<std::string> Closest_Words_;
	int current_ind = 0;
	while(current_ind <= return_size){
		if(current_ind == size){
			break;
		}
		int target_ind = current_ind;
		bool flag = false;
		for(int j = current_ind + 1; j < size; j++){
			if(WORDS[j].distance < WORDS[target_ind].distance){
				target_ind = j;
				flag = true;
			}
		}
		if(flag){
			Closest_Words_.push_back(WORDS[target_ind].word);
			std::string s_temp = WORDS[target_ind].word;
			WORDS[target_ind].word = WORDS[current_ind].word;
			WORDS[current_ind].word = s_temp;
			double temp = WORDS[target_ind].distance;
			WORDS[target_ind].distance = WORDS[current_ind].distance;
			WORDS[current_ind].distance = temp;
		}
		current_ind++;
	}
	
	return Closest_Words_;
}

void QuickSort(Edit_Distance words[], int low, int high){
    if(low < high){
        int pv = partition(words, low, high);
        
        QuickSort(words, low, pv - 1);
        QuickSort(words, pv + 1, high);
	}
}
int partition(Edit_Distance words[], int low, int high){
    int splitval = words[low].distance;
    
    while(low < high){
    	while(low < high && splitval <= words[high].distance){
    		high--;
		}
		if(low >= high){
			words[high].distance = splitval;
			return high;
		}
		words[low].word = words[high].word;
		words[low++].distance = words[high].distance;
		
		while(low < high && splitval >= words[low].distance){
			low++;
		}
		if(low >= high){
			words[high].distance = splitval;
			return high;
		}
		words[high].word = words[low].word;
		words[high--].distance = words[low].distance;
	}
	words[high].distance = splitval;
	return high;
}






bool isValidWord(std::string S){
	for(int i = 0; i < S.size(); i++){
		if(!isLetter(S[i])){
			return false;
		}
	} return true;
}
bool isLetter(char C){
	return ((C >= 65 && C <= 122) && !(C > 90 && C < 97)) ? true : false;
}

std::vector<std::string> Sort_alphabetical(std::vector<std::string> Words){
	size_t arr_size = Words.size() - 1;
	int Current_Index = 0;
	const int HEAD = 0;
	while(Current_Index < arr_size - 1){
		int Target_Index = Current_Index;
		bool flag = false;
		for(int Head_Index = Current_Index + 1; Head_Index <= arr_size; Head_Index++){
			if(Words[Head_Index][HEAD] < Words[Target_Index][HEAD]){
				Target_Index = Head_Index;
				flag = true;
			}
			int Word_Index = 0;
			while(Words[Head_Index][HEAD] == Words[Target_Index][HEAD] && Word_Index < min(Words[Head_Index].size(), Words[Target_Index].size()) - 1){
				Word_Index++;
				if(Words[Head_Index][Word_Index] < Words[Target_Index][Word_Index]){
					Target_Index = Head_Index;
					flag = true;
					break;
				}else if(Words[Head_Index][Word_Index] > Words[Target_Index][Word_Index]){
					break;
				}
			}
		}
		if(flag){
			swap(Words[Target_Index], Words[Current_Index]);
		}
		Current_Index += 1;
	}
	return Words;
}

bool word_Exists(const std::string input, std::vector<std::string> words){
	for(auto S : words){
		if(input == S){
			return true;
		}
	} return false;
}
void swap(std::string& a, std::string& b){
	std::string temp = a;
	a = b;
	b = temp;
}
int min(int a, int b){
	return (a < b) ? a : b;
}
int max(int a, int b){
	return (a > b) ? a : b;
}

std::vector<std::string> Sort_alphabetical_ignoring_Capitalization(std::vector<std::string> Words){
	size_t arr_size = Words.size()-1;
	int Current_Index = 0;
	const int HEAD = 0;
	while(Current_Index < arr_size - 1){
		int Target_Index = Current_Index;
		bool flag = false;
		for(int Head_Index = Current_Index + 1; Head_Index <= arr_size; Head_Index++){
			//////// HANDLE CAPITALIZATION
			if(handle_Capitalization(Words[Head_Index][HEAD]) < handle_Capitalization(Words[Target_Index][HEAD])){
				Target_Index = Head_Index;
				flag = true;
			}
			int Word_Index = 0;
			while(handle_Capitalization(Words[Head_Index][Word_Index]) == handle_Capitalization(Words[Target_Index][Word_Index]) && Word_Index < min(Words[Head_Index].size(), Words[Target_Index].size()) - 1){
				Word_Index++;
				if(handle_Capitalization(Words[Head_Index][Word_Index]) < handle_Capitalization(Words[Target_Index][Word_Index])){
					Target_Index = Head_Index;
					flag = true;
					break;
				}else if(handle_Capitalization(Words[Head_Index][Word_Index]) > handle_Capitalization(Words[Target_Index][Word_Index])){
					break;
				}
			}
		}
		if(flag){
			swap(Words[Target_Index], Words[Current_Index]);
		}
		Current_Index += 1;
	}
	return Words;
}

int handle_Capitalization(char C){
	return (C < 97) ? C+32 : C;
}

int lev(const std::string S1, const std::string S2){
	const int head = 0;
	if(S1.size() == 0){
		return S2.size();
	}
	if(S2.size() == 0){
		return S1.size();
	}
	std::string newS1 = S1.substr(1);
	std::string newS2 = S2.substr(1);
	
	return (S1[head] == S2[head]) ? lev(newS1, newS2) : 1 + min(min(lev(newS1, S2), lev(S1, newS2)), lev(newS1, newS2));
}

bool Exists(std::string S, std::vector<std::string> words){
	for(int i = 0; i < words.size(); i++){
		if(S == words[i]){
			return true;
		}
	} return false;
}

int LevenshteinDistance(std::string s1, std::string s2) {
    int m = s1.size();
    int n = s2.size();

    // Initialize a matrix of size (m+1) x (n+1)
    std::vector<std::vector<int>> D(m + 1, std::vector<int>(n + 1, 0));

    // Initialize the first row and column
    for (int i = 0; i <= m; i++	) {
        D[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        D[0][j] = j;
    }

    // Fill in the matrix
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            D[i][j] = min(min(D[i - 1][j] + 1,      // Deletion
                                D[i][j - 1] + 1),      // Insertion
                                D[i - 1][j - 1] + cost // Substitution
            );
        }
    }
    // The final cell in the matrix contains the edit distance
    return D[m][n];
}

int Same(std::string s1, std::string s2){
	if(s1.size() == 0 || s2.size() == 0){
		return -max(s1.size(), s2.size());
	}
	return 1 + Same(s1.substr(1), s2.substr(1));
}


