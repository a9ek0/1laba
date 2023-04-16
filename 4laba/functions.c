#include "functions.h"

void menu(NODE **root, const char *file_name)////позжe убрать поле file_name
{
    ////выбор листа
    char choose[strlen("Continue") + 1];
    printf("Do you want to start new game or continue?(New/Continue)\n");
    fgets(choose, strlen("Continue") + 1, stdin);

    while(strcmp(choose, "New") != 0 && strcmp(choose, "Continue") != 0)
    {
        fflush(stdin);
        fgets(choose, strlen("Continue") + 1, stdin);
    }
    ///если новая игра
    if(strcmp("New", choose) == 0){
        char *extension;

        extension = (char*) malloc((strlen("Yes") + 1) * sizeof (char));
        strcpy(extension, "Yes");

        while(strcmp(extension, "Yes") == 0) {
            print_tree(*root, 0);
            run_through_tree(root);

            printf("Do you want to continue?\n");
            fgets(choose, strlen("Yes") + 1, stdin);

            while(strcmp(extension, "Yes") != 0 || strcmp(extension, "No") != 0)
            {
                fflush(stdin);
                fgets(choose, strlen("Yes") + 1, stdin);
            }
        }
        //////сохранение дерева в файл
        printf("Do you want to save this game?\n");
        fgets(choose, strlen("Yes") + 1, stdin);

        while(strcmp(extension, "Yes") != 0 || strcmp(extension, "No") != 0)
        {
            fflush(stdin);
            fgets(choose, strlen("Yes") + 1, stdin);
        }
        if(strcmp(extension, "No") == 0)
        {
            exit(EXIT_SUCCESS);
        }else{//////проверять имя на нулевость может быть хз наверное нельзя вводить ничего
            /////// имя файла должен вводить пользователь
            /// и если пользователь ввел имя которое уже существует
            /// вывести ошибку и попросить ввести имя еще раз
            FILE *file;
            file = fopen(file_name, "wt");
            if(file == NULL)
            {
                return;
            }
            tree_to_file(*root, file, 0);
            fclose(file);
        }
        //////здесь выбор уже существующей игры
    } else if(strcmp("Continue", choose) == 0){
        //////добавить список игр в отдельном текстовом файле + выбор
        //char *file_name1;
        file_name = choose_game();
        FILE *file;
        file = fopen(file_name, "rt");
        if(file == NULL)
            return;
        file_to_tree(root, file, 0);
        ///// нужно как-то не замусорив код вернуться к предыдущему
        /// где новая игра чтобы была возможность изменять текущую игру
        /// функция некая
        print_tree(*root, 0);
        fclose(file);
    }
}

char *choose_game()
{///////////тут \n есть нужно file_name[strcspn(file_name, "\n")] = '\0'; воткнуть
/// но мб если с клавы вводить то проблем не будет если не будет то нужно будет в принтф добавить \n
    char *file_name;

    FILE *file;
    file = fopen("games/games_list.txt", "rt");
    if(file == NULL)
        return NULL;

    file_name = (char*) malloc(MAX_LINE_LENGTH * sizeof (char));

    printf("Choose game.\n");
    while(fgets(file_name, MAX_LINE_LENGTH, file) != NULL)
    {
        printf("%s", file_name);
    }

    fgets(file_name, MAX_LINE_LENGTH, stdin);
    while(!is_in_file(file, file_name))
    {
        printf("Incorrect name!\n");
        fflush(stdin);
        fgets(file_name, MAX_LINE_LENGTH, stdin);
    }


    fclose(file);

    return file_name;
}

int is_in_file(FILE *file, const char *word)
{
    char line[MAX_LINE_LENGTH];
    fseek(file, 0, SEEK_SET);
    while(fgets(line, MAX_LINE_LENGTH, file))
    {
        if(strcmp(word, line) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void run_through_tree(NODE **root)
{
    NODE *tmp = *root;

    char answer[4];

    int expression = 0;

    while (expression != 1) {

        if(tmp->data->question == NULL)
        {
            printf("It's %s\n", tmp->data->answer);
            rebase_tree(&tmp);
            return;
        }

        printf("%s\n", tmp->data->question);
        fflush(stdin);

        check_answer(answer);
        expression = ask_question(&tmp, answer);

    }

    printf("It's %s!\n", strcmp("Yes", answer) == 0 ? tmp->yes_branch->data->answer : tmp->no_branch->data->answer);

    rebase_tree(strcmp("Yes", answer) == 0 ? &tmp->yes_branch : &tmp->no_branch);
}

int ask_question(NODE **node, const char *answer)
{
    if((*node)->yes_branch->yes_branch == NULL && (*node)->no_branch->no_branch == NULL)
    {
        return 1;
    }
    if (strcmp("Yes", answer) == 0) {
        *node = (*node)->yes_branch;
    }else if (strcmp("No", answer) == 0) {
        *node = (*node)->no_branch;
    }
    return 0;
}

void rebase_tree(NODE **node)
{
    char answer[4];
    char new_question[256];
    char correct_answer[256];
    DATA *data;

    printf("I'm right?\n");
    check_answer(answer);

    if (strcmp("Yes", answer) == 0)
    {
        printf("Ura!!!\n");
    }
    else
    {
        printf("A kto eto bil?\n");
        fflush(stdin);
        gets(correct_answer);

        printf("A 4e kak otlichit'?\n");
        fflush(stdin);
        gets(new_question);

        DATA *yes_data = create_data(NULL , correct_answer);
        NODE *yes_node = create_node(yes_data);
        free(yes_data);

        DATA *no_data = create_data(NULL , (*node)->data->answer);
        NODE *no_node = create_node(no_data);
        free(no_data);

        NODE* tmp = *node;
        tmp->yes_branch = yes_node;
        tmp->no_branch = no_node;

        tmp->data->question = malloc(strlen(new_question) + 1);
        strcpy(tmp->data->question, new_question);

        char *new_answer = malloc(strlen(tmp->data->answer) + 1);
        strcpy(new_answer, tmp->data->answer);
        tmp->data->answer = new_answer;
    }
}

void check_answer(char *answer)
{
    scanf_s("%s", answer);
    while(strcmp(answer, "Yes") != 0 && strcmp(answer, "No") != 0) {
        printf("Wrong input!\n");
        fflush(stdin);
        scanf_s("%s", answer);
    }
}

DATA *create_data(const char *question, const char *answer)
{
    DATA *data = malloc(sizeof(DATA));
    if (!data) {
        return NULL;
    }
    data->question = NULL;
    data->answer = NULL;

    if (question) {
        data->question = malloc(strlen(question) + 1);
        if (!data->question) {
            free(data);
            return NULL;
        }
        strcpy(data->question, question);
    }

    if (answer) {
        data->answer = malloc(strlen(answer) + 1);
        if (!data->answer) {
            free(data->question);
            free(data);
            return NULL;
        }
        strcpy(data->answer, answer);
    }

    return data;
}

void print_tree(NODE *root, int depth) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (root->data->question != NULL) {
        printf("Question: %s\n", root->data->question);
    } else {
        printf("Answer: %s\n", root->data->answer);
    }

    print_tree(root->yes_branch, depth + 1);
    print_tree(root->no_branch, depth + 1);
}

void tree_to_file(NODE *root, FILE *file, int depth)
{
    if (root == NULL) {
        return;
    }

    if (root->data->question != NULL) {
        fputs("Q ", file);
        fputs(root->data->question, file);
        fputs("\n", file);
    } else {
        fputs("A ", file);
        fputs(root->data->answer, file);
        fputs("\n", file);
    }

    tree_to_file(root->yes_branch, file, depth + 1);
    tree_to_file(root->no_branch, file, depth + 1);
}

NODE *create_node(DATA *data) {
    NODE *node = (NODE *) malloc(sizeof(NODE));
    node->data = data;
    node->yes_branch = NULL;
    node->no_branch = NULL;
    return node;
}

char* first_number_from_string(const char* string) {
    char* number = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    int i = 0;
    while (string[i] != ' ' && string[i] != '\n' && string[i] != '\0') {
        number[i] = string[i];
        i++;
    }
    if (i != 0) {
        number[i] = '\0';
        return number;
    } else {
        free(number);
        return NULL;
    }
}

void add_node(NODE *parent, NODE *node, char direction) {
    if (direction == 'Y') {
        parent->yes_branch = node;
    } else {
        parent->no_branch = node;
    }
}

void free_data(DATA *data) {
    if (data->question != NULL) {
        free(data->question);
    }
    if (data->answer != NULL) {
        free(data->answer);
    }
    free(data);
}

void free_tree(NODE *node) {
    if (node->yes_branch != NULL) {
        free_tree(node->yes_branch);
    }
    if (node->no_branch != NULL) {
        free_tree(node->no_branch);
    }
    free_data(node->data);
    free(node);
}

///////хачи это правильно
void file_to_tree(NODE **root, FILE *file, int depth)
{
    char line[MAX_LINE_LENGTH];

    if (root == NULL || file == NULL) {
        return;
    }

    if(fgets(line, MAX_LINE_LENGTH, file) == NULL)
    {
        return;
    }

    line[strcspn(line, "\n")] = '\0';

    NODE *node = malloc(sizeof(NODE));
    node->data = malloc(sizeof(DATA));

    if (line[0] == 'Q') {
        DATA *data = node->data;
        data->question = strdup(line + 2);
        data->answer = NULL;
        node->yes_branch = NULL;
        node->no_branch = NULL;
        *root = node;
        file_to_tree(&((*root)->yes_branch), file, depth + 1);
        file_to_tree(&((*root)->no_branch), file, depth + 1);
    } else {
        DATA *data = node->data;
        data->answer = strdup(line + 2);
        data->question = NULL;
        node->yes_branch = NULL;
        node->no_branch = NULL;
        *root = node;
    }
}