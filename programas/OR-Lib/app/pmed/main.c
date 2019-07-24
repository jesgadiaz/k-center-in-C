/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Gon, Gon+, HS, CDS, CDSh, and CDSh+
 * for the pmed instances from OR-Lib
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
int get_n(int str){
    if(str == 1){
        return 100;
    }
    if(str == 2){
        return 100;
    }
    if(str == 3){
        return 100;
    }
    if(str == 4){
        return 100;
    }
    if(str == 5){
        return 100;
    }
    if(str == 6){
        return 200;
    }
    if(str == 7){
        return 200;
    }
    if(str == 8){
        return 200;
    }
    if(str == 9){
        return 200;
    }
    if(str == 10){
        return 200;
    }
    if(str == 11){
        return 300;
    }
    if(str == 12){
        return 300;
    }
    if(str == 13){
        return 300;
    }
    if(str == 14){
        return 300;
    }
    if(str == 15){
        return 300;
    }
    if(str == 16){
        return 400;
    }
    if(str == 17){
        return 400;
    }
    if(str == 18){
        return 400;
    }
    if(str == 19){
        return 400;
    }
    if(str == 20){
        return 400;
    }
    if(str == 21){
        return 500;
    }
    if(str == 22){
        return 500;
    }
    if(str == 23){
        return 500;
    }
    if(str == 24){
        return 500;
    }
    if(str == 25){
        return 500;
    }
    if(str == 26){
        return 600;
    }
    if(str == 27){
        return 600;
    }
    if(str == 28){
        return 600;
    }
    if(str == 29){
        return 600;
    }
    if(str == 30){
        return 600;
    }
    if(str == 31){
        return 700;
    }
    if(str == 32){
        return 700;
    }
    if(str == 33){
        return 700;
    }
    if(str == 34){
        return 700;
    }
    if(str == 35){
        return 800;
    }
    if(str == 36){
        return 800;
    }
    if(str == 37){
        return 800;
    }
    if(str == 38){
        return 900;
    }
    if(str == 39){
        return 900;
    }
    if(str == 40){
        return 900;
    }
    return 0;
}

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
    static float matrix[900][900];
    static float costs[810000];
    double exec_time[40];
    // To store mid_values
    static float mid_value[30];
    static bool bool_mid_value[30];
    int last_zero, last_value;
    printf("40 pmed instances from OR-Lib \n");
    printf("Experimental approximation factor \n");
    for(int instance=1;instance<41;instance++){
        // LOAD INSTANCE
        str1 = argv[2];
        char* str2 = "//pmed";
        int length = snprintf( NULL, 0, "%d", instance);
        char* str3 = malloc( length + 1 );
        snprintf( str3, length + 1, "%d", instance );
        char* str4 = ".txt";
        int sz1  = strlen(str1 );
        int sz2  = strlen(str2);
        int sz3  = strlen(str3);
        int sz4  = strlen(str4);
        char *path = (char*)malloc(sz1+sz2+sz3+sz4+1);
        memcpy( path             , str1  , sz1 );
        memcpy( path+sz1         , str2 , sz2 );
        memcpy( path+sz1+sz2     , str3 , sz3 );
        memcpy( path+sz1+sz2+sz3 , str4 , sz4 );
        path[sz1+sz2+sz3+sz4] = '\0';
        int i = 0;
        float cost;
        n = get_n(instance);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                matrix[i][j] = +INFINITY;
            }
        }
        FILE* file = fopen(path, "r");
        char line[20];
        if (file == NULL){
            printf("FILE NOT FOUND");
        }else{
            while(fgets(line, sizeof(line), file)) {
                int j = 0;
                int v1, v2, cost;
                char* token = strtok(line, " ");
                while (token) {
                    if(i == 0 && j == 0){
                        n = atoi(token);
                    }
                    if(i == 0 && j == 1){
                        m = atoi(token);
                    }
                    if(i == 0 && j == 2){
                        k = atoi(token);
                    }
                    if(i > 0 && j == 0){
                        v1 = atoi(token);
                    }
                    if(i > 0 && j == 1){
                        v2 = atoi(token);
                    }
                    if(i > 0 && j == 2){
                        cost = atoi(token);
                    }
                    if(i > 0 && j == 3){
                        matrix[v1-1][v2-1] = cost;
                        matrix[v2-1][v1-1] = cost;
                    }
                    token = strtok(NULL, " ");
                    j++;
                }
                free(token);
                i++;
            }
            fclose(file);
            // FLOYD-WARSHALL
            for(int i=0;i<n;i++){
                matrix[i][i] = 0;
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    for(int p=0;p<n;p++){
                        if(matrix[i][j] == +INFINITY || matrix[i][p] == +INFINITY){
                            cost = +INFINITY;
                        }else{
                            cost = matrix[i][j] + matrix[i][p];
                        }
                        if(cost < matrix[j][p]){
                            matrix[j][p] = cost;
                        }
                    }
                }
            }


            if(strcmp(algorithm, "HS") == 0 || strcmp(algorithm, "CDS") == 0
                    || strcmp(algorithm, "CDSh") == 0 || strcmp(algorithm, "CDSh+") == 0
                    || strcmp(algorithm, "HS+") == 0){
                for(int i=0;i<810000;i++){
                    costs[i] = 0;
                }
                int p = 0;
                for(int i=0;i<n;i++){
                    for(int j=0;j<n;j++){
                        costs[p] = matrix[i][j];
                        p++;
                    }
                }
                last_value = 810000;
                qsort(costs, sizeof(costs)/sizeof(float), sizeof(float), floatcomp);
                last_zero = 0;
                for(int i=0;i<810000;i++){
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
                if(worst_size < max_dist){
                    worst_size = max_dist;
                }
            }
            printf("%f \n", best_size);
            //printf("WS = %f \n", worst_size);
        }

        // HS
        if(strcmp(algorithm, "HS") == 0){
            int high = last_value;
            int low = last_zero;
            int mid;
            srand(seed);
            float distance[n];
            float best_size = +INFINITY;
            int counter = 0;
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
                        mid_value[counter] = r;
                        bool_mid_value[counter] = true;
                    }else{
                        low = mid;
                        mid_value[counter] = r;
                        bool_mid_value[counter] = false;
                    }
                    counter++;
                }
            }
            printf("%f \n", best_size);
        }

        // HS+
        if(strcmp(algorithm, "HS+") == 0){
            int high = last_value;
            int low = last_zero;
            int mid;
            srand(seed);
            float distance[n];
            float best_size = +INFINITY;
            float best_of_n;
            int counter = 0;
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
                        mid_value[counter] = r;
                        bool_mid_value[counter] = true;
                    }else{
                        low = mid;
                        mid_value[counter] = r;
                        bool_mid_value[counter] = false;
                    }
                    counter++;
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
            for(int p=last_zero;p<810000;p++){
                if(costs[p] != previous_value){
                        previous_value = costs[p];
                     //if(p>0){
                        //if(costs[p] != costs[p-1]){
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
            float best_size = +INFINITY;
            int high = last_value;
            int low = last_zero;
            int counter = 0;
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
                        mid_value[counter] = r;
                        bool_mid_value[counter] = true;
                    }else{
                        low = mid;
                        mid_value[counter] = r;
                        bool_mid_value[counter] = false;
                    }
                    counter++;
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
            int high = 810000;
            int low = last_zero;
            int counter = 0;
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
                        mid_value[counter] = r;
                        bool_mid_value[counter] = true;
                    }else{
                        low = mid;
                        mid_value[counter] = r;
                        bool_mid_value[counter] = true;
                    }
                    counter++;
                }
            }
            printf("%f \n", overall_best_size);
        }

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        total_time = total_time + time_spent;
        exec_time[instance-1] = time_spent;
    }
    printf("TIME SPENT PER INSTANCE \n");
    for(int i=0;i<40;i++){
        printf("%f \n", exec_time[i]);
    }
    printf("TOTAL TIME: %f \n", total_time);
    printf("AVERAGE TIME: %f", total_time / 40);
    return (EXIT_SUCCESS);
}
