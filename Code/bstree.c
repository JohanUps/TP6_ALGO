#include "bstree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


/*------------------------  BSTreeType  -----------------------------*/
typedef enum {red, black} NodeColor;

struct _bstree {
    BinarySearchTree* parent;
    BinarySearchTree* left;
    BinarySearchTree* right;
    NodeColor color;
    int key;
};

/*------------------------  BaseBSTree  -----------------------------*/

BinarySearchTree* bstree_create(void) {
    return NULL;
}

/* This constructor is private so that we can maintain the oredring invariant on
 * nodes. The only way to add nodes to the tree is with the bstree_add function
 * that ensures the invariant.
 */
BinarySearchTree* bstree_cons(BinarySearchTree* left, BinarySearchTree* right, int key) {
    BinarySearchTree* t = malloc(sizeof(struct _bstree));
    t->parent = NULL;
    t->left = left;
    t->right = right;
    t->color = red;
    if (t->left != NULL)
        t->left->parent = t;
    if (t->right != NULL)
        t->right->parent = t;
    t->key = key;
    return t;
}

void freenode(const BinarySearchTree* t, void* n) {
    (void)n;
    free((BinarySearchTree*)t);
}

void bstree_delete(ptrBinarySearchTree* t) {
    bstree_depth_postfix(*t, freenode, NULL);
    *t=NULL;
}

bool bstree_empty(const BinarySearchTree* t) {
    return t == NULL;
}

int bstree_key(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->key;
}

BinarySearchTree* bstree_left(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->left;
}

BinarySearchTree* bstree_right(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->right;
}

BinarySearchTree* bstree_parent(const BinarySearchTree* t) {
    assert(!bstree_empty(t));
    return t->parent;
}

/*------------------------  BSTreeDictionary  -----------------------------*/

/* Obligation de passer l'arbre par référence pour pouvoir le modifier */
void bstree_add(ptrBinarySearchTree* t, int v) {
    //Définition d'un curseur sur t
    ptrBinarySearchTree cursor = *t;
    ptrBinarySearchTree parent = NULL;

    //On traite dans un premier temps le cas ou l'arbre est vide
    if(bstree_empty(cursor)){
        *t = bstree_cons(NULL,NULL,v);
        return;
    }
    
    //Parcours de l'arbre jusqu'à ce que cursor pointe sur une feuille et parent sur le parent du noeud à ajouter
    while(!bstree_empty(cursor)){
        
        int cursor_key = bstree_key(cursor);
        parent = cursor;

        //Si la clé est déja dans l'arbre la fonction se stop
        if(cursor_key == v){
            return;
        }

        //Si v < cursor_key alors le nouveau cursor va pointer sur le fils gauche de l'actuel 
        if(v < cursor_key){
            cursor = bstree_left(cursor);
        }

        //Si v > cursor_key alors le nouveau cursor va pointer sur le fils droit de l'actuel 
        if(v > cursor_key){
            cursor = bstree_right(cursor);
        }    

    }

    //Creation du nouveau noeud
    ptrBinarySearchTree newNode = bstree_cons(NULL,NULL,v);

    //Mise a jour de pointeurs
    newNode->parent = parent;
    if(v > bstree_key(parent)){
        parent->right = newNode;
    }
    else{
        parent->left = newNode;
    }
}

const BinarySearchTree* bstree_search(const BinarySearchTree* t, int v) {

    const BinarySearchTree* cursor = t;
    while(!bstree_empty(cursor) && bstree_key(cursor) != v ){
        int cursor_key = bstree_key(cursor);
        if(cursor_key > v){
            cursor = bstree_left(cursor);
        }
        if(cursor_key < v){
            cursor = bstree_right(cursor);
        }

    }
    return cursor;
}

const BinarySearchTree* bstree_successor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    const BinarySearchTree* cursor = x;

    //Cas ou l'arbre a un fils droit
    if(!bstree_empty(bstree_right(cursor))){
        cursor = bstree_right(cursor);
        while(!bstree_empty(bstree_left(cursor))){
            cursor = bstree_left(cursor);
        }
    }
    //Cas ou l'arbre n'a pas de fils droit
    else{
        int cursor_key = bstree_key(cursor);
        int x_key = cursor_key;
        //On remonte jusqu'a trouver une clé supérieur a celle de x
        while(cursor_key <= x_key){
            cursor = bstree_parent(cursor);
            /*Si cursor est NULL alors il n'existe pas de sucesseur de x dans l'arbre on return une NULL*/
            if(bstree_empty(cursor)){
                return cursor;
            }
            cursor_key = bstree_key(cursor);
        }

    }
    return cursor;
}

const BinarySearchTree* bstree_predecessor(const BinarySearchTree* x) {
    assert(!bstree_empty(x));
    const BinarySearchTree* cursor = x;
    //Cas ou l'arbre a un fils gauche
    if(!bstree_empty(bstree_left(x))){
        cursor = bstree_left(cursor);
        while(!bstree_empty(bstree_right(cursor))){
            cursor = bstree_right(cursor);
        }
    }

    //Cas ou l'arbre n'a pas de fils gauche
    else{
        int cursor_key = bstree_key(cursor);
        int x_key = cursor_key;
        //On remonte jusqu'a trouver une clé inférieure a celle de x
        while(cursor_key >= x_key){
            cursor = bstree_parent(cursor);
            /*Si cursor est NULL alors il n'existe pas de predecesseur de x dans l'arbre on return une NULL*/
            if(bstree_empty(cursor)){
                return cursor;
            }
            cursor_key = bstree_key(cursor);
        }
    }
    return cursor;
}

void bstree_swap_nodes(ptrBinarySearchTree* tree, ptrBinarySearchTree from, ptrBinarySearchTree to) {
    assert(!bstree_empty(*tree) && !bstree_empty(from) && !bstree_empty(to));
    (void)tree; (void)from; (void)to;
}

// t -> the tree to remove from, current -> the node to remove
void bstree_remove_node(ptrBinarySearchTree* t, ptrBinarySearchTree current) {
    assert(!bstree_empty(*t) && !bstree_empty(current));
    (void)t; (void)current;
}

void bstree_remove(ptrBinarySearchTree* t, int v) {
    (void)t; (void)v;
}

/*------------------------  BSTreeVisitors  -----------------------------*/

void bstree_depth_prefix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if(!bstree_empty(t)){
        f(t,environment);
        bstree_depth_prefix(bstree_left(t),f,environment);
        bstree_depth_prefix(bstree_right(t),f,environment);
    }
}

void bstree_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if(!bstree_empty(t)){
        bstree_depth_infix(bstree_left(t),f,environment);
        f(t,environment);
        bstree_depth_infix(bstree_right(t),f,environment);
    }
}

void bstree_depth_postfix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    if(!bstree_empty(t)){
        bstree_depth_postfix(bstree_left(t),f,environment);
        bstree_depth_postfix(bstree_right(t),f,environment);
        f(t,environment);
    }
}

void bstree_iterative_breadth(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    Queue* q = create_queue();
    if(!bstree_empty(t)){
        queue_push(q,t);
    }
    while(!queue_empty(q)){
        const BinarySearchTree* elementATraiter = queue_top(q);
        BinarySearchTree* left = bstree_left(elementATraiter);
        BinarySearchTree* right = bstree_right(elementATraiter);
        queue_pop(q);
        
        f(elementATraiter,environment);

        if(!bstree_empty(left)){
            queue_push(q,left);
        }
        if(!bstree_empty(right)){
            queue_push(q,right);
        }
    }
}

void bstree_iterative_depth_infix(const BinarySearchTree* t, OperateFunctor f, void* environment) {
    Queue* noeudsATraiter = create_queue();
    const BinarySearchTree* cursor = t;

    while(!queue_empty(noeudsATraiter) || !bstree_empty(cursor)){
        while(!bstree_empty(cursor)){
            queue_push(noeudsATraiter,cursor);
            cursor = bstree_left(cursor);
        }

        if(bstree_empty(cursor) && !queue_empty(noeudsATraiter)){
            cursor = queue_top(noeudsATraiter);
            queue_pop(noeudsATraiter);
            f(cursor,environment);
        }
        cursor = bstree_right(cursor);

    } 
    
}

/*------------------------  BSTreeIterator  -----------------------------*/

struct _BSTreeIterator {
    /* the collection the iterator is attached to */
    const BinarySearchTree* collection;
    /* the first element according to the iterator direction */
    const BinarySearchTree* (*begin)(const BinarySearchTree* );
    /* the current element pointed by the iterator */
    const BinarySearchTree* current;
    /* function that goes to the next element according to the iterator direction */
    const BinarySearchTree* (*next)(const BinarySearchTree* );
};

/* minimum element of the collection */
const BinarySearchTree* goto_min(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* maximum element of the collection */
const BinarySearchTree* goto_max(const BinarySearchTree* e) {
	(void)e;
	return NULL;
}

/* constructor */
BSTreeIterator* bstree_iterator_create(const BinarySearchTree* collection, IteratorDirection direction) {
	(void)collection; (void)direction;
	return NULL;
}

/* destructor */
void bstree_iterator_delete(ptrBSTreeIterator* i) {
    free(*i);
    *i = NULL;
}

BSTreeIterator* bstree_iterator_begin(BSTreeIterator* i) {
    i->current = i->begin(i->collection);
    return i;
}

bool bstree_iterator_end(const BSTreeIterator* i) {
    return i->current == NULL;
}

BSTreeIterator* bstree_iterator_next(BSTreeIterator* i) {
    i->current = i->next(i->current);
    return i;
}

const BinarySearchTree* bstree_iterator_value(const BSTreeIterator* i) {
    return i->current;
}

/*------------------------  BSTreeAffichage  -----------------------------*/
void bstree_node_to_dot(const BinarySearchTree* t, void* stream) {
    FILE *file = (FILE *) stream;
    printf("%d ", bstree_key(t));

    // Affichage des nœuds avec une couleur rouge si leur couleur est "red"
    fprintf(file, "\tn%d [label=\"{%d|{<left>|<right>}}\", style=filled, fillcolor=%s];\n",
            bstree_key(t), 
            bstree_key(t),
            (t->color == red) ? "red" : "white");

    // Lien vers le fils gauche
    if (bstree_left(t)) {
        fprintf(file, "\tn%d:left:c -> n%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(bstree_left(t)));
    } else {
        fprintf(file, "\tlnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_key(t));
        fprintf(file, "\tn%d:left:c -> lnil%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(t));
    }

    // Lien vers le fils droit
    if (bstree_right(t)) {
        fprintf(file, "\tn%d:right:c -> n%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(bstree_right(t)));
    } else {
        fprintf(file, "\trnil%d [style=filled, fillcolor=grey, label=\"NIL\"];\n", bstree_key(t));
        fprintf(file, "\tn%d:right:c -> rnil%d:n [headclip=false, tailclip=false]\n",
                bstree_key(t), bstree_key(t));
    }
}
