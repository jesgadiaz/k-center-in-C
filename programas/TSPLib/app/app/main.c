/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Gon, Gon+, HS, CDS, CDSh, and CDSh+
 * for the medium instances from TSP-Lib (u1060, u1817, pcb3038)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

/*
 *
 */
int floatcomp(const void* elem1, const void* elem2)
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

const char* str1;
const char* algorithm;

int main(int argc, char** argv) {
    algorithm = argv[1];
    int ls = atoi(argv[4]);  // fourth argument. 0 for not local search. 1 for applying local search.
    int seed = atoi(argv[3]);
    int n, m, k;
    double total_time = 0;
    double exec_time[15];
    static float matrix[4663][4663];
    static float costs[21743569];
    float vertices_x_y[4663][2];
    int last_zero;
    str1 = argv[2];
    int len = strlen(str1);
    const char *last_chars = &str1[len-11];
    printf("Experimental approximation factor \n");
    int max_iter;
    max_iter = 15;
    for(int instance=0;instance<max_iter;instance++){
        // LOAD INSTANCE
        k = (instance + 1)*10;
        //k = (instance + 1)*10;
        //k = 40;
        if(instance==0){
            int i = 0;
           float cost;
           FILE* file = fopen(str1, "r");
           char line[50];
           if (file == NULL){
               printf("FILE NOT FOUND");
           }else{
               int v;
               float x, y;
               while(fgets(line, sizeof(line), file)) {
                   int j = 0;
                   char* token = strtok(line, " ");
                   while (token) {
                       if(i == 0 && j == 0){
                           n = atoi(token);
                       }
                       if(i > 0 && j == 0){
                           v = atoi(token);
                       }
                       if(i > 0 && j == 1){
                           x = atof(token);
                       }
                       if(i > 0 && j == 2){
                           y = atof(token);
                           vertices_x_y[v-1][0] = x;
                           vertices_x_y[v-1][1] = y;
                       }
                       token = strtok(NULL, " ");
                       j++;
                   }
                   free(token);
                   i++;
               }
               fclose(file);
               for(int i=0;i<n;i++){
                   for(int j=0;j<n;j++){
                       matrix[i][j] = sqrt( pow(vertices_x_y[i][0] - vertices_x_y[j][0],2)
                                          + pow(vertices_x_y[i][1] - vertices_x_y[j][1],2) );
                   }
               }
               if(strcmp(algorithm, "HS") == 0 || strcmp(algorithm, "CDS") == 0
                    || strcmp(algorithm, "CDSh") == 0 || strcmp(algorithm, "CDSh+") == 0
                    || strcmp(algorithm, "HS+") == 0){
                   for(int i=0;i<sizeof(costs)/sizeof(float);i++){
                       costs[i] = 0;
                   }
                   int p = 0;
                   for(int i=0;i<n;i++){
                       for(int j=0;j<n;j++){
                           costs[p] = matrix[i][j];
                           p++;
                       }
                   }
                   qsort(costs, sizeof(costs)/sizeof(float), sizeof(float), floatcomp);
                   last_zero = 0;
                   for(int i=0;i<sizeof(costs)/sizeof(float);i++){
                       if(costs[i]!=0){
                           last_zero = i - 1;
                           break;
                       }
                   }
               }
           }
        }

        clock_t begin = clock();

        int C[k];
        float best_size;

        // Sh
        if(strcmp(algorithm, "Sh") == 0){
            srand(seed);
            C[0] = rand() % n;
            // r = r*
            float r;
            float distance[n];
            float max_dist;
            for(int i=0;i<n;i++){
                distance[i] = +INFINITY;
            }
            for(int i=1;i<k;i++){
                // UPDATE DISTANCE
                for(int j=0;j<n;j++){
                    if(matrix[j][C[i-1]] < distance[j]){
                        distance[j] = matrix[j][C[i-1]];
                    }
                }
                // SEARCH FOR A SUFFICIENTLY FAR VERTEX
                int sufficiently_far_vertex = -1;
                for(int j=0;j<n;j++){
                    if(distance[j] > 2 * r){
                        sufficiently_far_vertex = j;
                        break;
                    }
                }
                C[i] = sufficiently_far_vertex;
            }
            // UPDATE DISTANCE AGAIN
            for(int i=0;i<n;i++){
                if(matrix[i][C[k-1]] < distance[i]){
                    distance[i] = matrix[i][C[k-1]];
                }
            }
            max_dist = 0;
            for(int i=0;i<n;i++){
                if(distance[i] > max_dist){
                    max_dist = distance[i];
                }
            }
            best_size = max_dist;
            printf("%f \n", max_dist);
        }

        // GON
        if(strcmp(algorithm, "Gon") == 0){
            srand(seed);
            C[0] = rand() % n;
            float distance[n];
            float max_dist;
            for(int i=0;i<n;i++){
                distance[i] = +INFINITY;
            }
            for(int i=1;i<k;i++){
                // UPDATE DISTANCE
                for(int j=0;j<n;j++){
                    if(matrix[j][C[i-1]] < distance[j]){
                        distance[j] = matrix[j][C[i-1]];
                    }
                }
                // SEARCH FOR FARTHEST VERTEX
                float max_dist = 0;
                int farthest_vertex = -1;
                for(int j=0;j<n;j++){
                    if(distance[j] > max_dist){
                        max_dist = distance[j];
                        farthest_vertex = j;
                    }
                }
                C[i] = farthest_vertex;
            }
            // UPDATE DISTANCE AGAIN
            for(int i=0;i<n;i++){
                if(matrix[i][C[k-1]] < distance[i]){
                    distance[i] = matrix[i][C[k-1]];
                }
            }
            max_dist = 0;
            for(int i=0;i<n;i++){
                if(distance[i] > max_dist){
                    max_dist = distance[i];
                }
            }
            best_size = max_dist;
            printf("%f \n", max_dist);
        }

        // GON+
        if(strcmp(algorithm, "Gon+") == 0){
            best_size = +INFINITY;
            float worst_size = 0;
            float distance[n];
            float max_dist;
            for(int p=0;p<n;p++){
                C[0] = p;
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                }
                for(int i=1;i<k;i++){
                    // UPDATE DISTANCE
                    for(int j=0;j<n;j++){
                        if(matrix[j][C[i-1]] < distance[j]){
                            distance[j] = matrix[j][C[i-1]];
                        }
                    }
                    // SEARCH FOR FARTHEST VERTEX
                    float max_dist = 0;
                    int farthest_vertex = -1;
                    for(int j=0;j<n;j++){
                        if(distance[j] > max_dist){
                            max_dist = distance[j];
                            farthest_vertex = j;
                        }
                    }
                    C[i] = farthest_vertex;
                }
                // UPDATE DISTANCE AGAIN
                for(int i=0;i<n;i++){
                    if(matrix[i][C[k-1]] < distance[i]){
                        distance[i] = matrix[i][C[k-1]];
                    }
                }
                max_dist = 0;
                for(int i=0;i<n;i++){
                    if(distance[i] > max_dist){
                        max_dist = distance[i];
                    }
                }
                if(max_dist < best_size){
                    best_size = max_dist;
                }
                //if(worst_size < max_dist){
                //    worst_size = max_dist;
                //}
            }
            printf("%f \n", best_size);
            //printf("W %f \n", worst_size);
        }

        // HS
        if(strcmp(algorithm, "HS") == 0){
            int high = sizeof(costs)/sizeof(float);
            int low = last_zero;
            int mid;
            srand(seed);
            float distance[n];
            best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                C[0] = rand() % n;
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                }
                for(int i=1;i<k;i++){
                    // UPDATE DISTANCE
                    for(int j=0;j<n;j++){
                        if(matrix[j][C[i-1]] < distance[j]){
                            distance[j] = matrix[j][C[i-1]];
                        }
                    }
                    // SEARCH FOR NEXT VERTEX (2r)
                    int center = -1;
                    for(int j=0;j<n;j++){
                        if(distance[j] > 2*r){
                            center = j;
                        }
                    }
                    if(center != -1){
                        C[i] = center;
                    }else{
                        C[i] = rand() % n;
                    }
                }
                // RE-UPDATE DISTANCE
                float max_dist = 0;
                for(int j=0;j<n;j++){
                    if(matrix[j][C[k-1]] < distance[j]){
                        distance[j] = matrix[j][C[k-1]];
                    }
                }
                for(int j=0;j<n;j++){
                    if(distance[j] > max_dist){
                        max_dist = distance[j];
                    }
                }
                if(max_dist < best_size){
                    best_size = max_dist;
                }
                if(max_dist <= 2 * r){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", best_size);
        }

        // HS+
        if(strcmp(algorithm, "HS+") == 0){
            int high = sizeof(costs)/sizeof(float);
            int low = last_zero;
            int mid;
            srand(seed);
            float distance[n];
            best_size = +INFINITY;
            float best_of_n;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                best_of_n = +INFINITY;
                for(int iter=0;iter<n;iter++){
                    C[0] = iter;
                    for(int i=0;i<n;i++){
                        distance[i] = +INFINITY;
                    }
                    for(int i=1;i<k;i++){
                        // UPDATE DISTANCE
                        for(int j=0;j<n;j++){
                            if(matrix[j][C[i-1]] < distance[j]){
                                distance[j] = matrix[j][C[i-1]];
                            }
                        }
                        // SEARCH FOR NEXT VERTEX (2r)
                        int center = -1;
                        for(int j=0;j<n;j++){
                            if(distance[j] > 2*r){
                                center = j;
                            }
                        }
                        if(center != -1){
                            C[i] = center;
                        }else{
                            C[i] = rand() % n;
                        }
                    }
                    // RE-UPDATE DISTANCE
                    float max_dist = 0;
                    for(int j=0;j<n;j++){
                        if(matrix[j][C[k-1]] < distance[j]){
                            distance[j] = matrix[j][C[k-1]];
                        }
                    }
                    for(int j=0;j<n;j++){
                        if(distance[j] > max_dist){
                            max_dist = distance[j];
                        }
                    }
                    if(max_dist < best_of_n){
                        best_of_n = max_dist;
                    }
                }
                if(best_of_n < best_size){
                    best_size = best_of_n;
                }
                if(best_of_n <= 2 * r){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", best_size);
        }

        // CDS
        if(strcmp(algorithm, "CDS") == 0){
            srand(seed);
            int farthest_vertex;
            float max_dist;
            int S[n];
            float distance[n];
            bool dominated[n];
            int score[n];
            best_size = +INFINITY;
            for(int p=last_zero;p<9229444;p++){
                if(p>0){
                    if(costs[p] != costs[p-1]){
                        // INITIALIZATION
                        for(int i=0;i<n;i++){
                            distance[i] = +INFINITY;
                            dominated[i] = false;
                            score[i] = 0;
                        }
                        for(int i=0;i<n;i++){
                            for(int j=0;j<n;j++){
                                if(i!=j & matrix[i][j] <= costs[p]){
                                    score[i] = score[i]+ 1;
                                }
                            }
                        }
                        for(int i=0;i<k;i++){
                            if(i>0){
                                // UPDATE DISTANCE
                                for(int j=0;j<n;j++){
                                    if(matrix[j][C[i-1]] < distance[j]){
                                        distance[j] = matrix[j][C[i-1]];
                                    }
                                }
                                max_dist = 0;
                                for(int j=0;j<n;j++){
                                    if(distance[j] > max_dist){
                                        max_dist = distance[j];
                                        farthest_vertex = j;
                                    }
                                }
                            }else{
                                farthest_vertex = rand() % n;
                            }
                            // GET MAX SCORE CRITICAL NEIGHBOR
                            int max_score = -1;
                            int max_score_neighbor;
                            for(int j=0;j<n;j++){
                                if(matrix[j][farthest_vertex] <= costs[p]){
                                    if(score[j] > max_score){
                                        max_score = score[j];
                                        max_score_neighbor = j;
                                    }
                                }
                            }
                            C[i] = max_score_neighbor;
                            // UPDATE SCORE
                            int s_size = 0;
                            for(int j=0;j<n;j++){
                                if(matrix[j][C[i]] <= costs[p] && dominated[j] == false){
                                    dominated[j] = true;
                                    S[s_size] = j;
                                    s_size++;
                                }
                            }
                            for(int j=0;j<s_size;j++){
                                for(int b=0;b<n;b++){
                                    if(matrix[S[j]][b] <= costs[p]
                                            && S[j] != b){
                                        score[b] = score[b] - 1;
                                    }
                                }
                            }
                        }
                        // RE-UPDATE DISTANCE
                        for(int j=0;j<n;j++){
                            if(matrix[j][C[k-1]] < distance[j]){
                                distance[j] = matrix[j][C[k-1]];
                            }
                        }
                        // GET MAX DIST
                        max_dist = 0;
                        for(int j=0;j<n;j++){
                            if(distance[j] > max_dist){
                                max_dist = distance[j];
                            }
                        }
                        if(max_dist < best_size){
                            best_size = max_dist;
                        }
                    }
                }
            }
            printf("%f \n", best_size);
        }

        // CDSh
        if(strcmp(algorithm, "CDSh") == 0){
            srand(seed);
            int farthest_vertex;
            float max_dist;
            int S[n];
            float distance[n];
            bool dominated[n];
            int score[n];
            best_size = +INFINITY;
            int high = sizeof(costs)/sizeof(float);
            int low = last_zero;
            int mid;
            best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                // INITIALIZATION
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                    dominated[i] = false;
                    score[i] = 0;
                }
                for(int i=0;i<n;i++){
                    for(int j=0;j<n;j++){
                        if(i!=j & matrix[i][j] <= r){
                            score[i] = score[i]+ 1;
                        }
                    }
                }
                for(int i=0;i<k;i++){
                    if(i>0){
                        // UPDATE DISTANCE
                        for(int j=0;j<n;j++){
                            if(matrix[j][C[i-1]] < distance[j]){
                                distance[j] = matrix[j][C[i-1]];
                            }
                        }
                        max_dist = 0;
                        for(int j=0;j<n;j++){
                            if(distance[j] > max_dist){
                                max_dist = distance[j];
                                farthest_vertex = j;
                            }
                        }
                    }else{
                        farthest_vertex = rand() % n;
                    }
                    // GET MAX SCORE CRITICAL NEIGHBOR
                    int max_score = -1;
                    int max_score_neighbor;
                    for(int j=0;j<n;j++){
                        if(matrix[j][farthest_vertex] <= r){
                            if(score[j] > max_score){
                                max_score = score[j];
                                max_score_neighbor = j;
                            }
                        }
                    }
                    C[i] = max_score_neighbor;
                    // UPDATE SCORE
                    int s_size = 0;
                    for(int j=0;j<n;j++){
                        if(matrix[j][C[i]] <= r && dominated[j] == false){
                            dominated[j] = true;
                            S[s_size] = j;
                            s_size++;
                        }
                    }
                    for(int j=0;j<s_size;j++){
                        for(int b=0;b<n;b++){
                            if(matrix[S[j]][b] <= r
                                    && S[j] != b){
                                score[b] = score[b] - 1;
                            }
                        }
                    }
                }
                // RE-UPDATE DISTANCE
                for(int j=0;j<n;j++){
                    if(matrix[j][C[k-1]] < distance[j]){
                        distance[j] = matrix[j][C[k-1]];
                    }
                }
                // GET MAX DIST
                max_dist = 0;
                for(int j=0;j<n;j++){
                    if(distance[j] > max_dist){
                        max_dist = distance[j];
                    }
                }
                if(max_dist < best_size){
                    best_size = max_dist;
                }
                if(best_size <= r){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", best_size);
        }

        // CDSh+
        if(strcmp(algorithm, "CDSh+") == 0){
            srand(seed);
            int farthest_vertex;
            float max_dist;
            int S[n];
            float distance[n];
            bool dominated[n];
            int score[n];
            int high = sizeof(costs)/sizeof(float);
            int low = last_zero;
            int mid;
            float overall_best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                float best_size = +INFINITY;
                for(int d=0;d<n;d++){
                    // INITIALIZATION
                    for(int i=0;i<n;i++){
                        distance[i] = +INFINITY;
                        dominated[i] = false;
                        score[i] = 0;
                    }
                    for(int i=0;i<n;i++){
                        for(int j=0;j<n;j++){
                            if(i!=j & matrix[i][j] <= r){
                                score[i] = score[i]+ 1;
                            }
                        }
                    }
                    for(int i=0;i<k;i++){
                        if(i>0){
                            // UPDATE DISTANCE
                            for(int j=0;j<n;j++){
                                if(matrix[j][C[i-1]] < distance[j]){
                                    distance[j] = matrix[j][C[i-1]];
                                }
                            }
                            max_dist = 0;
                            for(int j=0;j<n;j++){
                                if(distance[j] > max_dist){
                                    max_dist = distance[j];
                                    farthest_vertex = j;
                                }
                            }
                        }else{
                            farthest_vertex = d;
                        }
                        // GET MAX SCORE CRITICAL NEIGHBOR
                        int max_score = -1;
                        int max_score_neighbor;
                        if(i > 0){
                            for(int j=0;j<n;j++){
                                if(matrix[j][farthest_vertex] <= r){
                                    if(score[j] > max_score){
                                        max_score = score[j];
                                        max_score_neighbor = j;
                                    }
                                }
                            }
                        }else{
                            max_score_neighbor = farthest_vertex;
                        }
                        C[i] = max_score_neighbor;
                        // UPDATE SCORE
                        int s_size = 0;
                        for(int j=0;j<n;j++){
                            if(matrix[j][C[i]] <= r && dominated[j] == false){
                                dominated[j] = true;
                                S[s_size] = j;
                                s_size++;
                            }
                        }
                        for(int j=0;j<s_size;j++){
                            for(int b=0;b<n;b++){
                                if(matrix[S[j]][b] <= r
                                        && S[j] != b){
                                    score[b] = score[b] - 1;
                                }
                            }
                        }
                    }
                    // RE-UPDATE DISTANCE
                    for(int j=0;j<n;j++){
                        if(matrix[j][C[k-1]] < distance[j]){
                            distance[j] = matrix[j][C[k-1]];
                        }
                    }
                    // GET MAX DIST
                    max_dist = 0;
                    for(int j=0;j<n;j++){
                        if(distance[j] > max_dist){
                            max_dist = distance[j];
                        }
                    }
                    if(max_dist < best_size){
                        best_size = max_dist;
                    }
                }
                if(best_size < overall_best_size){
                    overall_best_size = best_size;
                }
                if(overall_best_size <= r){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", overall_best_size);
        }

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        total_time = total_time + time_spent;
        exec_time[instance] = time_spent;

        /*
        for(int i=0;i<k;i++){
            printf("%d, ", C[i]);
        }

        printf("\n \n ");*/

        // Apply Local Search
        //printf("WF: %d \n", ls);
        if(ls == 1){

            float distance[n];
            bool dominated[n];
            int score[n];
            // get solution size (max_dist) and distance (O(kn))
            float max_dist = 0;
            float min_dist;
            // initialization (O(n))
            /*
            for(int i=0;i<n;i++){
                distance[i] = 0;
            }
            for(int i=0;i<n;i++){
                min_dist = +INFINITY;
                for(int j=0;j<k;j++){
                    if(matrix[i][C[j]] < min_dist){
                        min_dist = matrix[i][C[j]];
                    }
                }
                distance[i] = min_dist;
                if(max_dist < min_dist){
                    max_dist = min_dist;
                }
            }*/
            //for(int iter=0;iter<10000;iter++){
            clock_t begin = clock();
            clock_t end = clock();
            double time_spent = 0;
            int tries_count = 0;
            float best_size_local;
            float max_dist_local;

            while(time_spent < 90000){ //4200
                clock_t end = clock();
                time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                 // initialization (O(n))
                for(int i=0;i<n;i++){
                    distance[i] = 0;
                }
                max_dist = 0;
                for(int i=0;i<n;i++){
                    min_dist = +INFINITY;
                    for(int j=0;j<k;j++){
                        if(matrix[i][C[j]] < min_dist){
                            min_dist = matrix[i][C[j]];
                        }
                    }
                    distance[i] = min_dist;
                    if(max_dist < min_dist){
                        max_dist = min_dist;
                    }
                }
                tries_count = tries_count + 1;
                // get solution size
                //max_dist = 0;
                //for(int i=0;i<n;i++){
                //    if(max_dist < distance[i]){
                //        max_dist = distance[i];
                //    }
                //}
                if(max_dist < best_size){
                    best_size = max_dist;
                    tries_count = 0;
                    printf("improved: %f \n", best_size);
                }

                if(tries_count > 10000000000000000){ // approx. 10 minute
                    //mutation
                    //printf("*****************************************init \n");
                    for(int i=0;i<k;i++){
                        if(rand() % 100 < 5){
                            C[i] = rand() % n;
                        }
                    }
                    max_dist_local = 0;
                    for(int i=0;i<n;i++){
                        min_dist = +INFINITY;
                        for(int j=0;j<k;j++){
                            if(matrix[i][C[j]] < min_dist){
                                min_dist = matrix[i][C[j]];
                            }
                        }
                        distance[i] = min_dist;
                        if(max_dist_local < min_dist){
                            max_dist_local = min_dist;
                        }
                    }
                    best_size_local = max_dist_local;
                    tries_count = 0;
                    //printf("*****************************************end \n");
                }

                if(max_dist_local < best_size_local){
                    tries_count = 0;
                }

                //printf("LS: %f \n", max_dist);
                // get dominated vertices (O(n))
                //for(int i=0;i<n;i++){
                //    if(distance[i] < best_size){
                //        dominated[i] = true;
                //    }
                //}
                // score (O(n^2))
                //for(int i=0;i<n;i++){
                //    for(int j=0;j<n;j++){
                //        if(i!=j & matrix[i][j] < best_size){
                //            score[i] = score[i] + 1;
                //        }
                //    }
                //}
                // get critical vertex (O(n))
                int critical_vertex = 0;
                for(int i=0;i<n;i++){
                    if(distance[i] == max_dist){
                        critical_vertex = i;
                        break;
                    }
                }
                // get the nearest center to the critical vertex (O(k))
                int nearest_center = 0;
                int nearest_center_index = 0;
                for(int i=0;i<k;i++){
                    if(distance[critical_vertex] == matrix[critical_vertex][C[i]]){
                        nearest_center = C[i];
                        nearest_center_index = i;
                    }
                }
                // update score (O(n^2))
                //int S[n];
                //for(int i=0;i<k;i++){
                //    if(i != nearest_center_index){
                //        int s_size = 0;
                //        for(int j=0;j<n;j++){
                //            if(matrix[j][C[i]] < best_size && dominated[j] == false){
                //                dominated[j] = true;
                //                S[s_size] = j;
                //                s_size++;
                //            }
                //        }
                //        for(int j=0;j<s_size;j++){
                //            for(int b=0;b<n;b++){
                //                if(matrix[S[j]][b] < best_size
                //                        && S[j] != b){
                //                    score[b] = score[b] - 1;
                //                }
                //            }
                //        }
                //    }
                //}
                // get the neighbor of the critical vertex that is closest to the nearest center(O(n))

                // get the max score neighbor of the critical vertex (O(n))
                //int max_score = -1;
                //int max_score_neighbor;
                //for(int i=0;i<n;i++){
                //    if(matrix[i][critical_vertex] < best_size){
                //        if(score[i] > max_score){
                //            max_score = score[i];
                //            max_score_neighbor = i;
                //        }
                //    }
                //}
                // interchange nearest center with max score neighbor
                //C[nearest_center_index] = max_score_neighbor;



                if(rand() % 100 < 60){
                    min_dist = +INFINITY;
                    int min_dist_neighbor;
                    for(int i=0;i<n;i++){
                        if(matrix[i][critical_vertex] < best_size){
                            if(matrix[i][nearest_center] < min_dist){
                                min_dist = matrix[i][nearest_center];
                                min_dist_neighbor = i;
                            }
                        }
                    }
                    C[nearest_center_index] = min_dist_neighbor;
                }else{
                    int N[n];
                    int N_size = 0;
                    for(int i=0;i<n;i++){
                        float smaller;
                        if(best_size < 337){
                            smaller = best_size;
                        }else{
                            smaller = 337;
                        }
                        if(matrix[i][critical_vertex] < smaller){
                            N[N_size] = i;
                            N_size++;
                        }
                    }
                    C[nearest_center_index] = N[rand() % N_size];
                }

                // update distance
                //for(int i=0;i<n;i++){
                //    if(matrix[i][C[nearest_center_index]] < distance[i]){
                //        distance[i] = matrix[i][C[nearest_center_index]];
                //    }
                //}
            }
            printf("improved: %f \n", best_size);
        }
        printf("********************************************************************");
    }
    printf("TIME SPENT PER INSTANCE \n");
    for(int i=0;i<max_iter;i++){
        printf("%f \n", exec_time[i]);
    }
    printf("TOTAL TIME: %f \n", total_time);
    printf("AVERAGE TIME: %f", total_time / max_iter);
    return (EXIT_SUCCESS);
}
