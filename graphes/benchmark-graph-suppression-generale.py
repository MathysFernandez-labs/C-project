import matplotlib.pyplot as plt

# Définition des 4 structures avec leurs fichiers et styles respectifs
# (Modifiez les noms de fichiers si vos .txt s'appellent différemment)
structures = {
    'Table de Hachage': {'fichier': 'TP1/debug/hash/mesures_hash.txt', 'couleur': 'green', 'marqueur': '^'},
    'Arbre Binaire': {'fichier': 'TP2/debug/tree/mesures_tree.txt', 'couleur': 'purple', 'marqueur': 'd'},
    'Arbre AVL': {'fichier': 'TP3/debug/mesures_avl.txt', 'couleur': 'blue', 'marqueur': 'o'},
    'Arbre Rouge-Noir': {'fichier': 'TP4/debug/mesures_rb.txt', 'couleur': 'red', 'marqueur': 's'}
}

plt.figure(figsize=(10, 6))
fichiers_trouves = 0

for nom_structure, config in structures.items():
    tailles_n = []
    temps_insertion = []
    
    try:
        with open(config['fichier'], "r") as fichier:
            next(fichier)
            
            for ligne in fichier:
                valeurs = ligne.strip().split()
                if len(valeurs) >= 2:
                    tailles_n.append(int(valeurs[0]))
                    temps_insertion.append(float(valeurs[3]))

        # Tracé de la courbe pour cette structure spécifique
        plt.plot(tailles_n, temps_insertion, label=nom_structure, color=config['couleur'], marker=config['marqueur'], markersize=4)
        fichiers_trouves += 1

    except FileNotFoundError:
        print(f"Attention mdr le fichier est intouvable de "+nom_structure)

if fichiers_trouves > 0:
    plt.title("Comparaison des temps de suppression")
    plt.xlabel("Nombre de données (N)")
    plt.ylabel("Temps d'exécution (secondes)")
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend()

    # Sauvegarde du graphique en image PNG
    nom_image = 'graphique_comparaison_suppression.png'
    plt.savefig(nom_image, dpi=300, bbox_inches='tight')
    print(f"\nGraphique généré avec succès : {nom_image}")

    # Affichage interactif
    plt.show()
else:
    print("\nErreur : Aucun fichier de mesures n'a été trouvé. Le graphique n'a pas pu être généré.")