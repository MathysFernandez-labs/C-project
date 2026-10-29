import matplotlib.pyplot as plt

# Initialisation des listes pour stocker les colonnes
tailles_n = []
temps_insertion = []
temps_recherche = []
temps_suppression = []

try:
    # with open("debug/mesures_avl_dict.txt", "r") as fichier:
    with open("debug/mesures_avl_int.txt", "r") as fichier:
        # Ignorer la première ligne
        next(fichier)
        
        for ligne in fichier:
            valeurs = ligne.strip().split()
            if len(valeurs) == 4:
                tailles_n.append(int(valeurs[0]))
                temps_insertion.append(float(valeurs[1]))
                temps_recherche.append(float(valeurs[2]))
                temps_suppression.append(float(valeurs[3]))

    plt.figure(figsize=(10, 6))

    # Tracé des trois courbes
    plt.plot(tailles_n, temps_insertion, label='Insertion', color='blue', marker='o', markersize=4)
    plt.plot(tailles_n, temps_recherche, label='Recherche', color='green', marker='s', markersize=4)
    plt.plot(tailles_n, temps_suppression, label='Suppression', color='red', marker='^', markersize=4)

    # Personnalisation du graphique
    plt.title("Performances de l'Arbre AVL")
    plt.xlabel("Nombre de données (N)")
    plt.ylabel("Temps d'exécution (secondes)")
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend()

    # Sauvegarde du graphique en image PNG
    # nom_image = 'result/graphique_performances_avl.png'
    nom_image = 'result/graphique_performances_avl.png'
    plt.savefig(nom_image, dpi=300, bbox_inches='tight')
    print(f"Graphique généré avec succès : {nom_image}")

    # Affichage interactif
    plt.show()

except FileNotFoundError:
    print("Erreur : Le fichier 'mesures_avl.txt' est introuvable.")
    

