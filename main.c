#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TABLE_SIZE 1009

typedef struct node{
    long long int voterid;
    struct node *next;
}node;

node*hash_table[TABLE_SIZE];

int hash(long long int id){
    return id % TABLE_SIZE;
}

int has_voted(long long int id){
    int index=hash(id);
    node *temp=hash_table[index];
    while(temp){
        if(temp->voterid==id)
         return 1;
        temp=temp->next;
    }
    return 0;
}

void insert_voter(long long int id){
    int index=hash(id);
    node *newnode=malloc(sizeof(node));
    newnode->voterid=id;
    newnode->next=hash_table[index];
    hash_table[index]=newnode;
}

typedef struct candidate{
    char name[50];
    char party[50];
    int age;
    long long int*votes;
    int top;
}candidate;

typedef struct voter{
    char name[50];
    int age;
    char address[100];
    long long int voterid;
    long long int aadhar;
    int voted;
    char voted_to_party[50];
}voter;

void push_vote(candidate*c,long long int voterid,int M) {
    if (c->top>=M-1) 
      return;
    c->votes[++c->top]=voterid;
}

int count_votes(candidate*c) {
    return c->top+1;
}

void record_vote(candidate*c,voter *v,int N,int M) {
    if (strcmp(v->voted_to_party,"None")==0)
     return;
    for (int i=0;i<N;i++) {
        if (c[i].age<25)
         continue;
        if(strcmp(c[i].party,v->voted_to_party)==0){
            push_vote(&c[i],v->voterid,M);
            break;
        }
    }
}

void display_results(candidate*candidates, int num_candidates) {
    printf("\nElection Results:\n");
    printf("%-20s %-15s %-5s %-5s\n", "Candidate", "Party", "Age", "Votes");
    for (int i=0;i<num_candidates;i++) {
        printf("%-20s %-15s %-5d %-5d\n",
               candidates[i].name,
               candidates[i].party,
               candidates[i].age,
               count_votes(&candidates[i]));
    }
    fflush(stdout);
}

int main() {
    setbuf(stdout, NULL);
    int num_candidates, max_voters;

    printf("Enter number of candidates:\n"); 
    fflush(stdout);
    scanf("%d", &num_candidates);
    getchar(); 

    printf("Enter maximum voters:\n");
     fflush(stdout);
    scanf("%d", &max_voters);
    getchar();

    candidate*candidates=malloc(num_candidates*sizeof(candidate));
    for (int i=0;i<num_candidates;i++) {
        printf("Candidate %d Name:\n", i + 1);
         fflush(stdout);
        fgets(candidates[i].name,sizeof(candidates[i].name),stdin);
        candidates[i].name[strcspn(candidates[i].name, "\n")]=0; 

        printf("Party:\n");
         fflush(stdout);
        fgets(candidates[i].party, sizeof(candidates[i].party), stdin);
        candidates[i].party[strcspn(candidates[i].party, "\n")]=0;

        printf("Age:\n"); 
        fflush(stdout);
        scanf("%d", &candidates[i].age);
        getchar();

        candidates[i].votes=malloc(max_voters*sizeof(long long int));
        candidates[i].top = -1;
    }

    for (int i = 0;i<TABLE_SIZE;i++)
         hash_table[i] = NULL;

    int choice;
    do {
        printf("\n1. Vote\n2. Show Results\n3. Exit\nEnter choice:\n"); fflush(stdout);
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            voter v;

            printf("Enter Name:\n");
             fflush(stdout);
            fgets(v.name, sizeof(v.name), stdin);
            v.name[strcspn(v.name, "\n")] = 0;

            printf("Enter Age:\n"); 
            fflush(stdout);
            scanf("%d", &v.age); 
            getchar();

            printf("Enter Address:\n");
             fflush(stdout);
            fgets(v.address, sizeof(v.address), stdin);
            v.address[strcspn(v.address, "\n")] = 0;

            printf("Enter Voter ID:\n");
             fflush(stdout);
            scanf("%lld", &v.voterid); 
            getchar();

            printf("Enter Aadhar:\n"); 
            fflush(stdout);
            scanf("%lld", &v.aadhar); 
            getchar();

            printf("Enter Party to Vote:\n"); 
            fflush(stdout);
            fgets(v.voted_to_party, sizeof(v.voted_to_party), stdin);
            v.voted_to_party[strcspn(v.voted_to_party, "\n")] = 0;

            if (v.age < 18){
                printf("Voter is not eligible (<18).\n");
                continue;
            }

            if (has_voted(v.voterid)){
                printf("This voter ID has already voted!\n");
                continue;
            }

            v.voted = 1;
            record_vote(candidates, &v, num_candidates, max_voters);
            insert_voter(v.voterid);
            printf("Vote recorded successfully!\n");
        } else if (choice == 2){
            display_results(candidates, num_candidates);
        }
    } while (choice != 3);

    for (int i = 0;i<num_candidates;i++)
     free(candidates[i].votes);
    free(candidates);

    for (int i=0;i<TABLE_SIZE;i++) {
        node*temp = hash_table[i];
        while(temp){
            node*del=temp;
            temp=temp->next;
            free(del);
        }
    }

    return 0;
}
