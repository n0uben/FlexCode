#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 6000
#define MAX_BUFFER 3000
#define MAX_CLIENTS 3
#define EXIT "exit"
#define ADRESSE_SERVEUR "127.0.0.1"

//Prototypes fonctions
void lireCommande(char tampon[], char *message);
int testQuitter(char tampon[]);
int ouvrirUneConnexionTcp(void);

int main(void)
{
    int fdSocket;
    int nbRecu;
    char tampon[MAX_BUFFER];

    fdSocket = ouvrirUneConnexionTcp();

    char *entete = "('-. .-.                _ (`-.                               _ .-') _     ('-.\n"
                   "( OO )  /               ( (OO  )                             ( (  OO) )  _(  OO)\n"
                   ",--. ,--. .-'),-----.  _.`     \\         .-----.  .-'),-----. \\     .'_ (,------.\n"
                   "|  | |  |( OO'  .-.  '(__...--''        '  .--./ ( OO'  .-.  ',`'--..._) |  .---'\n"
                   "|   .|  |/   |  | |  | |  /  | |        |  |('-. /   |  | |  ||  |  \\  ' |  |\n"
                   "|       |\\_) |  |\\|  | |  |_.' |       /_) |OO  )\\_) |  |\\|  ||  |   ' |(|  '--.\n"
                   "|  .-.  |  \\ |  | |  | |  .___.'       ||  |`-'|   \\ |  | |  ||  |   / : |  .--'\n"
                   "|  | |  |   `'  '-'  ' |  |           (_'  '--'\\    `'  '-'  '|  '--'  / |  `---.\n"
                   "`--' `--'     `-----'  `--'              `-----'      `-----' `-------'  `------'\n";

    printf("%s", entete);

    // on reçoit le catalogue produit du serveur
    nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);

    // si catalogue non reçu
    if (nbRecu <= 0)
    {
        exit(EXIT_FAILURE);
    }

    //on affiche le catalogue
    tampon[nbRecu] = 0;
    printf("Recu : %s\n", tampon);

    do
    {

        //on demande à l’utilisateur le produit désiré
        lireCommande(tampon, "Veuillez saisir le numéro (1, 2, 3, …) du produit désiré :");

        // on envoie au serveur l'id du produit désiré
        send(fdSocket, tampon, strlen(tampon), 0);

        // on reçoit les infos du produit désiré
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);
        if (nbRecu <= 0)
        {
            exit(EXIT_FAILURE);
        }
        tampon[nbRecu] = 0;

    } while (strcmp(tampon, "-1") == 0);

    //on affiche le produit désiré
    printf("%s\n", tampon);


    do
    {
        lireCommande(tampon, "Quelle quantité désirez vous ?");

        // on envoie au serveur la quantite désirée
        send(fdSocket, tampon, strlen(tampon), 0);

        // on reçoit la facture
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);

        if (nbRecu <= 0)
        {
            exit(EXIT_FAILURE);
        }
        tampon[nbRecu] = 0;

    } while (strcmp(tampon, "-1") == 0);

    printf("%s\n", tampon);


    close(fdSocket);

    return EXIT_SUCCESS;
}

int ouvrirUneConnexionTcp(void)
{
    int socketTemp;
    int longueurAdresse;
    struct sockaddr_in coordonneesServeur;

    socketTemp = socket(AF_INET, SOCK_STREAM, 0);

    if (socket < 0)
    {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    // On prépare les coordonnées du serveur
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coordonneesServeur, 0x00, longueurAdresse);

    // connexion de type TCP
    coordonneesServeur.sin_family = PF_INET;
    // adresse du serveur
    inet_aton("127.0.0.1", &coordonneesServeur.sin_addr);
    // le port d'écoute du serveur
    coordonneesServeur.sin_port = htons(PORT);

    if (connect(socketTemp, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1)
    {
        printf("connexion impossible\n");
        exit(EXIT_FAILURE);
    }

    printf("connexion ok\n");

    return socketTemp;
}

void lireCommande(char tampon[], char *message)
{
    printf("%s\n", message);
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[])
{
    return strcmp(tampon, EXIT) == 0;
}
