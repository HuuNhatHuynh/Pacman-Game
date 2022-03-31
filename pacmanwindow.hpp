#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class PacmanWindow : public QFrame
{
protected:
    Jeu jeu;
    QPixmap pixmapPacmanDroite, pixmapPacmanGauche, pixmapPacmanHaut, pixmapPacmanBas;
    QPixmap pixmapPacman, pixmapFantome, pixmapFantome_bleu, pixmapFantome_vert, pixmapMur;
    QPixmap pixmapBombe;
    QPixmap pixmapFeu;
    QPixmap pixmapGommage;
    QPixmap coeur;
    QLCDNumber *AfficherVie;
    QMessageBox *notification;

public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();
    void restart();
    void stopJeu();
    void continueJeu();
/*
    void clickBoutonAjout();
    void clickBoutonSuppr();
*/
    void modeFacile();
    void modeMoyen();
    void modeDifficile();

    void modePacman();
    void modeBomberman();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
};

class PacmanButton : public QPushButton
{
public:
    PacmanButton(QWidget *pParent=0);
    void keyPressEvent(QKeyEvent *);
};

#endif
