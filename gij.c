#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_ARRAY_SIZE 300

int temp_Situation[MAX_ARRAY_SIZE];
int number_Of_Commit;
int status_Len;
struct dict{
	int last_Ver;
	int last_Situation;
	int all_Situation[MAX_ARRAY_SIZE];
	int all_Ver[MAX_ARRAY_SIZE];
	char name[MAX_ARRAY_SIZE];
} status[MAX_ARRAY_SIZE];

void get_Format(char file[], char format[]){
	int len = strlen(file);
	int index = len;
	int lenF;
	for(lenF=0; file[index--] != '.'; lenF++);
	for(index=0; index<lenF; index++)
		format[index] = file[index+len-lenF];
}

void make_Copy(char origin[], char fileO[], char destination[], char fileD[]){
	char command[MAX_ARRAY_SIZE];
	sprintf(command, "copy %s\\%s %s\\%s > E#R#R#O#R.txt", origin, fileO, destination, fileD);
	system(command);
	remove("E#R#R#O#R.txt");
}

int dict_Index(char file[]){
	int index;
	for(index=0; index<status_Len; index++)
		if(!strcmp(status[index].name, file))
			return index;
	return -1;
}

void clean(char string[]){
	memset(string, '\0', sizeof(string));
}

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

void reset(char path[], int commitId){
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
		sprintf(path_Des, "%s\\vc_data\\descriptions\\%d.txt", path, index);
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
				sprintf(path_Des, "%s\\vc_data\\files\\%s\\%d.txt", path, status[index].name, indexV);
				remove(path_Des);
			}
			sprintf(path_Des, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV-1, formatV);
			remove(path_Des);
			
			for(indexV=1; indexV<status[index].all_Ver[commitId-1]; indexV++){
				sprintf(path_Ori, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV, formatV);
				sprintf(path_Des, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV+1, ".txt");
				sprintf(path_Res, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV+1, formatV);
				sprintf(command, "patch0.exe %s %s %s", path_Ori, path_Des, path_Res);
				system(command);
				if(indexV != 1)
					remove(path_Ori);
			}
			
			sprintf(path_Ori, "%s\\vc_data\\files\\%s, path", status[index].name);
			sprintf(name_Ori, "%d%s", status[index].all_Ver[commitId-1], formatV);
			make_Copy(path_Ori, name_Ori, path, name_Lashed);
			
		}
		else if(status[index].all_Situation[commitId-1] == 3){
			
			if(status[index].last_Situation >= 0){
				sprintf(path_Des, "%s\\%s", path, name_Lashed);
				remove(path_Des);
			}
			
			for(indexV=1; indexV<=status[index].last_Ver; indexV++){
				sprintf(path_Ori, "%s\\vc_data\\files\\%s\\%d%s", path, status[index].name, indexV, ".txt");
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

void commit(char path[], char selected[][MAX_ARRAY_SIZE], int numberOS){
	puts(path);
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
			
			puts(pathOS);
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
			
			sprintf(command, "diff0.exe %s\\%d%s %s\\%d%s %s\\%d%s", pathOS, verOS, formatOS, pathOS, verOS+1, formatOS, pathOS, verOS+1, ".txt");
			system(command);
			
			if(verOS != 1){
				sprintf(command, "%s\\%d%s", pathOS, verOS, formatOS);
				remove(command);
			}
			
			status[indInVer].last_Ver += 1;
			temp_Situation[status_Len] = 1;
		}
	}
	
	situation_Update(path);
	number_Of_Commit++;
	status_Update(path, 0);
}

void log_(char path[]){
	char pathD[MAX_ARRAY_SIZE];
	char description[MAX_ARRAY_SIZE];
	FILE *des;
	int index, indexF;
	
	status_Get(path);
	
	system("cls");
	
	if(number_Of_Commit == 0)
		printf("nothing to show\n");
	
	for(index=0; index<number_Of_Commit; index++){
		printf("commit-Id: %d\n", index+1);
		sprintf(pathD, "%s\\vc_data\\descriptions\\%d.txt", path, index+1);
		des = fopen(pathD, "r");
		fgets(description, sizeof(description), des);
		printf("description: %s\n\n", description);
		fclose(des);
	}
}

void status_(char path[], char selected[][MAX_ARRAY_SIZE], int selected_Len){
	char name_Lashed[MAX_ARRAY_SIZE];
	char conv_Situation[][MAX_ARRAY_SIZE] = {"deleted", "unchanged", "changed", "recently"};
	int index;
	
	status_Get(path);
	
	printf("\n\nStatus of files\n\n");
	if(status_Len == 0)
		printf("nothing to show\n");
	for(index=0; index<status_Len; index++)
		if(-1 <= status[index].last_Situation && status[index].last_Situation <= 2){
			hash_2_Lash(status[index].name, name_Lashed);
			printf("%s: \t %s\n", name_Lashed, conv_Situation[status[index].last_Situation+1]);
		}
}

void show_Selected(char selected[][MAX_ARRAY_SIZE], int selected_Len){
	int index;
	system("cls");
	
	printf("Selected files\n\n");
	if(selected_Len == 0)
		printf("no file selected\n");
	for(index=0; index<selected_Len; index++)
		puts(selected[index]);
}

int try_Unselect(char file[], char selected[][MAX_ARRAY_SIZE], int selected_Len){
	int index, shift;
	
	for(index=0; index<selected_Len; index++)
		if(!strcmp(selected[index], file)){
			for(shift=index; shift<selected_Len-1; shift++)
				strcpy(selected[shift], selected[shift+1]);
			return 1;
		}
	return 0;
}

int is_Dir(char pre[], char file[]){
	if(chdir(file))
		return 0;
	chdir(pre);
	return 1;
}

int select_All(char path[], char cur_Dir[], char selected[][MAX_ARRAY_SIZE], int selected_Len){
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
			
		if(is_Dir(path, temp_Dir)) // check file is directory or not
			selected_Len = select_All(path, temp_Dir, selected, selected_Len);
		else
			strcpy(selected[selected_Len++], temp_Dir);
	}
	
	return selected_Len;
}

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

int isNumerical(char number[]){
	int i=0;
	
	while(number[i])
		if(!isdigit(number[i++]))
			return 1;
	return 0; // 1 for ok
}

void showPath(){
	char cwd[MAX_ARRAY_SIZE];
	if(getcwd(cwd, sizeof(cwd)))
	    printf("Current working dir: %s", cwd);
	else
	    printf("Unable to show path ...");
}

void show_Screen(char path[]){
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
	puts(gij);
	printf("\t\t\t\t\t");
}

void make_Path(char path_list[][MAX_ARRAY_SIZE], int pl, char path[]){
	int index;
	for(index=0; index<pl; index++){
		strcat(path, "\\");
		strcat(path, path_list[index]);
	}
}

int main(){
	char conv_Situation[][MAX_ARRAY_SIZE] = {"deleted", "unchanged", "changed", "recently"};
	char line[MAX_ARRAY_SIZE];
	char input[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char path[MAX_ARRAY_SIZE];
	char selected[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
	char description[MAX_ARRAY_SIZE];
	char buffer[MAX_ARRAY_SIZE];
	int selected_Len = 0;
	int input_Len;
	int index = 0;
	int init = 0;
	while(1){
		
		show_Screen(path);
		gets(line);
		char *split = strtok(line, " ");
		input_Len = 0;
		while(split != NULL){
			strcpy(input[input_Len++], split);
			split = strtok(NULL, " ");
		}
		
		if(input_Len == 0)
			continue;
			
		if(!chdir("vc_data")){
			chdir("..");
			init = 1;
		}
		
		getcwd(path, sizeof(path));
		
		printf("\n\n");
		
		if(!strcasecmp(input[0], "cd")){
			if(input_Len > 2)
				printf("Invalid command!\n");
			else if(!chdir(input[1])){
				printf("directory changed succesfully!\n");
				if(strcmp(input[1], "."))
					init = 0;
			}
			else
				printf("invalid path!\n");
		}
		
		else if(!strcasecmp(input[0], "mkdir")){
			if(input_Len > 2)
				printf("Invalid command!\n");
			else if(!mkdir(input[1]))
				printf("%s directory made succesfully!\n", input[1]);
			else
				printf("%s directory already exist|!\n", input[1]);
		}
		
		else if(!strcasecmp(input[0], "help")){
			if(input_Len != 1)
				printf("unknown command\n");
			else{
				system("cls");
				printf(
					"cd <path>\t\t\t for change directory\n\n"
					"mkdir <name>\t\t\t make directory by given name in current directory\n\n"
					"init\t\t\t\t initial current directory\n\n"
					"status\t\t\t\t show situation of files\n\n"
					"select <file1> <file2> ...\t select given files\n\n"
					"select -all\t\t\t select all files in current directory and sub-directories\n\n"
					"unselect <file1> <file2> ...\t unselect given files\n\n"
					"unselect -all\t\t\t unselect all selected files\n\n"
					"selected\t show selected files\n\n"
					"commit <description>\t\t commit selected files\n\n"
					"log\t\t\t\t show allthings about commits\n\n"
					"reset <commit-Id>\t\t bla bla bla\n\n"
					"stash <commit-Id>\t\t bla bla bla\n\n"
					"stash pop\t\t\t bla bla bla\n\n"
				);
			}
		}
		
		else if(!strcasecmp(input[0], "init")){
			if(input_Len > 1)
				printf("unknown command!\n");
			else if(!mkdir("vc_data")){
				mkdir("vc_data\\files");
				mkdir("vc_data\\descriptions");
				FILE *st = fopen("vc_data\\status.txt", "w");
				fprintf(st, "%d\n%d\n", 0, 0);
				fclose(st);
			}
			else
				printf("already initialized\n");
			init = 1;
		}
		
		else if(!strcasecmp(input[0], "status")){
			if(input_Len > 1)
				printf("unknown command!\n");
			else if(!init)
				printf("no permission!\n");
			else
				status_(path, selected, selected_Len);
		}
		
		else if(!strcasecmp(input[0], "select")){
			if(input_Len == 1)
				printf("select what?!!\n");
			else if(!init)
				printf("no permission!\n");
			else if(!strcmp(input[1], "-all")){
				if(input_Len != 2)
					printf("unknown command!\n");
				else{
					int temp_Len = selected_Len;
					strcpy(buffer, ".");
					selected_Len = 0;
					selected_Len = select_All(path, buffer, selected, selected_Len);
					printf("all files in directory and its subdirectories selected succesfully\n\n");
					printf("%d file selected succesfully\n", selected_Len - temp_Len);
				}
			}
			else{
				for(index=1; index<input_Len; index++){
					if(check_File(path, input[index])){
						strcpy(selected[selected_Len++], input[index]);
						printf("%s file selected succesfully\n\n", selected[selected_Len-1]);
					}
					else
						printf("cannot find %s!\n\n", input[index]);
				}
			}
		}
		
		else if(!strcasecmp(input[0], "unselect")){
			if(input_Len == 1)
				printf("unselect what?!!\n");
			else if(!init)
				printf("no permission!\n");
			else if(!strcmp(input[1], "-all")){
				if(input_Len != 2)
					printf("unknown command!\n");
				else{
					printf("all selected files unselected succesfully\n");
					selected_Len = 0;
				}
			}
			else{
				for(index=1; index<input_Len; index++){
					if(check_File(path, input[index])){
						if(!try_Unselect(input[index], selected, selected_Len))
							printf("%s has not been selected\n\n", input[index]);
						else
							printf("%s unselected succesfully\n\n", input[index]);
					}
					else{
						printf("cannot find %s\n\n", input[index]);
					}
				}
			}
		}
		
		else if(!strcasecmp(input[0], "selected")){
			if(input_Len > 1)
				printf("unknown command\n");
			else
				show_Selected(selected, selected_Len);
		}
		
		else if(!strcasecmp(input[0], "commit")){
			if(selected_Len == 0)
				printf("no file selected!\n");
			else if(input_Len == 1)
				printf("add description!\n");
			else if(!init)
				printf("no permission!\n");
			else{
				// add description
				sprintf(buffer, "%s\\vc_data\\descriptions\\%d.txt", path, number_Of_Commit+1);
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
					printf("%s commited succesfully\n", selected[index]);
					version_Of_S = dict_Index(selected[index]);
					printf("last version: %d\n", status[version_Of_S].last_Ver);
					printf("last situation: %d\n\n", conv_Situation[status[version_Of_S].last_Situation+1]);
				}
				selected_Len = 0;
			}
		}
		
		else if(!strcasecmp(input[0], "log")){
			if(input_Len > 1)
				printf("unknown command!\n");
			else if(!init)
				printf("no permission!\n");
			else
				log_(path);
		}
		
		else if(!strcasecmp(input[0], "reset")){
			if(input_Len != 2)
				printf("unknown command!\n");
			else if(!init)
				printf("no permission!\n");
			if(isNumerical(input[1])){
				int commitId = atoi(input[1]);
				if(1 <= commitId && commitId < number_Of_Commit){
					reset(path, commitId);
					printf("reser done succesfully\n");
				}
				else if(commitId == 0){
					int n = 1;
				}
				else
					printf("Invalid commit-Id!\n");
			}
			else{
				printf("commit-Id must be numerical!\n");
			}
		}
		
		else if(!strcasecmp(input[0], "stash")){
			if(input_Len != 2)
				printf("unknown command!\n");
			else if(!init)
				printf("no permission!\n");
		}
		
		else
			printf("unknown command\n");
		
		printf("\n\n\n");
		system("pause");
	}
}
