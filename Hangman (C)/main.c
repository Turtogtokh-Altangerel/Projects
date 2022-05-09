#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define unsuccessful '~'

int max_length; //for defining the range of word lengths.
int min_length = 8;

typedef struct triplet   //defining new data type Triplet.
{
    char word[20];
    int count;
    double duration;
} Triplet;

typedef enum   //defining enum data type boolean.
{
    False,
    True
} boolean;

char getonechar()   //the function returns character read only if the user inputs a single char.
{
    char str[64];
    fgets(str, sizeof(str), stdin);
    int length = strlen(str) - 1;
    if (length != 1) //if the user input is more than one char, returns *.
    {
        return unsuccessful;
    }
    else
    {
        char ch = str[0];
        return ch;
    }
}

int getonenum()   //the function returns character read only if the input is numeric.
{
    int num = 0;
    int value;
    char ch;
    boolean numeric = True; //this variable stays true unless the user inputs non-numeric char.
    while (scanf("%c", & ch) == 1 && !isspace(ch))
    {
        if (isdigit(ch))
        {
            value = ch - '0';
            num = num + value;
            num = num * 10;
        }
        if (!isdigit(ch))   //in case the char is not numeric,
        {
            numeric = False;
        }
    }
    if (numeric == True)
        return num / 10;
    else
        return unsuccessful; //since numeric is false.
}

void print_line()
{
    for(int i = 0; i < 40; i++)
        printf("_");
    printf("\n");
}

char * stringcopy(char * d)   //the function creates a dynamic copy of the received string.
{
    size_t length;
    char * copy;
    length = strlen(d);
    copy = malloc(length + 1); //+1 Byte for the null character. the end of the string d is null.
    if (copy == NULL)
    {
        printf("Memory allocation failed");
        return NULL;
    }
    strcpy(copy, d); //copies the contents of d.
    return copy;
}

char * hide(char * d)   //the function creates a string of only _ (underscore) char.
{
    size_t length;
    char * copy;
    length = strlen(d);
    copy = malloc(length + 1);
    if (copy == NULL)
    {
        printf("Memory allocation failed");
        return NULL;
    }
    strcpy(copy, d); //copies the contents of d.
    for (int i = 0; i < length; i++) //changing each char to _(underscore) except the null character.
        copy[i] = '_';
    return copy;
}

boolean mistake_check(char * original, char ch, char ** p)   //the function checks if the letter guessed is correct and updates missed letters.
{
    if (strchr(original, ch) == NULL) //if the letter is not found.
    {
        ( ** p) = ch; //**p points to a content of misses array which ch is put into.
        ( * p) ++; //*p points to the address of the current array of index in interest, hence increments as to point to the next empty index.
        ( ** p) = ',';
        ( * p) ++;
        ( ** p) = ' ';
        ( * p) ++; //misses array is organized in such a way that ch, ch, ch, etc.
        return True; //if letter is not found.
    }

    return False; //if letter is found.
}

char * update_hidden(char * original, char * hidden, size_t length, char ch)   //the function updates hidden string in case of a successful guess.
{
    for (int i = 0; i < length; i++)
    {
        if (original[i] == ch) //finds the position.
            hidden[i] = ch; //replacing underscore with a guessed letter.
    }
    return hidden; //returns the updated hidden string.
}

boolean restart()   //the function asks if the user continues playing and returns the answer.
{
    char ch;
    print_line();
    printf("Would you like to play again?\n");
    do
    {
        printf("Please, choose Y or N:");
        ch = tolower(getonechar()); //converted to lowercase so the user can input either of upper or lower cases.
    }
    while (ch != 'y' && ch != 'n'); //the user has to enter either Y or N to continue.
    if (ch == 'y')
        return True;
    else
        return False;
}

boolean update_database(char * word, double duration)   //the function updates database file where guessed word triplets are written.
{
    FILE * fp;
    int x = 0;
    int linecount = 0;
    int count = 0;
    boolean repeatedword = False;
    char str[64]; //set to 64 to reduce the chances of buffer overflow issues.
    Triplet data; //one variable of triplet structure.

    fp = fopen("database.txt", "r"); //in read mode.
    if (fp == NULL)
        printf("File open failed.");

    while (fgets(str, sizeof(str), fp) != NULL)   //checks the number of lines in the database.txt file.
    {
        ++linecount;
    }
    Triplet arraydata[linecount]; //creates an array of triplet structures dynamically.

    fseek(fp, 0, SEEK_SET); //moves the file pointer back to the start of the file.

    while (fscanf(fp, "%s %d %lf\n", data.word, & data.count, & data.duration) == 3)   //reads the triplet data into a local variable 'data'.
    {
        arraydata[x] = data; //writes the scanned data into the array member.
        if (strcmp(arraydata[x].word, word) == 0) //checks if the word was guessed before.
        {
            arraydata[x].duration = (arraydata[x].duration * arraydata[x].count + duration) / (arraydata[x].count + 1); //duration is updated as the average duration.
            arraydata[x].count = arraydata[x].count + 1; //increments count.
            count = arraydata[x].count;
            repeatedword = True;
        }
        ++x;
    }
    fclose(fp);

    if (repeatedword == False)   //writes the triplet at the end of the file in case word is not guessed before.
    {
        fp = fopen("database.txt", "a"); //in append and write modes, checking the success of file open is unnecessary.
        fprintf(fp, "%s %d %f\n", word, 1, duration);
        fclose(fp);
        return False;
    }
    else   //needs to overwrite the whole update database; otherwise, there will be repetition of words in the file.
    {
        fp = fopen("database.txt", "w");
        for (int i = 0; i < linecount + 1; i++)
        {
            data = arraydata[i];
            fprintf(fp, "%s %d %f\n", data.word, data.count, data.duration);
        }
        fclose(fp);
        return True;
    }
}

void print_database(void)
{
    FILE * fp;
    char str[64]; //set to 64 to reduce the chances of buffer overflow issues.
    Triplet data; //one variable of triplet structure.

    fp = fopen("database.txt", "r"); //in read mode.
    if (fp == NULL)
        printf("File open failed.");
    printf("\nSCORES(word/count/average time)\n");
    while (fscanf(fp, "%s %d %lf\n", data.word, & data.count, & data.duration) == 3)   //reads the triplet data into a local variable 'data'.
    {
        printf("%-20s | %d | %.2fseconds\n", data.word, data.count, data.duration); //prints current word triplets.
    }

    fclose(fp);
}

void add_drawing(char drawing[10][12], int mistake, int mistake_limit)   //the function adds a drawing on the current hangman figure.
{
    switch (mistake_limit)   //Outer switch has three options depending on the difficulty level of the game.
    {
    //Each of the inner switch functions depends on how many mistakes the user has already made.
    case 10: //the figure is drawn piece by piece.
        switch (mistake)
        {
        case 0:
            drawing[2][5] = '|';
            break;
        case 1:
            drawing[3][5] = '|';
            break;
        case 2:
            drawing[4][5] = 'O';
            break;
        case 3:
            drawing[5][5] = '|';
            break;
        case 4:
            drawing[6][4] = '/';
            break;
        case 5:
            drawing[6][5] = '|';
            break;
        case 6:
            drawing[6][6] = '\\';
            break;
        case 7:
            drawing[7][5] = '|';
            break;
        case 8:
            drawing[8][4] = '/';
            break;
        case 9:
            drawing[8][6] = '\\';
            break;
        }
        break;
    case 5: //the figure is drawn by 2 pieces for each of 5 mistakes.
        switch (mistake)
        {
        case 0:
            drawing[2][5] = '|';
            drawing[3][5] = '|';
            break;
        case 1:
            drawing[4][5] = 'O';
            drawing[5][5] = '|';
            break;
        case 2:
            drawing[6][4] = '/';
            drawing[6][5] = '|';
            break;
        case 3:
            drawing[6][6] = '\\';
            drawing[7][5] = '|';
            break;
        case 4:
            drawing[8][4] = '/';
            drawing[8][6] = '\\';
            break;
        }
        break;
    case 3: //draws the figure in large pieces.
        switch (mistake)
        {
        case 0:
            drawing[2][5] = '|';
            drawing[3][5] = '|';
            drawing[4][5] = 'O';
            break;
        case 1:
            drawing[5][5] = '|';
            drawing[6][4] = '/';
            drawing[6][5] = '|';
            drawing[6][6] = '\\';
            break;
        case 2:
            drawing[7][5] = '|';
            drawing[8][4] = '/';
            drawing[8][6] = '\\';
            break;
        }
        break;
    }
}

boolean difficulty(int * chosen_length, int * mistake_limit)   //the function reads from the user input and sets the game difficulty parameters.
{
    char ch;
    int num;

    print_line();
    printf("THE DIFFICULTY:\n");
    printf("0. 10 misses -> Beginner\n");
    printf("1.  5 misses -> Advanced\n");
    printf("2.  3 misses -> Expert\n");
    print_line();
    printf("\nChoose the difficulty:");

    do
    {
        num = getonenum();
        if (num != 0 && num != 1 && num != 2)
            printf("\nPlease, choose from only 0,1 or 2:");
    }
    while (num != 0 && num != 1 && num != 2); //requires an input of single char either 0,1 or 2.
    switch (num)   //modifies the number of mistakes allowed through pointer.
    {
    case 0:
        *mistake_limit = 10;
        break;
    case 1:
        *mistake_limit = 5;
        break;
    case 2:
        *mistake_limit = 3;
        break;
    }

    print_line();
    printf("Would you like to choose the length of the word?\n");
    do
    {
        printf("\nY/N?:");
        ch = tolower(getonechar());
    }
    while (ch != 'y' && ch != 'n'); //requires an input of single char either Y or N.

    if (ch == 'y')
    {
        print_line();
        printf("Word lengths vary from %d to %d.\n", min_length, max_length);
        printf("\nChoose the length:");
        do
        {
            num = getonenum(); //num becomes unsuccessful if the input is not numeric.
            if (num < min_length || num > max_length)
                printf("\nEnter only numeric value in the range:");
        }
        while (num == unsuccessful || (num < min_length || num > max_length)); //requires number in the specified range.
        * chosen_length = num;
        return False; //means word length is not random.
    }

    return True; //word length is random.
}

boolean Gameloop(char * d[], int wordcount)   //the function is the main game loop, which receives 2D dynamic array of words and the number of words.
{
    srand(time(NULL)); //sets the seed number differently every time avoiding the same sequence of random integers.
    int random;
    size_t length;
    char ch;
    int mistake = 0; //when a new game starts, the mistake is set to 0.
    int chosen_length;
    int mistake_limit;
    boolean random_length; //false if the length is chosen by the user.

    random_length = difficulty( & chosen_length, & mistake_limit); //modifies the max number of mistakes allowed and word length if the user chooses so.

    char misses[3 * mistake_limit + 1]; //the size is dynamically set according to mistake_limit and each character in the array is followed with comma and space, hence multiplied by 3.
    for (int i = 0; i <= 3 * mistake_limit; i++) //initializing a char array with all elements set to NULL.
        misses[i] = '\0';
    char * p = & misses[0]; //sets a pointer *p to the first elements of misses array.

    clock_t START, FINISH;
    double DURATION;

    int row, column; //declaring 2D array.
    char drawing[10][12];
    for (row = 0; row < 10; row++)   //initializing in such a way that the array is empty.
    {
        for (column = 0; column < 12; column++)
            drawing[row][column] = '\0';
    }
    for (row = 0; row < 10; row++) //drawing the static figure of the drawing. As the user makes a mistake, the drawing will be added on this base drawing.
        drawing[row][0] = '|';
    for (column = 1; column < 12; column++)
    {
        drawing[1][column] = '_';
        drawing[9][column] = '_';
    }
    if (random_length == False)
        do
        {
            random = rand() % wordcount; //gets a random integer between 0 to the number of words.
            length = strlen(d[random]);
        }
        while (length != chosen_length); //finds the random word of a requested length.
    else   //the length is not specified.
    {
        random = rand() % wordcount;
        length = strlen(d[random]);
    }

    print_line();
    printf("\nYour word has %zu letters!Guess it!\n", length);

    char * original = stringcopy(d[random]); //copy of the random word.
    char * hidden = hide(d[random]); //hidden version of the random word.
    if (original == NULL || hidden == NULL) //if any of the previous two dynamic memory allocations has failed, exits the function.
    {
        free(original);
        free(hidden);
        original = NULL;
        hidden = NULL;
        return False;
    }
    printf("//the word is '%s'//\n\n", d[random]); //It is only here to help lab conductor check the program.

    START = clock(); //starting time of the game round.

    while (mistake >= 0)
    {
        print_line();
        printf("Enter a letter: ");
        do
        {
            ch = tolower(getonechar());
            if (ch == unsuccessful)
                printf("\nEnter only one letter:");
            else if (ch < 'a' || ch > 'z') //in case of the single char from the input is not an alphabet.
                printf("\nYou entered non-alphabet.\nEnter a letter:");
        }
        while (ch == unsuccessful || (ch < 'a' || ch > 'z')); //requires an input of alphabetical char.

        if (strchr(hidden, ch) != NULL || strchr(misses, ch) != NULL) //in case letter is repeated.
            printf("\nYou already entered this letter\n");
        else
        {
            if (mistake_check(original, ch, & p) == True)   //in case the user makes a mistake.
            {
                add_drawing(drawing, mistake, mistake_limit); //adds a piece of drawing accordingly.
                printf("\n/*Wrong letter! Focus more :)*/\n");
                mistake++;
            }
            else
            {
                printf("\n/*Good job! Your guess is correct :)*/\n");
                hidden = update_hidden(original, hidden, length, ch); //updates the hidden string so the guessed letters show up.
            }

            printf("YOUR WORD : %s\nMISSED LETTERS: %s\n", hidden, misses);
            for (row = 0; row < 10; row++)   //printing the current figure of the hangman on the screen.
            {
                for (column = 0; column < 12; column++)
                    printf("%c", drawing[row][column]);
                printf("\n");
            }
            printf("\n");
            if (strcmp(original, hidden) == 0)   //strcmp returns 0 in case of identical strings, hence the user wins.
            {
                FINISH = clock(); //marks the finishing time.
                DURATION = (double)(FINISH - START) / CLOCKS_PER_SEC; //finds the number of seconds taken to win the game.

                if(update_database(original, DURATION) == True)  //the function updates word triplets and returns True if the word is repeated.
                {
                    print_database();
                    printf("\n%s was guessed before.\n", original);
                }
                else
                {
                    print_database();
                    printf("\n%s was never guessed before.\n", original);
                }
                printf("Congratulations! You guessed it in %.3f seconds\n", DURATION);
                printf("You can find the scores above.\n");

                free(original); //freeing the respective memory allocations created in stringcopy() and hide() functions.
                free(hidden);
                original = NULL;
                hidden = NULL;
                return restart(); //restart() returns either True or False.
            }
            if (mistake == mistake_limit)   //in case the user loses by reaching the number of misses allowed.
            {
                print_database();
                printf("\nToo bad! Your word was %s\n", original);
                free(original);
                free(hidden);
                original = NULL;
                hidden = NULL;
                return restart();

            }
        }

    }
}

int main()   //main program. it first reads the words from text file into 2D dynamic array then starts the game loop.
{
    char restart;
    FILE * fp;
    char str[64];
    int x = 0;
    int wordcount = 0;
    size_t length;
    int num;

    printf("WELCOME TO THE HANGMAN!\n\n"); //Gamemenu.
    do
    {
        print_line();
        printf("1.)Start the game.\n");
        printf("2.)Winners' records.\n");
        printf("3.)Exit the game.\n\n");
        do
        {
            printf("Select 1,2 or 3:");
            num = getonenum(); //num becomes unsuccessful if the input is not numeric.
        }
        while(num != 1 && num != 2 && num != 3);
        if (num == 2)
            print_database();
        if (num == 3)
            return 0;
    }
    while (num != 1);

    fp = fopen("words.txt", "r"); //in read mode.
    if (fp == NULL)
    {
        printf("File open failed.");
        return 1;
    }
    while (fgets(str, sizeof(str), fp) != NULL)   //counts the number of words in the txt file.
    {
        ++wordcount;
        length = strlen(str) - 1;
        if (max_length < length)
            max_length = (int) length; //defines the longest length.
        if (min_length > length)
            min_length = (int) length; //defines the shortest length.
    }

    fseek(fp, 0, SEEK_SET); //moves the file pointer back to the start of the file.

    char ** d = (char ** ) malloc(wordcount * sizeof(char * )); //creates a double pointer pointing to an array of pointers as many as wordcount.
    if (d == NULL)
    {
        printf("Memory allocation failed");
        return 1;
    }

    while (fgets(str, sizeof(str), fp) != NULL)   //reads from the text line by line.
    {
        length = strlen(str); //this counts \n character as well.
        d[x] = (char * ) malloc(length * sizeof(char)); //allocating memory for each of the pointers in the array previously created, hence forming 2D dynamic array.
        if (d[x] == NULL)
        {
            printf("Memory allocation failed");
            return 1;
        }
        strcpy(d[x], str);
        d[x][length - 1] = '\0'; //replaces \n with \0, hence forming strings.
        ++x;
    }
    fclose(fp);

    do
    {
        restart = Gameloop(d, wordcount); //gameloop returns True if the user continues playing; otherwise, False.
        //If any of the file openings and memory allocations fails during gameloop(), they will all return to Gameloop and to main() with a value False.
        //this ensures freeing all of the successful allocations, if any, during the program and exiting the program from main function.
    }
    while (restart != False);

    for (x = 0; x < wordcount; x++)  //Freeing the 2D dynamic array.
    {
        free(d[x]);
        d[x] = NULL;
    }
    free(d);
    d = NULL;

    printf("\nTHANKS FOR PLAYING!\n");

    return 0;
}
