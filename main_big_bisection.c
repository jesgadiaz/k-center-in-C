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
    //int ls = atoi(argv[4]);  // fourth argument. 0 for not local search. 1 for applying local search.
    int seed = atoi(argv[5]);
    int n, m, k;
    n = atoi(argv[3]);
    k = atoi(argv[4]);
    double total_time = 0;
    double exec_time;
    float vertices_x_y[71009][2];
    int last_zero;
    str1 = argv[2];
    float higher_cost;
    float lower_cost;
    int len = strlen(str1);
    printf("solution size: ");
            int i = 0;
           float cost;
           FILE* file = fopen(str1, "r");
           char line[100];
           if (file == NULL){
               printf("FILE NOT FOUND");
           }else{
               int v;
               float x, y;
               while(fgets(line, sizeof(line), file)) {
                   int j = 0;
                   char* token = strtok(line, " ");
                   while (token) {
                       if(j == 0){
                           v = atoi(token);
                       }
                       if(j == 1){
                           x = atof(token);
                       }
                       if(j == 2){
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
               /*
               for(int i=0;i<n;i++){
                   for(int j=0;j<n;j++){
                       matrix[i][j] = sqrt( pow(vertices_x_y[i][0] - vertices_x_y[j][0],2)
                                          + pow(vertices_x_y[i][1] - vertices_x_y[j][1],2) );
                   }
               }*/
               if(strcmp(algorithm, "HS") == 0 || strcmp(algorithm, "CDSh") == 0){
                   float dist;
                   higher_cost = 0;
                   lower_cost = +INFINITY;

                   for(int i=0;i<n;i++){
                        for(int j=i;j<n;j++){
                            dist = pow(vertices_x_y[i][0] - vertices_x_y[j][0],2) + pow(vertices_x_y[i][1] - vertices_x_y[j][1],2);
                            if(dist < lower_cost & lower_cost != 0){
                                lower_cost = dist;
                            }
                            if(dist > higher_cost){
                                higher_cost = dist;
                            }
                        }
                   }
                   lower_cost = sqrt(lower_cost);
                   higher_cost = sqrt(higher_cost);
               }
           }


        clock_t begin = clock();

        int C[k];
        float best_size;


        // GON
        if(strcmp(algorithm, "Gon") == 0){
            srand(seed);
            C[0] = rand() % n;
            best_size = +INFINITY;
            float distance[n];
            float max_dist;
            float dist_j_C;
            float temp1, temp2;
            for(int i=0;i<n;i++){
                distance[i] = +INFINITY;
            }
            for(int i=1;i<k;i++){
                // UPDATE DISTANCE
                for(int j=0;j<n;j++){
                    //dist_j_C = sqrt( pow(vertices_x_y[j][0] - vertices_x_y[C[i-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[i-1]][1],2) );
                    //dist_j_C = pow(vertices_x_y[j][0] - vertices_x_y[C[i-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[i-1]][1],2);
                    temp1 = vertices_x_y[j][0] - vertices_x_y[C[i-1]][0];
                    temp2 = vertices_x_y[j][1] - vertices_x_y[C[i-1]][1];
                    dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                    if(dist_j_C < distance[j]){
                        distance[j] = dist_j_C;
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
                //dist_j_C = pow(vertices_x_y[i][0] - vertices_x_y[C[k-1]][0],2) + pow(vertices_x_y[i][1] - vertices_x_y[C[k-1]][1],2);
                temp1 = vertices_x_y[i][0] - vertices_x_y[C[k-1]][0];
                temp2 = vertices_x_y[i][1] - vertices_x_y[C[k-1]][1];
                dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                if(dist_j_C < distance[i]){
                    distance[i] = dist_j_C;
                }
            }
            max_dist = 0;
            for(int i=0;i<n;i++){
                if(distance[i] > max_dist){
                    max_dist = distance[i];
                }
            }
            best_size = sqrt(max_dist);
            printf("%f \n", best_size);
        }

        // GON+
        if(strcmp(algorithm, "Gon+") == 0){
            best_size = +INFINITY;
            float worst_size = 0;
            float distance[n];
            float dist_j_C;
            float temp1, temp2;
            float max_dist;
            for(int p=0;p<n;p++){
                C[0] = p;
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                }
                for(int i=1;i<k;i++){
                    // UPDATE DISTANCE
                    for(int j=0;j<n;j++){
                        temp1 = vertices_x_y[j][0] - vertices_x_y[C[i-1]][0];
                        temp2 = vertices_x_y[j][1] - vertices_x_y[C[i-1]][1];
                        dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                        if(dist_j_C < distance[j]){
                            distance[j] = dist_j_C;
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
                    temp1 = vertices_x_y[i][0] - vertices_x_y[C[k-1]][0];
                    temp2 = vertices_x_y[i][1] - vertices_x_y[C[k-1]][1];
                    dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                    if(dist_j_C < distance[i]){
                        distance[i] = dist_j_C;
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
                //printf("%d \n", p);
                //if(worst_size < max_dist){
                //    worst_size = max_dist;
                //}
            }
            printf("%f \n", sqrt(best_size));
            //printf("W %f \n", worst_size);
        }


        // HS
        if(strcmp(algorithm, "HS") == 0){
            float high = higher_cost;
            float low = lower_cost;
            float mid;
            float dist_j_C;
            srand(seed);
            float temp1, temp2;
            float distance[n];
            best_size = +INFINITY;
            int iterations = 2*(log(n) / log(2));
            //printf("%d \n", max_iter);
            //while(high - low > 1){
            for(int iter=0;iter<iterations;iter++){
                mid = (high + low) / 2.0;
                float r = mid;
                float r2 = r * r;
                C[0] = rand() % n;
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                }
                for(int i=1;i<k;i++){
                    // UPDATE DISTANCE
                    for(int j=0;j<n;j++){
                        //dist_j_C = pow(vertices_x_y[j][0] - vertices_x_y[C[i-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[i-1]][1],2);
                        temp1 = vertices_x_y[j][0] - vertices_x_y[C[i-1]][0];
                        temp2 = vertices_x_y[j][1] - vertices_x_y[C[i-1]][1];
                        dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                        if(dist_j_C < distance[j]){
                            distance[j] = dist_j_C;
                        }
                    }
                    // SEARCH FOR NEXT VERTEX (2r)
                    int center = -1;
                    for(int j=0;j<n;j++){
                        if(distance[j] > 2*r2){
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
                    //dist_j_C = pow(vertices_x_y[j][0] - vertices_x_y[C[k-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[k-1]][1],2);
                    temp1 = vertices_x_y[j][0] - vertices_x_y[C[k-1]][0];
                    temp2 = vertices_x_y[j][1] - vertices_x_y[C[k-1]][1];
                    dist_j_C = (temp1 * temp1) + (temp2 * temp2);
                    if(dist_j_C < distance[j]){
                        distance[j] = dist_j_C;
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
                if(max_dist <= 2 * r2){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", sqrt(best_size));
        }

        // CDSh
        if(strcmp(algorithm, "CDSh") == 0){
            srand(seed);
            int farthest_vertex;
            float max_dist;
            float dist;
            int S[n];
            float distance[n];
            bool dominated[n];
            int score[n];
            best_size = +INFINITY;
            float high = higher_cost;
            float low = lower_cost;
            float mid;
            float temp1, temp2;
            best_size = +INFINITY;
            int iterations = 2*(log(n) / log(2));
            //printf("%d \n", max_iter);
            //while(high - low > 1){
            for(int iter=0;iter<iterations;iter++){
            //while(high - low > 1){
                mid = (high + low) / 2;
                float r = mid;
                float r2 = r * r;
                // INITIALIZATION
                for(int i=0;i<n;i++){
                    distance[i] = +INFINITY;
                    dominated[i] = false;
                    score[i] = 0;
                }
                for(int i=0;i<n;i++){
                    for(int j=0;j<n;j++){
                        //dist = pow(vertices_x_y[j][0] - vertices_x_y[i][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[i][1],2);
                        temp1 = vertices_x_y[j][0] - vertices_x_y[i][0];
                        temp2 = vertices_x_y[j][1] - vertices_x_y[i][1];
                        dist = (temp1 * temp1) + (temp2 * temp2);
                        if(i!=j & dist <= r2){
                            score[i] = score[i]+ 1;
                        }
                    }
                }
                for(int i=0;i<k;i++){
                    if(i>0){
                        // UPDATE DISTANCE
                        for(int j=0;j<n;j++){
                            //dist = pow(vertices_x_y[j][0] - vertices_x_y[C[i-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[i-1]][1],2);
                            temp1 = vertices_x_y[j][0] - vertices_x_y[C[i-1]][0];
                            temp2 = vertices_x_y[j][1] - vertices_x_y[C[i-1]][1];
                            dist = (temp1 * temp1) + (temp2 * temp2);
                            if(dist < distance[j]){
                                distance[j] = dist;
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
                        //dist = pow(vertices_x_y[j][0] - vertices_x_y[farthest_vertex][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[farthest_vertex][1],2);
                        temp1 = vertices_x_y[j][0] - vertices_x_y[farthest_vertex][0];
                        temp2 = vertices_x_y[j][1] - vertices_x_y[farthest_vertex][1];
                        dist = (temp1 * temp1) + (temp2 * temp2);
                        if(dist <= r){
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
                        //dist = pow(vertices_x_y[j][0] - vertices_x_y[C[i]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[i]][1],2);
                        temp1 = vertices_x_y[j][0] - vertices_x_y[C[i]][0];
                        temp2 = vertices_x_y[j][1] - vertices_x_y[C[i]][1];
                        dist = (temp1 * temp1) + (temp2 * temp2);
                        if(dist <= r2 && dominated[j] == false){
                            dominated[j] = true;
                            S[s_size] = j;
                            s_size++;
                        }
                    }
                    for(int j=0;j<s_size;j++){
                        for(int b=0;b<n;b++){
                            //dist = pow(vertices_x_y[S[j]][0] - vertices_x_y[b][0],2) + pow(vertices_x_y[S[j]][1] - vertices_x_y[b][1],2);
                            temp1 = vertices_x_y[S[j]][0] - vertices_x_y[b][0];
                            temp2 = vertices_x_y[S[j]][1] - vertices_x_y[b][1];
                            dist = (temp1 * temp1) + (temp2 * temp2);
                            if(dist <= r2
                                    && S[j] != b){
                                score[b] = score[b] - 1;
                            }
                        }
                    }
                }
                // RE-UPDATE DISTANCE
                for(int j=0;j<n;j++){
                    //dist = pow(vertices_x_y[j][0] - vertices_x_y[C[k-1]][0],2) + pow(vertices_x_y[j][1] - vertices_x_y[C[k-1]][1],2);
                    temp1 = vertices_x_y[j][0] - vertices_x_y[C[k-1]][0];
                    temp2 = vertices_x_y[j][1] - vertices_x_y[C[k-1]][1];
                    dist = (temp1 * temp1) + (temp2 * temp2);
                    if(dist < distance[j]){
                        distance[j] = dist;
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
                if(best_size <= r2){
                    high = mid;
                }else{
                    low = mid;
                }
            }
            printf("%f \n", sqrt(best_size));
        }

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        exec_time = time_spent;

    printf("execution time: ");

    printf("%f \n", exec_time);


    return (EXIT_SUCCESS);
}
