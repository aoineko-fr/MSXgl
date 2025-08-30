#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
//#include <sys/time.h>
#include <vector>


// Based on kmeans function by Paul Ramsey
// Copyright (c) 2016, Paul Ramsey
// All rights reserved.

#include "kmeans.h"


// vector size - 8x8=64 pixels max
#define N (64)

typedef struct point
{
	double x[N];
} point;


static double pt_distance(const Pointer a, const Pointer b)
{
	int i;
	point* pa = (point*)a;
	point* pb = (point*)b;

	double somma_quadrati = 0;

	for(i = 0; i < N; i++) {
		double d = pa->x[i] - pb->x[i];
		somma_quadrati += d * d;
	}
	return somma_quadrati;
}

static void pt_centroid(const Pointer* objs, const int* clusters, size_t num_objs, int cluster, Pointer centroid)
{
	int i, j;
	int num_cluster = 0;
	point sum;
	point** pts = (point**)objs;
	point* center = (point*)centroid;

	for(j = 0; j < N; j++) {
		sum.x[j] = 0.0;
	}

	if(num_objs <= 0) return;		// no objects in the set

	for(i = 0; i < num_objs; i++) {

		if(clusters[i] != cluster) continue;	// Only process objects of this cluster 

		for(j = 0; j < N; j++) {
			sum.x[j] += pts[i]->x[j];
		}
		num_cluster++;
	}

	if(num_cluster)
	{
		for(j = 0; j < N; j++) {
			sum.x[j] /= num_cluster;
		}
		*center = sum;
	}
	return;
}

bool km_quantization(const std::vector<point>& image /*int nargs, char** argv*/)
{
	kmeans_config config;
	kmeans_result result;
	unsigned int i, j;

	point* pts;
	point* init;
	int print_results = 0;
	//unsigned long start;

	//FILE* fp;


	int k = 256;

	//if(nargs != 3) {
	//	printf("Use: %s <InputFileName> <InputVectorNumber>\n\n", argv[0]);
	//	printf("The input file is an array of chars with value 0 or 1, with size 64xVectorNumber\n");
	//	printf("NB: VectorNumber has to be > 256\n\n");
	//	printf("Two otput files are generated:\n");
	//	printf("- centroids.bin is an array of char with value 0 or 1, with size 64x256 (tileset)\n");
	//	printf("- classification.bin is an array of VectorNumber chars with the centroid number \n  associeted to each input vector (the PNT)\n");
	//	return 1;
	//}

	//char* inputfile = argv[1];
	//int      NumVec = atoi(argv[2]);
	int NumVec = image.size();

	//srand(time(NULL));
	srand(0);

	/* Constants */
	config.k = k;						// number of tiles to generate
	config.num_objs = NumVec; 			// in Screen 0 MSX1, it is 40x24 ; on MSX2 it is 80x24 or 80*27;
	config.max_iterations = 500;
	config.distance_method = pt_distance;
	config.centroid_method = pt_centroid;

	/* Inputs for K-means */
	config.objs = calloc(config.num_objs, sizeof(Pointer));
	config.centers = calloc(config.k, sizeof(Pointer));
	config.clusters = calloc(config.num_objs, sizeof(int));

	/* Storage for raw data */
	pts = calloc(config.num_objs, sizeof(point));
	init = calloc(config.k, sizeof(point));

	/* read binary data */
	//fp = fopen(inputfile, "rb");
	//if(fp == NULL) {
	//	printf("Error while opening the input file.\n");
	//	return 1;
	//}

	for(j = 0; j < config.num_objs; j++)
	{
		//char inp[N];
		//int nbytes = (int)fread(&(inp[0]), sizeof(char), N, fp);

		//if(nbytes != N)
		//{
		//	printf("Error while reading the input file.\n");
		//	fclose(fp);
		//	return 1;
		//}
		//for(i = 0; i < N; i++)
		//{
		//	pts[j].x[i] = (char)(inp[i] != 0);  // convert input to 1bpp
		//}

		for(i = 0; i < N; i++)
		{
			pts[j].x[i] = (char)(image[j][i] != 0);  // convert input to 1bpp
		}

		config.objs[j] = &(pts[j]);				// Pointer to raw data 
	}

	//fclose(fp);

	/* Populate the initial means vector with random start points */
	for(i = 0; i < config.k; i++)
	{
		int r = lround(config.num_objs * (1.0 * rand() / RAND_MAX));
		/* Populate raw data */
		init[i] = pts[r];				// copy vectors
		/* Pointers to raw data */
		config.centers[i] = &(init[i]);
	}

	/* run k-means! */
	//start = time(NULL);
	result = kmeans(&config);

	//printf("\n");
	//printf("Iteration count: %d\n", config.total_iterations);
	//printf("     Time taken: %ld seconds\n", (time(NULL) - start));
	//printf(" Iterations/sec: %.3g\n", (1.0 * config.total_iterations) / (time(NULL) - start));
	//printf("\n");



	/* save results */

	// Save the tileset from the kmeans (centroids of 64 bytes)
	fp = fopen("centroids.bin", "wb");
	if(fp == NULL) {
		printf("Error opening the centroid file.\n");
		return 1;
	}

	for(i = 0; i < config.k; i++)
	{

		char centroid[N];
		for(j = 0; j < N; j++) {
			centroid[j] = (char)(((point*)config.centers[i])->x[j] > 0.5);		// convert to 1bpp 
		}

		int nbytes = (int)fwrite(centroid, sizeof(char), N, fp);

		if(nbytes != N) {
			printf("Error while writng the centroid file.\n");
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);

	fp = fopen("classification.bin", "wb");
	if(fp == NULL) {
		printf("Error opening the classification file.\n");
		return 1;
	}
	for(i = 0; i < config.num_objs; i++) {
		char t = (char)config.clusters[i];
		fwrite(&t, sizeof(char), 1, fp);
	}
	fclose(fp);


	free(config.objs);
	free(config.clusters);
	free(config.centers);

	free(init);
	free(pts);

}

