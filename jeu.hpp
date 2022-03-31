#ifndef JEU_HPP
#define JEU_HPP

#include <list>
#include <vector>

using namespace std;

typedef enum {VIDE, MUR} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;
typedef enum {FACILE, MOYEN, DIFFICILE} Difficulte;//difficult� du jeu

class Jeu;

class Fantome
{
friend class Jeu;

protected:
    int previousPosX, previousPosY;//position pr�c�dente du fant�me
    int posX, posY;
    Direction dir;

public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
    int getPreviousPosX() const;
    int getPreviousPosY() const;
};

class Bomb
{
private:
    int x, y;//position de bombe
    int range;//rayon d'explosion de la bombe (en 4 direction)
    int timer;//horloge bombe
    int start, stop;//start est l'heure de d�but de l'explosion, stop est l'heure de mettre fin � l'explosion
public:
    Bomb(int, int, int, int, int);
    int getX() const;
    int getY() const;
    int getRange() const;
    void countTimer();//Mise � jour de l'horloge de Bomb
    bool BombStart() const;//vrai si la bombe est commence
    bool BombStop() const;//vrai si la bombe a explos�
};

class Jeu
{
  protected:
    int time_present, time_collision;//heure actuelle et heure de la collision
    Case *terrain;
    int pointTotal;//score total du jeu (nombre de cerises)
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPacmanX, posPacmanY;
    Direction dirPacman;
    int pointPacman;//score de pacman
    int viePacman;//nombre de vie de pacman
    int powerPacman;//le puissance de pacman
    bool power;//variable binaire pour reconna�tre l'�tat de pacman (vrai si pacman est en mode puissance)
    bool stop;
    Difficulte niveau;

    bool modePacman;//vrai pour mode Pacman et faux pour mode Bomberman

  public:
    std::list<Fantome> fantomes;
    std::vector<std::vector<bool>> eaten;//Tableau 2D repr�sentant l'�tat des cerises

    std::list<Bomb> bombes;
    std::vector<std::vector<bool>> fire;//Tableau 2D repr�sentant les explosions

  public:
    Jeu();
    Jeu(const Jeu &)=delete;
    ~Jeu();

    Jeu &operator=(const Jeu &)=delete;

    bool init(bool, Difficulte);//commencer le jeu avec un nombre donn� de fant�mes
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;

    // Retourne la case � une position donn�e
    Case getCase(int, int) const;

    // Indique si la case � une position donn�e existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // D�place Pacman dans une direction (si la case � atteindre est valide)
    bool deplacePacman(Direction);

/*
    void AjoutFantome();
    void SupprFantome();
*/

    bool collision();

    void Eat();//manger la cerise � l'emplacement actuel de pacman

    int getViePacman() const;//Renvoie le nombre de vies de pacman

    bool isDead() const; // on a perdu?
    bool isWin() const; // on a gagne?

    bool isPower();//Pacman est en mode puissance ?

    void setDifficulte(Difficulte);//changer difficulte de jeu
    Difficulte getDifficulte() const;//renvoie difficulte actuel de jeu

    void setDirection(Direction);//changer direction de pacman

    bool isOpposite(Fantome) const;//v�rifier si le fant�me est devant pacman ou non

    void setStop(bool);

    void Pacman();//choisissez le mode Pacman
    void Bomberman();//choisissez le mode Bomberman
    bool getMode() const;//retourner le mode courant (vrai pour mode Pacman et faux pour mode Bomberman

    void addBomb(int, int, int, int, int);//ajouter une bombe
    void updateTimer();//Mettre � jour l'horloge de chaque bombe
    void explode();//Mettre � jour les emplacements touch�s par les explosions
    void kill();//�liminez les fant�mes situ�s dans la zone d'explosion
    void deleteBomb();//supprimer la bombe de la liste si elle a explos�
};

#endif
