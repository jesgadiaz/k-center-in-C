/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Gon, Gon+, HS, CDS, CDSh, and CDSh+
 * for the small instances from TSP-Lib
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

bool previouslyComputed(float r, int counter, float mid_value[]){
    for(int i=0;i<counter;i++){
        if(r == mid_value[i]){
            return true;
        }
    }
    return false;
}

bool previouslyComputedState(float r, int counter, float mid_value[], bool bool_mid_value[]){
    for(int i=0;i<counter;i++){
        if(r == mid_value[i]){
            return bool_mid_value[i];
        }
    }
    return false;
}

const char* str1;
const char* algorithm;

int main(int argc, char** argv) {
    algorithm = argv[1];
    int seed = atoi(argv[3]);
    int n, m, k;
    double total_time = 0;
    static float matrix[657][657];
    static float costs[431649];
    float vertices_x_y[657][2];
    int last_zero, last_value;
    // To store mid_values
    static float mid_value[30];
    static bool bool_mid_value[30];
    double exec_time[40];
    printf("40 'small' instances from TSP-Lib \n");
    printf("Experimental approximation factor \n");
    printf("*************************************************** \n");
    for(int instance=0;instance<40;instance++){
        // LOAD INSTANCE
        str1 = argv[2];
        char* str2;
        switch(instance % 4){
            case 0: k=5;break;
            case 1: k=10;break;
            case 2: k=20;break;
            case 3: k=40;break;
        }
        if(instance==0){
            //printf("kroA200 with k in [5,10,20,40] \n");
            str2 = "\\kroA200";
        }
        if(instance==4){
            //printf("gr202 with k in [5,10,20,40] \n");
            str2 = "\\gr202";
        }
        if(instance==8){
            //printf("pr226 with k in [5,10,20,40] \n");
            str2 = "\\pr226";
        }
        if(instance==12){
            //printf("pr264 with k in [5,10,20,40] \n");
            str2 = "\\pr264";
        }
        if(instance==16){
            //printf("pr299 with k in [5,10,20,40] \n");
            str2 = "\\pr299";
        }
        if(instance==20){
            //printf("lin318 with k in [5,10,20,40] \n");
            str2 = "\\lin318";
        }
        if(instance==24){
            //printf("pr439 with k in [5,10,20,40] \n");
            str2 = "\\pr439";
        }
        if(instance==28){
            //printf("pcb442 with k in [5,10,20,40] \n");
            str2 = "\\pcb442";
        }
        if(instance==32){
            //printf("d493 with k in [5,10,20,40] \n");
            str2 = "\\d493";
        }
        if(instance==36){
            //printf("d657 with k in [5,10,20,40] \n");
            str2 = "\\d657";
        }
        char* str3 = ".tsp";
        int sz1  = strlen(str1 );
        int sz2  = strlen(str2);
        int sz3  = strlen(str3);
        char *path = (char*)malloc(sz1+sz2+sz3+1);
        memcpy( path             , str1  , sz1 );
        memcpy( path+sz1         , str2 , sz2 );
        memcpy( path+sz1+sz2     , str3 , sz3 );
        path[sz1+sz2+sz3] = '\0';
        int i = 0;
        float cost;
        FILE* file = fopen(path, "r");
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
                for(int i=0;i<431649;i++){
                    costs[i] = 0;
                }
                int p = 0;
                for(int i=0;i<n;i++){
                    for(int j=0;j<n;j++){
                        costs[p] = matrix[i][j];
                        p++;
                    }
                }
                last_value = 431649;
                qsort(costs, sizeof(costs)/sizeof(float), sizeof(float), floatcomp);
                last_zero = 0;
                for(int i=0;i<431649;i++){
                    if(costs[i]!=0){
                        last_zero = i - 1;
                        break;
                    }
                }
                for(int i=0;i<30;i++){
                    mid_value[i] = -1;
                    bool_mid_value[i] = false;
                }
            }
        }

        clock_t begin = clock();

        int C[k];

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
            //fflush(stdout);
            printf("%f \n", max_dist);
        }

        // GON+
        if(strcmp(algorithm, "Gon+") == 0){
            float best_size = +INFINITY;
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
            }
            printf("%f \n", best_size);
        }

        // HS
        if(strcmp(algorithm, "HS") == 0){
            int high = last_value;
            int low = last_zero;
            int mid;
            srand(seed);
            int counter = 0;
            float distance[n];
            float best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                if(previouslyComputed(r, counter, mid_value)){
                    bool temp = previouslyComputedState(r, counter, mid_value, bool_mid_value);
                    if(temp){
                        high = mid;
                    }else{
                        low = mid;
                    }
                }else{
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
            }
            //free(ptr_costs);
            printf("%f \n", best_size);
        }

        // HS+
        if(strcmp(algorithm, "HS+") == 0){
            int high = last_value;
            int low = last_zero;
            int mid;
            srand(seed);
            float distance[n];
            int counter = 0;
            float best_size = +INFINITY;
            float best_of_n;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                best_of_n = +INFINITY;
                if(previouslyComputed(r, counter, mid_value)){
                    bool temp = previouslyComputedState(r, counter, mid_value, bool_mid_value);
                    if(temp){
                        high = mid;
                    }else{
                        low = mid;
                    }
                }else{
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
            int previous_value = -1;
            float best_size = +INFINITY;
            for(int p=last_zero;p<431649;p++){
                if(costs[p] != previous_value){
                    previous_value = costs[p];
                                        //if(p>0){
                    //    if(costs[p] != costs[p-1]){
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
                        //}
                    //}
                }
            }
            //free(costs);
            //ptr_costs = NULL;
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
            int counter = 0;
            float best_size = +INFINITY;
            int high = last_value;
            int low = last_zero;
            int mid;
            best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                if(previouslyComputed(r, counter, mid_value)){
                    bool temp = previouslyComputedState(r, counter, mid_value, bool_mid_value);
                    if(temp){
                        high = mid;
                    }else{
                        low = mid;
                    }
                }else{
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
            }

            //free(costs);
            //ptr_costs = NULL;
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
            int counter = 0;
            int high = last_value;
            int low = last_zero;
            int mid;
            float overall_best_size = +INFINITY;
            while(high - low > 1){
                mid = (high + low) / 2;
                float r = costs[mid];
                if(previouslyComputed(r, counter, mid_value)){
                    bool temp = previouslyComputedState(r, counter, mid_value, bool_mid_value);
                    if(temp){
                        high = mid;
                    }else{
                        low = mid;
                    }
                }else{
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
            }
            printf("%f \n", overall_best_size);
        }

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        total_time = total_time + time_spent;
        exec_time[instance] = time_spent;
    }
    printf("TIME SPENT PER INSTANCE \n");
    for(int i=0;i<40;i++){
        printf("%f \n", exec_time[i]);
    }
    printf("TOTAL TIME: %f \n", total_time);
    printf("AVERAGE TIME: %f", total_time / 40);
    return (EXIT_SUCCESS);
}

