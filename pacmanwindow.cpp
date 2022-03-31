#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanButton::PacmanButton(QWidget *pParent):QPushButton(pParent){}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
    if (parent()!=NULL)
        QCoreApplication::sendEvent(parent(), e);
}

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (coeur.load("./data/heart.bmp")==false)//charger 'icone de vie
    {
        cout<<"Impossible d'ouvrir coeur"<<endl;
        exit(-1);
    }

    if (pixmapGommage.load("./data/cherry.bmp")==false)//charger l'icone de gommage
    {
        cout<<"Impossible d'ouvrir gommage"<<endl;
        exit(-1);
    }

    if (pixmapPacmanDroite.load("./data/pacmanR.bmp")==false)//charger l'icone pacman à droite
    {
        cout<<"Impossible d'ouvrir pacman"<<endl;
        exit(-1);
    }

    if (pixmapPacmanGauche.load("./data/pacmanL.bmp")==false)//charger l'icone pacman à gauche
    {
        cout<<"Impossible d'ouvrir pacman"<<endl;
        exit(-1);
    }

    if (pixmapPacmanHaut.load("./data/pacmanU.bmp")==false)//charger l'icone pacman à haut
    {
        cout<<"Impossible d'ouvrir pacman"<<endl;
        exit(-1);
    }

    if (pixmapPacmanBas.load("./data/pacmanD.bmp")==false)//charger l'icone pacman à bas
    {
        cout<<"Impossible d'ouvrir pacman"<<endl;
        exit(-1);
    }

    if (pixmapFantome_bleu.load("./data/fantome_bleu.bmp")==false)//charger l'icone fantome bleu
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapFantome_vert.load("./data/fantome_vert.bmp")==false)//charger l'icone fantome vert
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)//charger l'image de mur
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    if (pixmapBombe.load("./data/bomb.png")==false)//charger l'image de bombe
    {
        cout<<"Impossible d'ouvrir bomp.bmp"<<endl;
        exit(-1);
    }

    if (pixmapFeu.load("./data/fire.png")==false)//charger l'image de feu
    {
        cout<<"Impossible d'ouvrir fire.bmp"<<endl;
        exit(-1);
    }

    jeu.init(true, FACILE);//commencez le jeu avec le mode Pacman et le niveau facile

    pixmapPacman=pixmapPacmanDroite;//image initiale de pacman est pacman droite

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(10);//10ms au lieu de 100ms pour une meilleure détection des collisions

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, (jeu.getNbCasesY()+1)*hauteurCase);

/*
    PacmanButton *btnAjout = new PacmanButton(this);
    PacmanButton *btnSuppr = new PacmanButton(this);

    btnAjout->setText("Ajout");
    btnSuppr->setText("Suppr");

    btnAjout->move(222, 2);
    btnSuppr->move(322, 2);

    connect(btnAjout, &PacmanButton::clicked, this, &PacmanWindow::clickBoutonAjout);
    connect(btnSuppr, &PacmanButton::clicked, this, &PacmanWindow::clickBoutonSuppr);
*/

    AfficherVie = new QLCDNumber(this);//ecran pour afficher nombre de vies
    AfficherVie->move(540, 5);

    QMenuBar *menubar = new QMenuBar(this);//creer un barre de menu


    //créer un menu pour changer la difficulté du jeu (facile, moyen, difficile)
    QMenu *niveau = new QMenu("Level", this);//creer un menu "Level" pour changer difficulte de jeu
    QAction *facile = new QAction("Easy", this);//initier une action pour sélectionner le mode facile
    QAction *moyen = new QAction("Medium", this);//initier une action pour sélectionner le mode moyen
    QAction *difficile = new QAction("Hard", this);//initier une action pour sélectionner le mode difficle
    niveau->addAction(facile);//ajouter action à menu "Level"
    niveau->addAction(moyen);//ajouter action à menu "Level"
    niveau->addAction(difficile);//ajouter action à menu "Level"
    connect(facile, &QAction::triggered, this, &PacmanWindow::modeFacile);//l'action sera effectuée lorsque nous cliquons sur ce bouton d'action
    connect(moyen, &QAction::triggered, this, &PacmanWindow::modeMoyen);//l'action sera effectuée lorsque nous cliquons sur ce bouton d'action
    connect(difficile, &QAction::triggered, this, &PacmanWindow::modeDifficile);//l'action sera effectuée lorsque nous cliquons sur ce bouton d'action


    //créer un menu pour changer de mode de jeu (Pacman ou Bomberman)
    QMenu *mode = new QMenu("Mode", this);
    QAction *Pacman = new QAction("Pacman mode", this);
    QAction *Bomberman = new QAction("Bomberman mode", this);
    mode->addAction(Pacman);
    mode->addAction(Bomberman);
    connect(Pacman, &QAction::triggered, this, &PacmanWindow::modePacman);
    connect(Bomberman, &QAction::triggered, this, &PacmanWindow::modeBomberman);


    //créer un menu pour le tutoriel
    QMessageBox *HowToPlay = new QMessageBox(this);//créer un message pour guider le joueur
    HowToPlay->setWindowTitle("How to Play ?");//changer titre
    HowToPlay->setText(tr("Hello, Welcome to the game PACMAN-BOMBERMAN. "
        "This program is written by Huu Nhat Huynh (4A ACAD)\n\n"
        "=================Pacman Mode=================\n \n"
        "* Initially you will have 3 lives. When you touch a ghost, you lose 1 life. You will lose when you have no life left. \n \n"
        "* When you eat 10 cherries, you get strength. At this moment the ghosts will be green and you will have the ability to eat 1 ghost. "
        "After eating a ghost, you will lose your power and now the ghosts will be blue (Note that you can only eat the ghost if it is in "
        "front of you, otherwise you will still 1 life). \n \n"
        "* You will win if you have eaten all the cherries on the screen. \n \n"
        "================Bomberman Mode===============\n \n"
        "* In this game mode, you will initially have 6 lives. Just like Pacman, you will lose a life if you touch a ghost.\n \n"
        "* Use SPACEBAR to place bombs, each bomb will have an explosion radius of 3 units in 4 directions (left, right, top, bottom)"
        ", the explosion can't go through walls. The bomb will wait 3s and explode for 1s.\n \n"
        "* If you get hit by an exploding bomb, you lose instantly. Otherwise, you will lose when there are no lives left.\n \n"
        "* You win if you kill all the ghosts. \n \n"
        "==================GOOD LUCK=================="));
    QAction *instruction = new QAction("Instruction", this);//creer un menu pour afficher instruction
    connect(instruction, &QAction::triggered, HowToPlay, &QMessageBox::show);//le message apparaîtra lorsque le bouton "instruction" est pressé
    connect(instruction, &QAction::triggered, this, &PacmanWindow::stopJeu);//Le jeu s'arrête lorsque le bouton "instruction" est cliqué
    connect(HowToPlay, &QMessageBox::accepted, this, &PacmanWindow::continueJeu);//Le jeu continue lorsque le bouton OK est enfoncé
    connect(HowToPlay, &QMessageBox::rejected, this, &PacmanWindow::continueJeu);//Le jeu continue lorsque nous désactivons le message "instruction".


    //ajouter les 3 menus ci-dessus à la barre de menu
    menubar->addMenu(niveau);//ajouter menu "Level" à barre de menu
    menubar->addMenu(mode);
    menubar->addAction(instruction);//ajouter instruction à barre de menu


    //Créer une boîte de message pour annoncer le résultat du jeu (gagner ou perdre)
    notification = new QMessageBox(this);//créer un message qui indique quand on perd ou qu'on gagne
    notification->setWindowTitle("Notification");//changer le titre
    QPushButton *replay = notification->addButton("Replay", QMessageBox::AcceptRole);
    QPushButton *quit = notification->addButton("Quit", QMessageBox::AcceptRole);
    connect(replay, &QPushButton::clicked, this, &PacmanWindow::restart);
    connect(quit, &QPushButton::clicked, this, &QApplication::quit);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    list<Fantome>::const_iterator itFantome;
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    int decalageY = hauteurCase;

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
        for (x=0;x<jeu.getNbCasesX();x++)
            if (jeu.getCase(x,y)==MUR)
                painter.drawPixmap(x*largeurCase, y*hauteurCase+decalageY, pixmapMur);

    if (jeu.getMode())// Dessine les cerises si on est en mode Pacman
    {
        for (y=0;y<jeu.getNbCasesY();y++)
            for (x=0;x<jeu.getNbCasesX();x++)
                if ((!jeu.eaten[x][y])&&(jeu.getCase(x,y)!=MUR))
                    painter.drawPixmap(x*largeurCase, y*hauteurCase+decalageY, pixmapGommage);
    }
    else // Dessine les bombes si on est en mode Bomberman
    {
        for (auto itBombe=jeu.bombes.begin(); itBombe!=jeu.bombes.end(); itBombe++)
            painter.drawPixmap(itBombe->getX()*largeurCase, itBombe->getY()*hauteurCase+decalageY, pixmapBombe);

        for (int x=0; x<jeu.getNbCasesX(); x++)
            {
                for (int y=0; y<jeu.getNbCasesY(); y++)
                {
                    if (jeu.fire[x][y])
                        painter.drawPixmap(x*largeurCase, y*hauteurCase+decalageY, pixmapFeu);
                }
            }
    }

    // Dessine les fantomes
    if (jeu.getMode()&&jeu.isPower())
        pixmapFantome=pixmapFantome_vert;
    else
        pixmapFantome=pixmapFantome_bleu;

    for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
        painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase+decalageY, pixmapFantome);

    // Dessine Pacman
    painter.drawPixmap(jeu.getPacmanX()*largeurCase, jeu.getPacmanY()*hauteurCase+decalageY, pixmapPacman);

    AfficherVie->display(jeu.getViePacman());
    painter.drawPixmap(608, 0, coeur);

    if (jeu.isDead())
    {
        notification->setText("You're died! What do you want to do next ?");//changer le texte du message quand on perd
        notification->show();
    }
    if (jeu.isWin())
    {
        notification->setText("Good job! What do you want to do next ?");//changer le texte du message quand on gagne
        notification->show();
    }
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (!jeu.isDead()&&(!jeu.isWin()))
    {
        if (event->key()==Qt::Key_Left)
        {
            jeu.setDirection(GAUCHE);
            jeu.deplacePacman(GAUCHE);
            pixmapPacman=pixmapPacmanGauche;//changer la forme de pacman lorsque le bouton gauche est enfoncé
        }

        else if (event->key()==Qt::Key_Right)
        {
            jeu.setDirection(DROITE);
            jeu.deplacePacman(DROITE);
            pixmapPacman=pixmapPacmanDroite;//changer la forme de pacman lorsque le bouton droite est enfoncé
        }

        else if (event->key()==Qt::Key_Up)
        {
            jeu.setDirection(HAUT);
            jeu.deplacePacman(HAUT);
            pixmapPacman=pixmapPacmanHaut;//changer la forme de pacman lorsque le bouton haut est enfoncé
        }

        else if (event->key()==Qt::Key_Down)
        {
            jeu.setDirection(BAS);
            jeu.deplacePacman(BAS);
            pixmapPacman=pixmapPacmanBas;//changer la forme de pacman lorsque le bouton bas est enfoncé
        }
        else if (event->key()==Qt::Key_Space)
        {
            if (!jeu.getMode())//en mode Bomberman, appuyez sur la barre d'espace pour placer la bombe
                jeu.addBomb(jeu.getPacmanX(), jeu.getPacmanY(), 3, 300, 400);
        }

        jeu.Eat();

        update();
    }
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    update();
}

/*
void PacmanWindow::clickBoutonAjout()
{
    jeu.AjoutFantome();
}

void PacmanWindow::clickBoutonSuppr()
{
    jeu.SupprFantome();
}
*/

void PacmanWindow::restart()
{
    jeu.init(jeu.getMode(), jeu.getDifficulte());
}

void PacmanWindow::modeFacile()
{
    jeu.init(jeu.getMode(), FACILE);
}

void PacmanWindow::modeMoyen()
{
    jeu.init(jeu.getMode(), MOYEN);
}

void PacmanWindow::modeDifficile()
{
    jeu.init(jeu.getMode(), DIFFICILE);
}


void PacmanWindow::stopJeu()
{
    jeu.setStop(true);
}

void PacmanWindow::continueJeu()
{
    jeu.setStop(false);
}

void PacmanWindow::modePacman()
{
    jeu.Pacman();
}

void PacmanWindow::modeBomberman()
{
    jeu.Bomberman();
}




