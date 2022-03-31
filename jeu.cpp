#include <iostream>
#include <cmath>
#include <cassert>
#include "jeu.hpp"

using namespace std;

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
}

Jeu::~Jeu()
{
    if (terrain!=NULL)
        delete[] terrain;
}

bool Jeu::init(bool m, Difficulte d)
{
	modePacman=m;
	niveau=d;

	int x, y;
	list<Fantome>::iterator itFantome;

	const char terrain_defaut[15][21] =
	{
		"####################",
		"#........##........#",
		"#.#####..##...####.#",
		"#........##........#",
		"#..................#",
		"#......#....#......#",
		"#......#...##......#",
		"#####..#....#..#####",
		"#......##...#......#",
		"#......#....#......#",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####################"
    };

	largeur = 20;
	hauteur = 15;

	terrain = new Case[largeur*hauteur];

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else
                terrain[y*largeur+x] = VIDE;

    int nbFantomes;
    if (modePacman)
        nbFantomes=3*(d+1);//3, 6, 9 fantomes pour mode Pacman
    else
        nbFantomes=6*(d+1);//6, 12, 18 fantomes pour mode Bomberman

    fantomes.resize(nbFantomes);

	for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do
        {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }

    do
    {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    posPacmanX = x,
    posPacmanY = y;

    if (modePacman)//initialiser les variables nécessaires pour mode Pacman
    {
        eaten.assign(largeur, std::vector<bool>(hauteur, false));
        //faire une carte aléatoire pour les cerises
        for (int i=0; i<largeur; i++)
            for (int j=0; j <hauteur; j++)
                if ((rand()%2==0)&&(terrain[j*largeur+i]==VIDE))
                    eaten[i][j]=true;

        eaten[posPacmanX][posPacmanY]=true;

        //compter les cerises (pointTotal)
        pointTotal=0;
        for (int i=0; i<largeur; i++)
            for (int j=0; j <hauteur; j++)
                if ((!eaten[i][j])&&(terrain[j*largeur+i]==VIDE))
                    pointTotal++;

        viePacman=3;
        pointPacman=0;
        powerPacman=0;
        power=false;
    }
    else//initialiser les variables nécessaires au mode Bomberman
    {
        bombes.clear();
        fire.assign(largeur, std::vector<bool>(hauteur, false));
        viePacman=6;
    }

    time_present=0;
    time_collision=0;

    return true;
}

void Jeu::evolue()
{
    if ((!isDead())&&(!isWin())&&(!stop))
    {
        int testX, testY;
        list<Fantome>::iterator itFantome;

        int depX[] = {-1, 1, 0, 0};
        int depY[] = {0, 0, -1, 1};

        if (time_present%10==0)//parce que le paramètre de timer->start() est de 10 ms au lieu de 100 ms
        //il faut faire comme ça pour s'assurer que les fantomes bougent toutes les 100 ms
        {
            for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
            {
                testX = itFantome->posX + depX[itFantome->dir];
                testY = itFantome->posY + depY[itFantome->dir];

                if (terrain[testY*largeur+testX]==VIDE)
                {
                    itFantome->previousPosX = itFantome->posX;
                    itFantome->previousPosY = itFantome->posY;
                    itFantome->posX = testX;
                    itFantome->posY = testY;
                }
                else
                    itFantome->dir = (Direction)(rand()%4);
            }
        }
        time_present++;
        collision();//Vérifier la collision toutes les 10 ms
        if (!modePacman)//evolution de jeu en mode Bomberman
        {
            updateTimer();
            explode();
            kill();
            deleteBomb();
        }
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

int Jeu::getPacmanX() const
{
    return posPacmanX;
}

int Jeu::getPacmanY() const
{
    return posPacmanY;
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]==VIDE);
}

bool Jeu::deplacePacman(Direction dir)
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    testX = posPacmanX + depX[dir];
    testY = posPacmanY + depY[dir];

    if (posValide(testX, testY))
    {
        posPacmanX = testX;
        posPacmanY = testY;
        return true;
    }
    else
        return false;
}

/*
void Jeu::AjoutFantome()
{
    int x, y;
    do
    {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);
    Fantome f;
    f.posX=x;
    f.posY=y;
    f.dir=(Direction)(rand()%4);
    fantomes.push_back(f);
}

void Jeu::SupprFantome()
{
    if (!fantomes.empty())
        fantomes.pop_back();
}
*/

bool Jeu::collision()
{
    for (auto it=fantomes.begin(); it!=fantomes.end(); ++it)
    {
        int x=it->getPosX();
        int y=it->getPosY();
        if ((x==posPacmanX)&&(y==posPacmanY))
        {
            if (isPower()&&isOpposite(*it))//si pacman est en mode puissance et fantome est devant pacman
            {
                fantomes.erase(it);//supprimer ce fantome de la liste des fantomes
                powerPacman=0;//réinitialiser puissance de pacman
                power=false;
            }
            else
            {
                if (time_present-time_collision>=20)//la collision pacman et fantôme se produit si longeur
                //de temps entre l'heure actuelle et l'heure de la collision précédente est d'au moins 200ms
                {
                    time_collision=time_present;
                    viePacman--;
                    return true;
                }
            }
        }
    }
    return false;
}

void Jeu::Eat()
{
    if (modePacman)
        if (!eaten[posPacmanX][posPacmanY])//si la cerise n'a pas encore été mangée
        {
            eaten[posPacmanX][posPacmanY]=true;//marquer les cerises qui ont été mangées
            pointPacman++;//augmenter le score de pacman
            powerPacman++;//augmenter puissance de pacman
        }
}

bool Jeu::isDead() const
{
    return (viePacman==0);
}

bool Jeu::isWin() const
{
    //en mode de jeu Pacman, on gagne si on obtient le score maximum
    if (modePacman)
        return (pointPacman==pointTotal);
    //en mode Bomberman on gagne si tous les fantômes meurent
    else
        return (fantomes.size()==0);
}

int Jeu::getViePacman() const
{
    return viePacman;
}

bool Jeu::isPower()
{
    if (modePacman)
    {
        if ((!power)&&(powerPacman>=10))//si la puissance de pacman est supérieure à 10 et que pacman n'est pas encore en mode puissance
            power = true;//pacman est en mode puissance
        return power;
    }
}

Difficulte Jeu::getDifficulte() const
{
    return niveau;
}

void Jeu::setDirection(Direction dir)
{
    dirPacman = dir;
}

bool Jeu::isOpposite(Fantome f) const
{
    int xf = f.previousPosX;
    int yf = f.previousPosY;
    int x = posPacmanX;
    int y = posPacmanY;
    bool a = (dirPacman==HAUT)&&(x==xf)&&(y>yf);//vérifier si le fantôme est devant pacman quand pacman est à haut
    bool b = (dirPacman==BAS)&&(x==xf)&&(yf>y);//vérifier si le fantôme est devant pacman quand pacman est à bas
    bool c = (dirPacman==GAUCHE)&&(y==yf)&&(x>xf);//vérifier si le fantôme est devant pacman quand pacman est à gauche
    bool d = (dirPacman==DROITE)&&(y==yf)&&(xf>x);//vérifier si le fantôme est devant pacman quand pacman est à droite
    return (a||b||c||d);
}

void Jeu::Pacman()
{
    Difficulte d=niveau;
    init(true, d);
}

void Jeu::Bomberman()
{
    Difficulte d=niveau;
    init(false, d);
}

bool Jeu::getMode() const
{
    return modePacman;
}

void Jeu::setStop(bool s)
{
    stop = s;
}

Bomb::Bomb(int a, int b, int r, int sta, int sto)
{
    x=a; y=b; range=r; start=sta; stop=sto; timer=0;
}

int Bomb::getX() const
{
    return x;
}

int Bomb::getY() const
{
    return y;
}

int Bomb::getRange() const
{
    return range;
}

void Bomb::countTimer()
{
    timer++;
}

bool Bomb::BombStart() const
{
    return (timer>=start)&&(timer<=stop);
}

bool Bomb::BombStop() const
{
    return timer>stop;
}

void Jeu::addBomb(int posX, int posY, int range, int start, int stop)
{
    Bomb b(posX, posY, range, start, stop);
    bombes.push_back(b);
}

void Jeu::deleteBomb()
{
    //voici la solution pour le cas général (les bombes ont des paramètres différents). Dans le cas de bombes
    //identiques (avec les mêmes paramètres start et stop), il suffit de retirer la première bombe de la liste.
    for (auto it=bombes.begin(); it!=bombes.end(); it++)
    {
        if (it->BombStop())
            bombes.erase(it);
    }
}

void Jeu::updateTimer()
{
    for (auto it=bombes.begin(); it!=bombes.end(); it++)
        it->countTimer();
}

void Jeu::explode()
{
    for (auto it=bombes.begin(); it!=bombes.end(); it++)
    {
        bool state=it->BombStart();
        int x=it->getX(); int y=it->getY(); int range=it->getRange();
        fire[x][y]=state;
        int i=1; int j=1; int k=1; int l=1;
        //mise à jour du statut des positions de droite
        while ((terrain[y*largeur+(x+i)]!=MUR)&&(i<=range))
        {
            fire[x+i][y]=state;
            i++;
        }
        //mise à jour du statut des positions de gauche
        while ((terrain[y*largeur+(x-j)]!=MUR)&&(j<=range))
        {
            fire[x-j][y]=state;
            j++;
        }
        //mise à jour du statut des position en bas
        while ((terrain[(y+k)*largeur+x]!=MUR)&&(k<=range))
        {
            fire[x][y+k]=state;
            k++;
        }
        //mise à jour du statut des position en haut
        while ((terrain[(y-l)*largeur+x]!=MUR)&&(l<=range))
        {
            fire[x][y-l]=state;
            l++;
        }
    }
}

void Jeu::kill()
{
    //effacer les fantômes affectés par les explosions
    for (auto it=fantomes.begin(); it!=fantomes.end(); ++it)
    {
        if (fire[it->getPosX()][it->getPosY()])
            fantomes.erase(it);
    }
    //Pacman meurt également s'il est touché par une bombe qui explose
    if (fire[posPacmanX][posPacmanY])
        viePacman=0;
}
