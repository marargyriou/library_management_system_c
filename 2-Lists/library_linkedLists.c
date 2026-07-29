#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book
{
    char *title;
    int release_date;
    float price;
    struct book *next;
}book;

typedef struct author
{
    int author_id;
    char *surname;
    char *name;
    int num_of_books;
    struct author *next;
}author;

typedef struct writes
{
    char *title;
    int author_id;
    struct writes *next;
}writes;

//global variables
book *b_head = NULL;
author *a_head = NULL;
writes *w_head = NULL;

//functions
void load_file();
void save_file();
void insert_author();
void insert_book();
void search_author();
void search_book();
void delete_author();
void delete_book_node(char *title);
void delete_book();
void clean_stdin();
char* read_string(FILE *fp);
int monograph(char *title);
void split_books(book *list, book **leftRef, book **rightRef);
book* merge_books(book *left, book *right);
void sort_books(book **headRef);
void split_writes(writes *list, writes **leftRef, writes **rightRef);
writes* merge_writes(writes *left, writes *right);
void sort_writes(writes **headRef);
void sort_lists();

int main()
{
    load_file();
    int choice;

    do
    {
        printf("\nLibrary Management System\n");
        printf("1.Insert Author\n");
        printf("2.Insert Book\n");
        printf("3.Search Author\n");
        printf("4.Search Book\n");
        printf("5.Delete Author\n");
        printf("6.Delete Book\n");
        printf("7.Exit & Save\n");
        printf("Please enter number:");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1: insert_author();
                break;
            case 2:insert_book();
                break;
            case 3:search_author();
                break;
            case 4: search_book();
                break;
            case 5: delete_author();
                break;
            case 6: delete_book();
                break;
            case 7: save_file();
                break;
            default: printf("Invalid choice. Try again\n");
        }
    } while (choice !=7);

    return 0;
}

//FUNCTIONS' DEFINITIONS

void clean_stdin()
{
    int c;
    while ((c= getchar()) != '\n' && c !=EOF);
}

char* read_string(FILE *fp)
{
    char buffer[256];
    if (fgets(buffer, 256, fp) == NULL) return NULL;
    buffer[strcspn(buffer, "\n")] = 0;
    char *str = malloc((strlen(buffer) + 1) * sizeof(char));
    if (str) strcpy(str,buffer);
    return str;
}

int monograph(char *title)
{
    int c = 0;
    writes *curr = w_head;
    while(curr!=NULL)
    {
        if(strcmp(curr->title, title)==0) c++;
        curr = curr->next;
    }
    if (c==1) return 1;
    else return 0;
}

void split_books(book *list, book **leftRef, book **rightRef)
{
    int l = 0;
    book *curr = list;

    while(curr!=NULL)
    {
        l++;
        curr=curr->next;
    }
    
    int m = (l-1)/2;

    curr = list;
    for(int i=0; i<m; i++)
    {
        curr = curr->next;
    }

    *leftRef = list;
    *rightRef = curr->next;
    curr->next = NULL;
}

book* merge_books(book *left, book *right)
{
    book *res = NULL;

    if(left==NULL) return right;
    if(right==NULL) return left;

    if(strcmp(left->title, right->title) <=0)
    {
        res = left;
        res->next= merge_books(left->next, right);
    }
    else
    {
        res = right;
        res->next=merge_books(left, right->next);
    }
    return res;
}
void sort_books(book **headRef)
{
    book *head = *headRef;
    book *left = NULL;
    book *right = NULL;

    if((head == NULL) || (head->next == NULL)) return;

    split_books(head, &left, &right);
    sort_books(&left);
    sort_books(&right);

    *headRef = merge_books(left, right);
}
void split_writes(writes *list, writes **leftRef, writes **rightRef)
{
    int l = 0;
    writes *curr = list;

    while(curr!=NULL)
    {
        l++;
        curr=curr->next;
    }
    
    int m = (l-1)/2;

    curr = list;
    for(int i=0; i<m; i++)
    {
        curr = curr->next;
    }

    *leftRef = list;
    *rightRef = curr->next;
    curr->next = NULL;
}
writes* merge_writes(writes *left, writes *right)
{
    writes *res = NULL;

    if(left==NULL) return right;
    if(right==NULL) return left;

    int cmp = strcmp(left->title, right->title);

    if(cmp==0)
    {
        if(left->author_id < right->author_id) cmp= -1;
        else cmp=1;
    }

    if(cmp <= 0)
    {
        res = left;
        res->next= merge_writes(left->next, right);
    }
    else
    {
        res = right;
        res->next=merge_writes(left, right->next);
    }
    return res;

}
void sort_writes(writes **headRef)
{
    writes *head = *headRef;
    writes *left = NULL;
    writes *right = NULL;

    if((head == NULL) || (head->next == NULL)) return;

    split_writes(head, &left, &right);
    sort_writes(&left);
    sort_writes(&right);

    *headRef = merge_writes(left, right);
}

void sort_lists()
{
    if(b_head !=NULL) sort_books(&b_head);
    if(w_head !=NULL) sort_writes(&w_head);
}

void load_file()
{
    FILE *fp;
    int c=0;

    //authors
    fp = fopen("authors.txt", "r");
    if(fp)
    {
        fscanf(fp, "%d", &c);
        author *a_tail = NULL;

        for(int i =0; i < c; i++)
        {
            author *a_node = malloc(sizeof(author));

            fscanf(fp, "%d", &a_node->author_id);
            a_node->surname = read_string(fp);
            a_node->name = read_string(fp);
            fscanf(fp, "%d", &a_node->num_of_books);
            a_node->next = NULL;

            if(a_head == NULL) a_head = a_node;
            else a_tail->next = a_node;

            a_tail = a_node;
        }
    }
    fclose(fp);
    printf("Loaded %d authors.\n", c);

    //books
    fp = fopen("books.txt", "r");
    if(fp)
    {
        fscanf(fp, "%d", &c);
        book *b_tail = NULL;

        for(int i =0; i < c; i++)
        {
            book *b_node = malloc(sizeof(book));

            b_node->title = read_string(fp);
            fscanf(fp, "%d", &b_node->release_date);
            fscanf(fp, "%f", &b_node->price);
            b_node->next = NULL;

            if(b_head == NULL) b_head = b_node;
            else b_tail->next = b_node;

            b_tail = b_node;
        }
    }
    fclose(fp);
    printf("Loaded %d books.\n", c);

    //writes
    fp = fopen("writes.txt", "r");
    if(fp)
    {
        fscanf(fp, "%d", &c);
        writes *w_tail = NULL;

        for(int i =0; i < c; i++)
        {
            writes *w_node = malloc(sizeof(writes));

            w_node->title = read_string(fp);
            fscanf(fp, "%d", &w_node->author_id);
            w_node->next = NULL;

            if(w_head == NULL) w_head = w_node;
            else w_tail->next = w_node;

            w_tail = w_node;
        }
    }
    fclose(fp);
    printf("Loaded %d writes.\n", c);
    sort_lists();
}

void save_file()
{
    sort_lists();
    FILE *fp;

    //authors
    fp = fopen("authors.txt", "w");
    if(fp)
    {
        int c = 0;
        author *current = a_head;
        while(current != NULL)
        {
            c++;
            current = current -> next;
        }
        fprintf(fp, "%d\n", c);

        current = a_head;
        while(current != NULL)
        {
            fprintf(fp, "%d\n %s\n %s\n %d\n", 
                    current-> author_id, current->surname, current->name, current->num_of_books);
            current = current-> next;
        }
        fclose(fp);
    }

    //books
    fp = fopen("books.txt", "w");
    if(fp)
    {
        int c = 0;
        book *current = b_head;
        while(current != NULL)
        {
            c++;
            current = current -> next;
        }
        fprintf(fp, "%d\n", c);

        current = b_head;
        while(current != NULL)
        {
            fprintf(fp, "%s\n %d\n %.2f\n", 
                    current-> title, current->release_date, current->price);
            current = current-> next;
        }
        fclose(fp);
    }

    //writes
     fp = fopen("writes.txt", "w");
    if(fp)
    {
        int c = 0;
        writes *current = w_head;
        while(current != NULL)
        {
            c++;
            current = current -> next;
        }
        fprintf(fp, "%d\n", c);

        current = w_head;
        while(current != NULL)
        {
            fprintf(fp, "%s\n %d\n", 
                    current-> title, current->author_id);
            current = current-> next;
        }
        fclose(fp);
    }
    printf("All data saved successfully.\n");
}

void insert_author()
{
    printf("Insert New Author\n");
    clean_stdin();

    printf("Enter surname:\n");
    char *surname = read_string(stdin);
    printf("Enter name:\n");
    char *name = read_string(stdin);

    author *curr = a_head;
    author *last = NULL;

    while(curr !=NULL)
    {
        if(strcmp(curr->surname, surname) == 0 && strcmp(curr->name, name) == 0)
        {
            printf("Author already registered.\n");
            free(surname);
            free(name);
            return;
        }
        last = curr;
        curr = curr->next;
    }
    int new_id;
    
    if(last == NULL) new_id=1;
    else new_id = last->author_id +1;

    author *a_new = malloc(sizeof(author));
    a_new->author_id = new_id;
    a_new->surname = surname;
    a_new->name = name;
    a_new->num_of_books = 0;
    a_new->next = NULL;

    if(a_head == NULL) a_head = a_new;
    else last->next = a_new;

    printf("Author inserted successfully (ID:%d).\n", new_id);
}

void insert_book()
{
    printf("Insert New Book\n");
    clean_stdin();

    printf("Title:\n");
    char *title_temp = read_string(stdin);

    book *curr = b_head;
    book *last = NULL;

    while(curr !=NULL)
    {
        if(strcmp(curr->title, title_temp) == 0)
        {
            printf("Book already registered.\n");
            free(title_temp);
            return;
        }
        last = curr;
        curr = curr->next;
    }

    int year;
    float price;

     printf("Enter Release Date:\n");
    scanf("%d", &year);
    printf("Enter Price:\n");
    scanf("%f", &price);

    book *b_new = malloc(sizeof(book));
    b_new->title = title_temp;
    b_new->release_date = year;
    b_new->price = price;
    b_new->next = NULL;

    if(b_head == NULL) b_head = b_new;
    else last->next = b_new;

    clean_stdin();
    
    int num_authors;
    printf("How many Authors does this book have?");
    scanf("%d", &num_authors);
    clean_stdin();
    
    for(int j=0; j< num_authors; j++)
    {
        printf("Author %d of %d\n", j+1, num_authors);
        printf("Enter Author's Surname: ");
        char *surname = read_string(stdin);

        int found_id = -1;
        author *a_curr = a_head;
        author *a_last = NULL;

        while(a_curr !=NULL)
        {
            if(strcmp(a_curr->surname, surname)== 0)
            {
                found_id = a_curr->author_id;
                a_curr->num_of_books++;
                printf("Author found (ID: %d)\n", found_id);
                break;
            }
            a_last = a_curr;
            a_curr = a_curr->next;
        }
        if(found_id ==-1)
        {
            printf("Author not found. Help us register them.\n");
            printf("Enter Author's Name: ");
            char *name = read_string(stdin);

            int new_id;
            if(a_last != NULL) new_id = a_last->author_id + 1;
            else if (a_head == NULL) new_id = 1;

            author *a_new = malloc(sizeof(author));
            a_new->author_id = new_id;
            a_new->surname = surname;
            a_new->name = name;
            a_new->num_of_books = 1;
            a_new->next = NULL;
        
            if(a_head == NULL) a_head= a_new;
            else a_last->next = a_new;

            found_id = new_id;
            printf("New author registered (ID:%d).\n", new_id);
        }
        else free(surname);

        writes *w_new = malloc(sizeof(writes));

        w_new->title=malloc(strlen(title_temp) +1);
        strcpy(w_new->title, title_temp);

        w_new->author_id=found_id;
        w_new->next= NULL;

        if(w_head == NULL) w_head = w_new;
        else
        {
            writes *w_curr = w_head;
            while(w_curr->next !=NULL) w_curr = w_curr->next;
            w_curr->next = w_new;

        }
        printf("Author connected.\n");
    }
    sort_lists();
    printf("Book registered successfully.\n");
}

void search_author()
{
    printf("Search Author\n");
    clean_stdin();
    
    printf("Enter surname:\n");
    char *surname = read_string(stdin);

    int author_temp = 0;
    for(author *curr_a = a_head; curr_a !=NULL; curr_a = curr_a->next)
    {
        if(strcmp(curr_a->surname, surname) == 0)
        {
            author_temp = 1;
            printf("Author found:\n Surname: %s\n Name: %s\n ID: %d\n Books: %d\n",
                    curr_a->surname, curr_a->name, curr_a->author_id, curr_a->num_of_books);
           
            printf("Book Information:\n");
            int book_temp = 0;
            
            for(writes *curr_w = w_head; curr_w !=NULL; curr_w = curr_w->next)
            {
                if(curr_w->author_id == curr_a->author_id)
                {
                    for(book *curr_b = b_head; curr_b !=NULL; curr_b=curr_b->next)
                    {
                        if(strcmp(curr_b->title, curr_w->title) == 0)
                        {
                            printf("Title: %s\n Relase Date:%d\n Price:%.2f\n",
                                    curr_b->title, curr_b->release_date, curr_b->price);
                            book_temp=1;
                        }
                    }
                }
            }
            if(!book_temp) printf("No information available.\n");
        }
    }
    if(!author_temp) printf("Author not found.\n");
    free(surname);
}

void search_book()
{
    printf("Search Book\n");
    clean_stdin();

    if(b_head == NULL)
    {
        printf("No books registered yet.\n");
        return;
    }
    printf("Enter Title:");
    char *title_temp = read_string(stdin);

    book *curr = b_head;
    book *idx = NULL;

    while(curr!=NULL)
    {
        int r = strcmp(curr->title, title_temp);
        if(r==0)
        {
            idx = curr;
            break;
        }
        else if(r>0) break;
        
        curr = curr->next;
    }

    if(idx !=NULL)
    {
        printf("Book found.\n Title: %s\n Release Date: %d\n Price: %.2f\n",
                curr->title, curr->release_date, curr->price);
        
        printf("Authors:");
        int author_temp = 0;
        writes *w_curr = w_head;

        while(w_curr !=NULL)
        {
            int r = strcmp(w_curr->title, title_temp);

            if(r==0)
            {
                int id = w_curr->author_id;

                author *a_curr = a_head;
                while(a_curr !=NULL)
                {
                    if(a_curr->author_id==id)
                    {
                        if(author_temp>0) printf(",");
                        printf("%s %s", a_curr->name, a_curr->surname);
                        author_temp++;
                        break;
                    }
                    a_curr = a_curr-> next;
                }
            }
            else if(r>0) break;
            w_curr = w_curr->next;
        }
        if(author_temp==0) printf("No Authors linked.\n");
    }
    else printf("Book not found.\n");
    free(title_temp);
}

void delete_book_node(char *title)
{
    book *curr = b_head;
    book *prev = NULL;

    while(curr!= NULL)
    {
        if(strcmp(curr->title, title)==0)
        {
            if(prev == NULL) b_head = curr->next;
            else prev->next = curr->next;
        }
        free(curr->title);
        free(curr);
        return;
    }
    prev = curr;
    curr = curr->next;
}

void delete_book()
{
    printf("Delete Book\n");
    clean_stdin();

    printf("Enter Title:");
    char *title = read_string(stdin);

    book *b_curr = b_head;
    int idx = 0;
    while(b_curr!=NULL)
    {
        if(strcmp(b_curr->title, title)==0)
        {
            idx=1;
            break;
        }
        b_curr = b_curr->next;
    }

    if(idx==0)
    {
        printf("Book not found.\n");
        free(title);
        return;
    }

    delete_book_node(title);
    printf("Book deleted successfully.\n");

    writes *curr_w = w_head;
    writes *prev_w = NULL;

    while(curr_w !=NULL)
    {
        if(strcmp(curr_w->title, title)==0)
        {
            writes *temp = curr_w;

            if(prev_w == NULL)
            {
                w_head = curr_w->next;
                curr_w = w_head;
            }
            else
            {
                prev_w->next = curr_w->next;
                curr_w = curr_w->next;
            }
            free(temp->title);
            free(temp);
        }
        else
        {
            prev_w=curr_w;
            curr_w = curr_w->next;
        }
    }
    printf("Removed link for Author ID: %d.\n", curr_w->author_id);
    free(title);
}

void delete_author()
{
    printf("Delete Author\n");
    clean_stdin();

    printf("Enter Author's ID:");
    int id;
    scanf("%d", &id);
    clean_stdin();

    author *curr_a = a_head;
    author *prev_a = NULL;
    int idx = 0;

    while(curr_a != NULL)
    {
        if(curr_a->author_id==id)
        {
            idx = 1;
            if(prev_a==NULL) a_head=curr_a->next;
            else prev_a->next= curr_a->next;
            free(curr_a->surname);
            free(curr_a->name);
            free(curr_a);
            printf("Author deleted (ID %d).\n", id);
            break;
        }
        prev_a=curr_a;
        curr_a=curr_a->next;
    }
    if(idx==0)
    {
        printf("Author not found.\n");
        return;
    }

    writes *curr_w = w_head;
    writes *prev_w = NULL;

    while(curr_w !=NULL)
    {
        if(curr_w->author_id==id)
        {
            if(monograph(curr_w->title)==1)
            {
                printf("Monograph deleted: %s\n", curr_w->title);
                delete_book_node(curr_w->title);
            }

            writes *temp = curr_w;
            if(prev_w==NULL)
            {
                w_head=curr_w->next;
                curr_w=w_head;
            }
            else
            {
                prev_w->next= curr_w->next;
                curr_w = curr_w->next;
            }
            free(temp->title);
            free(temp);
        }
        else
        {
            prev_w = curr_w;
            curr_w = curr_w->next;
        }
    }
}