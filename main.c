#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>

int score = 0;
int gameover = 0;
int fruit_x, fruit_y;
int body_x[100], body_y[100]; //int array käärmeen kropalle, kroppa max koko pelilaudan koko
int head_x, head_y;
int width = 30, height = 30;
int worm_size = 1;
int dir = 1;
int speed = 80;

void snake_board();
void init_game();
void input_check();
char kb_press();
void move_snake();
void make_fruit();
void fruit_collision();
void gameover_check();
void game();

int main()
{
    init_game();
    make_fruit();
    game();
    return 0;
}

//Alustetaan peli niin, että mato on keskellä pelipöytää
void init_game()
{
    head_x = height / 2;
    head_y = width / 2;
}

//Peli-loop
void game()
{
    while(!gameover)
    {
        snake_board();
        input_check();
        move_snake();
        fruit_collision();
        gameover_check();
    }
    system("cls"); //Tarvitaan windowsille
    printf("\nPisteet: %d\n\nPeli loppu!\n\n", score);
}

//Peli päättyy jos mato törmää seinään tai itseensä
void gameover_check()
{
    int i;
    for(i = 0; i < worm_size; i++)
    {
        if(head_x == body_x[i] && head_y == body_y[i])                      //Tilanne jossa mato törmää itseensä
        {
            gameover = 1;
        }
    }
    if(head_x == width || head_x == 0 || head_y == height || head_y == 0)   //Tilanne jossa mato törmää pelialueen reunoihin
    {
        gameover = 1;
    }
}


//Pelin renderöinti
void snake_board()
{
    system("cls");                                              //Tarvitaan windowsille
    int i,j,k;
    int no_body;                                                //Boolean-tyyppinen muuttuja joka määrittää onko madolla kroppa
    for(j = 0; j <= height; j++)                                //Ulompi loop kiertää korkeuden verran
    {
        for(i = 0; i <= width; i++)                             //Sisempi loop kiertää leveyden verran
        {
            no_body = 1;
            if(i == 0 || j == 0 || j == height || i == width)   //Rajojen tarkistus
                printf("#");                                    //Pelin seinien tulostus alueen rajoille
            else if(i == fruit_x && j == fruit_y)               //Tulostetaan hedelmä jos kordinaatit vastaavat hedelmän sijaintia
                printf("*");                                    //Hedelmä
            else if(i == head_x && j == head_y)                 //Tulostetaan pää jos kordinaatit vastaavat pään sijaintia
                printf("Q");                                    //Madon pää
            else
            {
                for(k = 0; k < worm_size; k++)                  //Madon kropan sijainnin määritys
                {
                    if(i == body_x[k] && j == body_y[k])        //Tulostetaan kroppa jos kordinaatit vastaavat kropan sijaintia
                    {
                        printf("o");                            //Madon kroppa
                        no_body = 0;
                    }
                }
                if(no_body)                                     //Mikäli ei käärmeen kroppaa, tulostaa tyhjää
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Pisteet: %d\n", score);
    fflush(stdout);                                             // Tarvitaan macOS:lle
}

//Näppäimistön painallus ja painetun näppäimen palautus
char kb_press()
{
    initscr();
    timeout(speed);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    char key = getch();
    endwin();
    return key;
}

/**
Käsittelee näppäimen painalluksen sekä määrittää mihin suuntaan mato liikkuu.
Dir-muuttuja on numeerinen muuttuja, joka symboloi madon suuntaa.
Suunnat ovat 1,2,3 ja 4 eli ylös, oikea, alas ja vasen.
If-lohkot tarvitaan estämään vastakkaisten liikkeiden samanaikaisuus.
**/
void input_check()
{
    switch (kb_press())
    {
        case 'w':
            if(dir!=3)
                dir=1;
            break;
        case 'd':
            if(dir!=4)
                dir=2;
            break;
        case 's':
            if(dir!=1)
                dir=3;
            break;
        case 'a':
            if(dir!=2)
                dir=4;
            break;
    }
}


//Käärmeen liikkeet
void move_snake() 
{
    int x1,x2,y1,y2;
    int i;
    if(worm_size == 1)
    {
        body_x[0] = head_x;                 //Madon pää määritetään arrayn ensimmäiseksi
        body_y[0] = head_y;
    }
    else
    {
        x1 = head_x;                        //Pään sijainti
        y1 = head_y;
        for(i = 0; i < worm_size; i++)      //Pään paikasta kokoa kasvatetaan madon koon verran
        {
            x2 = body_x[i];
            y2 = body_y[i];
            body_x[i] = x1;
            body_y[i] = y1;
            x1 = x2;
            y1 = y2;
        }
    }
    switch (dir)                            //Määritellään mihin suuntaan mato liikkuu
    {
        case 1:
            head_y--;
            break;
        case 2:
            head_x++;
            break;
        case 3:
            head_y++;
            break;
        case 4:
            head_x--;
            break;
    }
}

//Luo uusia hedelmiä pelipöydälle
void make_fruit()
{
    reset_fruit_x: fruit_x = rand() % 20;
    if(fruit_x == 0 ||fruit_x == width)
        goto reset_fruit_x;
    reset_fruit_y: fruit_y = rand() % 20;
    if(fruit_y == 0 || fruit_y == height)
        goto reset_fruit_y;
}

//Tilanne, jossa käärme syö hedelmän
void fruit_collision()
{
    if(head_x == fruit_x && head_y == fruit_y)
    {
        score += 10;
        worm_size++;
        make_fruit();
    }
}
