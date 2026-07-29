#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book
{
char *title;
int release_date;
float price;
} book;

typedef struct author
{
int author_id;
char *surname;
char *name;
int num_of_books;
} author;

typedef struct writes
{
char *title;
int author_id;
} writes;

//global variables
book *book_arr = NULL;
int book_count = 0;

author *author_arr = NULL;
int author_count = 0;

writes *writes_arr = NULL;
int writes_count = 0;

//functions
void load_file();
void save_file();
void insert_author();
void insert_book();
void search_author();
void search_book();
void delete_author();
void delete_book_auto(char *title);
void delete_book_manual();
void clean_stdin();
char* read_string(FILE *fp);
int monograph(char *title);
int sort_books(const void *a, const void *b);
int sort_writes(const void *a, const void *b);
void sort_arrays();

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
            case 6: delete_book_manual();
                break;
            case 7: save_file();
                break;
            default: printf("Invalid choice. Try again.\n");
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
    
    char *str = malloc((strlen(buffer) + 1) * sizeof(char));
    if (str) strcpy(str,buffer);
    return str;
}

int sort_books(const void *a, const void *b)
{
    book *b1 = (book *)a;
    book *b2 = (book *)b;
    return strcmp(b1->title, b2->title);
}

int sort_writes(const void *a, const void *b)
{
    writes *w1 = (writes *)a;
    writes *w2 = (writes *)b;

    if(w1->author_id < w2->author_id) return -1;
    if(w1->author_id > w2->author_id) return 1;
    return strcmp(w1->title, w2->title);
}

int monograph(char *title)
{
    int c =0;
    
    for(int i = 0; i < writes_count; i++)
    {
        if(strcmp(writes_arr[i].title, title) == 0) c++;
    }

    if(c == 1) return 1;
    else return 0;
}

void sort_arrays()
{
    if(book_count>1)
    {
        qsort(book_arr, book_count, sizeof(book), sort_books);
    }

    if(writes_count>1)
    {
        qsort(writes_arr, writes_count, sizeof(writes), sort_writes);
    }
}

void load_file()
{
    FILE *fp;
    int i;

    //authors
    fp = fopen("authors.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d", &author_count);
        
        if (author_count > 0)
        {
            author_arr = malloc(author_count * sizeof(author));
            for(i =0; i < author_count; i++)
            {
                fscanf(fp, "%d", &author_arr[i].author_id);
                author_arr[i].surname = read_string(fp);
                author_arr[i].name = read_string(fp);
                fscanf(fp, "%d", &author_arr[i].num_of_books);
            }
        }
        fclose(fp);
        printf("Loaded %d authors.\n", author_count);
    }

    //books
    fp = fopen("books.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d", &book_count);
        
        if (book_count > 0)
        {
            book_arr = malloc(book_count * sizeof(book));
            for(i =0; i < book_count; i++)
            {
                book_arr[i].title = read_string(fp);
                fscanf(fp, "%d", &book_arr[i].release_date);
                fscanf(fp, "%.2f",&book_arr[i].price);
            }
        }
        fclose(fp);
        printf("Loaded %d books.\n", book_count);
    }

    //writes
    fp = fopen("writes.txt", "r");
    if (fp)
    {
        fscanf(fp, "%d", &writes_count);
        
        if (writes_count > 0)
        {
            writes_arr = malloc(writes_count * sizeof(writes));
            for(i =0; i < writes_count; i++)
            {
                writes_arr[i].title = read_string(fp);
                fscanf(fp, "%d", &writes_arr[i].author_id);
            }
        }
        fclose(fp);
        printf("Loaded %d writes.\n", writes_count);
    }
}

void save_file()
{
    FILE *fp;
    int i;

    //authors
    fp = fopen("authors.txt", "w");
    if(fp)
    {
        fprintf(fp, "%d\n", author_count);
        for(i=0; i < author_count; i++)
        {
            fprintf(fp, "%d\n%s\n%s\n%d\n", 
                    author_arr[i].author_id, author_arr[i].surname, author_arr[i].name, author_arr[i].num_of_books);
        }
        fclose(fp);
    }

    //books
    fp = fopen("books.txt", "w");
    if(fp)
    {
        fprintf(fp, "%d\n", book_count);
        for(i=0; i < book_count; i++)
        {
            fprintf(fp, "%s\n%d\n%.2f\n", 
                    book_arr[i].title, book_arr[i].release_date, book_arr[i].price);
        }
        fclose(fp);
    }

    //writes
    fp = fopen("writes.txt", "w");
    if(fp)
    {
        fprintf(fp, "%d\n", writes_count);
        for(i=0; i < writes_count; i++)
        {
            fprintf(fp, "%s\n%d\n", writes_arr[i].title, writes_arr[i].author_id);
        }
        fclose(fp);
    }

    printf("All data saved succesfully.\n");
}

void insert_author()
{
    printf("Insert New Author\n");
    clean_stdin();

    printf("Enter surname:\n");
    char *surname = read_string(stdin);
    printf("Enter name:\n");
    char *name = read_string(stdin);

    for(int i = 0; i < author_count; i++)
    {
        if (strcmp(author_arr[i].surname, surname) == 0 &&
            strcmp(author_arr[i].name,name) == 0)
        {
                printf("Author already registered.\n");
                free(surname);
                free(name);
                return;
        }
    }

    int new_id;
    if(author_count == 0)
    {
        new_id = 1;
    }
    else
    {
        new_id = author_arr[author_count -1].author_id + 1;
    }

    author *temp = realloc(author_arr, (author_count + 1) * sizeof(author));
    if(!temp)
    {
        printf("Memory Error\n");
        free(surname);
        free(name);
        return;
    }
    author_arr = temp;

    author_arr[author_count].author_id = new_id;
    author_arr[author_count].surname = surname;
    author_arr[author_count].name = name;
    author_arr[author_count].num_of_books = 0;

    author_count++;
    printf("Author inserted sucsessfully (ID: %d).\n", new_id);
}

void insert_book()
{
    printf("Insert New Book\n");
    clean_stdin();

    printf("Title:\n");
    char *title_temp = read_string(stdin);

    for(int i =0; i < book_count; i++)
    {
        if(strcmp(book_arr[i].title, title_temp) == 0)
        {
            printf("Book already registered.\n");
            free(title_temp);
            return;
        }
    }

    int year;
    float price;

    printf("Enter Release Date:\n");
    scanf("%d", &year);
    printf("Enter Price:\n");
    scanf("%f", &price);

    book *book_temp = realloc(book_arr, (book_count + 1) * sizeof(book));
    if(!book_temp)
    {
        printf("Memory Error\n");
        free(title_temp);
        return;
    }
    book_arr = book_temp;

    book_arr[book_count].title = title_temp;
    book_arr[book_count].release_date = year;
    book_arr[book_count].price = price;
    book_count++;
    
    int num_authors;
    printf("How many Authors does this book have?");
    scanf("%d", &num_authors);
    clean_stdin();

    for(int j=0; j<num_authors; j++)
    {
        printf("Author %d of %d\n", j+1, num_authors);
        printf("Enter Author's Surname:\n");
        char *surname = read_string(stdin);
 
        int found_id = -1;

        for(int i =0; i < author_count; i++)
        {
            if(strcmp(author_arr[i].surname, surname) == 0)
            {
                found_id = author_arr[i].author_id;
                author_arr[i].num_of_books++;
                printf("Author found (ID: %d).\n", found_id);
                break;
            }
        }

        if(found_id == -1)
        {
            printf("Author not found. Help us register them.\n");
            printf("Enter Author's Name:");
            char *name = read_string(stdin);

            int new_id;
            if(author_count == 0)
            {
                new_id = 1;
            }
            else
            {
                new_id = author_arr[author_count - 1].author_id + 1;
            }

            author *author_temp = realloc(author_arr, (author_count + 1) * sizeof(author));
            if(!author_temp)
            {
                printf("Memory Error\n");
                free(surname);
                free(name);
                return;
            }
            author_arr = author_temp;

            author_arr[author_count].author_id = new_id;
            author_arr[author_count].surname = surname;
            author_arr[author_count].name = name;
            author_arr[author_count].num_of_books = 1;
            author_count++;

            found_id = new_id;
            printf("New author registered (ID: %d).\n", new_id);
        }
        else free(surname);

        //updating writes
        writes *temp = realloc(writes_arr, (writes_count + 1) * sizeof(writes));
        if(!temp)
        {
            printf("Memory Error\n");
            free(surname);
            return;
        }
        writes_arr = temp;

        writes_arr[writes_count].title = malloc((strlen(title_temp) + 1) * sizeof(char));
        strcpy(writes_arr[writes_count].title, title_temp);
        writes_arr[writes_count].author_id = found_id;
        writes_count++;
    }
    sort_arrays();
    printf("Book registered successfully.\n");
}

void search_author() //linear search, sorted by id
{
    printf("Search Author\n");
    clean_stdin();
    
    printf("Enter surname:\n");
    char *surname = read_string(stdin);

    int author_temp = 0;
    for(int i=0; i<author_count; i++)
    {
        if(strcmp(author_arr[i].surname, surname) == 0)
        {
            author_temp = 1;
            printf("Author found:\n Surname: %s\n Name: %s\n ID: %d\n Books: %d\n",
                    author_arr[i].surname, author_arr[i].name, author_arr[i].author_id, author_arr[i].num_of_books);
           
            printf("Book Information:\n");
            int book_temp = 0;
            
            for(int w=0; w < writes_count; w++)
            {
                if(writes_arr[w].author_id == author_arr[i].author_id)
                {
                    for(int b = 0; b < book_count; b++)
                    {
                        if(strcmp(book_arr[b].title, writes_arr[w].title) == 0)
                        {
                            printf("Title: %s\n Relase Date:%d\n Price:%.2f\n",
                                    book_arr[b].title, book_arr[b].release_date, book_arr[b].price);
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

void search_book() //binary search
{
    printf("Search Book\n");
    clean_stdin();
    
    if(book_count == 0) 
    {
        printf("No books registered yet.\n"); 
        return;
    }

    printf("Enter Title:");
    char *title_temp = read_string(stdin);

    sort_arrays();

    int l = 0, h = book_count - 1, idx =-1;

    while(l<=h)
    {
        int m = (l + h) / 2;
        int r = strcmp(book_arr[m].title, title_temp);
        if(r==0) 
        {
            idx = m; 
            break;
        }
        else if(r<0) l = m + 1;
        else h = m - 1;
    }

    if(idx !=-1)
    {
        printf("Book found.\n Title: %s\n Release Date: %d\n Price: %.2f\n",
                book_arr[idx].title, book_arr[idx].release_date, book_arr[idx].price);
        
        printf("Authors:");
        int authors_temp = 0;
        for(int i=0; i<writes_count; i++)
        {
            if(strcmp(writes_arr[i].title, title_temp) == 0)
            {
                int id = writes_arr[i].author_id;
                for(int j = 0; j < author_count; j++)
                {
                    if(author_arr[j].author_id == id)
                    {
                        printf("Surname: %s\n Name: %s\n", author_arr[j].surname, author_arr[j].name);
                        authors_temp = 1;
                    }
                }
            }
        }
        if (!authors_temp) printf("No Authors linked.\n");
    } else printf("Book not found.\n");
    free(title_temp);
}

void delete_book_auto(char *title)
{
    int idx = -1;
    for(int i =0; i < book_count; i++)
    {
        if(strcmp(book_arr[i].title, title) == 0)
        {
            idx=i;
            break;
        }
    }
    if(idx == -1)
    {
        printf("Book %s not found.\n", title);
        return;
    }

    int w =0;
    while(w<writes_count)
    {
        if(strcmp(writes_arr[w].title, title) ==0)
        {
            int id = writes_arr[w].author_id;
            for(int a=0; a<author_count; a++)
            {
                if(author_arr[a].author_id ==id)
                {
                    author_arr[a].num_of_books--;
                }
            }
            free(writes_arr[w].title);
            for(int k = w; k< writes_count -1; k++)
            {
                writes_arr[k] = writes_arr[k+1];
            }
            writes_count--;
            writes_arr = realloc(writes_arr, writes_count * sizeof(writes));
        }
        else w++;

    }
    free(book_arr[idx].title);
    for(int j = idx; j < book_count - 1; j++)
    {
        book_arr[j] = book_arr[j+1];
    }
    book_count--;
    book_arr = realloc(book_arr, book_count * sizeof(book));
    printf("Deleted book: %s\n", title);
    sort_arrays();
}

void delete_book_manual()
{
    printf("Delete Book\n");
    clean_stdin();
    printf("Enter Title:\n");
    char *title = read_string(stdin);
    delete_book_auto(title);
    free(title);
    printf("Book %s deleted.\n", title);
}

void delete_author()
{
    printf("Delete Author\n");
    printf("Enter Author ID:\n");
    
    int id;
    if(scanf("%d", &id) !=1) return;

    int idx = -1;
    for(int i =0; i< author_count; i++)
    {
        if(author_arr[i].author_id == id) idx = i;
    }
    if(idx == -1) 
    {
        printf("ID not found"); 
        return;
    }

    for(int i = writes_count - 1; i >=0; i--)
    {
        if(writes_arr[i].author_id == id)
        {
            char *title_temp = malloc((strlen(writes_arr[i].title) + 1));
            strcpy(title_temp, writes_arr[i].title);

            if(monograph(title_temp))
            {
                printf("Deleting monograph: %s\n", title_temp);
                delete_book_auto(title_temp);
            }
            else
            {
                printf("Removing from collaboration: %s\n", title_temp);
                free(writes_arr[i].title);
            
                for(int w = i; w< writes_count -1; w++) writes_arr[w] = writes_arr[w+1];
                writes_count--;
                writes_arr=realloc(writes_arr, writes_count * sizeof(writes));
            }
            free(title_temp);
        }
    }
    free(author_arr[idx].surname);
    free(author_arr[idx].name);
    
    for(int k = idx; k < author_count -1; k++) author_arr[k] = author_arr[k+1];
    author_count--;
    if(author_count > 0)
    {
        author_arr = realloc(author_arr, author_count * sizeof(author));
    }

    printf("Author deleted\n");
    sort_arrays();
}