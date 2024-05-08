#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DIM_3d 100
#define DIM_2d 100
#define DIM_1d 1000000

// declarations of the original functions in orig_funcs.o
int f1_orig(int ***);
int f2_orig(int *);
int **f3_orig(int **, int **);

// TODO: This function has bad spatial locality. Make it more efficient!
// Hint: What is the access pattern for the flattened 3D array in memory?
int f1(int ***array_3d) {
	int sum = 0;
	for (int k = 0; k < DIM_3d; k++) {
		for (int j = 0; j < DIM_3d; j++) {
			for (int i = 0; i < DIM_3d; i++) {
				sum += array_3d[k][j][i];
			}
		}
	}
	return sum;
}

// TODO: This function has bad temporal locality. Make it more efficient!
// Hint: Is there a better way to track a running sum?
int f2(int *array) {
	int sums = array[0];
	for (int i = 1; i < DIM_1d; i++) {
		sums += array[i];
	}
	return sums;
}

// TODO: This function has bad spatial AND temporal locality. Make it more efficient!
// Hint: Is the access pattern optimal? Can you save entries to reduce the number of memory accesses?
// Bonus: Try to implement matrix multiplication with blocking for further speedups if you are ambitious
int **f3(int **array1, int **array2) {
	// initialize result array
	int **result = malloc(DIM_2d * sizeof(int*));
	for (int i = 0; i < DIM_2d; i++) {
		result[i] = malloc(DIM_2d * sizeof(int));
		memset(result[i], 0, DIM_2d * sizeof(int));
	}
	// actual computation
	for (int i = 0; i < DIM_2d; i++) {
		for (int k = 0; k < DIM_2d; k++) {
			int array1_ik = array1[i][k];
			for (int j = 0; j < DIM_2d; j++) {
				result[i][j] += array1_ik * array2[k][j]; 
			}
		}
	}
	return result;
}

// main function, don't modify this!
int main(int argc, char *argv[]) {

	// declare and initialize arrays to use for lab functions
	int i, j, k;
	int array_1d[DIM_1d];
	int ***array_3d = malloc(DIM_3d * sizeof(int*));
	int **array_2d_1 = malloc(DIM_2d * sizeof(int*));
	int **array_2d_2 = malloc(DIM_2d * sizeof(int*));
	
	for (i = 0; i < DIM_1d; i++) {
		array_1d[i] = rand() % 20;
	}
	for (i = 0; i < DIM_3d; i++) {
		array_3d[i] = malloc(DIM_3d * sizeof(int*));
		for (j = 0; j < DIM_3d; j++) {
			array_3d[i][j] = malloc(DIM_3d * sizeof(int));
			for (k = 0; k < DIM_3d; k++) {
				array_3d[i][j][k] = rand() % 20;
			}
		}
	}
	for (i = 0; i < DIM_2d; i++) {
		array_2d_1[i] = malloc(DIM_2d * sizeof(int*));
		array_2d_2[i] = malloc(DIM_2d * sizeof(int*));
		for (j = 0; j < DIM_2d; j++) {
			array_2d_1[i][j] = rand() % 20;
			array_2d_2[i][j] = rand() % 20;
		}
	}

	// timing to test if edits have an effect
	clock_t t1 = clock();
	int s1 = f1(array_3d);
	clock_t t2 = clock();
	printf("Your function f1 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	t1 = clock();
	int s2 = f1_orig(array_3d);
	t2 = clock();
	printf("Unmodified, f1 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	// need to ensure correctness as well
	if (s1 != s2) {
		printf("Error, incorrect result from modified f1\n");
	}

	t1 = clock();
	s1 = f2(array_1d);
	t2 = clock();
	printf("Your function f2 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	t1 = clock();
	s2 = f2_orig(array_1d);
	t2 = clock();
	printf("Unmodified, f2 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	if (s1 != s2) {
		printf("Error, incorrect result from modified f2\n");
	}

	t1 = clock();
	int **res = f3(array_2d_1, array_2d_2);
	t2 = clock();
	printf("Your function f3 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	t1 = clock();
	int **res_orig = f3_orig(array_2d_1, array_2d_2);
	t2 = clock();
	printf("Unmodified, f3 took %f seconds\n", ((double) t2 - t1) / CLOCKS_PER_SEC);

	int flag = 1;
	for (i = 0; i < DIM_2d; i++) {
		for (j = 0; j < DIM_2d; j++) {
			if (res[i][j] != res_orig[i][j]) {
				flag = 0;
			}
		}
	}
	if (!flag) {
		printf("Error, incorrect result from modified f3\n");
	}

	// free the malloc's arrays
	for (i = 0; i < DIM_3d; i++) {
		for (j = 0; j < DIM_3d; j++) {
			free(array_3d[i][j]);
		}
		free(array_3d[i]);
	}
	free(array_3d);
	for (i = 0; i < DIM_2d; i++) {
		free(array_2d_1[i]);
		free(array_2d_2[i]);
		free(res[i]);
		free(res_orig[i]);
	}
	free(array_2d_1);
	free(array_2d_2);
	free(res);
	free(res_orig);

	return 0;
}


