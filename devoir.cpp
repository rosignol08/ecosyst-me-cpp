#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
//test automatisme
#include <chrono>
#include <thread>
#define POPULATION_LIMIT 50
using namespace std;
#define INT_MAX 9000//histoire d'être tranquille
//de https://stackoverflow.com/questions/27490762/how-can-i-convert-to-size-t-from-int-safely
size_t int2size_t(int val) {
    return (val < 0) ? __SIZE_MAX__ : (size_t)((unsigned)val);
}
//et 
int size_t2int(size_t val) {
    return (val <= INT_MAX) ? (int)((ssize_t)val) : -1;
}


// Les classes : mouton, loup, herbe, minéraux

struct Herbe {
    public:
    int positionX;
    int positionY;
    int tour;
    bool estMangee;
    bool sous_mouton;
    bool sous_loup;
    bool mineral;
    bool vient_de_repousser;
    Herbe() {
        this->estMangee = false;
        this->tour = 0;
        this->sous_mouton = false;
        this->sous_loup = false;
        this->mineral = false;
        this->vient_de_repousser = false;
    }
};

class Mineraux {
public:
    bool existe;
    int positionX;
    int positionY;
    Mineraux(int positionX,int positionY) {
        this->existe = true;
        this->positionX = positionX;
        this->positionY = positionY;
    }
};

class Animal {
public:
    int faim;
    int age;
    int sexe;//0 femelle 1 male
    bool vivant;
    int rechargementReproduction;
    int positionX;
    int positionY;
};

class Mouton : public Animal {
public:
    Mouton(int faim, int age, int sexe, bool vivant) {
        this->faim = faim;
        this->age = age;
        this->sexe = sexe;//0 = femmelle 
        this->vivant = vivant;
        this->rechargementReproduction = 0;
    }
    void mange(Herbe& herbe) {
        if (!herbe.estMangee) {
            herbe.estMangee = true;
            herbe.tour = 0; 
            this->faim = 0;
            cout << "Un mouton mange de l'herbe en " << this->positionX << "," << this->positionY << endl;
        }
    }
    void seReproduire(Mouton& mouton, vector<Mouton>& moutons) {
        if (this->sexe != mouton.sexe) {
            Mouton moutonEnfant(0,0,rand() % 2, true);
            moutonEnfant.positionX = this->positionX;
            moutonEnfant.positionY = this->positionY;
            moutons.push_back(moutonEnfant); // Ajouter le mouton enfant à la liste des moutons
            cout << "Un mouton nee en " << moutonEnfant.positionX << "," << moutonEnfant.positionY << endl;
        }
    }

    void estMort() {
        if (this->faim == 5) {
            this->vivant = false;
            cout << "Un mouton est mort en " << this->positionX << "," << this->positionY << endl;
        }
    }
};





class Loup : public Animal {
public:
    Loup(int faim, int age, int sexe, bool vivant) {
        this->faim = faim;
        this->age = age;
        this->sexe = sexe; //0 = femmelle 
        this->vivant = vivant;
        this->rechargementReproduction = 0;

    }
    void manger(Mouton& mouton) {
        if(this->positionX == mouton.positionX && this->positionY == mouton.positionY){
            if (mouton.vivant) {
                mouton.vivant = false;
                this->faim = 0;
                cout << "Un loup mange un mouton en " << this->positionX << "," << this->positionY << endl;
            }
        }
    }
    void seReproduire(Loup& loup, vector<Loup>& loups) {
        if (this->sexe != loup.sexe) {
            Loup loupEnfant(0,0,rand() % 2, true);
            loupEnfant.positionX = this->positionX;
            loupEnfant.positionY = this->positionY;
            loups.push_back(loupEnfant); // Ajouter le loup enfant à la liste des moutons
            cout << "Un loup nee en " << loupEnfant.positionX << "," << loupEnfant.positionY << endl;
        }
    }
    void estMort() {
        if (this->faim == 10) {
            this->vivant = false;
            cout << "Un loup est mort en " << this->positionX << "," << this->positionY << endl;
        }
    }
};


//repousse de l'herbe
void repousse(Herbe& herbe) {
    if (herbe.estMangee == true && herbe.tour % 5 == 0 && herbe.sous_mouton == false && herbe.sous_loup == false) {//elle repousse que si aucun animal n'est dessus
        herbe.estMangee = false;
        herbe.vient_de_repousser = true;
        cout << "L'herbe repousse en " << herbe.positionX << "," << herbe.positionY << endl;
    }
    if (herbe.estMangee == true && herbe.mineral == true && herbe.sous_mouton == false && herbe.sous_loup == false) {//elle repousse que si aucun animal n'est dessus
        herbe.estMangee = false;
        herbe.mineral = false;
        herbe.vient_de_repousser = true;
        cout << "L'herbe repousse en " << herbe.positionX << "," << herbe.positionY << endl;
    }
}
      
void deplacement_mouton(Mouton& mouton, std::vector<std::vector<Herbe>>& univers) {
    int x;
    int y; 
    do {
        int direction = rand() % 4;
        x = mouton.positionX;
        y = mouton.positionY;
        switch (direction) {
        case 0:
            x--; //deplace vers le haut
            break;
        case 1:
            x++; //deplace vers le bas
            break;
        case 2:
            y--; //deplace vers la gauche
            break;
        case 3:
            y++; //deplace vers la droite
            break;
        }
    }
    
    while (!
            (
                (static_cast<size_t>(x) >= 0 && static_cast<size_t>(x) < univers.size() 
                    && static_cast<size_t>(y) >= 0 && static_cast<size_t>(y) < univers[0].size())
                &&
                (univers[x][y].estMangee == true || univers[x][y].estMangee == false)
            )
    );
    mouton.positionX = x;
    mouton.positionY = y;
    return;
}

void deplacement_loup(Loup& loup, std::vector<std::vector<Herbe>>& univers) {
    //srand(time(0));
    int x;
    int y; 
    do {
        int direction = rand() % 4;
        x = loup.positionX;
        y = loup.positionY;
        switch (direction) {
        case 0:
            x--; //deplace vers le haut
            break;
        case 1:
            x++; //deplace vers le bas
            break;
        case 2:
            y--; //deplace vers la gauche
            break;
        case 3:
            y++; //deplace vers la droite
            break;
        }
    }
    
    while (!
            (
                (static_cast<size_t>(x) >= 0 && static_cast<size_t>(x) < univers.size() 
                    && static_cast<size_t>(y) >= 0 && static_cast<size_t>(y) < univers[0].size())
                &&
                (univers[x][y].estMangee == true || univers[x][y].estMangee == false)
            )
    );
    loup.positionX = x;
    loup.positionY = y;
    return;
}


//fonction pour afficher l'univers avec les animaux
void afficherUnivers(std::vector<std::vector<Herbe>>& univers, const std::vector<Mouton>& moutons, const std::vector<Loup>& loups, int tour) {
    cout << "Tour " << tour << " | Loups : " << loups.size() << " | Moutons : " << moutons.size() << endl;

    //affiche de l'univers
    cout << " ";
    for (size_t i = 0; i < univers[0].size(); ++i) {
        //if (univers[0][i].estMangee == false)
        if (i < 10){
            cout << "  " << i << " ";
        }else{
            cout << " " << i << " ";
        }
        //else
        //    cout << "   ";
    }
    cout << endl;
    for (int i = 0; i < (size_t2int(univers.size())); ++i) {
    cout << " +";
    for (size_t j = 0; j < univers[i].size(); ++j) {
        cout << "---+";
    }
    cout << endl << char('A' + i) << "|";
    for (int j = 0; j < (size_t2int(univers[i].size())); ++j) {
        bool afficheMouton = false;
        bool afficheLoup = false;

        //verif si un mouton se trouve à cette position
        for (const Mouton& mouton : moutons) {
            if (mouton.positionX == i && mouton.positionY == j) {
                cout << " \x1B[34mM\033[0m |";
                afficheMouton = true;
                break;
            }
        }

        //verif si un loup se trouve à cette position
        for (const Loup& loup : loups) {
            if (loup.positionX == i && loup.positionY == j) {
                cout << " \x1B[31mL\033[0m |";
                afficheLoup = true;
                break;
            }
        }

        //si aucun animal n'est présent, afficher l'état de l'herbe
        if (!afficheMouton && !afficheLoup) {
            if(univers[i][j].mineral == true){
                cout << " X |"; // Herbe avec mineraux
                univers[i][j].mineral = false;
                univers[i][j].estMangee = false;
                univers[i][j].vient_de_repousser = true;
            }else if (univers[i][j].estMangee) {
                cout << "   |"; // Herbe mangée
            }else if(univers[i][j].vient_de_repousser==true){
                cout << " \x1B[32mH\033[0m |"; // Herbe repoussée
                univers[i][j].vient_de_repousser = false;
            }else {
                cout << " \x1B[32mH\033[0m |"; // Herbe non mangée
            }
        }
    }
    cout << endl;
}

    cout << " +";
    for (size_t i = 0; i < univers[0].size(); ++i) {
        cout << "---+";
    }
    cout << endl;
}

void ajoute_mouton(vector<Mouton>& moutons, int m, int n){
    int sexe = 2;
    cout << "indiquer le sexe de l'animal : 0 femelle et 1 male";
    cin >> sexe;
    while (sexe != 0 && sexe != 1){
        cout << "indiquer le sexe de l'animal : 0 femelle et 1 male";
        cin >> sexe;
    }
    Mouton ajoute = Mouton(0, 0, sexe, true);
    ajoute.positionX = rand() % m;
    ajoute.positionY = rand() % n;
    moutons.push_back(ajoute);
}

void ajoute_loup(vector<Loup>& loups, int m, int n){
    int sexe = 2;
    cout << "indiquer le sexe de l'animal : 0 femelle et 1 male";
    cin >> sexe;
    while (sexe != 0 && sexe != 1){
        cout << "indiquer le sexe de l'animal : 0 femelle et 1 male";
        cin >> sexe;
    }
    Loup ajoute = Loup(0, 0, sexe, true);
    ajoute.positionX = rand() % m;
    ajoute.positionY = rand() % n;
    loups.push_back(ajoute);
}

/*
void ajoute_mouton2(vector<Mouton> moutons){
    int sexe = 2;
}

void ajoute_loup2(vector<Loup> loups){
    int sexe = 2;
}
*/

int main(void) {
    srand(time(0)); //pour les deplacements aleratoires
    //bool enPause = false;
    //initialisation de la taille de l'univers
    int m, n, tour = 0;
    tour = 1;
    while (m == 0 || m > 20){
        cout << "Entrez le nombre de lignes de l'univers (max 20) : ";
        cin >> m;
    }
    while (n == 0 || n > 20){
        cout << "Entrez le nombre de colonnes de l'univers (max 20) : ";
        cin >> n;
    }
    int population_mouton = 0;
    int population_loup = 0;
    
    //initialisation de l'univers avec de l'herbe et des animaux
    std::vector<std::vector<Herbe>> univers(m, vector<Herbe>(n));
    //initialisation des moutons et des loups
    vector<Mouton> moutons = {Mouton(0, 0, 0, true), Mouton(0, 0, 0, true), Mouton(0, 0, 1, true), Mouton(0, 0, 0, true), Mouton(0, 0, 1, true), Mouton(0, 0, 0, true), Mouton(0, 0, 1, true), Mouton(0, 0, 0, true), Mouton(0, 0, 1, true)}; // Ajoute des moutons avec des attributs initiaux
    vector<Loup> loups = {Loup(0,0,0,true), Loup(0,0,1,true), Loup(0,0,0,true), Loup(0,0,1,true), Loup(0,0,0,true), Loup(0,0,1,true)}; // Ajoute 1 loup avec des attributs initiaux
    cout << "Dimensions de l'univers : " << m << " lignes x " << n << " colonnes" << endl;

    //initialisation des positions des moutons et des loups
    for (Mouton& mouton : moutons) {
        mouton.positionX = rand() % m;
        mouton.positionY = rand() % n;
        //cout << "Position du mouton : " << mouton.positionX << ", " << mouton.positionY << endl;
    }

    for (Loup& loup : loups) {
        loup.positionX = rand() % m;
        loup.positionY = rand() % n;
    }

    //initialisation des positions des moutons et des loups
    for (Mouton& mouton : moutons) {
        population_mouton++;
        univers[mouton.positionX][mouton.positionY].sous_mouton = true;
        
    }

    for (Loup& loup : loups) {
        population_loup++;
        univers[loup.positionX][loup.positionY].sous_loup = true;
    }
    //boucle de simulation : tant qu'il y a des moutons et des loups
    while (!moutons.empty() || !loups.empty()) {

        //enlève les mineraux au tour d'apres
        for (size_t i = 0; i < univers.size(); ++i) {
            for (size_t j = 0; j < univers[i].size(); ++j) {
                univers[i][j].tour++;
                if (univers[i][j].estMangee == true) {
                    repousse(univers[i][j]);
                }
            }
        }

        //deplacement des moutons et mise a jour faim des loups
        for (Mouton& mouton : moutons) {
            mouton.faim++;
            mouton.age++;
            mouton.rechargementReproduction++;
            deplacement_mouton(mouton, univers);
            mouton.mange(univers[mouton.positionX][mouton.positionY]);
            mouton.age++;
            cout << "position du mouton : " << mouton.positionX << "," << mouton.positionY << endl;
        }
    
        //deplacement des loups et mise a jour faim des loups
        for (Loup& loup : loups) {
            loup.faim++;
            loup.age++;
            loup.rechargementReproduction++;
            deplacement_loup(loup, univers);
            cout << "position du loup : " << loup.positionX << "," << loup.positionY << endl;
            //verif si le loup est sur la même case qu'un mouton
            for (auto it = moutons.begin(); it != moutons.end(); ) {
                if (loup.positionX == it->positionX && loup.positionY == it->positionY) {
                    loup.manger(*it); //appel de la fonction mange du loup
                    it = moutons.erase(it); //enlève le mouton mangé de la liste
                } else {
                    ++it;
                }
            }
        }
        //verif de la mort des moutons
        for (auto it = moutons.begin(); it != moutons.end();) {
            //cout << "faim du mouton : " << it->faim << endl;
            if (it->faim == 5 || it->age == 50) {
                univers[it->positionX][it->positionY].mineral = true; //met du sel minéral sur la case
                univers[it->positionX][it->positionY].sous_mouton = false; //met à jour l'état de la case
                cout << "Un mouton est mort en " << it->positionX << "," << it->positionY << endl;
                it = moutons.erase(it); //retire le mouton mort de la liste des moutons
                population_mouton--;
            } else if(it->vivant == false){
                it = moutons.erase(it); //retire le mouton mort de la liste des moutons
                population_mouton--;}
            else {
                ++it;
            }
        }
        
        //verif de la mort des loups
        for (auto it = loups.begin(); it != loups.end();) {
            //cout << "faim du loup : " << it->faim << endl;
            if (it->faim == 10 || it->age == 60) {
                it = loups.erase(it); //retire le loup mort de la liste des loups
                univers[it->positionX][it->positionY].mineral = true; //met du sel mineral sur la case
                univers[it->positionX][it->positionY].sous_loup = false; //met à jour l'état de la case
                cout << "Un loup est mort en " << it->positionX << "," << it->positionY << endl;
                population_loup--;
            } else {
                ++it;
            }
        }

        //gros bloc ridiculement long pour la reproduction des moutons
        for (size_t i = 0; i < moutons.size(); ++i) {
            Mouton& mouton1 = moutons[i];
            if (mouton1.sexe == 0 && mouton1.rechargementReproduction < 3) {//verifie si le mouton est une femelle
                for (size_t j = 0; j < moutons.size(); ++j) {
                    if (i != j) {
                        Mouton& mouton2 = moutons[j];
                        if (mouton1.positionX == mouton2.positionX && mouton1.positionY == mouton2.positionY && mouton1.sexe != mouton2.sexe) {
                            //verif si une case adjacente à la femelle est libre pour le bébé
                            int x = mouton1.positionX;
                            int y = mouton1.positionY;
                            bool adjacentCasesFree = false;
                            if (x >= 0 && x < (size_t2int(univers.size())) && y >= 0 && y < (size_t2int(univers[0].size()))) {
                                //verif des cases adjacentes (bazard)
                                if (x > 0 && (univers[x-1][y].sous_mouton == true || univers[x-1][y].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (x < (size_t2int(univers.size()))-1 && (univers[x+1][y].sous_mouton == true || univers[x+1][y].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (y > 0 && (univers[x][y-1].sous_mouton == true || univers[x][y-1].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (y < (size_t2int(univers[0].size()))-1 && (univers[x][y+1].sous_mouton == true || univers[x][y+1].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                //on se deplace pas pendant 1 tour
                                //if (univers[x][y].estMangee == true && adjacentCasesFree) {
                                //    mouton1.positionX = x;
                                //    mouton1.positionY = y;
                                //}
                            }
                            //pour la reproduction des moutons
                            mouton1.estMort();
                            mouton2.estMort();
                            if (population_mouton < POPULATION_LIMIT && adjacentCasesFree == true){
                                mouton1.seReproduire(mouton2, moutons);
                                mouton1.rechargementReproduction = 0;
                                population_mouton++;
                            }
                            break; //sort de la boucle interne apres la reproduction
                        }
                    }
                }
            }
        }

        //gros bloc ridiculement long pour la reproduction des loups
        for (size_t i = 0; i < loups.size(); ++i) {
            Loup& loup1 = loups[i];
            if (loup1.sexe == 0 && loup1.rechargementReproduction > 3) { //verifie si le mouton est une femelle
                for (size_t j = 0; j < loups.size(); ++j) {
                    if (i != j) {
                        Loup& loup2 = loups[j];
                        if (loup1.positionX == loup2.positionX && loup1.positionY == loup2.positionY && loup1.sexe != loup2.sexe) {
                            //verif si une case adjacente à la femelle est libre pour le bébé
                            int x = loup1.positionX;
                            int y = loup1.positionY;
                            bool adjacentCasesFree = true;
                            if (x >= 0 && x < (size_t2int(univers.size())) && y >= 0 && y < (size_t2int(univers[0].size()))) {
                                //verif des cases adjacentes (bazard)
                                if (x > 0 && (univers[x-1][y].sous_mouton == true || univers[x-1][y].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (x < (size_t2int(univers.size()))-1 && (univers[x+1][y].sous_mouton == true || univers[x+1][y].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (y > 0 && (univers[x][y-1].sous_mouton == true || univers[x][y-1].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                                if (y < (size_t2int(univers[0].size()))-1 && (univers[x][y+1].sous_mouton == true || univers[x][y+1].sous_loup == true)) {
                                    adjacentCasesFree = false;
                                }
                            }
                            //pour la reproduction des loups
                            loup1.estMort();
                            loup2.estMort();
                            if (population_loup < POPULATION_LIMIT && adjacentCasesFree == true){
                                loup1.seReproduire(loup2, loups);
                                loup1.rechargementReproduction = 0;
                                population_loup++;
                            }
                            break; //sort de la boucle interne apres la reproduction
                        }
                    }
                }
            }
        }
        // Affichage de l'univers et mise en pause si nécessaire
        afficherUnivers(univers, moutons, loups, tour);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (tour % 10 == 0) {
            int choix;
            cout << "Pause taper 1 pour ajouter un mouton, 2 pour ajouter un loup et 3 pour continuer la simulation" << endl;
            cin >> choix;
            while (choix < 1 && choix > 3){
                cout << "Pause taper 1 pour ajouter un mouton, 2 pour ajouter un loup et 3 pour continuer la simulation";
                cin >> choix;
            }
            if (choix == 1){
                ajoute_mouton(moutons, n, m);
            }
            else if (choix == 2){
                ajoute_loup(loups, n, m);
            }else{
                cin.ignore();
            }
        }
        ++tour;
        //cout << "actuellement : " << enPause << endl; // Affichage de l'état de la pause
        /*
        https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
        pour mettre en pause:

        char valeur = '\0';
        // Vérifier s'il y a une entrée utilisateur
        // Vérifier si une pause est demandée
        if (cin.peek() == 'p') {
            enPause = true;
            cin.ignore(); // Ignorer le caractère 'p' dans le buffer d'entrée
        }
        if (!enPause) {
            ++tour;
            afficherUnivers(univers, moutons, loups, tour);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        } else {
            cout << "Simulation en pause. Appuyez sur une touche pour continuer...";
            while (cin.get() != '\n'); // Attend une entrée utilisateur pour reprendre la simulation
            enPause = false; // Met à jour l'état de la pause
        }
        */
    }
    cout << "Appuyez sur Entrée pour fermer le programme...";
    cin.get(); //ça attend l'entrée de l'utilisateur
    cin.ignore(); //ça igniore la nouvelle ligne dans le buffer
    return 0;
}