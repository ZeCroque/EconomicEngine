### General
- remplacer observer par signal slot **croque**

### Economic:
- add economic engine.cpp
- time rate pour cycle journalier
- temps travail relatif à durée cycle journalier
- trader décorellés de tick de la bourse et suivant logique tour par tour (boulot/vente)
- bourse à chaque offre
- bouffe à chaque action

### Graphic:
- affichage 
    - actor **croque**
    - dezoom = réduire taille case grille graphique **croque**
    - scroll **croque**
        - bouger intervalle grille logique **croque**
        - gérer position souris avec accélération mini jeu adn **croque**
- déplacements
    - vitesse des ias corellées à timeRate journalier **maxence**
    - astar **maxence**
- génération bâtiments **vincent**

### Gui:
- investiguer et clean les intéractions entre le turnManager et la gui **croque**
- speed = daycycle speed **croque**
- *virer step-by-step & step*


### Polish :
- générer hdv intervalle régulier **vincent**
- cycle jour nuit et tâche dodo
- remplacer tour par tour traders par algo meilleur choix **maxence**
- hint au dessus workshop **maxence**