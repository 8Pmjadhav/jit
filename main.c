#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_ROUNDS 200

void store_opponent_inputs(int current_round,char* prev_response){
    FILE *fptr;
    if(current_round == 1){
        // deleting data of previous opponent
        fptr = fopen("./helpers/prev.txt", "w");
        fclose(fptr);
    }
    fptr = fopen("./helpers/prev.txt", "a"); // Append mode

    if (fptr == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    fprintf(fptr, "%s\n", prev_response);
    fclose(fptr);

}

void check_for_conditions(int current_round,char* prev_response){
    FILE *fptr;
    
    fptr = fopen("./helpers/prev.txt", "r");

    if (fptr == NULL) {
        fprintf(stderr, "Error opening file for reading.\n");
        exit(1);
    }
    char** content = (char**)malloc(200*sizeof(char*));
    char* resp = (char*)malloc(5*sizeof(char));
    int i=0;
    while (fgets(resp,5,fptr))
    {
        // printf("check %s |",resp);
        if(resp[0]=='Y'){
            content[i] = "YES\0";
        }
        else if(strcmp(resp,"NONE")== 0){
            content[i] = "NONE\0";
        }
        else{
            content[i] = "NO\0";
        }
        
        // printf("%d %s ||",(int)strlen(content[i]),content[i]);
        i++;
    }
    fclose(fptr);

    if(strcmp(prev_response,"NONE")==0){
        // printf("NONE ");
        printf("NO\n");
        return;
    }
    int count_yes=0,count_no=0,alternates=0;
    for(int j=0;j<i;j++){
        if(j>1 && strcmp(content[j],content[j-1]) != 0){
            alternates++;
        }
        if(strcmp(content[j],"YES")==0){
            count_yes++;
        }
        else if(strcmp(content[j],"NO")==0){
            count_no++;
        }
    }
    // printf("%d %d %d",i,alternates,(3*i)/4);
    if(alternates > (3*i)/4){     
        // printf("alternatives ");      // greater than 75%
        if(strcmp(prev_response,"YES")==0){
            printf("NO\n");     // NO-NO
        }
        else{
            printf("YES\n");    // YES-YES
        }
    }
    else if(count_no > count_yes){
        // printf("n>y ");
        printf("NO\n");
    }
    else{
        // printf("n<y ");
        printf("YES\n");
    }
    
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s CURRENT_ROUND_NUM OPPONENT_PLAYER_PREVIOUS_ROUND_INPUT\n", argv[0]);
        return 1;
    }
    
    int current_round = atoi(argv[1]);
    if(current_round > MAX_ROUNDS){
        exit(0);
    }
    char* prev_response = argv[2];
    store_opponent_inputs(current_round,prev_response);
    check_for_conditions(current_round,prev_response);
    
    // printf("\n%d %s\n",current_round,prev_response);
    return 0;
}