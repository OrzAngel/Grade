#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

void read_data();
int database_check();

eval** database;
int size;

/*
Initialize an array of evaluation information on heap called database
Read the local file into the database
*/
void database_init(int init_size, char* filename) 
{
	database = malloc(sizeof(eval*) * init_size);
	read_data(filename);
	if (database_check())
	{
		fprintf(stderr, "sth wrong\n");
	}
}

int database_check()
{
	// printf("%d\n", size);
	return 0;
}

/*
Read the file specified by the user input
Parse the file into eval structs and update the database
*/
void read_data(char* filename){

	FILE* src_file;
	char str1[100];
	char str2[100];
	int enroll;
	double c_quality;
	double diff;
	double i_quality;

	src_file = fopen(filename, "r");
	if(src_file == NULL){
		return;
	}

	size = 0;
	while(fscanf(src_file, "%[^,], %[^,], %d, %lf, %lf, %lf", str1, str2, &enroll, &c_quality, &diff, &i_quality) == 6){

		eval* e = malloc(sizeof(eval));

		e->course_number = malloc(strlen(str1)+1);
		strcpy(e->course_number, str1);
		e->instructor = malloc(strlen(str2)+1);
		strcpy(e->instructor, str2);
		e->enrollment = enroll;
		e->course_quality = c_quality;
		e->difficulty = diff;
		e->instructor_quality = i_quality;

		database[size++] = e;

	}

	fclose(src_file);
}

/* 
Convert each eval struct into a formatted string and print it out
*/
char* eval_to_string(eval* e)
{	
	char* res = malloc(150);
	//char* res = malloc(sizeof(char)*150);
	sprintf(res,"<tr><td>%s</td><td>%s</td><td>%d</td><td>%.2lf</td><td>%.2lf</td><td>%.2lf</td></tr>",e->course_number,e->instructor,e->enrollment,e->course_quality,e->difficulty,e->instructor_quality);
	return res;
}

/* 
Get the database by copying it into a new array of evals 
*/
eval** get_all()
{
	eval** res = malloc(sizeof(eval*) * (size + 1) );
	for (int i = 0 ; i < size ; i++)
	{
		res[i] = database[i];
	}
	res[size] = NULL;

	return res;
}

/*
If any invalid request has been received, return an empty database
*/
eval** get_404()
{
	eval** res = malloc(sizeof(eval*) * 1);
	res[0] = NULL;
	return res;
}

/*
A helper function to help with sorting course difficulty ascendingly
*/
int cmp_diff_asd(const void* e1, const void* e2)
{
	double r = (*((eval**) e1)) -> difficulty - (*((eval**) e2)) -> difficulty;
	
	return r > 0 ? 1 : -1; 
}

/*
A helper function to help with sorting course difficulty descendingly
*/
int cmp_diff_dse(const void* e1, const void* e2)
{
	double r = (*((eval**) e1)) -> difficulty - (*((eval**) e2)) -> difficulty;
	
	return r > 0 ? -1 : 1; 
}

/*
A helper function to help with sorting enrollment ascendingly
*/
int cmp_enroll_asd(const void* e1, const void* e2)
{
	int r = (*((eval**) e1)) -> enrollment- (*((eval**) e2)) -> enrollment;
	
	return r; 
}

/*
A helper function to help with sorting enrollment descendingly
*/
int cmp_enroll_dse(const void* e1, const void* e2)
{
	int r = (*((eval**) e1)) -> enrollment- (*((eval**) e2)) -> enrollment;
	
	return -r; 
}

/*
A helper function to help with sorting course quality ascendingly
*/
int cmp_course_quality_asd(const void* e1, const void* e2)
{
	double r = (*((eval**) e1)) -> course_quality - (*((eval**) e2)) -> course_quality;
	
	return r > 0 ? 1 : -1; 
}

/*
A helper function to help with sorting course quality descendingly
*/

int cmp_course_quality_dse(const void* e1, const void* e2)
{
	double r = (*((eval**) e1)) -> course_quality - (*((eval**) e2)) -> course_quality;
	
	return r > 0 ? -1 : 1; 
}

/*
Sort course difficulty either ascendinly or descendingly
*/
eval** sort_diff(char* sorting)
{
	eval** res = get_all();
	
	if(strcmp(sorting,"ascending") == 0){
		qsort(res,size,sizeof(eval*), &cmp_diff_asd);
	}else{
		qsort(res,size,sizeof(eval*), &cmp_diff_dse);
	}
	
	return res;
}


/*
Sort course enrollment either ascendinly or descendingly
*/
eval** sort_enroll(char* sorting)
{	
	eval** res = get_all();
	if(strcmp(sorting,"ascending") == 0){
		qsort(res,size,sizeof(eval*), &cmp_enroll_asd);
	}else{
		qsort(res,size,sizeof(eval*), &cmp_enroll_dse);
	}
	return res;
	
}


/*
Sort course quality either ascendinly or descendingly
*/
eval** sort_course_quality(char* sorting)
{	
	eval** res = get_all();
	if(strcmp(sorting,"ascending") == 0){
		qsort(res,size,sizeof(eval*), &cmp_course_quality_asd);
	}else{
		qsort(res,size,sizeof(eval*), &cmp_course_quality_dse);
	}
	return res;
	
}


/*
Filter course difficulty according to a bar specified by user
*/
eval** filter_diff(double bar)
{
	eval** res = get_all();

	qsort(res,size,sizeof(eval*), &cmp_diff_asd);
	
	eval** unfiltered = res;
	eval** filtered = res;

	while(*unfiltered){
		if((*(unfiltered)) -> difficulty > bar){
			*(filtered) =  *(unfiltered);
				unfiltered++;
				filtered++;
		}
		else{
				unfiltered++;
		}
	}
	*filtered = NULL;

	return res;
}

/*
Filter course enrollment according to a bar specified by user
*/
eval** filter_enroll(double bar)
{	
	eval** res = get_all();

	qsort(res,size,sizeof(eval*), &cmp_enroll_asd);
	
	eval** unfiltered = res;
	eval** filtered = res;

	while(*unfiltered){
		if((*(unfiltered)) -> enrollment > bar){
			*(filtered) =  *(unfiltered);
				unfiltered++;
				filtered++;
		}
		else{
				unfiltered++;
		}
	}

	*filtered = NULL;

	return res;
	
}

/*
Filter course quality according to a bar specified by user
*/
eval** filter_course_quality(double bar)
{	
	eval** res = get_all();

	qsort(res,size,sizeof(eval*), &cmp_course_quality_asd);
	
	eval** unfiltered = res;
	eval** filtered = res;

	while(*unfiltered){
		if((*(unfiltered)) -> course_quality > bar){
			*(filtered) =  *(unfiltered);
				unfiltered++;
				filtered++;
		}
		else{
				unfiltered++;
		}
	}

	*filtered = NULL;

	return res;
	
}

/*
Filter course number according to a bar specified by user
*/
eval** filter_course_number(char* bar)
{	
	eval** res = get_all();

	//qsort(res,size,sizeof(eval*), &cmp_course_quality);
	
	eval** unfiltered = res;
	eval** filtered = res;

	while(*unfiltered){
		if(strstr((*(unfiltered)) -> course_number, bar)){
			*(filtered) =  *(unfiltered);
			unfiltered++;
			filtered++;
		}
		else{
			unfiltered++;
		}
	}

	*filtered = NULL;

	return res;
	
}

/*
Sort a specified column either ascendingly or descendingly
*/
eval** get_sorted(char* column, char* sorting)
{

	if (strcmp(column,"difficulty") == 0)
	{
		return sort_diff(sorting);
	}
	if(strcmp(column,"enrollment") == 0){
		return sort_enroll(sorting);
	}
	if(strcmp(column,"course_quality") == 0){
		return sort_course_quality(sorting);
	}
	return get_404();
}

/*
Sort a specified column according to a bar
*/
eval** get_filtered(char* column, char* bar)
{

	if (strcmp(column,"difficulty") == 0)
	{	
		double b;
		sscanf(bar,"%lf", &b);
		return filter_diff(b);
	}
	if(strcmp(column,"enrollment") == 0){
		double b;
		sscanf(bar,"%lf", &b);
		return filter_enroll(b);
	}
	if(strcmp(column,"course_quality") == 0){
		double b;
		sscanf(bar,"%lf", &b);
		return filter_course_quality(b);
	}
	
	if(strcmp(column,"course_number") == 0){
		return filter_course_number(bar);
	}

	return get_404();
}

/*
Calculate the average of each column in database
*/
eval* calculate_average(eval** e){
	 int total_enrollment = 0;
  double total_course_quality = 0;
  double total_difficulty = 0;
  double total_instructor_quality = 0;
  int i = 0;
  while(e[i]){
  	total_enrollment += e[i]->enrollment;
  	total_course_quality += e[i]->course_quality;
  	total_difficulty += e[i]->difficulty;
  	total_instructor_quality += e[i]->instructor_quality;
  	i++;
  }

  eval* average = malloc(sizeof(eval));
  if(average == NULL){
  	return NULL;
  }

  average->course_number = malloc(sizeof(char));
  average->course_number[0] = '\0';

  average->instructor= malloc(sizeof(char)*10);
  strcpy(average->instructor, "Average: ");

  average->enrollment = total_enrollment/size;
  average->course_quality = total_course_quality/size;
  average->difficulty = total_difficulty/size;
  average->instructor_quality = total_instructor_quality/size;

  return average;

}

/*
Calculate the min of each column in database
*/
eval* calculate_min(eval** e){
	 int min_enrollment = e[0]->enrollment;
  double min_course_quality = e[0]->course_quality;
  double min_difficulty = e[0]->difficulty;
  double min_instructor_quality = e[0]->instructor_quality;
int i = 0;
  while(e[i]){
  	if(min_enrollment > e[i]->enrollment){
  		min_enrollment = e[i]->enrollment;
  	}
  	if(min_course_quality > e[i]->course_quality){
  		min_course_quality = e[i]->course_quality;
  	}
  	if(min_difficulty > e[i]->difficulty){
  		min_difficulty = e[i]->difficulty;
  	}	
  	if(min_instructor_quality > e[i]->instructor_quality){
  		min_instructor_quality = e[i]->instructor_quality;
  	}  	
  	i++;
  }

  eval* min = malloc(sizeof(eval));
  if(min == NULL){
  	return NULL;
  }

  min->course_number = malloc(sizeof(char));
  min->course_number[0] = '\0';
  //strcpy(min->course_number, " ");

  min->instructor= malloc(sizeof(char)*10);
  strcpy(min->instructor, "min: ");

  min->enrollment = min_enrollment;
  min->course_quality = min_course_quality;
  min->difficulty = min_difficulty;
  min->instructor_quality = min_instructor_quality;

  return min;

}


/*
Calculate the max of each column in database
*/
eval* calculate_max(eval** e){
	 int max_enrollment = e[0]->enrollment;
  double max_course_quality = e[0]->course_quality;
  double max_difficulty = e[0]->difficulty;
  double max_instructor_quality = e[0]->instructor_quality;
int i = 0;
  while(e[i]){
  	if(max_enrollment < e[i]->enrollment){
  		max_enrollment = e[i]->enrollment;
  	}
  	if(max_course_quality < e[i]->course_quality){
  		max_course_quality = e[i]->course_quality;
  	}
  	if(max_difficulty < e[i]->difficulty){
  		max_difficulty = e[i]->difficulty;
  	}	
  	if(max_instructor_quality < e[i]->instructor_quality){
  		max_instructor_quality = e[i]->instructor_quality;
  	}  	
  	i++;
  }

  eval* max = malloc(sizeof(eval));
  if(max == NULL){
  	return NULL;
  }

  max ->course_number = malloc(sizeof(char));
  max ->course_number[0] = '\0';

  max->instructor= malloc(sizeof(char)*10);
  strcpy(max->instructor, "Max: ");

  max->enrollment = max_enrollment;
  max->course_quality = max_course_quality;
  max->difficulty = max_difficulty;
  max->instructor_quality = max_instructor_quality;

  return max;

}

/*
When user terminates the program, free all memeory allocated by database.
*/
void database_free()
{
	for (int i = 0 ; i < size ; i++) 
	{
		free(database[i]->course_number);
		free(database[i]->instructor);
		free(database[i]);
	}
	free(database);
}