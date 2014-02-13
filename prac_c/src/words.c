#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct node {
    struct node *left;
    struct node *right;
    char *word;
};

static struct node *root = NULL;

void memory_error(void)
{
    fprintf(stderr, "Error:Out of memory\n");
    exit(8);
}

char *save_string(char *string)
{
    char *new_string;

    new_string = malloc((unsigned) (strlen(string)) + 1);

    if (new_string == NULL)
        memory_error();
    
    strcpy(new_string, string);
    return (new_string);
}

void enter(struct node **node, char *word)
{
    int result;

    char *save_string(char *);

    if ((*node) == NULL) {
        (*node) = malloc(sizeof(struct node));
        if ((*node) == NULL)
            memory_error();

        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->word = save_string(word);
        return;
    }

    result = strcmp((*node)->word, word);

    if (result == 0)
        return;
    if (result < 0)
        enter(&(*node)->right, word);
    else
        enter(&(*node)->left, word);
}

void scan(char *name)
{
    char word[100];
    int index;
    int ch;
    FILE *in_file;

    in_file = fopen(name, "r");
    if (in_file == NULL) {
        fprintf(stderr, "Error:Unable to open %s\n", name);
        exit(8);
    }
    while (1) {
        while (1) {
            ch = fgetc(in_file);
            if (isalpha(ch) || (ch == EOF))
                break;
        }

        if (ch == EOF)
            break;

        word[0] = ch;
        for (index = 1; index < sizeof(word); ++index) {
            ch = fgetc(in_file);
            if (!isalpha(ch))
                break;
            word[index] = ch;
        }
        word[index] = '\0';

        enter(&root, word);
    }
    fclose(in_file);
}

void print_tree(struct node *top)
{
    if (top == NULL)
        return;
    print_tree(top->left);
    printf("%s\n", top->word);
    print_tree(top->right);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Error:Wrong number of parameters\n");
        fprintf(stderr, "      on the command line\n");
        fprintf(stderr, "Usage is:\n");
        fprintf(stderr, "    words 'file'\n");
        exit(8);
    }
    scan(argv[1]);
    print_tree(root);
    return 0;
}

