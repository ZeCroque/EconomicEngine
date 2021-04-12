### DesignPattern
- add signal slot **(croque)**

### General
- wait for thread closing in sfml project by using signal slots **croque**
- remplacer observer par signal slot **croque**
- enlever les références des singletons dans nos objets et remplacer les accès par waitForLock(Singleton<T>* singleton) **croque**

### Economic:
- time rate pour cycle journalier
- temps travail relatif à durée cycle journalier
- trader décorellés de tick de la bourse et suivant logique tour par tour (boulot/vente)
- bourse à chaque offre
- bouffe à chaque action

### Graphic:
- ActorFactory **croque**
- affichage **croque**
    - actor **croque**
    - dezoom = réduire taille case grille graphique
    - scroll 
        - bouger intervalle grille logique
        - gérer position souris avec accélération mini jeu adn
- déplacements
    - vitesse des ias corellées à timeRate journalier **maxence**
    - astar **maxence**
- génération bâtiments **vincent**

### Gui:
- investiguer et clean les intéractions entre le turnManager et la gui **croque**
- speed = daycycle speed **croque**
- *virer step-by-step & step*


### Polish :
- énérer hdv intervalle régulier **vincent**
- cycle jour nuit et tâche dodo
- remplacer tour par tour traders par algo meilleur choix **maxence**
- hint au dessus workshop **maxence**