// database_test.c

#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main()
{
	database_init(700);
	eval** e = get_all();

	// for (int i = 0 ; e[i] && i <= 10 ; i++)
	// {
	// 	// fprintf(stdout, "%s\n", eval_to_string(e[i]));
	// }

	// free(e);

	// e = get_sorted("difficulty");

	// for (int i = 0 ; e[i] && i <= 10 ; i++)
	// {
	// 	fprintf(stdout, "%s\n", eval_to_string(e[i]));
	// }


	eval* average = calculate_average(e);
	fprintf(stdout, "%s\n", eval_to_string(average));

	free(e);

	database_free();
}