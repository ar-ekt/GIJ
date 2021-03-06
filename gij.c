#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>

#define MAX_ARRAY_SIZE 300

/**
 * address of current code(gij.exe) is running
**/

char gij_Path[MAX_ARRAY_SIZE];

/**
 * a temp array for saving situation of files
**/

int temp_Situation[MAX_ARRAY_SIZE];

/**
 * number of commits!!!!!!!!
**/

int number_Of_Commit;

/**
 * length of status dictionary
**/

int status_Len;

struct dict{
	int last_Ver;
	int last_Situation;
	int all_Situation[MAX_ARRAY_SIZE];
	int all_Ver[MAX_ARRAY_SIZE];
	char name[MAX_ARRAY_SIZE];
} status[MAX_ARRAY_SIZE];

/** 
 * This function gets a color's name and turns output color into it
 * 
 * @param color a color's name
 *
 * @return integer 1 in case of success and 0 in opposite case
 *
 */

int changeColor(char color[]){
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if (!strcmp(color,"green"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		
	else if (!strcmp(color,"red"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		
	else if (!strcmp(color,"white"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
		
	else if (!strcmp(color,"blue"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		
	else if (!strcmp(color,"yellow"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_GREEN);
		
	else if (!strcmp(color,"purple"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
		
	else if (!strcmp(color,"light-blue"))
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY|FOREGROUND_BLUE);
		
	else
		return 0;
		
	return 1;
}

/** 
 * This function get a message and output it with the given color
 * 
 * @param message is the message itself we want to print
 * @param color of the message
 *
 */

void printMessage(char message[], char color[]){
	changeColor(color);
	printf(message);
	changeColor("white");
}

/** 
 * This function get name of a file and return format of it in a string
 * 
 * @param file is name of the given file
 * @param format format of the given file
 *
 */
 
void get_Format(char file[], char format[]){
	int len = strlen(file);
	int index = len;
	int lenF;
	for(lenF=0; file[index--] != '.'; lenF++);
	for(index=0; index<lenF; index++)
		format[index] = file[index+len-lenF];
}

/** 
 * This function make one copy of the given file in wanted address
 * 
 * @param origin address of original file
 * @param fileO name of original file
 * @param destination the address to make a copy of file
 * @param fileD name of copy file
 *
 */

void make_Copy(char origin[], char fileO[], char destination[], char fileD[]){
	char command[MAX_ARRAY_SIZE];
	sprintf(command, "copy %s\\%s %s\\%s > M#S#G.txt", origin, fileO, destination, fileD);
	system(command);
	remove("M#S#G.txt");
}

/** 
 * This function find the index of given file in dictionry
 * 
 * @param file name of the file given for search
 *
 * @return index of file in dictionary if file is exsit otherwise return -1
 *
 */

int dict_Index(char file[]){
	int index;
	for(index=0; index<status_Len; index++)
		if(!strcmp(status[index].name, file))
			return index;
	return -1;
}

/** 
 * This function get a string and clean it by memset function
 * 
 * @param string ...
 *
 */

void clean(char string[]){
	memset(string, '\0', sizeof(string));
}

/** 
 * This function get a address and replace all backslash(\) with hash(#) 
 * 
 * @param name1 given address
 * @param name2 converted address
 *
 */

void lash_2_Hash(char name1[], char name2[]){
	int len = strlen(name1);
	int index;
	for(index=0; index<len; index++)
		if(name1[index] == '\\')
			name2[index] = '#';
		else
			name2[index] = name1[index];
	name2[index] = '\0';
}

/** 
 * This function get a address and replace all hash(#) with backslash(\)
 * 
 * @param name1 given address
 * @param name2 converted address
 *
 */

void hash_2_Lash(char name1[], char name2[]){
	int len = strlen(name1);
	int index;
	for(index=0; index<len; index++)
		if(name1[index] == '#')
			name2[index] = '\\';
		else
			name2[index] = name1[index];
	name2[index] = '\0';
}

/** 
 * This function update status-dictionry with ststus-file
 * 
 * @param path address of main folder
 *
 */

void status_Get(char path[]){
	int index, indexS;
	
	char path_Ver[MAX_ARRAY_SIZE];
	sprintf(path_Ver, "%s\\vc_data\\status.txt", path);
	FILE *vF = fopen(path_Ver, "r");
	
	fscanf(vF, "%d %d", &number_Of_Commit, &status_Len);
	
	for(index=0; index<status_Len; index++){
		
		fscanf(vF, "%s %d %d", status[index].name, &status[index].last_Ver, &status[index].last_Situation);
		
		for(indexS=0; indexS<number_Of_Commit; indexS++){
			fscanf(vF, "%d", &status[index].all_Situation[indexS]);
			temp_Situation[index] = 0;
		}
		for(indexS=0; indexS<number_Of_Commit; indexS++)
			fscanf(vF, "%d", &status[index].all_Ver[indexS]);
	}
	fclose(vF);
}

/** 
 * This function update ststus-file with status-dictionry
 * 
 * @param path address of main folder
 * @param minus number of deleted files after reset
 *
 */

void status_Update(char path[], int minus){
	int index, indexS;
	
	char path_Ver[MAX_ARRAY_SIZE];
	sprintf(path_Ver, "%s\\vc_data\\status.txt", path);
	FILE *vF = fopen(path_Ver, "w");
	
	fprintf(vF, "%d\n", number_Of_Commit);
	fprintf(vF, "%d\n", status_Len-minus);
	
	for(index=0; index<status_Len; index++){
		if(temp_Situation[index] == 4)
			continue;
		fprintf(vF, "%s %d %d\n", status[index].name, status[index].last_Ver, status[index].last_Situation);
		
		for(indexS=0; indexS<number_Of_Commit-1; indexS++)
			fprintf(vF, "%d ", status[index].all_Situation[indexS]);
		fprintf(vF, "%d\n", status[index].last_Situation);
		
		for(indexS=0; indexS<number_Of_Commit-1; indexS++)
			fprintf(vF, "%d ", status[index].all_Ver[indexS]);
		fprintf(vF, "%d\n", status[index].last_Ver);
	}
	
	fclose(vF);
}

/** 
 * This function update situation of files after every commit
 * 
 * @param path address of main folder
 *
 */

void situation_Update(char path[]){
	char name_Lashed[MAX_ARRAY_SIZE];
	char buffer[MAX_ARRAY_SIZE];
	FILE *temp;
	int index;
	for(index=0; index<status_Len; index++){
		if(temp_Situation[index] == 1 || temp_Situation[index] == 2){
			status[index].last_Situation = temp_Situation[index];
		}
		else{
			hash_2_Lash(status[index].name, name_Lashed);
			sprintf(buffer, "%s\\%s", path, name_Lashed);
			temp = fopen(buffer, "r");
			clean(buffer);
			clean(name_Lashed);
			if(temp == NULL){
				if(status[index].last_Situation == -1 || status[index].last_Situation == -2)
					status[index].last_Situation = -2;
				else
					status[index].last_Situation = -1;
			}
			else{
				fclose(temp);
				status[index].last_Situation = 0;
			}
		}	
	}
}

/** 
 * This function reset commits to given Id and delete all commits between now to given id
 * 
 * @param path address of main folder
 * @param commitId ...
 *
 */

void reset_And_Delete(char path[], int commitId){
	int index, indexV, ban=0;
	char path_Des[MAX_ARRAY_SIZE];
	char path_Ori[MAX_ARRAY_SIZE];
	char path_Res[MAX_ARRAY_SIZE];
	char name_Lashed[MAX_ARRAY_SIZE];
	char name_Ori[MAX_ARRAY_SIZE];
	char formatV[MAX_ARRAY_SIZE];
	char command[MAX_ARRAY_SIZE];
	
	status_Get(path);
	
	// delete descriptions
	for(index=commitId+1; index<=number_Of_Commit; index++){
		sprintf(path_Des, "%s\\vc_data\\descriptions\\des%d.txt", path, index);
		remove(path_Des);
	}
	
	for(index=0; index<status_Len; index++){
		
		clean(formatV);
		get_Format(status[index].name, formatV);
		
		clean(name_Lashed);
		hash_2_Lash(status[index].name, name_Lashed);
		
		if(0 <= status[index].all_Situation[commitId-1] && status[index].all_Situation[commitId-1] <= 2){
			
			// delete the verison in main folder
			if(status[index].last_Situation >= 0){
				sprintf(path_Des, "%s\\%s", path, name_Lashed);
				remove(path_Des);
			}
			
			for(indexV=status[index].all_Ver[commitId-1]+1; indexV<=status[index].last_Ver; indexV++){
				sprintf(path_Des, "%s\\vc_data\\files\\%s\\dif%d.txt", path, status[index].name, indexV);
				remove(path_Des);
			}
			
			sprintf(path_Des, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV-1, formatV);
			remove(path_Des);
			
			for(indexV=1; indexV<status[index].all_Ver[commitId-1]; indexV++){
				sprintf(path_Ori, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV, formatV);
				sprintf(path_Des, "%s\\vc_data\\files\\%s\\dif%d%s", path, status[index].name, indexV+1, ".txt");
				sprintf(path_Res, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV+1, formatV);
				sprintf(command, "%s\\patch0.exe %s %s %s", gij_Path, path_Ori, path_Des, path_Res);
				system(command);
				if(indexV != 1)
					remove(path_Ori);
			}
			
			sprintf(path_Ori, "%s\\vc_data\\files\\%s", path, status[index].name);
			sprintf(name_Ori, "%d%s", status[index].all_Ver[commitId-1], formatV);
			make_Copy(path_Ori, name_Ori, path, name_Lashed);
		}
		else if(status[index].all_Situation[commitId-1] == 3){
			
			if(status[index].last_Situation >= 0){
				sprintf(path_Des, "%s\\%s", path, name_Lashed);
				remove(path_Des);
			}
			
			for(indexV=1; indexV<=status[index].last_Ver; indexV++){
				sprintf(path_Ori, "%s\\vc_data\\files\\%s\\dif%d%s", path, status[index].name, indexV, ".txt");
				remove(path_Ori);
				if(indexV == 1 || indexV == status[index].last_Ver){
					sprintf(path_Ori, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV, formatV);
					remove(path_Ori);
				}
			}
			sprintf(path_Ori, "%s\\vc_data\\files\\%s", path, status[index].name);
			rmdir(path_Ori);
			temp_Situation[index] = 4;
			ban++;
		}
		status[index].last_Situation = status[index].all_Situation[commitId-1];
		status[index].last_Ver = status[index].all_Ver[commitId-1];
	}
	number_Of_Commit = commitId;
	status_Update(path, ban);
}

/** 
 * This function reset commits to given Id
 * 
 * @param path address of main folder
 * @param commitId ...
 *
 */

void reset_(char path[], int commitId){
	int index, indexV;
	char path_Des[MAX_ARRAY_SIZE];
	char path_Ori[MAX_ARRAY_SIZE];
	char path_Res[MAX_ARRAY_SIZE];
	char name_Lashed[MAX_ARRAY_SIZE];
	char name_Ori[MAX_ARRAY_SIZE];
	char formatV[MAX_ARRAY_SIZE];
	char command[MAX_ARRAY_SIZE];
	char buffer[MAX_ARRAY_SIZE];
	
	status_Get(path);
	
	for(index=0; index<status_Len; index++){
		
		clean(formatV);
		get_Format(status[index].name, formatV);
		
		clean(name_Lashed);
		hash_2_Lash(status[index].name, name_Lashed);
		
		// delete the verison in main folder
		sprintf(path_Des, "%s\\%s", path, name_Lashed);
		remove(path_Des);
		
		if(0 <= status[index].all_Situation[commitId-1] && status[index].all_Situation[commitId-1] <= 2){
			if(status[index].all_Ver[commitId-1] != status[index].last_Ver){
				// make the wnated verion of file step by step
				for(indexV=1; indexV<status[index].all_Ver[commitId-1]; indexV++){
					sprintf(path_Ori, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV, formatV);
					sprintf(path_Des, "%s\\vc_data\\files\\%s\\dif%d%s", path, status[index].name, indexV+1, ".txt");
					sprintf(path_Res, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV+1, formatV);
					sprintf(command, "%s\\patch0 %s %s %s", gij_Path, path_Ori, path_Des, path_Res);
					system(command);
					if(indexV != 1){
						remove(path_Ori);
						puts(path_Ori);
					}
				}
				
				// copy wanted version of the file into main folder
				sprintf(path_Ori, "%s\\vc_data\\files\\%s", path, status[index].name);
				sprintf(name_Ori, "%d%s", status[index].all_Ver[commitId-1], formatV);
				make_Copy(path_Ori, name_Ori, path, name_Lashed);
				
				// remove 
				if(status[index].all_Ver[commitId-1] != 1){
					sprintf(buffer, "%s\\%s", path_Ori, name_Ori);
					remove(buffer);
				}
			}
			else{
				// copy wanted version of the file into main folder
				sprintf(path_Ori, "%s\\vc_data\\files\\%s", path, status[index].name);
				sprintf(name_Ori, "%d%s", status[index].all_Ver[commitId-1], formatV);
				make_Copy(path_Ori, name_Ori, path, name_Lashed);
			}
		}
	}
}

/** 
 * This function delete all files those haven;t commited
 * 
 * @param path address of main folder
 * @param selected array of files
 * @param selected_Len number of selected files
 *
 */

void delete_Not_Commited(char path[], char selected[][MAX_ARRAY_SIZE], int selected_Len){
	int index;
	char file_Name[MAX_ARRAY_SIZE];
	char hashed_Name[MAX_ARRAY_SIZE];
	
	for(index=0; index<selected_Len; index++){
		lash_2_Hash(selected[index], hashed_Name);
		if(dict_Index(hashed_Name) == -1){
			sprintf(file_Name, "%s\\%s", path, selected[index]);
			remove(file_Name);
		}
	}
}

/** 
 * This function do something about stash (I don't know)
 * 
 * @param path address of main folder
 * @param commitId ...
 * @param selected array of files
 * @param selected_Len number of selected files
 * @param stashed array of files have been stashed
 * @param stashed_Len number of files have been stashed
 *
 * @return 1 if file exist otherwise 0
 *
 */

void stash_(char path[], int commitId, char selected[][MAX_ARRAY_SIZE], int selected_Len, char stashed[][MAX_ARRAY_SIZE], int stashed_Len){
	char hashed_Name[MAX_ARRAY_SIZE];
	char stash_Addr[MAX_ARRAY_SIZE];
	char main_Addr[MAX_ARRAY_SIZE];
	int index;
	
	for(index=0; index<stashed_Len; index++){
		sprintf(stash_Addr, "%s\\vc_data\\stash\\%s", path, stashed[index]);
		remove(stash_Addr);
	}
	
	for(index=0; index<selected_Len; index++){
		lash_2_Hash(selected[index], hashed_Name);
		
		sprintf(stash_Addr, "%s\\vc_data\\stash", path);
		make_Copy(path, selected[index], stash_Addr, hashed_Name);
		strcpy(stashed[index], hashed_Name);
		
		sprintf(main_Addr, "%s\\%s", path, selected[index]);
		remove(main_Addr);
	}
}

/** 
 * This function do something about stash-pop (I don't know)
 * 
 * @param path address of main folder
 * @param stashed array of files have been stashed
 * @param stashed_Len number of files have been stashed
 *
 */

void stash_Pop(char path[], char stashed[][MAX_ARRAY_SIZE], int stashed_Len){
	int index;
	char stash_Addr[MAX_ARRAY_SIZE];
	char lashed_Name[MAX_ARRAY_SIZE];
	
	for(index=0; index<stashed_Len; index++){
		sprintf(stash_Addr, "%s\\vc_data\\stash", path);
		hash_2_Lash(stashed[index], lashed_Name);
		make_Copy(stash_Addr, stashed[index], path, lashed_Name);
	}
}

/** 
 * This function commit selected files and updates their diffs
 * 
 * @param path address of main folder
 * @param selected array of files for commit
 * @param numberOS number of selected files
 *
 * in this function OS means "of selected file\s"
 *
 */

void commit(char path[], char selected[][MAX_ARRAY_SIZE], int numberOS){
	int indexOS;
	int verOS;
	int indInVer;
	
	char nameOS[MAX_ARRAY_SIZE];
	char pathOS[MAX_ARRAY_SIZE];
	char formatOS[MAX_ARRAY_SIZE];
	char command[MAX_ARRAY_SIZE];
	char buffer[MAX_ARRAY_SIZE];
	
	status_Get(path);
	
	for(indexOS=0; indexOS<numberOS; indexOS++){
		
		//clean_strings
		clean(nameOS);
		clean(pathOS);
		clean(formatOS);
		
		lash_2_Hash(selected[indexOS], nameOS);
		sprintf(pathOS, "%s\\vc_data\\files\\%s", path, nameOS);
		get_Format(selected[indexOS], formatOS);
		
		indInVer = dict_Index(nameOS);
		if(indInVer == -1){
			verOS = 1;
			
			mkdir(pathOS);
			
			sprintf(buffer, "%d%s", verOS, formatOS);
			make_Copy(path, selected[indexOS], pathOS, buffer);
			
			int index;
			for(index=0; index<number_Of_Commit; index++){
				status[status_Len].all_Situation[index] = 3;
				status[status_Len].all_Ver[index] = 0;
			}
				
			strcpy(status[status_Len].name, nameOS);
			status[status_Len].last_Ver = 1;
			temp_Situation[status_Len] = 2;
			status_Len++;
		}
		else{
			verOS = status[indInVer].last_Ver;
			
			sprintf(buffer, "%d%s", verOS+1, formatOS);
			make_Copy(path, selected[indexOS], pathOS, buffer);
			
			sprintf(command, "%s\\diff0 %s\\%d%s %s\\%d%s %s\\dif%d%s", gij_Path, pathOS, verOS, formatOS, pathOS, verOS+1, formatOS, pathOS, verOS+1, ".txt");
			system(command);
			
			if(verOS != 1){
				sprintf(command, "%s\\%d%s", pathOS, verOS, formatOS);
				remove(command);
			}
			
			status[indInVer].last_Ver += 1;
			temp_Situation[indInVer] = 1;
		}
	}
	
	situation_Update(path);
	number_Of_Commit++;
	status_Update(path, 0);
}

/** 
 * This function show allthing about commits
 * 
 * @param path address of main folder
 *
 */

void log_(char path[]){
	char conv_Situation[][MAX_ARRAY_SIZE] = {"deleted", "unchanged", "changed", "recently"};
	char pathD[MAX_ARRAY_SIZE];
	char description[MAX_ARRAY_SIZE];
	char name_Lashed[MAX_ARRAY_SIZE];
	char message[MAX_ARRAY_SIZE];
	FILE *des;
	int index, indexF;
	
	status_Get(path);
	
	system("cls");
	
	printMessage("\nLogs\n\n\n", "blue");
	if(number_Of_Commit == 0)
		printMessage("nothing to show\n", "yellow");
	for(index=0; index<number_Of_Commit; index++){
		sprintf(message, "commit-Id: %d\n", index+1);
		printMessage(message, "purple");
		sprintf(pathD, "%s\\vc_data\\descriptions\\des%d.txt", path, index+1);
		des = fopen(pathD, "r");
		fgets(description, sizeof(description), des);
		sprintf(message, "description: %s\n\n", description);
		printMessage(message, "purple");
		fclose(des);
		for(indexF=0; indexF<status_Len; indexF++){
			if(-1 <= status[indexF].all_Situation[index] && status[indexF].all_Situation[index] <= 2){
				hash_2_Lash(status[indexF].name, name_Lashed);
				sprintf(message, "%s\n", name_Lashed);
				printMessage(message, "light-blue");
				sprintf(message, "    %s", conv_Situation[status[indexF].all_Situation[index]+1]);
				printMessage(message, "light-blue");
				sprintf(message, "\t\\  version: %d\n", status[indexF].all_Ver[index]);
				printMessage(message, "light-blue");
			}
		}
		if(index != number_Of_Commit)
			printMessage("\n*****************************************\n\n", "yellow");
	}
}

/** 
 * This function show allthing about files
 * 
 * @param path address of main folder
 *
 */

void status_(char path[]){
	char name_Lashed[MAX_ARRAY_SIZE];
	char conv_Situation[][MAX_ARRAY_SIZE] = {"deleted", "unchanged", "changed", "recently"};
	char message[MAX_ARRAY_SIZE];
	int index;
	
	status_Get(path);
	
	system("cls");
	
	printMessage("\nStatus of files\n\n\n", "blue");
	if(status_Len == 0)
		printMessage("nothing to show\n", "yellow");
	for(index=0; index<status_Len; index++)
		if(-1 <= status[index].last_Situation && status[index].last_Situation <= 2){
			hash_2_Lash(status[index].name, name_Lashed);
			sprintf(message, "%s \n    %s\t\\  version: %d\n\n", name_Lashed, conv_Situation[status[index].last_Situation+1], status[index].last_Ver);
			if(index % 2 == 0)
				printMessage(message, "light-blue");
			else
				printMessage(message, "purple");
		}
}

/** 
 * This function show selected files
 * 
 * @param selected array of files
 * @param selected_Len number of selected files
 *
 */

void show_Selected(char selected[][MAX_ARRAY_SIZE], int selected_Len){
	int index;
	char message[MAX_ARRAY_SIZE];
	system("cls");
	
	printMessage("\nSelected files\n\n\n", "blue");
	if(selected_Len == 0)
		printMessage("no file selected\n", "yellow");
	for(index=0; index<selected_Len; index++){
		sprintf(message, "%s\n\n", selected[index]);
		printMessage(message, "light-blue");
	}
}

/** 
 * This function check a file is in selected files or not if yes remove it
 * 
 * @param file the name of given file to check
 * @param selected array of files
 * @param selected_Len number of selected files
 *
 * @return 1 if file exsit in selected files otherwise 0
 *
 */

int try_Unselect(char file[], char selected[][MAX_ARRAY_SIZE], int selected_Len){
	int index, shift;
	
	for(index=0; index<selected_Len; index++)
		if(!strcmp(selected[index], file)){
			for(shift=index; shift<selected_Len-1; shift++)
				strcpy(selected[shift], selected[shift+1]);
			clean(selected[shift]);
			return 1;
		}
	return 0;
}

/** 
 * This function check the given file is directory or not
 * 
 * @param pre path of the 
 * @param file name of file
 *
 * @return 1 if file is directory otherwise 0
 *
 */

int is_Dir(char pre[], char file[]){
	if(chdir(file))
		return 0;
	chdir(pre);
	return 1;
}

/** 
 * This function search all of main directory and ite sub-directories to find files and select them
 * 
 * @param path address of main folder
 * @param cur_Dir path of where is being searched
 * @param selected array of files
 * @param selected_Len number of selected files
 *
 * @return updated number of selected files
 *
 */

int select_All(char path[], char cur_Dir[], char selected[][MAX_ARRAY_SIZE], int selected_Len, int type){
	char file_Name[MAX_ARRAY_SIZE];
	char temp_Dir[MAX_ARRAY_SIZE];
	
	struct dirent *de; 			// explained in __generate__ function
	DIR *dr = opendir(cur_Dir); // pointer to current directory
	
	while ((de = readdir(dr)) != NULL){
		strcpy(file_Name, de->d_name);
		
		// "." and ".." are directory paths that are always present
		if(!strcasecmp(file_Name, "..") || !strcasecmp(file_Name, ".") || !strcasecmp(file_Name, "vc_data"))
			continue;
			
		if(!strcmp(cur_Dir, "."))
			strcpy(temp_Dir, file_Name);
		else
			sprintf(temp_Dir, "%s\\%s", cur_Dir, file_Name);
			
		if(type == 1 || !is_Dir(path, temp_Dir)) // file not a directory or this finvtion is used for "dir command
			strcpy(selected[selected_Len++], temp_Dir);
		else
			selected_Len = select_All(path, temp_Dir, selected, selected_Len, 2);
	}
	
	return selected_Len;
}

/** 
 * This function check the given file exist or not
 * 
 * @param path address of main folder
 * @param file name of file will be checked
 *
 * @return 1 if file exist otherwise 0
 *
 */

int check_File(char path[], char file[]){
	char pathF[MAX_ARRAY_SIZE];
	sprintf(pathF, "%s\\%s", path, file);
	
	FILE *f = fopen(pathF, "r");
	if(f != NULL){
		fclose(f);
		return 1;
	}
	return 0;
}

/** 
 * This function gets a string and checks if it's all numerical
 * 
 * @param number is the string needed to be checked
 *
 * @return integer 1 if "number" string is numerical otherwise 0
 *
 */

int isNumerical(char number[]){
	int index=0;
	while(number[index])
		if(!isdigit(number[index++]))
			return 0;
	return 1;
}

/** 
 * This function show where user is
 *
 */

void showPath(){
	char cwd[MAX_ARRAY_SIZE];
	if(getcwd(cwd, sizeof(cwd)))
	    printf("Current working dir: %s", cwd);
	else
	    printf("Unable to show path ...");
}

/** 
 * This function update main screen
 *
 */

void show_Screen(){
	system("cls");
	showPath();
	char gij[]= "\n\n\n\n\n"
				"\t\t\t\t\t __________  _________   _________ \n"
				"\t\t\t\t\t/          ||         | |         | \n"
				"\t\t\t\t\t|   _______||__     __| |___     _| \n"
				"\t\t\t\t\t|   |  _____   |   |    _   |   |   \n"
				"\t\t\t\t\t|   |___|  | __|   |__ | \\__|   |  \n"
				"\t\t\t\t\t|          ||         ||        |   \n"
				"\t\t\t\t\t\\__________||_________|\\________/ \n"
				"\n\n";
	printMessage(gij, "blue");
	printf("\t\t\t\t    >>> ");
}

int main(){
	char conv_Situation[][MAX_ARRAY_SIZE] = {"deleted", "unchanged", "changed", "recently"};
	char selected[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char dir_List[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char stashed[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char input[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char description[MAX_ARRAY_SIZE];
	char message[MAX_ARRAY_SIZE];
	char buffer2[MAX_ARRAY_SIZE];
	char buffer[MAX_ARRAY_SIZE];
	char line[MAX_ARRAY_SIZE];
	char path[MAX_ARRAY_SIZE];
	
	int selected_Len = 0;
	int stashed_Len = 0;
	int dir_Len = 0;
	int input_Len;
	int index = 0;
	int init = 0;
	int pop = -1;
	
	getcwd(gij_Path, sizeof(gij_Path));
	
	while(1){
		
		show_Screen();
		gets(line);
		char *split = strtok(line, " ");
		input_Len = 0;
		while(split != NULL){
			strcpy(input[input_Len++], split);
			split = strtok(NULL, " ");
		}
		
		if(input_Len == 0)
			continue;
		
		getcwd(path, sizeof(path));
			
		if(!chdir("vc_data")){
			chdir("..");
			status_Get(path);
			init = 1;
		}
		
		printf("\n\n");
		
		if(!strcasecmp(input[0], "dir")){
			if(input_Len != 1)
				printMessage("Invalid command!\n", "red");
			else{
				strcpy(buffer, ".");
				dir_Len = select_All(path, buffer, dir_List, 0, 1);
				system("cls");
				printMessage("\nFiles in current directory\n\n\n", "blue");
				for(index=0; index<dir_Len; index++){
					printMessage(dir_List[index], "light-blue");
					printf("\n\n");
				}
			}
		}
		
		else if(!strcasecmp(input[0], "cd")){
			if(input_Len > 2)
				printMessage("Invalid command!\n", "red");
			else if(!strcmp(input[1], "vc_data"))
				printMessage("no permission!\n", "yellow");
			else if(!chdir(input[1])){
				printMessage("directory changed succesfully!\n", "green");
				if(strcmp(input[1], "."))
					init = 0;
				pop = -1;
			}
			else
				printMessage("invalid path!\n", "red");
		}
		
		else if(!strcasecmp(input[0], "mkdir")){
			if(input_Len > 2)
				printMessage("Invalid command!\n", "red");
			else if(!strcmp(input[1], "vc_data"))
				printMessage("no permission!\n", "yellow");
			else if(!mkdir(input[1])){
				sprintf(message, "%s directory made succesfully!\n", input[1]);
				printMessage(message, "green");
			}
			else{
				sprintf(message, "%s directory already exist!\n", input[1]);
				printMessage(message, "yellow");
			}
		}
		
		else if(!strcasecmp(input[0], "help")){
			if(input_Len != 1)
				printMessage("unknown command\n", "red");
			else{
				system("cls");
				char star[MAX_ARRAY_SIZE] = "*************************************";
				char help[][MAX_ARRAY_SIZE] = {
					"dir\t\t\t  show files in current directory\n",
					"cd <path>\t\t\t for change directory\n",
					"mkdir <name>\t\t\t make directory by given name in current directory\n",
					"init\t\t\t\t initial current directory\n",
					"status\t\t\t\t show situation of files\n",
					"select <file1> <file2> ...\t select given files\n",
					"select -all\t\t\t select all files in current directory and sub-directories\n",
					"unselect <file1> <file2> ...\t unselect given files\n",
					"unselect -all\t\t\t unselect all selected files\n",
					"selected\t\t\t show selected files\n",
					"commit <description>\t\t commit selected files\n",
					"log\t\t\t\t show allthings about commits\n",
					"reset <commit-Id>\t\t reset files to id-th commit\n",
					"reset -delete <commit-Id>\t delete all changes from id-th commit to the end\n",
					"stash <commit-Id>\t\t bla bla bla\n",
					"stash pop\t\t\t bla bla bla\n"};
				
				for(index=0; index<16; index++){
					printMessage(help[index], "blue");
					printMessage("\n*************************************\n\n", "yellow");
				}
			}
		}
		
		else if(!strcasecmp(input[0], "init")){
			if(input_Len > 1)
				printMessage("unknown command!\n", "red");
			else if(!mkdir("vc_data")){
				mkdir("vc_data\\files");
				mkdir("vc_data\\descriptions");
				mkdir("vc_data\\stash");
				FILE *st = fopen("vc_data\\status.txt", "w");
				fprintf(st, "%d\n%d\n", 0, 0);
				fclose(st);
				printMessage("initialized succesfully\n", "green");
				init = 1;
				pop = -1;
			}
			else{
				printMessage("already initialized\n", "yellow");
				init = 1;
				pop = -1;
			}
		}
		
		else if(!strcasecmp(input[0], "status")){
			if(input_Len > 1)
				printMessage("unknown command!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else
				status_(path);
		}
		
		else if(!strcasecmp(input[0], "select")){
			if(input_Len == 1)
				printMessage("select what?!!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else if(!strcmp(input[1], "-all")){
				if(input_Len != 2)
					printMessage("unknown command!\n", "red");
				else{
					int temp_Len = selected_Len;
					strcpy(buffer, ".");
					selected_Len = 0;
					selected_Len = select_All(path, buffer, selected, selected_Len, 2);
					printMessage("all files in directory and its subdirectories selected succesfully\n\n", "green");
					sprintf(message, "%d file selected succesfully\n", selected_Len - temp_Len);
					printMessage(message, "green");
					pop = -1;
				}
			}
			else{
				for(index=1; index<input_Len; index++){
					if(check_File(path, input[index])){
						strcpy(selected[selected_Len++], input[index]);
						sprintf(message, "%s selected succesfully\n\n", selected[selected_Len-1]);
						printMessage(message, "green");
						pop = -1;
					}
					else{
						sprintf(message, "cannot find %s!\n\n", input[index]);
						printMessage(message, "red");
					}
				}
			}
		}
		
		else if(!strcasecmp(input[0], "unselect")){
			if(input_Len == 1)
				printMessage("unselect what?!!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else if(!strcmp(input[1], "-all")){
				if(input_Len != 2)
					printMessage("unknown command!\n", "red");
				else{
					selected_Len = 0;
					printMessage("all selected files unselected succesfully\n", "green");
					pop = -1;
				}
			}
			else{
				for(index=1; index<input_Len; index++){
					if(check_File(path, input[index])){
						if(try_Unselect(input[index], selected, selected_Len)){
							sprintf(message, "%s unselected succesfully\n\n", input[index]);
							selected_Len -= 1;
							printMessage(message, "green");
							pop = -1;
						}
						else{
							sprintf(message, "%s has not been selected\n\n", input[index]);
							printMessage(message, "yellow");
						}
					}
					else{
						sprintf(message, "cannot find %s\n\n", input[index]);
						printMessage(message, "red");
					}
				}
			}
		}
		
		else if(!strcasecmp(input[0], "selected")){
			if(input_Len > 1)
				printMessage("unknown command\n", "red");
			else
				show_Selected(selected, selected_Len);
		}
		
		else if(!strcasecmp(input[0], "commit")){
			if(selected_Len == 0)
				printMessage("no file selected!\n", "red");
			else if(input_Len == 1)
				printMessage("add description!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else{
				// add description
				sprintf(buffer, "%s\\vc_data\\descriptions\\des%d.txt", path, number_Of_Commit+1);
				strcpy(description, input[1]);
				for(index=2; index<input_Len; index++){
					strcat(description, input[index]);
					strcat(description, " ");
				}
				FILE *des = fopen(buffer, "w");
				fprintf(des, "%s", description);
				fclose(des);
				
				commit(path, selected, selected_Len);
				
				int version_Of_S;
				for(index=0; index<selected_Len; index++){
					sprintf(message, "%s commited succesfully\n", selected[index]);
					printMessage(message, "green");
					lash_2_Hash(selected[index], buffer);
					version_Of_S = dict_Index(buffer);
					sprintf(message, "last version: %d\n\n", status[version_Of_S].last_Ver);
					printMessage(message, "green");
				}
				selected_Len = 0;
				pop = -1;
			}
		}
		
		else if(!strcasecmp(input[0], "log")){
			if(input_Len > 1)
				printMessage("unknown command!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else
				log_(path);
		}
		
		else if(!strcasecmp(input[0], "reset")){
			if(input_Len > 3)
				printMessage("unknown command!\n", "red");
			else if(!init)
				printMessage("no permission!\n", "yellow");
			else if(!strcmp(input[1], "-delete")){
				if(input_Len != 3)
					printMessage("unknown command!\n", "red");
				else{
					if(isNumerical(input[2])){
						int commitId = atoi(input[2]);
						if(0 <= commitId && commitId < number_Of_Commit){
							reset_And_Delete(path, commitId);
							strcpy(buffer, ".");
							selected_Len = 0;
							selected_Len = select_All(path, buffer, selected, selected_Len, 2);
							delete_Not_Commited(path, selected, selected_Len);
							selected_Len = 0;
							printMessage("reset-and-delete done succesfully\n", "green");
						}
						else
							printMessage("Invalid commit-Id!\n", "red");
					}
					else{
						printMessage("commit-Id must be numerical!\n", "red");
					}
				}
			}
			else if(isNumerical(input[1])){
				int commitId = atoi(input[1]);
				if(0 <= commitId && commitId <= number_Of_Commit){
					reset_(path, commitId);
					printMessage("reset done succesfully\n", "green");
				}
				else
					printMessage("Invalid commit-Id!\n", "red");
			}
			else{
				printMessage("commit-Id must be numerical!\n", "red");
			}
		}
		
		else if(!strcasecmp(input[0], "stash")){
			if(0)
				printMessage("not supported in this version!\n", "yellow");
			else{
				if(input_Len != 2)
					printMessage("unknown command!\n", "red");
				else if(!init)
					printMessage("no permission!\n", "yellow");
				else if(!strcmp(input[1], "pop")){
					if(pop == 0){
						stash_Pop(path, stashed, stashed_Len);
						pop = 1;
						printMessage("stash-pop done succesfully\n", "green");
					}
					else if(pop == 1){
						printMessage("you have done stash-pop recently!\n", "red");
					}
					else{
						printMessage("you haven't done any stash'!\n", "red");
					}
				}
				else if(isNumerical(input[1])){
					int commitId = atoi(input[1]);
					if(1 <= commitId && commitId <= number_Of_Commit){
						strcpy(buffer, ".");
						selected_Len = select_All(path, buffer, selected, 0, 2);
						
						sprintf(buffer2, "%s\\vc_data\\stash", path);
						stashed_Len = select_All(buffer2, buffer, stashed, 0, 2);
						
						stash_(path, commitId, selected, selected_Len, stashed, stashed_Len);
						reset_(path, commitId);
						stashed_Len = selected_Len;
						selected_Len = 0;
						
						pop = 0;
						printMessage("stash done succesfully\n", "green");
					}
					else
						printMessage("Invalid commit-Id!\n", "red");
				}
				else{
					printMessage("commit-Id must be numerical!\n", "red");
				}
			}
		}
		
		else
			printMessage("unknown command!\n", "red");
		
		printf("\n\n\n");
		system("pause");
	}
}
